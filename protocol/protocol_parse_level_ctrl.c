#include "bytebuffer.h"
#include "protocol_cmdtype.h"

//设备Level ctrl
//-------
//标识位 1 byte "0xCE"
//消息长度 2 bytes 标识位(含)字节长度长度
//消息ID 2 bytes 0x80 0x11
//SerialNum 4 bytes 序列号
//DeviceID 8 bytes 设备ID
//Endpoint 1 byte 
//Level 1 byte 达到指定的level
//TransTime 2 bytes 达到指定level所用的时间(单位s)
//校验码 (从开头到校验位前一位的^)
//标识位 1 byte
//-------
//
//
void protocol_parse_level_ctrl(unsigned char * buf, unsigned short len, struct protocol_cmdtype_level_ctrl_ieee * level_ctrl_ieee){ 
	const unsigned char * p = buf;
	bytebuffer_skipbytes(&p, 9);
	bytebuffer_readquadword(&p, &level_ctrl_ieee->ieee);
	bytebuffer_readbyte(&p, &level_ctrl_ieee->endpoint);
	bytebuffer_readbyte(&p, &level_ctrl_ieee->level);
	bytebuffer_readword(&p, &level_ctrl_ieee->trans_time);
}
