/* a server in the unix domain.  The pathname of the socket address is passed as an argument */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>

#define UNIX_SOCK "u_server.sock"

void error(const char *);

int main(int argc, char *argv[])
{
    int lfd, cfd, n;
    socklen_t servlen, clilen;
    struct sockaddr_un cli_addr, serv_addr;
    char buf[80];

    if ((lfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        error("creating socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, UNIX_SOCK);

    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    // servlen = strlen(serv_addr.sun_path) + offsetof(struct sockaddr_un, sun_path);

    unlink(UNIX_SOCK);

    if (bind(lfd, (struct sockaddr *)&serv_addr, servlen) < 0)
        error("binding socket");

    listen(lfd, 5);

    clilen = sizeof(cli_addr);
    cfd = accept(lfd, (struct sockaddr *)&cli_addr, &clilen);
    if (cfd < 0)
        error("accepting");
    printf("A connection has been established.\n");
    clilen -= offsetof(struct sockaddr_un, sun_path);
    cli_addr.sun_path[clilen] = '\0';
    printf("client bind sock pathname: %s \n", cli_addr.sun_path);

    n = read(cfd, buf, 80);

    write(STDOUT_FILENO, buf, n);

    char msg[] = "I got your message.";
    write(cfd, msg, sizeof(msg));

    close(cfd);

    close(lfd);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}