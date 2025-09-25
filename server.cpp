#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <signal.h>
#define BUFFSIZE 2048
#define DEFAULT_PORT 16555 // 指定端口
#define MAXLINK 2048
int sockfd, connfd;
void stopServerRunning(int p)
{
    close(sockfd);
    printf("Close Server\n");
    exit(0);
}
int main()
{
    struct sockaddr_in servaddr;
    char buff[BUFFSIZE];
    // socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Failed to create server socket");
        return -1;
    }

    // bind
    memset(&servaddr, 0, sizeof(servaddr));
    // bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(DEFAULT_PORT); // DEFAULT_PORT是自己定义的端口宏

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("Failed to bind");
        return -1;
    }

    // listen
    if (listen(sockfd, MAXLINK) == -1)
    {
        perror("Failed to listen");
        return -1;
    }

    while(1)
    {
        signal(SIGINT, stopServerRunning); // ctrl+c停止运行。第二个参数是个句柄，传入自定义停止函数
        // connfd = accept(sockfd)
        connfd = accept(sockfd, 0, 0);
        if (connfd == -1)
        {
            perror("Failed to create connect socket");
            return -1;
        }

        memset(buff, 0, sizeof(buff));

        // recv, 使用客户端连接的connfd接收
        recv(connfd, buff, BUFFSIZE - 1, 0);
        
        // send
        send(connfd, buff, strlen(buff), 0);

        close(connfd);
    }

    close(sockfd);
    return 0;
}