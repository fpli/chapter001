#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>
#include <linux/fb.h>
#include <stdbool.h>

unsigned int *lcdptr = NULL; // lcd映射内存首地址
int lcd_width;               // lcd屏幕宽
int lcd_height;              // lcd屏幕高

int camera_width = 640;  // camere屏幕宽
int camera_height = 480; // camera屏幕高

void lcd_show_rgb(unsigned char *rgbdata, int width, int height);                         //在lcd上显示
int YUV2RGB(void *pYUV, void *pRGB, int width, int height, bool alphaYUV, bool alphaRGB); // YUYV格式转换为RGB格式

int main(void)
{
    int lcdfd = open("/dev/fb0", O_RDWR); //打开 LCD屏幕
    if (lcdfd < 0)
    {
        perror("/dev/fb0");
        exit(-1);
    }
    //获取屏幕宽高
    struct fb_var_screeninfo lcdvar;
    ioctl(lcdfd, FBIOGET_VSCREENINFO, &lcdvar);
    lcd_width = lcdvar.xres;
    lcd_height = lcdvar.yres;
    // 4 = 透明度 + RGB
    lcdptr = (unsigned int *)mmap(NULL, lcd_width * lcd_height * 4, //映射fb内存空间长度
                                  PROT_READ | PROT_WRITE,           //可读可写
                                  MAP_SHARED,                       //进程间共享机制
                                  lcdfd,                            // lcd的文件描述符
                                  0);

    // 1.打开摄像头设备
    int fd = open("/dev/video1", O_RDWR); // video0 或 video1
    if (fd < 0)
    {
        perror("打开设备失败");
        return -1;
    }

    // 2.获取摄像头支持的格式 ioctl(文件描述符，命令，与命令对应的结构体)
    //查询并显示所有支持的格式：VIDIOC_ENUM_FMT ，获取对应结构体
    /*struct v4l2_fmtdesc
      {
          u32 index; // 要查询的格式序号，应用程序设置
          enum v4l2_buf_type type; // 帧类型，应用程序设置
          u32 flags; // 是否为压缩格式
          u8 description[32]; // 格式名称
          u32 pixelformat; // 格式
          u32 reserved[4]; // 保留
      };
      */

    // 3.配置摄像头采集格式
    //查看或设置当前格式： VIDIOC_G_FMT, VIDIOC_S_FMT
    /*
    struct v4l2_format
    {
        enum v4l2_buf_type type; // 帧类型，应用程序设置
        union fmt
        {
            struct v4l2_pix_format pix; // 视频设备使用
            struct v4l2_window win;
            struct v4l2_vbi_format vbi;
            struct v4l2_sliced_vbi_format sliced;
            u8 raw_data[200];
        };
    };

    struct v4l2_pix_format
    {
        u32 width; // 帧宽，单位像素
        u32 height; // 帧高，单位像素
        u32 pixelformat; // 帧格式
        enum v4l2_field field;
        u32 bytesperline;
        u32 sizeimage;
        enum v4l2_colorspace colorspace;
        u32 priv;
    };
    */
    struct v4l2_format vfmt;
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;      //设置类型摄像头采集
    vfmt.fmt.pix.width = camera_width;            //设置宽
    vfmt.fmt.pix.height = camera_height;          //设置高
    vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; //根据摄像头设置格式
    int ret;
    ret = ioctl(fd, VIDIOC_S_FMT, &vfmt);
    if (ret < 0)
    {
        perror("设置格式失败");
    }
    printf("Current data format information:\n\twidth:%d\n\theight:%d\n", vfmt.fmt.pix.width, vfmt.fmt.pix.height);

    // 4.申请内核空间
    //应用程序和设备有三种交换数据的方法，直接 read/write、内存映射(memory mapping) 和用户指针。
    //向设备申请缓冲区 VIDIOC_REQBUFS
    /*相关函数：int ioctl(int fd, int request, struct v4l2_requestbuffers *argp);
    相关结构体：
    struct v4l2_requestbuffers
    {
        u32 count; // 缓冲区内缓冲帧的数目
        enum v4l2_buf_type type; // 缓冲帧数据格式
        enum v4l2_memory memory; // 区别是内存映射还是用户指针方式
        u32 reserved[2];
    };
    enum v4l2_memory
    {
        V4L2_MEMORY_MMAP, V4L2_MEMORY_USERPTR
    };
    //count,type,memory 都要应用程序设置
    */
    struct v4l2_requestbuffers reqbuffer;
    reqbuffer.count = 4;                          //申请4个缓冲区
    reqbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // 缓冲帧数据格式
    reqbuffer.memory = V4L2_MEMORY_MMAP;          //内存映射
    ret = ioctl(fd, VIDIOC_REQBUFS, &reqbuffer);
    if (ret < 0)
    {
        perror("申请队列空间失败");
    }

    // 5.把内核的缓冲区队列映射到用户空间
    //获取缓冲帧的地址，长度：VIDIOC_QUERYBUF
    //相关函数：int ioctl(int fd, int request, struct v4l2_buffer *argp);
    /*相关结构体：
    struct v4l2_buffer
    {
        u32 index; //buffer 序号
        enum v4l2_buf_type type; //buffer 类型
        u32 byteused; //buffer 中已使用的字节数
        u32 flags; // 区分是MMAP 还是USERPTR
        enum v4l2_field field;
        struct timeval timestamp; // 获取第一个字节时的系统时间
        struct v4l2_timecode timecode;
        u32 sequence; // 队列中的序号
        enum v4l2_memory memory; //IO 方式，被应用程序设置
        union m
        {
            u32 offset; // 缓冲帧地址，只对MMAP 有效
            unsigned long userptr;
        };
        u32 length; // 缓冲帧长度
        u32 input;
        u32 reserved;
    };
    */

    unsigned char *mptr[4]; //保护映射后用户空间的首地址
    unsigned int size[4];
    struct v4l2_buffer mapbuffer;
    for (unsigned int n_buffers = 0; n_buffers < reqbuffer.count; n_buffers++) // count=4个缓冲区
    {
        memset(&mapbuffer, 0, sizeof(mapbuffer));     //清空
        mapbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; //设置类型摄像头采集
        mapbuffer.memory = V4L2_MEMORY_MMAP;          //内存映射  IO 方式，被应用程序设置
        mapbuffer.index = n_buffers;                  // buffer 序号
        // 查询序号为n_buffers 的缓冲区，得到其起始物理地址和大小
        ret = ioctl(fd, VIDIOC_QUERYBUF, &mapbuffer); //从内核空间中查询一个空间做映射
        if (ret < 0)
        {
            perror("查询内核空间队列失败");
        }

        mptr[n_buffers] = (unsigned char *)mmap(NULL, mapbuffer.length, //被映射内存块的长度
                                                PROT_READ | PROT_WRITE, //可读可写
                                                MAP_SHARED,             //进程间共享机制
                                                fd,                     //摄像头的文件描述符
                                                mapbuffer.m.offset);    // 缓冲帧地址，只对MMAP 有效

        size[n_buffers] = mapbuffer.length; //保存长度，释放用

        //通知使用完毕--‘放回去’
        // VIDIOC_QBUF// 把帧放入队列
        // VIDIOC_DQBUF// 从队列中取出帧
        ret = ioctl(fd, VIDIOC_QBUF, &mapbuffer);
        if (ret < 0)
        {
            perror("帧放入队列失败");
        }
    }

    // 6.开始采集
    //冲区处理好之后，就可以开始获取数据了
    //启动 或 停止数据流 VIDIOC_STREAMON， VIDIOC_STREAMOFF
    
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret < 0)
    {
        perror("开启失败");
    }
    //定义一个空间存储解码后的RGB数据
    unsigned char rgbdata[camera_width * camera_height * 3];
    while (1)
    {
        // 7.采集数据
        //从队列中提取一帧数据
        struct v4l2_buffer readbuffer;
        readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ret = ioctl(fd, VIDIOC_DQBUF, &readbuffer); // 从缓冲区取出一个缓冲帧
        if (ret < 0)
        {
            perror("提取数据失败");
        }

        /*  //把一帧数据写入文件
        FILE *file = fopen("my.yuyv","w+");
        //mptr[readbuffer.index]
        fwrite(mptr[readbuffer.index],readbuffer.length,file);
        fclose(file); */

        YUV2RGB(mptr[readbuffer.index], rgbdata, camera_width, camera_height, 0, 0); //把YUYV数据解码为RGB数据
        lcd_show_rgb(rgbdata, camera_width, camera_height);                          //在开发板lcd上显示

        //通知内核已经使用完毕
        ret = ioctl(fd, VIDIOC_QBUF, &readbuffer);
        if (ret < 0)
        {
            perror("放回队列失败");
        }
    }

    // 8.停止采集
    ret = ioctl(fd, VIDIOC_STREAMOFF, &type);

    // 9.释放映射
    for (int i = 0; i < 4; i++)
        munmap(mptr[i], size[i]); // 断开映射

    // 10.关闭设备
    close(fd);

    return 0;
}

