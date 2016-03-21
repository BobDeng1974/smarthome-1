#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "zcl_down_cmd.h"
#include "zcl_general.h"
#include "gateway.h"
#include "zcl_ss.h"
#include "zcl_register_cluster.h"

#define IDENTYFYTIME 30

void zcl_down_cmd_identify(unsigned long long ieee, struct protocol_cmdtype_identify * identify){ 
	struct device * d = gateway_getdevice(getgateway(), ieee);
	if(d){
		fprintf(stdout, " **identify endpoint %d\n", identify->endpoint);
		fprintf(stdout, " **identify nwkwork %d\n", d->shortaddr);
		zclGeneral_SendIdentify(APP_DEVICETYPEID_SS_ENDPOINT, 
				identify->endpoint,
				d->shortaddr,
				IDENTYFYTIME, 0, 0);
	}
}

void zcl_down_cmd_warning(unsigned long long ieee, struct protocol_cmdtype_warning* warning){ 
	struct endpoint * dstep = gateway_get_endpoint(ieee, warning->endpoint);

	if(dstep){
		fprintf(stdout, " **warning endpoint %d\n", warning->endpoint);
		fprintf(stdout, " **warning warnmode %d\n", warning->start_warning.warningmessage.warningbits.warnMode);
		fprintf(stdout, " **warning warnstrobe %d\n", warning->start_warning.warningmessage.warningbits.warnStrobe);
		fprintf(stdout, " **warning warnSirenlevel%d\n", warning->start_warning.warningmessage.warningbits.warnSirenLevel);
		fprintf(stdout, " **warning warningDuration %d\n", warning->start_warning.warningDuration);
		fprintf(stdout, " **warning strobeDutyCycle%d\n", warning->start_warning.strobeDutyCycle);
		fprintf(stdout, " **warning strobeLevel%d\n", warning->start_warning.strobeLevel);
		zclss_send_ias_wd_start_warning_cmd(APP_DEVICETYPEID_SS_ENDPOINT, 
				warning->endpoint,
				dstep->simpledesc.simpledesc.NwkAddr,
				&warning->start_warning,
				0,0);
	}
}

void zcl_down_cmd_onoff(struct protocol_cmdtype_onoff_ieee * onoff){
	struct device * d = gateway_getdevice(getgateway(), onoff->ieee);

	afAddrType_t addrtype;
	if(d){
		fprintf(stdout, "send onoff command\n");
		addrtype.addr.shortAddr = d->shortaddr;
		addrtype.endPoint = onoff->endpoint;
		if(onoff->onoff){
			zclGeneral_SendOnOff_CmdOn(APP_DEVICETYPEID_SS_ENDPOINT, &addrtype,0,0);
		}else{
			zclGeneral_SendOnOff_CmdOff(APP_DEVICETYPEID_SS_ENDPOINT, &addrtype,0,0);
		}

	}

}
