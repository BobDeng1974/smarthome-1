#ifndef __CE_GATEWAY_H_H
#define __CE_GATEWAY_H_H

#include "list.h"
#include "cluster.h"
#include "mtZdo.h"

#define MAXNAMELEN 256
#define MAXEPCOUNT 64


struct endpoint{
	SimpleDescRspFormat_t simpledesc;
	struct list_head list;
};

struct device{
	char devicename[MAXNAMELEN];
	unsigned long long ieeeaddr;

	unsigned char epcurse;
	ActiveEpRspFormat_t activeep;
	struct list_head eplisthead;
	struct list_head list;
};

struct gateway{
	unsigned long long gatewayid;
	char gatewayname[MAXNAMELEN];
	unsigned short devicecount;
	unsigned char boxversion;
	unsigned char protocolversion;
	struct list_head head;
};

// endpoint 
struct endpoint * endpoint_create(SimpleDescRspFormat_t * simpledesc);
void endpoint_destroy(struct endpoint * ep);

// device
struct device * device_create(unsigned long long deviceieee);
void device_addendpoint(struct device * d, struct endpoint * ep);
unsigned char device_getepcount(struct device * d);
void device_destroy(struct device * d);
void device_setep(struct device * d, ActiveEpRspFormat_t * activeep);
static void device_increase(struct device * d){
	d->epcurse++;
}

// gateway
struct gateway * getgateway();
void gateway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion);
void gateway_adddevice(struct gateway * gw, struct device * d);
void gateway_deldevice(struct gateway * gw, struct device *d);
struct device * gateway_getdevice(struct gateway * gw, unsigned long long ieee);

#endif
