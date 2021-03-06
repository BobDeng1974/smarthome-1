#include <pthread.h>
#include <stdio.h>

#include <stdlib.h>

#include "rpc.h"
#include "dbgPrint.h"
#include "appinit.h"

int g_znpwfd = -1;

void *rpctask(void *argument)
{
	while (1) {
		rpcProcess();
	}

	dbg_print(PRINT_LEVEL_WARNING, "rpcTask exited!\n");

	return NULL;
}

void *apptask(void *argument)
{
	appProcess(argument);

	return NULL;
}

void *appinmessagetask(void *argument)
{
	while (1) {
		appMsgProcess(NULL);
	}

	return NULL;
}

int znp_start(int wfd, int znprfd, char * serialport){ 

	g_znpwfd = wfd;

	pthread_t rpcthread, appthread, inmthread;

	int serialportfd = rpcOpen(serialport, 0);
	if(serialportfd == -1){
		dbg_print(PRINT_LEVEL_ERROR, "could not open serial port %s\n", serialport);
		return -1;
	}

	rpcInitMq();

	appInit();

	//Start the Rx thread
	dbg_print(PRINT_LEVEL_INFO, "creating RPC thread\n");
	pthread_create(&rpcthread, NULL, rpctask, (void *) &serialportfd);

	//dbg_print(PRINT_LEVEL_INFO, "creating  thread\n");
	int * _znprfd = (int *)malloc(sizeof(int));
	*_znprfd = znprfd;
	fprintf(stdout, "znp_start fd %d \n", *_znprfd);
	pthread_create(&appthread, NULL, apptask, (void *)_znprfd);
	pthread_create(&inmthread, NULL, appinmessagetask, NULL);

	return 0;
}
