#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    // www.daemoncoder.com
    const char *ip = "127.0.0.1";
    int port = 10086;

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd > 0);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    if (-1 == ret)
    {
        if (errno == EACCES)
        {
            exit(1); // 没有权限
        }
        else if (errno == EADDRINUSE)
        {
            exit(2); // 端口被占用
        }
        else
        {
            exit(errno);
        }
    }

    ret = listen(sockfd, 1024);
    assert(0 == ret);

    int i = 0;
    while (i++ < 3)
    {
        struct sockaddr_in client;
        socklen_t client_addrlen = sizeof(client);
        int connfd = accept(sockfd, (struct sockaddr *)&client, &client_addrlen);
        if (connfd < 0)
        {
            printf("accept failed, errno:%d\n", errno);
        }
        else
        {
            char client_ip[INET_ADDRSTRLEN];
            printf("connected with %s:%d\n", inet_ntop(AF_INET, &client.sin_addr, client_ip, INET_ADDRSTRLEN), ntohs(client.sin_port));
            char buffer[1024];
            recv(connfd, buffer, sizeof(buffer), 0);
            printf("receive data:%s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}