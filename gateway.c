#include <stdlib.h>
#include <string.h>
#include "gateway.h"

#define min(a,b) a>b?b:a

static struct gateway gatewayinstance;
// ---------------endpoint---------------
struct endpoint * endpoint_create(SimpleDescRspFormat_t * simpledesc){
	struct endpoint * endpoint = (struct endpoint *)malloc(sizeof(struct endpoint));
	memset(endpoint, 0, sizeof(struct endpoint));
	memcpy(&endpoint->simpledesc, simpledesc, sizeof(SimpleDescRspFormat_t));
	INIT_LIST_HEAD(&endpoint->list);

	return endpoint;
}

void endpoint_destroy(struct endpoint * ep){
	list_del(&ep->list);
	free(ep);
}
// ---------------endpoint---------------
//
// ---------------device---------------
struct device * device_create(unsigned long long deviceieee){
	struct device * device = (struct device *)malloc(sizeof(struct device)); 
	memset(device, 0, sizeof(struct device));
	INIT_LIST_HEAD(&device->list);
	INIT_LIST_HEAD(&device->eplisthead);
	device->ieeeaddr = deviceieee;

	return device; 
}

void device_addendpoint(struct device * d, struct endpoint * ep){ 
	list_add_tail(&ep->list, &d->eplisthead);
}

void device_setep(struct device * d, ActiveEpRspFormat_t * activeep){
	memcpy(&d->activeep, activeep, sizeof(ActiveEpRspFormat_t));
}

void device_destroy(struct device * d){ 
	struct endpoint * ep;
	struct list_head * pos, *n;
	list_for_each_safe(pos, n, &d->eplisthead){ 
		ep = list_entry(pos, struct endpoint, list); 
		endpoint_destroy(ep);
	}
	list_del(&d->list);
	free(d);
}

// ---------------device---------------
//
// ---------------gateway---------------

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
	device_destroy(d);
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

unsigned char device_getepcount(struct device * d){
	unsigned char count = 0;
	struct list_head * pos, *n;
	list_for_each_safe(pos, n, &d->eplisthead){ 
		count++;
	}

	return count;
}
// ---------------gateway---------------
