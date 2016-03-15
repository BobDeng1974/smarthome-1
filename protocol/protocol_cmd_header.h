#ifndef __PROTOCOL_CMD_HEADER_H_H_
#define __PROTOCOL_CMD_HEADER_H_H_ 
#include "protocol_cmdtype.h"


unsigned long long protocol_parse_arm(unsigned char * buf, unsigned short len, struct protocol_cmdtype_arm * arm, unsigned int * serialnum, unsigned char * endpoint);

unsigned long long protocol_parse_identify(unsigned char * buf, unsigned short len, struct protocol_cmdtype_identify * identify);

unsigned long long protocol_parse_warning(unsigned char * buf, unsigned short len, struct protocol_cmdtype_warning * warning);


unsigned int protocol_encode_add_del_device(unsigned char * buf, unsigned long long ieeeaddr, unsigned char add);
struct zclzonechangenotification;
unsigned int protocol_encode_alarm(unsigned char *buf, struct zclzonechangenotification * notification);
unsigned int protocol_encode_heart(unsigned char *buf);
unsigned int protocol_encode_login(unsigned char *buf);
unsigned int protocol_encode_arm_feedback(unsigned char * buf, unsigned long long ieee, unsigned char result);
#endif
