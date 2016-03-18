#ifndef __PROTOCOL_CMD_HEADER_H_H_
#define __PROTOCOL_CMD_HEADER_H_H_ 
#include "protocol_cmdtype.h"


unsigned long long protocol_parse_arm(unsigned char * buf, unsigned short len, struct protocol_cmdtype_arm * arm, unsigned int * serialnum, unsigned char * endpoint); 
unsigned long long protocol_parse_identify(unsigned char * buf, unsigned short len, struct protocol_cmdtype_identify * identify); 
unsigned long long protocol_parse_warning(unsigned char * buf, unsigned short len, struct protocol_cmdtype_warning * warning);
struct protocol_datatype_del_device;
void protocol_parse_del_device( unsigned char * buf, unsigned short len, struct protocol_datatype_del_device * del);
struct protocol_datatype_get_device_attr;
void protocol_parse_get_device_attr(unsigned char * buf, unsigned short len, struct protocol_datatype_get_device_attr * get_attr);
struct protocol_datatype_set_device_name;
void protocol_parse_set_device_name( unsigned char * buf, unsigned short len, struct protocol_datatype_set_device_name * name);

unsigned int protocol_encode_add_del_device(unsigned char * buf, unsigned long long ieeeaddr, unsigned char add);
struct zclzonechangenotification;
unsigned int protocol_encode_alarm(unsigned char *buf, struct zclzonechangenotification * notification);
unsigned int protocol_encode_heart(unsigned char *buf);
unsigned int protocol_encode_login(unsigned char *buf);
unsigned int protocol_encode_arm_feedback(unsigned char * buf, unsigned long long ieee, unsigned char result);
unsigned int protocol_encode_add_del_device(unsigned char * buf, unsigned long long ieeeaddr, unsigned char add);
unsigned int protocol_encode_del_device_feedback(unsigned char * buf, struct protocol_datatype_del_device * del_device,unsigned char  result);
unsigned int protocol_encode_deviceattr(unsigned char * buf, struct protocol_datatype_get_device_attr * get_attr);
#endif
