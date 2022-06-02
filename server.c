#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024
int main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in sockaddr;
    /*-----结构体sockaddr_in-----
    头文件#include<arpa/inet.h>
    struct sockaddr_in{
        short sin_family; //所采用的地址协议
        u_short sin_port; //端口号
        struct in_addr sin_addr;//存放32位的IP地址
        char sin_zero[8]; //填充用
    }
    要传入socket的信息的集合体
    ------------------------------
    结构体in_addr
    struct in_addr{
        in_addr_t s_addr; 
        //32位的IPv4地址，为网络字节序，本质上是unsigned int
    }
    */

    char buff[MAXLINE];
    int n;

    memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET; //AF_INET是IPv4

    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*
    1. INADDR_ANY：指定地址为0.0.0.0的地址，也即任意地址
    2. htonl(): 将32位主机字节序转换为网络字节序
    ---------------------
    主机字节序
    整数在内存中的存储形式。
    有两种存储形式，数据小的存储在低序地址，是little-endian(小序)
                  数据大的存储在低序地址，是big-endian(大序)
    --------------------
    网络字节序号
    协商好的字节序，采用big-endian(大序)
    */

    sockaddr.sin_port = htons(10004);
    //这里10004是server开放的用于客户端连接的监听端口，

    /*
    htons(): 将无符号短整形数转换成网络字节序
    这里就是将端口号转为网络字节序。
    输出仍然是无符号整形
    */


    listenfd = socket(AF_INET, SOCK_STREAM, 0);


    bind(listenfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));

    listen(listenfd, 1024);

    printf("Please wait for the client information\n");

    for (;;)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            printf("accpet socket error: %s errno :%d\n", strerror(errno), errno);
            continue;
        }

        n = recv(connfd, buff, MAXLINE, 0);


        buff[n] = '\0';
        printf("recv msg from client:%s", buff);

        send(connfd,buff,strlen(buff),0);
        close(connfd);
    }
    close(listenfd);
}