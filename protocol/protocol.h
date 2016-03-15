#ifndef __PROTOCOL_H_H_H
#define __PROTOCOL_H_H_H

#define ILLEGAL          0
#define HALFPACK         255

#define LOGINFEEDBACK    0x8001
#define HEARTFEEDBACK    0x8002
#define REQDEVICELIST    0x8003
#define REQOPERATE       0x8004
#define REQSETDEVICENAME 0x8008
#define REQDELDEVICE     0x800A
#define DEVICEPROPERTIES 0x800B
#define DEVICE_IDENTIFY  0x800C
#define DEVICE_WARNING   0x800D
#define APP_LOGIN        0x800E
#define DEVICE_SETARM    0x800F

#define LOGIN 0x0001
#define HEART     0x0002
#define ADDDELDEVICE 0x0005
#define ALARM  0X0006
#define DEVICE_ARM_FEEDBACK 0x000F 

#define PROTOCOL_START_FLAG 0xCE
#define PROTOCOL_END_FLAG 0xCE

unsigned char protocol_checksum(unsigned char * buf, unsigned int buflen);
struct connection;
int protocol_check(struct connection * c, unsigned short * messageid);

#endif