//将解码后的摄像头数据显示在开发板屏幕上
void lcd_show_rgb(unsigned char *rgbdata, int width, int height) // width、height为摄像头图片的宽高
{
    unsigned int *ptr = lcdptr;

    //以字节对齐的方式，将RGB颜色数据转换成ARGB的LCD数据
    unsigned int lcd_buf[width * height];
    //对齐像素
    for (int i = 0; i < width * height; i++)
    {
        lcd_buf[i] = rgbdata[3 * i + 0] | rgbdata[3 * i + 1] << 8 | rgbdata[3 * i + 2] << 16 | 0x00 << 24;
        //蓝色				//绿色			//红色              //透明度
    }
    //数据不能超过lcd屏幕尺寸以及摄像头图片尺寸
    for (int x = 0; x < lcd_width && x < width; x++)
    {
        for (int y = 0; y < lcd_height && y < height; y++)
        {
            // 获取lcd屏幕中的偏移量
            int lcd_offset = x + lcd_width * y;
            // 获取camera图片中的偏移量
            int camera_offset = x + width * (height - y - 1);

            *(ptr + lcd_offset) = lcd_buf[camera_offset];
        }
    }

    return;
}

// YUV2RGB
// pYUV         point to the YUV data
// pRGB         point to the RGB data
// width        width of the picture
// height       height of the picture
// alphaYUV     is there an alpha channel in YUV
// alphaRGB     is there an alpha channel in RGB
// YUYV格式转换为RGB格式
int YUV2RGB(void *pYUV, void *pRGB, int width, int height, bool alphaYUV, bool alphaRGB)
{
    if (NULL == pYUV)
    {
        return -1;
    }
    unsigned char *pYUVData = (unsigned char *)pYUV;
    unsigned char *pRGBData = (unsigned char *)pRGB;
    /*if (NULL == pRGBData)
    {
        if (alphaRGB)
        {
            pRGBData = new unsigned char[width*height*4];
        }
        else
            pRGBData = new unsigned char[width*height*3];
    }*/

    int Y1, U1, V1, Y2, alpha1, alpha2, R1, G1, B1, R2, G2, B2;
    int C1, D1, E1, C2;
    if (alphaRGB)
    {
        if (alphaYUV)
        {
            for (int i = 0; i < height; ++i)
            {
                for (int j = 0; j < width / 2; ++j)
                {
                    Y1 = *(pYUVData + i * width * 3 + j * 6); // i*width*3 = i*(width/2)*6
                    U1 = *(pYUVData + i * width * 3 + j * 6 + 1);
                    Y2 = *(pYUVData + i * width * 3 + j * 6 + 2);
                    V1 = *(pYUVData + i * width * 3 + j * 6 + 3);
                    alpha1 = *(pYUVData + i * width * 3 + j * 6 + 4);
                    alpha2 = *(pYUVData + i * width * 3 + j * 6 + 5);
                    C1 = Y1 - 16;
                    C2 = Y2 - 16;
                    D1 = U1 - 128;
                    E1 = V1 - 128;
                    R1 = ((298 * C1 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 409 * E1 + 128) >> 8);
                    G1 = ((298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B1 = ((298 * C1 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 516 * D1 + 128) >> 8);
                    R2 = ((298 * C2 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 409 * E1 + 128) >> 8);
                    G2 = ((298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B2 = ((298 * C2 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 516 * D1 + 128) >> 8);
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 2) = R1 < 0 ? 0 : R1;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 1) = G1 < 0 ? 0 : G1;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8) = B1 < 0 ? 0 : B1;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 3) = alpha1;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 6) = R2 < 0 ? 0 : R2;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 5) = G2 < 0 ? 0 : G2;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 4) = B2 < 0 ? 0 : B2;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 7) = alpha2;
                }
            }
        }
        else
        {
            int alpha = 255;
            for (int i = 0; i < height; ++i)
            {
                for (int j = 0; j < width / 2; ++j)
                {
                    Y1 = *(pYUVData + i * width * 2 + j * 4);
                    U1 = *(pYUVData + i * width * 2 + j * 4 + 1);
                    Y2 = *(pYUVData + i * width * 2 + j * 4 + 2);
                    V1 = *(pYUVData + i * width * 2 + j * 4 + 3);
                    C1 = Y1 - 16;
                    C2 = Y2 - 16;
                    D1 = U1 - 128;
                    E1 = V1 - 128;
                    R1 = ((298 * C1 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 409 * E1 + 128) >> 8);
                    G1 = ((298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B1 = ((298 * C1 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 516 * D1 + 128) >> 8);
                    R2 = ((298 * C2 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 409 * E1 + 128) >> 8);
                    G2 = ((298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B2 = ((298 * C2 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 516 * D1 + 128) >> 8);
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 2) = R1 < 0 ? 0 : R1;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 1) = G1 < 0 ? 0 : G1;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8) = B1 < 0 ? 0 : B1;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 3) = alpha;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 6) = R2 < 0 ? 0 : R2;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 5) = G2 < 0 ? 0 : G2;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 4) = B2 < 0 ? 0 : B2;
                    *(pRGBData + (height - i - 1) * width * 4 + j * 8 + 7) = alpha;
                }
            }
        }
    }
    else
    {
        if (alphaYUV)
        {
            for (int i = 0; i < height; ++i)
            {
                for (int j = 0; j < width / 2; ++j)
                {
                    Y1 = *(pYUVData + i * width * 3 + j * 4);
                    U1 = *(pYUVData + i * width * 3 + j * 4 + 1);
                    Y2 = *(pYUVData + i * width * 3 + j * 4 + 2);
                    V1 = *(pYUVData + i * width * 3 + j * 4 + 3);
                    C1 = Y1 - 16;
                    C2 = Y2 - 16;
                    D1 = U1 - 128;
                    E1 = V1 - 128;
                    R1 = ((298 * C1 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 409 * E1 + 128) >> 8);
                    G1 = ((298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B1 = ((298 * C1 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 516 * D1 + 128) >> 8);
                    R2 = ((298 * C2 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 409 * E1 + 128) >> 8);
                    G2 = ((298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B2 = ((298 * C2 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 516 * D1 + 128) >> 8);
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 2) = R1 < 0 ? 0 : R1;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 1) = G1 < 0 ? 0 : G1;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6) = B1 < 0 ? 0 : B1;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 5) = R2 < 0 ? 0 : R2;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 4) = G2 < 0 ? 0 : G2;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 3) = B2 < 0 ? 0 : B2;
                }
            }
        }
        else
        {
            for (int i = 0; i < height; ++i)
            {
                for (int j = 0; j < width / 2; ++j)
                {
                    Y1 = *(pYUVData + i * width * 2 + j * 4);
                    U1 = *(pYUVData + i * width * 2 + j * 4 + 1);
                    Y2 = *(pYUVData + i * width * 2 + j * 4 + 2);
                    V1 = *(pYUVData + i * width * 2 + j * 4 + 3);
                    C1 = Y1 - 16;
                    C2 = Y2 - 16;
                    D1 = U1 - 128;
                    E1 = V1 - 128;
                    R1 = ((298 * C1 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 409 * E1 + 128) >> 8);
                    G1 = ((298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C1 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B1 = ((298 * C1 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C1 + 516 * D1 + 128) >> 8);
                    R2 = ((298 * C2 + 409 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 409 * E1 + 128) >> 8);
                    G2 = ((298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8 > 255 ? 255 : (298 * C2 - 100 * D1 - 208 * E1 + 128) >> 8);
                    B2 = ((298 * C2 + 516 * D1 + 128) >> 8 > 255 ? 255 : (298 * C2 + 516 * D1 + 128) >> 8);
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 2) = R1 < 0 ? 0 : R1;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 1) = G1 < 0 ? 0 : G1;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6) = B1 < 0 ? 0 : B1;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 5) = R2 < 0 ? 0 : R2;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 4) = G2 < 0 ? 0 : G2;
                    *(pRGBData + (height - i - 1) * width * 3 + j * 6 + 3) = B2 < 0 ? 0 : B2;
                }
            }
        }
    }
    return 0;
}