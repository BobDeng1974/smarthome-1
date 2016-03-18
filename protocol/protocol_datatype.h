#ifndef __PROTOCOL_DATATYPE_H_H
#define __PROTOCOL_DATATYPE_H_H

struct protocol_datatype_del_device{
	unsigned long long ieee;
	unsigned int serialnum;
};

struct protocol_datatype_get_device_attr{
	unsigned long long ieee;
	unsigned int serialnum;
	unsigned char endpoint;
};

struct protocol_datatype_set_device_name{
	unsigned long long ieee;
	unsigned int serialnum;
	char name[256];
};

#endif
