//
//  sigaction.cpp
//  signal
//
//  Created by sbx_fc on 16/1/5.
//  Copyright © 2016年 me.rungame. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void func(int sig)
{
    printf("I get a signal!\n");
}

int main()
{
    //
    //运行后,在控制台下执行如下指令
    //~user$ kill -INT {getpid}
    //
    
    char buffer[100];
    
    struct sigaction act;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    if(sigaction(SIGINT,&act, NULL) == -1)
    {
        printf("sigaction error exit now\n");
        exit(0);
    }
    
    printf("pid:%ld\n",(long)getpid());
    
    for(;;)
    {
        fgets(buffer,sizeof(buffer),stdin);
        printf("buffer is:%s\n",buffer);
    }
    
    return 0;
    
}