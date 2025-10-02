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
    int lfd = socket(AF_INET, SOCK_STREAM, 0); // lfd: listen fd
    if (lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    // bind
    memset(&servaddr, 0, sizeof(servaddr));
    // bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(DEFAULT_PORT); // DEFAULT_PORT是自己定义的端口宏

    if (bind(lfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind error");
        exit(1);
    }

    // listen
    if (listen(lfd, MAXLINK) == -1)
    {
        perror("listen error");
        exit(1);
    }

    // 读描述符集合
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(lfd, &readset);

    while (1)
    {
        // 每一轮都更新tmp
        fd_set tmp = readset;
        int ret = select(1024, &tmp, 0, 0, 0);
        // 判断listen fd是否在写集合中
        if (FD_ISSET(lfd, &tmp))
        {
            int cfd = accept(lfd, 0, 0);
            FD_SET(cfd, &readset);
        }
        // 遍历fd找出可用的cfd
        for (int i = 0; i < 1024; i++)
        {
            if (i != lfd && FD_ISSET(i, &readset))
            {
                memset(buff, 0, sizeof(buff));

                // recv, 使用客户端连接的cfd接收
                int len = recv(i, buff, BUFFSIZE - 1, 0);
                if (len == -1)
                {
                    perror("recv error");
                    exit(1);
                }
                else if (len == 0) // 客户端断开
                {
                    printf("client closed connection");
                    FD_CLR(i, &readset);
                    break;
                }
        
                setRespond(buff);

                // send
                if (send(i, buff, strlen(buff), 0) == -1)
                {
                    perror("send error");
                    exit(1);
                }

                close(i);
            }
        }
    }

    // // 阻塞等待连接
    // while (1)
    // {
    //     // cfd = accept(lfd)
    //     int cfd = accept(lfd, 0, 0);
    //     if (cfd == -1)
    //     {
    //         perror("accept error");
    //         exit(1);
    //     }

    //     memset(buff, 0, sizeof(buff));

    //     // recv, 使用客户端连接的cfd接收
    //     int len = recv(cfd, buff, BUFFSIZE - 1, 0);
    //     if (len == -1)
    //     {
    //         perror("recv error");
    //         exit(1);
    //     }
    //     else if (len == 0) // 客户端断开
    //     {
    //         printf("client closed connection");
    //         close(cfd);
    //         break;
    //     }
        
    //     setRespond(buff);

    //     // send
    //     if (send(cfd, buff, strlen(buff), 0) == -1)
    //     {
    //         perror("send error");
    //         exit(1);
    //     }
    // }

    close(lfd);
    return 0;
}