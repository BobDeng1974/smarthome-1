#include <stdlib.h>
#include <string.h>
#include "gateway.h"

#define min(a,b) a>b?b:a

struct device * device_create(unsigned long long deviceieee){
	struct device * device = (struct device *)malloc(sizeof(struct device)); 
	memset(device, 0, sizeof(struct device));
	INIT_LIST_HEAD(&device->list);
	device->ieeeaddr = deviceieee;

	return device; 
}

void device_addcluster(struct device *d, unsigned short groupid, unsigned short clusterid, unsigned char srcep, unsigned char dstep){
	d->clusterids[d->clusteridcount].clusterid = clusterid;
	d->clusterids[d->clusteridcount].groupid = groupid;
	d->clusterids[d->clusteridcount].srcep = srcep;
	d->clusterids[d->clusteridcount].dstep = dstep;
	d->clusteridcount++;
}

static struct gateway gatewayinstance;

struct gateway * getgateway(){
	return &gatewayinstance;
}

void gateway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion){ 
	gw->gatewayid = gatewayid;
	memset(gw->gatewayname, 0, MAXNAMELEN);
	memcpy(gw->gatewayname, gatewayname, min(strlen(gatewayname), MAXNAMELEN-1));
	gw->boxversion = boxversion;
	gw->protocolversion = protocolversion;
	INIT_LIST_HEAD(&gw->head);
}

void gateway_adddevice(struct gateway * gw, struct device * d){
	list_add_tail(&d->list, &gw->head);
}

void gateway_deldevice(struct gateway * gw, struct device *d){
	list_del(&d->list);
	free(d);
}

struct device * gateway_getdevice(struct gateway * gw, unsigned long long ieee){
	struct device * d;
	struct list_head * pos, *n;
	list_for_each_safe(pos, n, &gw->head){ 
		d = list_entry(pos, struct device, list); 
		if(d->ieeeaddr == ieee){
			return d;
		}
	}

	return NULL;
}

struct clusterid * device_getclusterid(struct device * d, unsigned short clusterid){
	unsigned char i = 0;
	for(i = 0; i < d->clusteridcount; i++){
		if(d->clusterids[i].clusterid == clusterid){
			return &d->clusterids[i];
		}
	}

	return NULL;
}
