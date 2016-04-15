//
//  setitimer.cpp
//  signal
//
//  Created by sbx_fc on 16/1/4.
//  Copyright © 2016年 me.rungame. All rights reserved.
//

#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

static void catch_function(int signal) {
    puts("Alarm clock.");
}

int main(void) {
    if (signal(SIGALRM, catch_function) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    puts("Raising the interactive attention signal.");
    
    struct itimerval tick;
    // Initialize struct
    memset(&tick, 0, sizeof(tick));
    // Timeout to run function first time
    tick.it_value.tv_sec = 5;  // 5(sec)
    tick.it_value.tv_usec = 0; // micro sec.
    setitimer(ITIMER_REAL, &tick, NULL);
    
    for (;;) {}
    return 0;
}
