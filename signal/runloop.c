//
//  raise.c
//  signal
//
//  Created by sbx_fc on 16/1/4.
//  Copyright © 2016年 me.rungame. All rights reserved.
//
//
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	for(;;){
		printf(".\n");
		sleep(1);
	}
    puts("Exiting.");
    return 0;
}
