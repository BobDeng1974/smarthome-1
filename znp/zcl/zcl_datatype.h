#ifndef __ZCL_DATATYPE_H_H_
#define __ZCL_DATATYPE_H_H_

#define ZCLZONEENROLLREQ    0x00000001
#define ZCLZONECHANGENOTIFICATION 0x00000002

struct zclzoneenrollreq{
	unsigned long long ieeeaddr;
	unsigned short shortaddr;
	unsigned short zonetype;
	unsigned short clusterid;
	unsigned char zoneid; 
};

struct zclzonechangenotification{
	unsigned short zonestatus;
	unsigned char extendedstatus;
	unsigned char zoneid;
	unsigned short delay;
};

#endif
