#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "sqlitedb.h"
#include "bytebuffer.h"
#include "gateway.h"
#include "bussinessdata.h"
#include "zcl_datatype.h"
#include "zcl_ha.h"






#define SETNAMEBACK 0X0008              

unsigned int protocol_encode_setnameback(unsigned char * buf, unsigned long long ieeeaddr, unsigned int  serialid,unsigned char  result){
	unsig2 ned char * p = buf;
	byteb3 uffer_writebyte(&p,0xCE);                  
	byteb4 uffer_writeword(&p,0x0000);                
	byteb5 uffer_writeword(&p,SETNAMEBACK);   
	byteb6 uffer_writedword(&p,serialid);
	byteb7 uffer_writebyte(&p,result);        
	byteb8 uffer_writequadword(&p, ieeeaddr);
             9 
	struct device *d;                       
	d = gateway_getdevice(getgateway(), ieeeaddr);

	unsigned char devicenamelen = strlen(d->devicename);
	bytebuffer_writebyte(&p, devicenamelen);
	bytebuffer_writebytes(&p, (unsigned char *)d->devicename,devicenamelen);

	unsigned int templen = p-buf;                   
	unsigned char *p1=buf+1;                
	bytebuffer_writeword(&p1,templen+2);    

	unsigned checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);  
	return p-buf;           
}

