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
#define DEVICE_ACTIVE 128

struct simpledesc{
	SimpleDescRspFormat_t simpledesc; 
	unsigned short zonetype; // used for ss device
};

struct endpoint{
	struct simpledesc simpledesc;
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

	unsigned int endpoint_zonetype[8];

	unsigned char epcursor;
	ActiveEpRspFormat_t activeep;
	struct list_head eplisthead;
	struct list_head list;
};

struct gateway_endpoint_inout_clusterlist{
	unsigned char endpoint;
	unsigned char inclustercount;
	unsigned short inclusterlist[16];
	unsigned char outclustercount;
	unsigned short outclusterlist[16];
};

struct gateway{
	unsigned long long gatewayid;
	char gatewayname[MAXNAMELEN];
	unsigned short devicecount;
	unsigned char boxversion;
	unsigned char protocolversion;
	struct gateway_endpoint_inout_clusterlist endpoint_inout_clusterlist[3];
	struct list_head head;
};

// endpoint 
struct endpoint * endpoint_create(struct simpledesc * simpledesc);
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

void device_set_zonetype(struct device *d, unsigned char endpoint, unsigned short zonetype);

int device_get_index(struct device *d, unsigned char endpoint);
int device_has_enpoint(struct device * d, unsigned char endpoint);

struct endpoint * device_get_endpoint(struct device * d, unsigned char endpoint);

// gateway
struct gateway * getgateway();
void gateway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion);
void gateway_adddevice(struct gateway * gw, struct device * d);
void gateway_deldevice(struct gateway * gw, struct device *d);
struct device * gateway_getdevice(struct gateway * gw, unsigned long long ieee);


//unsigned char gateway_get_outcluster_endpoint(unsigned short clusterid);
int gateway_update_device_networkaddr(unsigned long long ieee, unsigned short shortaddr);
//struct endpoint * gateway_get_endpoint_incluster(unsigned long long ieee, unsigned short clusterid);

#endif
