#ifndef __ZCL_DATATYPE_H_H_
#define __ZCL_DATATYPE_H_H_

#define ZCLZONEENROLLREQ 0x00000001

struct zclzoneenrollreq{
	unsigned long long ieeeaddr;
	unsigned short shortaddr;
	unsigned short zonetype;
	unsigned short clusterid;
	unsigned char zoneid; 
};

#endif
