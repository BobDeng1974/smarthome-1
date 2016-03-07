#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gateway.h"
#include "sqlitedb.h"

#define min(a,b) a>b?b:a

static struct gateway gatewayinstance;
// ---------------endpoint---------------
struct endpoint * endpoint_create(struct simpledesc * simpledesc){
	struct endpoint * endpoint = (struct endpoint *)malloc(sizeof(struct endpoint));
	memset(endpoint, 0, sizeof(struct endpoint));
	memcpy(&endpoint->simpledesc, simpledesc, sizeof(struct simpledesc));
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
	unsigned int i;
	for(i = 0; i < 8; i++){ 
		if(d->endpoint_zonetype[i] != 0){
			if((d->endpoint_zonetype[i] >> 16) == ep->simpledesc.simpledesc.Endpoint){
				ep->simpledesc.zonetype = d->endpoint_zonetype[i]&0x00FF;
				d->endpoint_zonetype[i] = 0;
			}
		}
	}

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

struct device * device_create2(unsigned long long ieee, char * name, unsigned char status,
		unsigned char zclversion, unsigned char applicationversion, 
		unsigned char stackversion, unsigned char hwversion,
		char * manufacturername, char * modelidentifier, char * datecode){
	struct device * d = device_create(ieee);
	d->status = status;
	d->zclversion = zclversion;
	d->applicationversion = applicationversion;
	d->stackversion = stackversion;
	d->hwversion = hwversion;
	memcpy(d->manufacturername, manufacturername, strlen(manufacturername));
	memcpy(d->modelidentifier, modelidentifier, strlen(modelidentifier));
	memcpy(d->datecode, datecode, strlen(datecode));

	return d;
}

unsigned char device_getepcount(struct device * d){
	unsigned char count = 0;
	struct list_head * pos, *n;
	list_for_each_safe(pos, n, &d->eplisthead){ 
		count++;
	}

	return count;
}

void device_set_status(struct device * d, unsigned int status) { 
	d->status |= status;
	sqlitedb_update_device_status(d);
}

struct endpoint * _device_get_enpint(struct device *d,  unsigned char endpoint){ 
	struct list_head *pos, *n;
	struct endpoint * ep;
	list_for_each_safe(pos, n, &d->eplisthead){
		ep = list_entry(pos, struct endpoint, list); 
		if(ep->simpledesc.simpledesc.Endpoint == endpoint){
			return ep;
		}
	}

	return NULL;
}

void device_set_zonetype(struct device *d, unsigned char endpoint, unsigned short zonetype){
	struct endpoint * ep = _device_get_enpint(d, endpoint);
	if(ep){
		ep->simpledesc.zonetype = zonetype;
		sqlitedb_update_device_endpoint_zonetype(d, endpoint, zonetype);
	}else{ 
		unsigned char i;
		for(i = 0; i < 8; i++){
			if(d->endpoint_zonetype[i]==0){
				d->endpoint_zonetype[i] = (endpoint << 16)+zonetype;
			}
		}
	}
}

int device_get_index(struct device *d, unsigned char endpoint){
	int result = -1;
	struct list_head *pos, *n;
	struct endpoint * ep;
	list_for_each_safe(pos, n, &d->eplisthead){
		result++;
		ep = list_entry(pos, struct endpoint, list); 
		if(ep->simpledesc.simpledesc.Endpoint == endpoint){
			return result;
		}
	}

	return -1;
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

// ---------------gateway---------------
