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
#include "bytebuffer.h" 
#include "protocol_cmd_header.h"
#include "protocol_cmdtype.h"
#include "zcl_down_cmd.h"
#include "sqlitedb.h"
#include "zcl_ss.h"

extern int g_main_to_znp_write_fd;
extern struct connection * g_serverconn;

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
			hbuflen = protocol_encode_heart(heart_buf);
			broadcast(heart_buf, hbuflen);
		}
		if( _check_command(buf, buflen, CESEND[0])){
			unsigned char buf[2048] = {0}; 
			unsigned int buflen = protocol_encode_login(buf); 
			sendnonblocking(connlist_getserverfd(), buf, buflen);
		}
	}else if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER)){
		connection_put(c, buf, buflen); 

		for(;;){
			unsigned short messageid = 0;
			int messagelen = protocol_check(c, &messageid);
			if(messageid == ILLEGAL || messageid == HALFPACK){
				break;
			}
			unsigned char buffer[1024] = {0};
			connection_get(c,buffer, messagelen);
			toolkit_printbytes(buffer, messagelen);
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
				case DEVICEPROPERTIES:{
							      //						      unsigned int serialid;
							      //						      unsigned long long IEEE;
							      //						      unsigned char *p = buffer;
							      //
							      //						      serialid = bytebuffer_getdword(p+5);
							      //						      IEEE = bytebuffer_getquadword(p+9);
							      //						      unsigned char devicebuf[2048] = {0};
							      //						      unsigned int buflen = encode_deviceattr(devicebuf, IEEE, serialid);
							      //						      sendnonblocking(connlist_getserverfd(), devicebuf, buflen);
						      }
						      break;
				case DEVICE_IDENTIFY:
						      {
							      struct protocol_cmdtype_identify_ieee_cmd identify_ieee_cmd;
							      identify_ieee_cmd.cmdid = PROTOCOL_IDENTIFY;
							      identify_ieee_cmd.identify_ieee.ieee = protocol_parse_identify(buffer, messagelen,&identify_ieee_cmd.identify_ieee.identify); 
							      sendnonblocking(g_main_to_znp_write_fd, &identify_ieee_cmd, sizeof(struct protocol_cmdtype_identify_ieee_cmd));
							      //		      zcl_down_cmd_identify(ieee,&identify);

						      }
						      break;
				case DEVICE_WARNING:
						      {
							      struct protocol_cmdtype_warning_ieee_cmd warning_ieee_cmd;
							      warning_ieee_cmd.cmdid = PROTOCOL_WARNING;
							      warning_ieee_cmd.warning_ieee.ieee = protocol_parse_warning(buffer, messagelen, &warning_ieee_cmd.warning_ieee.warning);
							      sendnonblocking(g_main_to_znp_write_fd, &warning_ieee_cmd, sizeof(struct protocol_cmdtype_warning_ieee_cmd)); 

							      //				      zcl_down_cmd_warning(ieee, &cmd);
						      }
						      break;
				case APP_LOGIN:
						      {
							      unsigned char buf[2048] = {0}; 
							      unsigned int buflen = protocol_encode_login(buf); 
							      sendnonblocking(fd, buf, buflen);
						      }
						      break;
				case DEVICE_SETARM:
						      {
							      struct protocol_cmdtype_arm arm;
							      unsigned int serialnum = 0;
							      unsigned char endpoint = 0;
							      unsigned long long ieee = protocol_parse_arm(buffer, messagelen, &arm, &serialnum, &endpoint);
							      unsigned char result = (unsigned char)sqlitedb_update_device_arm(ieee, endpoint, &arm);
							      result = (result == 0)?0:1;
							      if(result == 0){ 
								      struct endpoint * ep = gateway_get_endpoint(ieee, endpoint);
								      memcpy(&ep->simpledesc.arm, &arm, sizeof(struct protocol_cmdtype_arm));
							      }
							      unsigned char buf[128] = {0};
							      unsigned int buflen = protocol_encode_arm_feedback(buf, ieee, result);
						      }
						      break;

				case ILLEGAL:
						      break;
			}
		}
	}
}

