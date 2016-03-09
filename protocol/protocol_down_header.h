#ifndef __PROTOCOL_DOWN_HEADER_H_H_
#define __PROTOCOL_DOWN_HEADER_H_H_

struct zcl_down_cmd_identify_t;
void protocol_parse_identify(unsigned char * buf, unsigned short len, struct zcl_down_cmd_identify_t * command); 

#endif
