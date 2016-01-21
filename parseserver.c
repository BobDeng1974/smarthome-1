#include <stdio.h>
#include <sys/time.h>
#include "sq.h"
#include "bytebuffer.h"
#include "gateway.h"
#include "bussinessdata.h"
#include "zcl_datatype.h"
                                             
unsigned char encode_checksum(unsigned char * buf, unsigned int buflen);

#define  LOGIN     0x0003
unsigned int encode_login(struct gateway *gw, unsigned char *buf)
{
	unsigned char *p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0x0000);
	bytebuffer_writeword(&p,LOGIN);
	bytebuffer_writemac(&p,gw->gatewayid);
	bytebuffer_writeword(&p,gw->devicecount);

	struct list_head *pos, *n;
	struct device *temp;
	list_for_each_safe(pos, n,&gw->head)
	{
		temp=list_entry(pos, struct device, list);
		//		unsigned char deviceidlen = strlen(temp->deviceid);
		bytebuffer_writebyte(&p, 0X06);
		bytebuffer_writemac(&p, temp->deviceid);
		bytebuffer_writeword(&p,temp->company);
		bytebuffer_writebyte(&p,temp->devicestatus);
		unsigned char devicenamelen = strlen(temp->devicename);
		bytebuffer_writebyte(&p, devicenamelen);
		bytebuffer_writebytes(&p, temp->devicename,devicenamelen);
	}	
	unsigned int templen = p-buf;

	unsigned char *p1=buf+1;

	bytebuffer_writeword(&p1,templen+2);
	unsigned checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	unsigned int buflen = p-buf;

	return buflen;
}

#define  HEART     0x0002
unsigned int encode_heart(struct gateway *gw, unsigned char *buf)
{
	unsigned char *p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0x0D);
	bytebuffer_writeword(&p,HEART);
	bytebuffer_writemac(&p,gw->gatewayid);

	unsigned int templen = p-buf;
	unsigned char checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	unsigned int buflen = p-buf;


	return buflen;
} 

#define OPERBACK   0x0004
unsigned int encode_operback(struct gateway *gw, unsigned char *buf,unsigned short serialID, unsigned char result)
{
	unsigned char *p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0X12);
	bytebuffer_writeword(&p,OPERBACK);
	bytebuffer_writemac(&p,gw->gatewayid);
	bytebuffer_writedword(&p,serialID);
	bytebuffer_writebyte(&p,result);

	unsigned int templen = p-buf;
	unsigned char checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	return 0X12;

}
/*
 * Attribute Value       | Zone Type                  | Alarm1                                    Alarm2
 * 0x0000                | Standard CIE               | System Alarm                              -
 * 0x000d                | Motion sensor              | Intrusion indication Presence indication 
 * 0x0015                | Contact switch             | 1 st portal Open-Close                    2 nd portal Open-Close 
 * 0x0028                | Fire sensor                | Fire indication                           -
 * 0x002a                | Water sensor               | Water overflow indication                 -
 * 0x002b                | Gas sensor                 | CO indication                             Cooking indication 
 * 0x002c                | Personal emergency device  | Fall / Concussion                         Emergency button 
 * 0x002d                | Vibration / Movement sensor| Movement indication                       Vibration 
 * 0x010f                | Remote Control             | Panic                                     Emergency 
 * 0x0115                | Key fob                    | Panic                                     Emergency 
 * 0x021d                | Keypad                     | Panic                                     Emergency 
 * 0x0225                | Standard Warning Device    | -                                         -
 *                       | (see [B5] part 3)          | 
 * Other values < 0x7fff | Reserved                   | 
 * 0x8000-0xfffe         | Reserved for manufacturer  |
 *                       | specific types             |
 * 0xffff                | Invalid Zone Type          | 
 *---------------------------------------------------------------------------------------------------------------
 */
#define STANDARDCIE             0x0000
#define MOTIONSENSOR            0x000d
#define CONTACTSWITCH           0x0015
#define FIRESENSOR              0x0028
#define WATERSENSOR             0x002a
#define GASSENSOR               0x002b
#define PERSONALEMERGENCYDEVICE 0x002c
#define VIBRATIONMOVEMENTSENSOR 0x002d
#define REMOTECONTROL           0x010f
#define KEYFOB                  0x0115
#define KEYPAD                  0x021d
#define STANDARDWARNINGDEVICE   0x0225
#define INVALIDZONETYPE         0xffff

#define ALARM  0X0006
unsigned int encode_alarm(unsigned char *buf, struct zclzonechangenotification * notification) {
	unsigned char *p = buf;
	unsigned long long ctime;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0);
	bytebuffer_writeword(&p,ALARM);
	bytebuffer_writeword(&p,notification->clusterid);
	bytebuffer_writeword(&p,notification->zonetype);
	bytebuffer_writequadword(&p,notification->ieeeaddr);

	ctime = time(NULL);
	bytebuffer_writequadword(&p,ctime);
	switch(notification->zonetype){
		case CONTACTSWITCH:
			break;
	}
	bytebuffer_writebyte(&p,notification->zonechangenotification.zonestatus.alarm1);
	bytebuffer_writebyte(&p,notification->zonechangenotification.zonestatus.alarm2);

	unsigned int templen = p-buf;
	unsigned char * len = buf + 1;
	bytebuffer_writeword(&len, templen + 2);
	unsigned char checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	return templen + 2;
}

#define ADDDELDEVICE 0x0005
unsigned int encode_adddeldevice(unsigned char * buf, unsigned long long ieeeaddr, unsigned char add){
	unsigned char * p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,16); 
	bytebuffer_writeword(&p,ADDDELDEVICE); 
	bytebuffer_writebyte(&p,add); 
	bytebuffer_writequadword(&p, ieeeaddr);

	unsigned char checksum = encode_checksum(buf,p-buf);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	return p-buf;
}

unsigned char encode_checksum(unsigned char * buf, unsigned int buflen){
	unsigned char temp = buf[0];

	int i = 1;
	for(; i < buflen; i++){
		temp ^= buf[i];
	}

	return temp;
}
