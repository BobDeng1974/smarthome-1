#include <stdlib.h>
#include <assert.h>
#include "zcl_down_cmd.h"
#include "zcl_general.h"
#include "gateway.h"
#include "zcl_register_cluster.h"

#define IDENTYFYTIME 10

void zcl_down_cmd_identify(struct zcl_down_cmd_identify_t * identify){ 
	struct device * d = gateway_getdevice(getgateway(), identify->ieee);
	struct endpoint * dstep = NULL;
	if(d){
		dstep = device_get_endpoint(d, identify->endpoint);
	}
	assert(dstep);
	if(dstep){
		zclGeneral_SendIdentify(APP_DEVICETYPEID_SS_ENDPOINT, 
				identify->endpoint,
			       	dstep->simpledesc.simpledesc.NwkAddr, 
				IDENTYFYTIME, 0, 0);
	}
}
