#include "mtAf.h"
#include "commands.h"
#include "zcl.h"
#include "zcl_register_cluster.h"

int zcl_register_cluster(unsigned char in_cluster_count, unsigned short * in_cluster,unsigned char out_cluster_count, unsigned short * out_cluster, unsigned char endpoint, unsigned short appdevicetypeid);

int zcl_register_cluster_ss(){
	unsigned char in_cluster_count = 1;
	unsigned short in_cluster[MAX_CLUSTER_COUNT];
	memset(in_cluster, 0, MAX_CLUSTER_COUNT * sizeof(unsigned short));
	in_cluster[0] = ZCL_CLUSTER_ID_GEN_ON_OFF;

	unsigned char out_cluster_count = 9;
	unsigned short out_cluster[MAX_CLUSTER_COUNT];
	memset(out_cluster, 0, MAX_CLUSTER_COUNT * sizeof(unsigned short));
	out_cluster[0] = ZCL_CLUSTER_ID_GEN_BASIC;
	out_cluster[1] = ZCL_CLUSTER_ID_GEN_POWER_CFG;
	out_cluster[2] = ZCL_CLUSTER_ID_GEN_IDENTIFY;
	out_cluster[3] = ZCL_CLUSTER_ID_GEN_ON_OFF_SWITCH_CONFIG;
	out_cluster[4] = ZCL_CLUSTER_ID_GEN_COMMISSIONING;
	out_cluster[5] = ZCL_CLUSTER_ID_GEN_POLL_CONTROL;
	out_cluster[6] = ZCL_CLUSTER_ID_HA_DIAGNOSTIC;
	out_cluster[7] = ZCL_CLUSTER_ID_SS_IAS_ZONE;
	out_cluster[8] = ZCL_CLUSTER_ID_SS_IAS_WD;


	zcl_register_cluster(in_cluster_count, in_cluster, out_cluster_count, out_cluster, APP_DEVICETYPEID_SS_ENDPOINT, APP_DEVICETYPEID_SS_ENDPOINT);

	return APP_DEVICETYPEID_SS_ENDPOINT;
}

int zcl_register_cluster(unsigned char in_cluster_count, unsigned short * in_cluster,unsigned char out_cluster_count, unsigned short * out_cluster, unsigned char endpoint, unsigned short appdevicetypeid){ 
	if(in_cluster_count > MAX_CLUSTER_COUNT || out_cluster_count > MAX_CLUSTER_COUNT){
		return 1;
	} 
	RegisterFormat_t req;
	memset(&req, 0, sizeof(RegisterFormat_t)); 
	req.EndPoint= endpoint;
	req.AppProfId = 0x0104;
	req.AppDeviceId = appdevicetypeid;
	req.AppNumInClusters = in_cluster_count;
	memcpy(req.AppInClusterList, in_cluster, sizeof(unsigned short)*in_cluster_count);
	req.AppNumOutClusters = out_cluster_count;
	memcpy(req.AppOutClusterList, out_cluster, sizeof(unsigned short)*out_cluster_count);
	sendcmd((unsigned char *)&req, AF_REGISTER);

	return 0;
}
