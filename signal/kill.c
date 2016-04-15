//
//  kill.c
//  signal
//
//  Created by sbx_fc on 16/1/4.
//  Copyright © 2016年 me.rungame. All rights reserved.
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void catch_function(int signal) {
    puts("Interactive attention signal caught.");
}

/**
 * ./kill {pid}
 */
int main(int argc,char* argv[]) {
    
	int pid;
	if(argc == 2){
		pid = atoi(argv[1]);
	}
	else{
		pid = getpid();
	}
	printf("kill --->%i\n",pid);

	if (signal(SIGALRM, catch_function) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
   
	puts("Raising the interactive attention signal.");
    if (kill(pid,SIGALRM) != 0) {
        fputs("Error raising the signal.\n", stderr);
        return EXIT_FAILURE;
    }
    puts("Exiting.");
    return 0;
}

