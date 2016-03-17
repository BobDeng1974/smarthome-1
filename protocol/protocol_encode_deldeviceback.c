#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "sqlitedb.h"
#include "bytebuffer.h"
#include "gateway.h"
#include "bussinessdata.h"
#include "zcl_datatype.h"
#include "zcl_ha.h"





#define DELDEVICEBACK 0x000A
unsigned int protocol_encode_deldeviceback(unsigned char * buf, unsigned long long ieeeaddr, unsigned int  serialid,unsigned char  result){

	unsigned char * p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0x0000);
	bytebuffer_writeword(&p,DELDEVICEBACK);
	bytebuffer_writedword(&p,serialid);
	bytebuffer_writebyte(&p,result);
	bytebuffer_writequadword(&p, ieeeaddr);


	unsigned int templen = p-buf;
	unsigned char *p1=buf+1;
	bytebuffer_writeword(&p1,templen+2);

	unsigned checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);
	return p-buf;
}

