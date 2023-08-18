#include <stdio.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd > 0);

    char *server_ip = "127.0.0.1";
    int server_port = 10086;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    server_addr.sin_port = htons(server_port);

    int ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (-1 == ret)
    {
        printf("connect failed, errno:%d\n", errno);
    }

    char data[] = "www.daemoncoder.com";
    send(sockfd, data, strlen(data), 0);

    close(sockfd);
    return 0;
}