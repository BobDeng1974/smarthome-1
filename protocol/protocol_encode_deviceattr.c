#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "sqlitedb.h"
#include "bytebuffer.h"
#include "gateway.h"
#include "bussinessdata.h"
#include "zcl_datatype.h"
#include "zcl_ha.h"





#define DEVICEATTRIBUTE 0x000B
unsigned int protocol_encode_deviceattr(unsigned char * buf, unsigned long long ieeeaddr, unsigned int  serialid){
	unsign2 ed char * p = buf;
	bytebu3 ffer_writebyte(&p,0xCE);                
	bytebu4 ffer_writeword(&p,0x0000);              
	bytebu5 ffer_writeword(&p,ADDDELDEVICE);
	bytebu6 ffer_writedword(&p,serialid);
	bytebu7 ffer_writequadword(&p, ieeeaddr);       
	struct8  device *d;                             
	d = ga9 teway_getdevice(getgateway(), ieeeaddr);
	bytebuffer_writebyte(&p,&d->zclversion);
	bytebuffer_writebyte(&p,&d->applicationversion);
	bytebuffer_writebyte(&p,&d->stackversion);
	bytebuffer_writebyte(&p,&d->hwversion);

	unsigned char manufacturernamelen = strlen(d->manufacturername);
	bytebuffer_writebyte(&p, manufacturernamelen);
	bytebuffer_writebytes(&p, (unsigned char *)d->manufacturername,manufacturernamelen);

	unsigned char modelidentifierlen = strlen(d->modelidentifier);
	bytebuffer_writebyte(&p, modelidentifierlen);
	bytebuffer_writebytes(&p, (unsigned char *)d->modelidentifier,modelidentifierlen);

	unsigned char datecodelen = strlen(d->datecode);
	bytebuffer_writebyte(&p, datecodelen);        
	bytebuffer_writebytes(&p, (unsigned char *)d->datecode,datecodelen);

	bytebuffer_writebyte(&p, d->powersource);

	unsigned char deviceepcount = device_getepcount(d);
	bytebuffer_writebyte(&p, deviceepcount);

	struct list_head *pos, *n;
	struct endpoint *e;
	list_for_each_safe(pos, n,&d->list)
	{
		e=list_entry(pos, struct endpoint, list);
		bytebuffer_writebyte(&p,e->simpledesc.simpledesc.Endpoint);
		bytebuffer_writeword(&p,e->simpledesc.simpledesc.ProfileID);
		bytebuffer_writeword(&p,e->simpledesc.simpledesc.DeviceID);
		bytebuffer_writebyte(&p,e->simpledesc.simpledesc.DeviceVersion);
	}
	unsigned int templen = p-buf;
	unsigned char *p1=buf+1;
	bytebuffer_writeword(&p1,templen+2);

	unsigned checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);
	return p-buf;
}

