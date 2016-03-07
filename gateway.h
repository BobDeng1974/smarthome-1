#ifndef __CE_GATEWAY_H_H
#define __CE_GATEWAY_H_H

#include "list.h"
#include "cluster.h"
#include "mtZdo.h"

#define MAXNAMELEN 256
#define MAXEPCOUNT 64

#define DEVICE_NULL 0
#define DEVICE_APP_DEL 1
#define DEVICE_SEND_ATTR 2
#define DEVICE_GET_ATTR 4
#define DEVICE_SEND_ACTIVEEP 8
#define DEVICE_SEND_SIMPLEDESC 16
#define DEVICE_GET_ACTIVEEP 32
#define DEVICE_GET_SIMPLEDESC 64



struct endpoint{
	SimpleDescRspFormat_t simpledesc;
	struct list_head list;
};

struct device{
	unsigned long long ieeeaddr;
	char devicename[MAXNAMELEN];
	unsigned int status;
	unsigned char zclversion;
	unsigned char applicationversion;
	unsigned char stackversion;
	unsigned char hwversion;
	char manufacturername[33];
	char modelidentifier[33];
	char datecode[17];
	unsigned char powersource;

	unsigned char epcursor;
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
struct device * device_create2(unsigned long long ieee, char * name, unsigned char status,
		unsigned char zclversion, unsigned char applicationversion, 
		unsigned char stackversion, unsigned char hwversion,
		char * manufacturername, char * modelidentifier, char * datecode);

void device_addendpoint(struct device * d, struct endpoint * ep);
unsigned char device_getepcount(struct device * d);
void device_destroy(struct device * d);
void device_setep(struct device * d, ActiveEpRspFormat_t * activeep);
static void device_increase(struct device * d){
	d->epcursor++;
}

void device_set_status(struct device * d, unsigned int status);

static int device_check_status(struct device * d, unsigned int status){
	return d->status & status;
}

// gateway
struct gateway * getgateway();
void gateway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion);
void gateway_adddevice(struct gateway * gw, struct device * d);
void gateway_deldevice(struct gateway * gw, struct device *d);
struct device * gateway_getdevice(struct gateway * gw, unsigned long long ieee);

#endif
