#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFFSIZE 2048
#define SERVER_IP "127.0.0.1" // localhost
#define SERVER_PORT 16555
int main()
{
    // socket
    struct sockaddr_in servaddr;
    char buff[BUFFSIZE];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Failed to create socket");
        return -1;
    }

    // connect
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr); // 点分十进制转网络序，无需赋值直接用指针写入
    servaddr.sin_port = htons(SERVER_PORT);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("Failed to connect");
        return -1;
    }

    // send
    printf("Please input: ");
    scanf("%s", buff);
    send(sockfd, buff, strlen(buff), 0);

    // recv
    memset(buff, 0, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    printf("%s\n", buff);

    close(sockfd);
    return 0;
}