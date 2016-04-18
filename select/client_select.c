//
//  client_select.cpp
//  SocketClient
//
//  Created by sbx_fc on 16/1/11.
//  Copyright © 2016年 me.rungame. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 4096

int max(int a,int b)
{
    return a>b ? a : b;
}

void str_cli(FILE *fp,int sockfd){
    
    int maxfd,stdineof;
    fd_set rset;
    char buf[MAXLINE];
    ssize_t n;
    
    stdineof = 0;
    //初始化fd_set
    FD_ZERO(&rset);
    for (; ; ) {
        //向fd_set添加描述符
        if(stdineof == 0)
            FD_SET(fileno(fp),&rset);
        //向fd_set添加描述符sockfd
        FD_SET(sockfd,&rset);
        maxfd = max(fileno(fp),sockfd) + 1;
        select(maxfd, &rset, NULL, NULL, NULL);
        
        if(FD_ISSET(sockfd,&rset)){/*socket is readable*/
            printf("socket is readable \n");
            if((n = read(sockfd, buf , MAXLINE)) == 0){
                if(stdineof == 1){
                    return;
                }
                else{
                    printf("Server terminated permaturely \n");
                }
            }
            write(fileno(stdout), buf, n);
        }
        

        if(FD_ISSET(fileno(fp), &rset)){/*input is readable*/
            printf("input is readable \n");
            if((n = read(fileno(fp), buf , MAXLINE)) == 0){
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);/*send FIN*/
                FD_CLR(fileno(fp),&rset);
                continue;
            }
            write(sockfd, buf, n);
        }
    }

}

int main(int argc,char **argv) {
    
    //套接字对端地址
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9876);
    servaddr.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
    
    //建立连接
    //创建一个TCP套接字,TCP是一个字节流协议,仅支持SOCK_STREAM套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (-1 == sockfd){
        perror("创建套接字失败! \n");
        exit(EXIT_FAILURE);
    }
    
    //若成功返回为0,若出错则为 -1
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
        printf("套接字连接失败! \n");
        exit(EXIT_FAILURE);
    }
    
    printf("套接字连接成功! \n");
    
    //发送和接受
    str_cli(stdin,sockfd);
    
    return 0;
}

