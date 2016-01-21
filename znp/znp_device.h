#ifndef __ZNP_DEVICE_H_H
#define __ZNP_DEVICE_H_H

#include "rbtree.h"

struct znp_ss_cluster{
	unsigned short clusterid;
	unsigned short zonetype;
};

struct znp_device{
	struct rb_node node;
	unsigned short shortaddr; 
	unsigned long long ieeeaddr;
	union {
		struct znp_ss_cluster ss_device;
	}device_data;
};

struct znp_device * znp_device_get(unsigned short shortaddr);
void znp_device_insert(struct znp_device * d);
void znp_device_del(unsigned short shortaddr);

#endif
