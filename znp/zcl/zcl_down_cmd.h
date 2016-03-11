#ifndef __ZCL_DOWN_CMD_H_H
#define __ZCL_DOWN_CMD_H_H
#include "zcl_ss.h"

#define ZCL_DOWN_IDENTIFY 0x800C

struct zcl_down_cmd_identify_t{
	unsigned long long ieee;
	unsigned char endpoint;
};

void zcl_down_cmd_identify(struct zcl_down_cmd_identify_t * identify);

struct zcl_down_cmd_warning_t{ 
	unsigned long long ieee;
	unsigned char endpoint;
	zclWDStartWarning_t start_warning;
};

void zcl_down_cmd_warning(struct zcl_down_cmd_warning_t * warning);

#endif
