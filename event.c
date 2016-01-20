#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "connection.h"
#include "protocol.h"
#include "toolkit.h"
#include "eventhub.h" 
#include "cetimer.h"
#include "socket.h"
#include "termcontrol.h" 
#include "gateway.h"
#include "list.h"
#include "zcl_datatype.h"

void event_accept(int fd){
	struct connection * c = freeconnlist_getconn();
	connection_init(c, fd, CONNSOCKETCLIENT);
	connrbtree_insert(c);
}


int _check_command(unsigned char * buffer, int buflen, unsigned char command){
	int i = 0;
	for(; i < buflen; i++){
		if(buffer[i] == command){
			return 1;
		}
	}

	return 0;
}

void event_recvmsg(struct eventhub * hub, int fd, unsigned char * buf, int buflen){
	//toolkit_printbytes(buf, buflen);
	struct connection * c = connrbtree_getconn(fd);
	if(c && connection_gettype(c) == CONNSOCKETCMD){ 
		if( _check_command(buf, buflen, CECHECK[0])){
			time_t t = time(NULL);
			connlist_checkstatus(t);
		}
		if( _check_command(buf, buflen, HEARTBEAT[0])){

			unsigned char heart_buf[255];
			unsigned int hbuflen;

			hbuflen = encode_heart(getgateway(), heart_buf);
			struct list_head *pos, *n;
			list_for_each_safe(pos, n, connlist_get()){
				struct connection *c = list_entry(pos, struct connection, list);
				if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER))
				{
					for(;;){
						int n = write(connection_getfd(c),heart_buf,hbuflen);
						if(n < 0){
							fprintf(stdout, "%s \n", strerror(errno));
						}
						break;
					}
				}
			}
		}
	}else if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER)){
	//}else if(c && connection_gettype(c) == CONNSERIALPORT){
		connection_put(c, buf, buflen); 
		unsigned short messageid = 0;
		int messagelen = protocol_check(c, &messageid);
		char buffer[1024] = {0};
		connection_get(c,buffer, messagelen);
		switch(messageid){
			case LOGINFEEDBACK:
			case HEARTFEEDBACK:
				break;
			case REQDEVICELIST:
				break;
			case REQOPERATE:
				break;
			case REQSETDEVICENAME:
				break;
			case REQDELDEVICE:
				break;
			case ILLEGAL:
				break;
		}
	//}else if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER)){
	}
}

void event_recvznp(struct eventhub * hub, int fd){ 
	int znpdatatype = 0;
	readnonblocking(fd, &znpdatatype, sizeof(int));
	switch(znpdatatype){
		case ZCLZONEENROLLREQ: 
			{ 
				struct zclzoneenrollreq req;
				readnonblocking(fd, &req, sizeof(struct zclzoneenrollreq));
				struct device * device = device_create(req.shortaddr);
				device_addcluster(device, req.ieeeaddr, 1, req.clusterid, "");
			}
			break;
		case ZCLZONECHANGENOTIFICATION:
			{
				struct zclzonechangenotification req;
				readnonblocking(fd, &req, sizeof(struct zclzonechangenotification));
				fprintf(stdout, "-----------------------%u\n", req.zonestatus);
				fprintf(stdout, "-----------------------%d\n", req.extendedstatus);
				fprintf(stdout, "-----------------------%d\n", req.zoneid);
				fprintf(stdout, "-----------------------%d\n", req.delay);
			}
	}
}

void event_close(int fd){
	struct connection * c = connrbtree_getconn(fd);
	if(c){ 
		connrbtree_del(c);
	}
}
