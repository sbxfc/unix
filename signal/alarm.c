//
//  sigqueue.cpp
//  signal
//
//  Created by sbx_fc on 16/1/4.
//  Copyright © 2016年 me.rungame. All rights reserved.
//

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void catch_function(int signal) {
    puts("Alarm clock.");
}

int main(void) {
    if (signal(SIGALRM, catch_function) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    puts("Raising the interactive attention signal.");
    alarm(5);//5s
    
    for (;;) {}
    return 0;
}