void event_recvznp(struct eventhub * hub, int fd){ 
	unsigned char buf[128] = {0};
	unsigned int buflen = 0;

	int znpdatatype = 0;
	readnonblocking(fd, &znpdatatype, sizeof(int));
	switch(znpdatatype){
		case ZCLZONEENROLLREQ: 
			{ 
				struct zclzoneenrollreq req;
				readnonblocking(fd, &req, sizeof(struct zclzoneenrollreq));
				fprintf(stdout, "********event recv znp enroll ieee %llX \n", req.ieeeaddr);
				buflen = protocol_encode_add_del_device(buf, req.ieeeaddr, 1);

				broadcast(buf, buflen);
			}
			break;
		case ZCLZONECHANGENOTIFICATION:
			{
				struct zclzonechangenotification req;
				readnonblocking(fd, &req, sizeof(struct zclzonechangenotification));
				fprintf(stdout, "********event recv znp notification %llX \n", req.ieeeaddr);
				time_t curtime = time(NULL);
				struct tm * tm = localtime(&curtime);
				unsigned char hour = (unsigned char )tm->tm_hour;
				unsigned char minute = (unsigned char )tm->tm_min; 
				struct endpoint * ep = gateway_get_endpoint(req.ieeeaddr, req.endpoint); 
				if(endpoint_check_arm(ep, hour, minute)){
					switch(ep->simpledesc.zonetype){
						case SS_IAS_ZONE_TYPE_CONTACT_SWITCH:
							{
								struct protocol_cmdtype_warning cmd;
								memset(&cmd, 0, sizeof(struct protocol_cmdtype_warning));
								struct endpoint * wd_ep = gateway_get_warning_device();
								cmd.endpoint = wd_ep->simpledesc.simpledesc.Endpoint;
								cmd.start_warning.warningmessage.warningbits.warnStrobe = SS_IAS_START_WARNING_STROBE_USE_STPOBE_IN_PARALLEL_TO_WARNING;
								cmd.start_warning.warningmessage.warningbits.warnSirenLevel = SS_IAS_SIREN_LEVEL_VERY_HIGH_LEVEL_SOUND;
								cmd.start_warning.warningDuration = 0xffff;
								cmd.start_warning.strobeDutyCycle = 1; // magic number
								cmd.start_warning.strobeLevel = SS_IAS_STROBE_LEVEL_VERY_HIGH_LEVEL_STROBE;
								fprintf(stdout, "alarm 1 %d alarm2 %d\n", req.zonechangenotification.zonestatus.alarm1, req.zonechangenotification.zonestatus.alarm2);

								struct device * contact_switch = gateway_getdevice(getgateway(), req.ieeeaddr);
								if(req.zonechangenotification.zonestatus.alarm1 || req.zonechangenotification.zonestatus.alarm2){
									if(!device_check_status(contact_switch,DEVICE_SS_SEND_ALARM_NOTIFICATION)){
										device_set_status(contact_switch, DEVICE_SS_SEND_ALARM_NOTIFICATION);
										device_set_status(contact_switch, ~DEVICE_SS_SEND_NO_ALARM_NOTIFICATION);
										cmd.start_warning.warningmessage.warningbits.warnMode = SS_IAS_START_WARNING_WARNING_MODE_BURGLAR;

										buflen = protocol_encode_alarm(buf, &req);
										broadcast(buf, buflen);
										zcl_down_cmd_warning(req.ieeeaddr, &cmd);
									}

								}else{
									if(!device_check_status(contact_switch, DEVICE_SS_SEND_NO_ALARM_NOTIFICATION)){
										device_set_status(contact_switch, DEVICE_SS_SEND_NO_ALARM_NOTIFICATION);
										device_set_status(contact_switch, ~DEVICE_SS_SEND_ALARM_NOTIFICATION);
										cmd.start_warning.warningmessage.warningbits.warnMode = SS_IAS_START_WARNING_WARNING_MODE_STOP;

										buflen = protocol_encode_alarm(buf, &req);
										broadcast(buf, buflen);
										zcl_down_cmd_warning(req.ieeeaddr, &cmd);
									}
								}
							}
							break;
					}
				}

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
