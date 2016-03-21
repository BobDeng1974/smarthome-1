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
#include "bytebuffer.h" 
#include "protocol_cmd_header.h"
#include "protocol_cmdtype.h"
#include "protocol_datatype.h"
#include "zcl_down_cmd.h"
#include "sqlitedb.h"
#include "zcl_ss.h"

extern int g_main_to_znp_write_fd;
extern struct connection * g_serverconn;

void event_send_warning(struct endpoint * wd_ep, unsigned long long warning_device_ieee, unsigned char cmdid,unsigned char warning_mode);

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
			int serverfd = connlist_getserverfd();
			if(serverfd != -1){
				unsigned int buflen = protocol_encode_login(buf); 
				sendnonblocking(serverfd, buf, buflen);
			}
		}
	}else if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER)){ 
		c->timestamp = time(NULL);
		connection_put(c, buf, buflen); 

		for(;;){ // in case of receive two packets one time.
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
				case DEVICE_SET_NAME:
					{ 
						struct protocol_datatype_set_device_name set_device_name;
						protocol_parse_set_device_name(buffer, messagelen, &set_device_name);
						unsigned char result = 0;
						if(getgateway()->gatewayid == set_device_name.ieee){ 
							result = (sqlitedb_update_gatewayname(set_device_name.ieee, set_device_name.name) == 0)?1:0;
							if(result){
								struct gateway * gw = getgateway();
								if(gw){
									memcpy(gw->gatewayname,set_device_name.name, strlen(set_device_name.name));
									gw->gatewayname[strlen(set_device_name.name)] = 0;
								}
							}
						}else{
							result = (sqlitedb_update_devicename(set_device_name.ieee, set_device_name.name) == 0)?1:0;
							if(result){
								struct device * d = gateway_getdevice(getgateway(), set_device_name.ieee);
								if(d){
									memcpy(d->devicename,set_device_name.name, strlen(set_device_name.name));
									d->devicename[strlen(set_device_name.name)] = 0;
								}
							}
							
						}
						unsigned char buf[512] = {0};
						unsigned int len = protocol_encode_set_name_feedback(buf, &set_device_name, result);

						sendnonblocking(fd, buf, len);
					}
					break;
				case DEVICE_DEL:
					{ 
						unsigned char result = 0;
						struct protocol_datatype_del_device del_device;
						protocol_parse_del_device(buffer, messagelen, &del_device);
						struct device * d = gateway_getdevice(getgateway(), del_device.ieee);
						if(d){
							device_set_status(d, DEVICE_APP_DEL);
							result = 1;
						}
						unsigned char buf[128] = {0}; 
						unsigned int len = protocol_encode_del_device_feedback(buf, &del_device, result);

						sendnonblocking(fd, buf,len);
					}
					break;
				case DEVICE_ATTR:{
							     struct protocol_datatype_get_device_attr get_attr;
							     protocol_parse_get_device_attr(buffer, messagelen, &get_attr); 
							     unsigned char buf[512] = {0};
							     unsigned int len = protocol_encode_deviceattr(buf, &get_attr);

							     sendnonblocking(fd, buf, len); 
						     }
						     break;
				case DEVICE_IDENTIFY:
						     {
							     struct protocol_cmdtype_identify_ieee_cmd identify_ieee_cmd;
							     identify_ieee_cmd.cmdid = PROTOCOL_IDENTIFY;
							     identify_ieee_cmd.identify_ieee.ieee = protocol_parse_identify(buffer, messagelen,&identify_ieee_cmd.identify_ieee.identify); 
							     int n = sendnonblocking(g_main_to_znp_write_fd, &identify_ieee_cmd, sizeof(struct protocol_cmdtype_identify_ieee_cmd));
							     fprintf(stdout, "send identify %d %d\n", n, sizeof(struct protocol_cmdtype_identify_ieee_cmd));
						     }
						     break;
				case DEVICE_WARNING:
						     {
							     struct protocol_cmdtype_warning_ieee_cmd warning_ieee_cmd;
							     warning_ieee_cmd.cmdid = PROTOCOL_WARNING;
							     warning_ieee_cmd.warning_ieee.ieee = protocol_parse_warning(buffer, messagelen, &warning_ieee_cmd.warning_ieee.warning);

							     sendnonblocking(g_main_to_znp_write_fd, &warning_ieee_cmd, sizeof(struct protocol_cmdtype_warning_ieee_cmd)); 
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
							     result = (result == 0)?1:0;
							     if(result == 1){ 
								     struct endpoint * ep = gateway_get_endpoint(ieee, endpoint);
								     memcpy(&ep->simpledesc.arm, &arm, sizeof(struct protocol_cmdtype_arm));
							     }
							     unsigned char buf[128] = {0};
							     unsigned int buflen = protocol_encode_arm_feedback(buf, ieee, result);

							     sendnonblocking(fd, buf, buflen);
						     }
						     break;
				case DEVICE_ONOFF:
						     {
							     struct protocol_cmdtype_onoff_ieee_cmd onoff;
							     onoff.cmdid = PROTOCOL_ONOFF;
							     protocol_parse_onoff(buffer, messagelen, &onoff.onoff_ieee);
							     sendnonblocking(g_main_to_znp_write_fd, &onoff, sizeof(struct protocol_cmdtype_onoff_ieee_cmd));
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
				struct device * d = gateway_getdevice(getgateway(), req.ieeeaddr);
				if(d->status & DEVICE_APP_DEL){
					return;
				}
				buflen = protocol_encode_add_del_device(buf, req.ieeeaddr, 1);

				broadcast(buf, buflen);
			}
			break;
		case ZCLZONECHANGENOTIFICATION:
			{
				struct zclzonechangenotification req;
				readnonblocking(fd, &req, sizeof(struct zclzonechangenotification));
				struct device * d = gateway_getdevice(getgateway(), req.ieeeaddr);
				if(d->status & DEVICE_APP_DEL){
					return;
				}
				fprintf(stdout, "********event recv znp notification %llX \n", req.ieeeaddr);
				time_t curtime = time(NULL);
				struct tm * tm = localtime(&curtime);
				unsigned char hour = (unsigned char )tm->tm_hour;
				unsigned char minute = (unsigned char )tm->tm_min; 
				struct endpoint * wd_ep = gateway_get_warning_device_endpoint();
				struct device * wd_device = gateway_get_warning_device();
				struct device * sensor = gateway_getdevice(getgateway(), req.ieeeaddr);
				struct endpoint * ep = gateway_get_endpoint(req.ieeeaddr, req.endpoint); 
				if(ep){
					if(endpoint_check_arm(ep, hour, minute)){
						switch(ep->simpledesc.zonetype){
							case SS_IAS_ZONE_TYPE_CONTACT_SWITCH:
								{

									if(req.zonechangenotification.zonestatus.alarm1 || req.zonechangenotification.zonestatus.alarm2){
										fprintf(stdout, "------start alarm\n");
										if((sensor->status & DEVICE_SS_SEND_ALARM_NOTIFICATION) == 0){
											sensor->status |= DEVICE_SS_SEND_ALARM_NOTIFICATION;
											sensor->status &= ~DEVICE_SS_SEND_NO_ALARM_NOTIFICATION;
											if(wd_device){
												event_send_warning(wd_ep, wd_device->ieeeaddr,PROTOCOL_WARNING, SS_IAS_START_WARNING_WARNING_MODE_BURGLAR);
											}
											buflen = protocol_encode_alarm(buf, &req);
											broadcast(buf, buflen);
										}

									}else{
										if((sensor->status & DEVICE_SS_SEND_NO_ALARM_NOTIFICATION)==0){ 
											fprintf(stdout, "------stop alarm\n");
											sensor->status |= DEVICE_SS_SEND_NO_ALARM_NOTIFICATION;
											sensor->status &= ~DEVICE_SS_SEND_ALARM_NOTIFICATION;

											if(wd_device){ 
												event_send_warning(wd_ep, wd_device->ieeeaddr,PROTOCOL_WARNING, SS_IAS_START_WARNING_WARNING_MODE_STOP);
											}
											buflen = protocol_encode_alarm(buf, &req);
											broadcast(buf, buflen);
										}
									}
								}
								break;
							case SS_IAS_ZONE_TYPE_GAS_SENSOR:
								{
									if(req.zonechangenotification.zonestatus.alarm1 || req.zonechangenotification.zonestatus.alarm2){
										fprintf(stdout, "------start alarm\n");
										if(sensor && ((sensor->status & DEVICE_SS_SEND_ALARM_NOTIFICATION) == 0)){
											sensor->status |= DEVICE_SS_SEND_ALARM_NOTIFICATION;
											sensor->status &= ~DEVICE_SS_SEND_NO_ALARM_NOTIFICATION;
											if(wd_device){
												event_send_warning(wd_ep, wd_device->ieeeaddr,PROTOCOL_WARNING, SS_IAS_START_WARNING_WARNING_MODE_FIRE);
											}
											buflen = protocol_encode_alarm(buf, &req);
											broadcast(buf, buflen);
										}

									}else{
										if(sensor && ((sensor->status & DEVICE_SS_SEND_NO_ALARM_NOTIFICATION)==0)){ 
											fprintf(stdout, "------stop alarm\n");
											sensor->status |= DEVICE_SS_SEND_NO_ALARM_NOTIFICATION;
											sensor->status &= ~DEVICE_SS_SEND_ALARM_NOTIFICATION;

											if(wd_device){
												event_send_warning(wd_ep, wd_device->ieeeaddr,PROTOCOL_WARNING, SS_IAS_START_WARNING_WARNING_MODE_STOP);
											}
											buflen = protocol_encode_alarm(buf, &req);
											broadcast(buf, buflen);
										}
									}
								}
								break;
						}
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


void event_send_warning(struct endpoint * wd_ep, unsigned long long warning_device_ieee, unsigned char cmdid,unsigned char warning_mode){
	struct protocol_cmdtype_warning_ieee_cmd warning_ieee_cmd;
	warning_ieee_cmd.cmdid = cmdid;
	warning_ieee_cmd.warning_ieee.ieee = warning_device_ieee;
	warning_ieee_cmd.warning_ieee.warning.endpoint = wd_ep->simpledesc.simpledesc.Endpoint;
	warning_ieee_cmd.warning_ieee.warning.start_warning.warningmessage.warningbits.warnStrobe = SS_IAS_START_WARNING_STROBE_USE_STPOBE_IN_PARALLEL_TO_WARNING;
	warning_ieee_cmd.warning_ieee.warning.start_warning.warningmessage.warningbits.warnSirenLevel = SS_IAS_SIREN_LEVEL_MEDIUM_LEVEL_SOUND;
	warning_ieee_cmd.warning_ieee.warning.start_warning.warningDuration = 0xffff;
	warning_ieee_cmd.warning_ieee.warning.start_warning.strobeDutyCycle = 1; // magic number
	warning_ieee_cmd.warning_ieee.warning.start_warning.strobeLevel = SS_IAS_STROBE_LEVEL_MEDIUM_LEVEL_STROBE;
	warning_ieee_cmd.warning_ieee.warning.start_warning.warningmessage.warningbits.warnMode = warning_mode;

	sendnonblocking(g_main_to_znp_write_fd, &warning_ieee_cmd, sizeof(struct protocol_cmdtype_warning_ieee_cmd)); 
}

