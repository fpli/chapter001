#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<arpa/inet.h>
#include<ctype.h>

#define PORT 8000
#define IP "127.0.0.1"

int main(int argc, char const *argv[])
{
    /* code */
    struct sockaddr_in serv_add;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_add, sizeof(serv_add));
    serv_add.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &serv_add.sin_addr);
    serv_add.sin_port = htons(PORT);

    char buf[BUFSIZ];
    int n = 0;
    while (fgets(buf, BUFSIZ, stdin) != NULL)
    {
        /* code */
        n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serv_add, sizeof(serv_add));
        if (n == -1)
            perror("sendto error");

        n = recvfrom(sockfd, buf, BUFSIZ, 0, NULL, 0);
        if (n == -1)
        {
            perror("recvfrom error");
        }
        
        write(STDOUT_FILENO, buf, n);
    }
    
    close(sockfd);

    return 0;
}