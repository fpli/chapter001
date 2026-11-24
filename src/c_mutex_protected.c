#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

// static init pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * pthread_cleanup_push 是POSIX线程库中用于‌注册线程清理函数‌的关键机制。它与 pthread_cleanup_pop 配对使用，
 * 旨在确保线程在异常退出（如被取消）或显式调用 pthread_exit 时，
 * 能够自动释放持有的资源（如互斥锁、动态内存、文件描述符等），从而避免资源泄漏或死锁。
 * 
 *  #include <pthread.h>
 *  // 注册清理函数
 *  void pthread_cleanup_push(void (*routine)(void *), void *arg);
 *  routine‌：清理函数的指针，其签名必须为 void func(void *)
 *  arg‌：传递给清理函数的参数，通常是资源的指针（如互斥锁、动态分配的内存等）。
 *  
 *  // 弹出并可选执行清理函数
 *  void pthread_cleanup_pop(int execute);
 *  execute‌：非 0‌：弹出栈顶的清理函数并‌立即执行‌。 0‌：仅从栈中移除记录，‌不执行‌清理函数。
 *  
 * 2. 触发清理函数的场景
    注册的清理函数会在以下三种情况下被自动执行（按‌后进先出 LIFO‌ 顺序）：
    线程被取消‌：当其他线程调用 pthread_cancel 且目标线程到达取消点时。
    主动退出‌：线程调用 pthread_exit() 终止时。
    手动弹出执行‌：调用 pthread_cleanup_pop(非0) 时。
 */
void cleanup_handler(void *arg)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
    printf("cleanup handler: unlocked mutex \n");
    pthread_mutex_unlock(mutex);
}

void do_heavy_work()
{
    // 模拟大量计算工作
    for (volatile int i = 0; i < 100000000; ++i)
    {
        // 执行一些计算
    }
}

void *tfn(void *arg)
{
    pthread_mutex_lock(&mutex);
    printf("thread 1: locked mutex \n");
    pthread_cleanup_push(cleanup_handler, (void *)&mutex);
    printf("thread 1: push complete \n");
    
    // sleep(2);

    // 模拟工作... 
    while(1){ 
    //    执行大量的计算工作...
        do_heavy_work();

        // 显式检查是否有取消请求
        // 如果没有这一行，线程可能无法响应 pthread_cancel
        pthread_testcancel();
        // 如果在此处发生pthread_cancel，cleanup_handler 会自动执行解锁
    };

    // 3. 正常路径：手动弹出并执行清理（或者 pop(0) 后手动 unlock）
    // 这里使用 pop(1) 确保无论是否被取消，锁都会被释放
    // 重要注意‌:如果线程通过 return 语句正常返回主函数，‌不会‌触发任何已注册的清理函数。
    // 因此，若希望正常返回时也执行清理，需在 return 前调用 pthread_cleanup_pop(1)。
    pthread_cleanup_pop(1); 

    // dead code 
    printf("thread 1: pop complete \n");
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, tfn, NULL);
    // 模拟主线程等待后取消子线程（如果子线程还没结束）
    sleep(1); 
    pthread_cancel(tid);
    // pthread_cancel 是 POSIX 线程库中用于向指定线程发送终止请求的函数，‌调用后目标线程不会立即停止‌，
    // 而是根据取消状态和取消类型，在到达取消点或满足条件时才真正终止。
    /**
    取消点和资源清理怎么做
    取消点‌：是线程检查取消请求并执行终止动作的特定位置，常见取消点包括 sleep、pthread_cond_wait、read、write、pthread_join 等系统调用及阻塞函数。
    手动设置取消点‌：在线程循环中可调用 pthread_testcancel() 主动检查取消请求，执行前需确保相关资源已释放。
    资源清理‌：推荐使用 pthread_cleanup_push 注册清理函数（如关闭文件、释放互斥量），当线程被取消或正常退出时自动执行清理，避免内存泄漏[citation]。
    完整流程‌：创建线程→设置取消状态/类型→线程运行中定期检查取消点→主线程调用 pthread_cancel 发送请求→主线程调用 pthread_join 等待线程退出并回收资源。‌‌‌
     */

    pthread_join(tid, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}

/**
 * 3. 核心特性与约束
A.宏实现而非普通函数
在大多数 POSIX 实现（如 Linux glibc）中，pthread_cleanup_push 和 pthread_cleanup_pop 是‌宏‌。

必须成对出现‌：它们必须在‌同一函数‌、‌同一词法作用域‌内成对使用。
编译检查‌：push 宏通常包含 {，pop 宏包含 }。如果不成对或跨作用域使用，会导致编译错误。
B. 后进先出 (LIFO) 顺序
如果一个线程注册了多个清理函数，它们会形成一个栈。执行时，最后注册的函数最先执行。
这符合资源依赖逻辑（例如：先解锁后释放内存，或者先关闭文件后释放缓冲区）。

C. 安全性限制
禁止 longjmp‌：不要在清理函数中使用 longjmp 或 siglongjmp 跳出，否则行为未定义，可能导致栈状态不一致。
异步取消风险‌：建议在线程取消类型为 PTHREAD_CANCEL_DEFERRED（默认）时使用。
若设置为异步取消，清理函数的执行时机不可控，可能引发竞态条件。
 */