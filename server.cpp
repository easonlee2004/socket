#include "server.h"

// http respond
void setRespond(char *buff)
{
    memset(buff, 0, sizeof(buff));
    strcat(buff, "HTTP/1.1 200 OK\r\n");
    strcat(buff, "Connection: close\r\n");
    strcat(buff, "\r\n");
    strcat(buff, "Hello!\n");
}

int main()
{
    struct sockaddr_in servaddr;
    char buff[BUFFSIZE];
    // socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
        // connfd = accept(sockfd)
        int connfd = accept(sockfd, 0, 0);
        if (connfd == -1)
        {
            perror("Failed to create connect socket");
            return -1;
        }

        memset(buff, 0, sizeof(buff));

        // recv, 使用客户端连接的connfd接收
        recv(connfd, buff, BUFFSIZE - 1, 0);
        
        setRespond(buff);

        // send
        send(connfd, buff, strlen(buff), 0);

        close(connfd);
    }

    close(sockfd);
    return 0;
}