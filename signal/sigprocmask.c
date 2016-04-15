//
//  sigprocmask.cpp
//  signal
//
//  Created by sbx_fc on 16/1/4.
//  Copyright © 2016年 me.rungame. All rights reserved.
//

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>

void alarm_note(int signo)
{
    printf("/* alarm clock */\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    if (signal(SIGALRM, alarm_note) == SIG_ERR){
        perror("signal(SIGALRM) error!");
    }
    
    /**
     * 创建一个新的阻塞信号集合并把 SIGALRM 信号放进去
     */
    sigset_t maskset ,maskset_copy;
    sigemptyset(&maskset);
    sigaddset(&maskset,SIGALRM);
    
    if(sigismember(&maskset,SIGALRM)){
        printf("maskset阻塞信号集合包含了SIGALRM信号!\n");
    }
    
    /*
     * 设置阻塞信号集合maskset,并将当前已有的阻塞信号集合
     * 保存到 maskset_copy 阻塞信号集合。
     */
    if (sigprocmask(SIG_BLOCK, &maskset, &maskset_copy) < 0){
        printf("error");
    }
    
    /**
     * 发送SIGALRM信号。
     */
    raise(SIGALRM);
    
    /**
     * 休眠5秒
     */
    sleep(5);
    
    /*
     * 恢复成阻塞前的 maskset_copy 阻塞信号集
     */
    if (sigprocmask(SIG_SETMASK, &maskset_copy, NULL) < 0){
        printf("error");
    }
    
    if(sigismember(&maskset_copy,SIGALRM)){
        /*应该不会执行*/
        printf("maskset_copy阻塞信号集合包含了 SIGALRM \n");
    }
    
    return 0;
}