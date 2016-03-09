#include <assert.h>
#include "zcl_down_cmd.h"
#include "zcl_general.h"
#include "gateway.h"

#define IDENTYFYTIME 10

void zcl_down_cmd_identify(struct zcl_down_cmd_identify_t * identify){ 
	unsigned char srcep = gateway_get_outcluster_endpoint(ZCL_CLUSTER_ID_GEN_IDENTIFY);
	struct endpoint * dstep = gateway_get_endpoint_incluster(identify->ieee, ZCL_CLUSTER_ID_GEN_IDENTIFY);
	assert(dstep);
	if(dstep){
		zclGeneral_SendIdentify(srcep, dstep->simpledesc.simpledesc.Endpoint, dstep->simpledesc.simpledesc.NwkAddr, IDENTYFYTIME, 0, 0);
	}
}
