#include <stdlib.h>
#include <assert.h>
#include "zcl_down_cmd.h"
#include "zcl_general.h"
#include "gateway.h"
#include "zcl_ss.h"
#include "zcl_register_cluster.h"

#define IDENTYFYTIME 10

void zcl_down_cmd_identify(struct zcl_down_cmd_identify_t * identify){ 
	struct endpoint * dstep = gateway_get_endpoint(identify->ieee, identify->endpoint);
	if(dstep){
		zclGeneral_SendIdentify(APP_DEVICETYPEID_SS_ENDPOINT, 
				identify->endpoint,
			       	dstep->simpledesc.simpledesc.NwkAddr, 
				IDENTYFYTIME, 0, 0);
	}
}

void zcl_down_cmd_warning(struct zcl_down_cmd_warning_t * warning){ 
	struct endpoint * dstep = gateway_get_endpoint(warning->ieee, warning->endpoint);

	if(dstep){
		zclss_send_ias_wd_start_warning_cmd(APP_DEVICETYPEID_SS_ENDPOINT, 
				warning->endpoint,
				dstep->simpledesc.simpledesc.NwkAddr,
				&warning->start_warning,
				0,0);
	}
}
