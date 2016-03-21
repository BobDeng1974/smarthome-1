#ifndef __ZCL_DOWN_CMD_H_H
#define __ZCL_DOWN_CMD_H_H

#include "protocol_cmdtype.h"
#define ZCL_DOWN_IDENTIFY 0x800C


void zcl_down_cmd_identify(unsigned long long ieee,struct protocol_cmdtype_identify *  identify);

void zcl_down_cmd_warning(unsigned long long ieee, struct protocol_cmdtype_warning * warning);

void zcl_down_cmd_onoff(struct protocol_cmdtype_onoff_ieee * onoff);

#endif
