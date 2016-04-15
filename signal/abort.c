//
//  abort.cpp
//  signal
//
//  Created by sbx_fc on 16/1/4.
//  Copyright © 2016年 me.rungame. All rights reserved.
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void catch_function(int signal) {
    puts("abort!");
}

int main(void) {
    if (signal(SIGABRT, catch_function) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    puts("Raising the interactive attention signal.");
    
    abort();
    return 0;
}

