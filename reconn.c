#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include "eventhub.h"
#include "connection.h"
#include "socket.h"
#include "parseserver.h"
#include "gateway.h"
#include "innercmd.h"

struct reconn{ 
	int rfd;
	int wfd;
	pthread_t reconnthread;
	struct eventhub * hub;

};

void event_reconnect(struct eventhub * hub, int wfd){
	struct list_head * head = connlist_get();
	if(!connlist_check(CONNSOCKETSERVER)){
		struct connection * serverconn = connectserver();
		if(serverconn){
			eventhub_register(hub,connection_getfd(serverconn)); 

			int n = write(wfd, CESEND, 1);
			if(n < 0){
				fprintf(stdout, "%d %s \n ", errno, strerror(errno));
			}
		}
	}
}

void * ceconnect(void * args){ 
	struct reconn * rc = (struct reconn *)args;
	int rfd = rc->rfd;
	int wfd = rc->wfd;
	struct eventhub * hub = rc->hub;
	for(;;){
		ssize_t count;
		char buf[1024];

		count = read (rfd, buf, sizeof buf);
		event_reconnect(hub, wfd);
	}
}

void reconn_start(int rfd, int wfd, struct eventhub * hub){
	struct reconn  * rc = (struct reconn *)malloc(sizeof(struct reconn));
	memset(rc, 0, sizeof(struct reconn));
	rc->rfd = rfd;
	rc->hub = hub;
	rc->wfd = wfd;
	pthread_t threadid;
	pthread_create(&threadid, NULL, &ceconnect, rc);
}
