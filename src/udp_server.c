#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<arpa/inet.h>
#include<ctype.h>

#define PORT 8000

int main(int argc, char const *argv[])
{
    /* code */
    struct sockaddr_in serv_add, cli_addr;
    socklen_t cli_addr_len;
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_add, sizeof(serv_add));
    serv_add.sin_family = AF_INET;
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_port = htons(PORT);


    bind(sock_fd, (struct sockaddr *)&serv_add, sizeof(serv_add));

    char buf[BUFSIZ];
    int i, n;
    char str[INET_ADDRSTRLEN];

    printf("ready ...\n");

    while (1)
    {
        /* code */
        cli_addr_len = sizeof(cli_addr);
        n = recvfrom(sock_fd, buf, BUFSIZ, 0, (struct sockaddr *)&cli_addr, &cli_addr_len);
        if (n == -1){
            perror("recvfrom error");
        }

        printf("received from %s at port %d. \n", inet_ntop(AF_INET, &cli_addr.sin_addr, str, INET_ADDRSTRLEN), ntohs(cli_addr.sin_port));

        for (i = 0; i < n; i++)
        {
            /* code */
            buf[i] = toupper(buf[i]);
        }
        
        n = sendto(sock_fd, buf, n, 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
        if (n == -1)
            perror("sendto error");

    }
    
    close(sock_fd);

    return 0;
}
