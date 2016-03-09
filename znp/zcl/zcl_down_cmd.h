#ifndef __ZCL_DOWN_CMD_H_H
#define __ZCL_DOWN_CMD_H_H

#define ZCL_DOWN_IDENTIFY 0x800C

struct zcl_down_cmd_identify_t{
	unsigned long long ieee;
};

void zcl_down_cmd_identify(struct zcl_down_cmd_identify_t * identify);

#endif
