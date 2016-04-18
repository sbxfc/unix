//
//  main.cpp
//  SocketServer
//
//  Created by sbx_fc on 15/12/31.
//  Copyright © 2015年 me.rungame. All rights reserved.
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

#define MAXPENDING 5
#define MAXSIZE 1024
#define MAXLINE 1000

int main(int argc,char **argv) {
    
    int i,maxi,maxfd,listenfd,connfd,sockfd;
    int nready,client[FD_SETSIZE];
    ssize_t n;
    fd_set rset,allset;
    char buff[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;
    
    //创建一个TCP套接字,TCP是一个字节流协议,仅支持SOCK_STREAM套接字
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if (listenfd == -1){
        perror("创建套接字失败!");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9876);
    servaddr.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
    
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    /* Mark the socket so it will listen for incoming connections */
    if (listen(listenfd, MAXPENDING) < 0){
        perror("listen() failed!");
        exit(EXIT_FAILURE);
    }
    
    perror("listen!");
    
    maxfd = listenfd;   /*initialize*/
    maxi = -1;          /*index into client[] arraty */
    
    for (i =0; i < FD_SETSIZE; i++) {
        client[i] = -1;/*-1 indicates available entry */
    }
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);
    
    for (;;) {
        rset = allset;/*structure assignment*/
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        
        if(FD_ISSET(listenfd,&rset)){
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
            for (i =0; i < FD_SETSIZE; i++) {
                if (client[i] < 0){
                    /*记录每次连接的客户端fd放进描述符集合client里去*/
                    client[i] = connfd;/*save descriptor*/
                    break;
                }
            }
            
            if(i == FD_SETSIZE){
                perror("too many clients");
            }
            
            FD_SET(connfd, &allset);	/* add new descriptor to set */
            if (connfd > maxfd)
                maxfd = connfd;			/* for select() */
            if (i > maxi)
                maxi = i;				/* max index in client[] array */
            
            if(--nready <= 0)
                continue;
        }
        
        //遍历当前已连接的fd描述符集合,maxi为已连接的最大下标
        for (i = 0; i <= maxi; i++) {	/* check all clients for data */
            
            if ( (sockfd = client[i]) < 0){
                continue;
            }
            
            if (FD_ISSET(sockfd, &rset)){
                if ((n = read(sockfd, buff, MAXLINE)) == 0){
                    //客户端断开连接
                    close(sockfd);
                    FD_CLR(sockfd,&allset);
                    client[i] = -1;
                }
                else{
                    write(sockfd, buff, n);
                }
                
                if(--nready <= 0){
                    break;
                }
            }
        }
    }
    
    
    return 0;
}

