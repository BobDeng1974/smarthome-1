#ifndef __CE_GATEWAY_H_H
#define __CE_GATEWAY_H_H

#include "list.h"
#include "cluster.h"

#define MAXNAMELEN 256

struct clusterid{
	unsigned short clusterid;
	unsigned short groupid;
	unsigned char srcep;
	unsigned char dstep;
};

struct device{
	char devicename[MAXNAMELEN];
	unsigned long long ieeeaddr;
	struct clusterid clusterids[CLUSTERCOUNT]; //clusterids:w
	unsigned char clusteridcount;
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

struct device * device_create(unsigned long long deviceieee);

void device_addcluster(struct device *d,unsigned short groupid, unsigned short clusterid,unsigned char srcep, unsigned char dstep);

struct gateway * getgateway();
void gateway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion);
void gateway_adddevice(struct gateway * gw, struct device *d);
void gateway_deldevice(struct gateway * gw, struct device *d);

struct device * gateway_getdevice(struct gateway * gw, unsigned long long ieee);
struct clusterid * device_getclusterid(struct device * d, unsigned short clusterid);

#endif
