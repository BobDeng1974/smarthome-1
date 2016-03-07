#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
#include "innercmd.h"
#include "parseserver.h"


extern struct connection * g_serverconn;

void event_accept(int fd){
	struct connection * c = freeconnlist_getconn();
	connection_init(c, fd, CONNSOCKETCLIENT);
	connrbtree_insert(c);

	unsigned char buf[2048] = {0}; 
	unsigned int buflen = encode_login(getgateway(), buf); 
	sendnonblocking(fd, buf, buflen);
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
//	fprintf(stdout, "recv ");
//	toolkit_printbytes(buf, buflen);
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
			broadcast(heart_buf, hbuflen);
		}
		if( _check_command(buf, buflen, CESEND[0])){
			unsigned char buf[2048] = {0}; 
			unsigned int buflen = encode_login(getgateway(), buf); 
			sendnonblocking(connlist_getserverfd(), buf, buflen);
		}
	}else if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER)){
		connection_put(c, buf, buflen); 
		unsigned short messageid = 0;
		int messagelen = protocol_check(c, &messageid);
		unsigned char buffer[1024] = {0};
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
            case DEVICEPROPERTIES:
			unsigned int serialid;
			unsigned long long IEEE;
			unsigned char *p = buffer;

			serialid = bytebuffer_getdword(p+5);
			IEEE = bytebuffer_getquadword(p+9);
			unsigned char devicebuf[2048] = {0};
			unsigned int buflen = encode_deviceattr(devicebuf, IEEE, serialid);
			sendnonblocking(connlist_getserverfd(), devicebuf, buflen);
			case ILLEGAL:
				break;
		}
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
				struct device *d = gateway_getdevice(getgateway(),req.ieeeaddr);
				if(d == NULL){ 
					d = device_create(req.ieeeaddr);
					gateway_adddevice(getgateway(), d);
				}
				unsigned char buf[64] = {0};
				unsigned int buflen = encode_adddeldevice(buf, req.ieeeaddr, 1);
				broadcast(buf, buflen);
			}
			break;
		case ZCLZONECHANGENOTIFICATION:
			{
				struct zclzonechangenotification req;
				readnonblocking(fd, &req, sizeof(struct zclzonechangenotification));
				unsigned char buf[128] = {0};
				unsigned int buflen = encode_alarm(buf, &req);
				broadcast(buf, buflen);
			}
			break;
	}
}

void event_close(int fd){
	struct connection * c = connrbtree_getconn(fd);
	if(c){ 
		connrbtree_del(c);
	}
}
