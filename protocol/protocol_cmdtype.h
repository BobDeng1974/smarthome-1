#ifndef __PROTOCOL_CMDTYPE_H_H_
#define __PROTOCOL_CMDTYPE_H_H_
#include "zcl_ss.h" 

#define ARM 0
#define DISARM 1
#define ARMTIME 2

#define ARMTIMECOUNT 4

#define PROTOCOL_IDENTIFY 1
#define PROTOCOL_WARNING 2

struct protocol_cmdtype_arm{
	unsigned char armmodel;
	unsigned char starthour;
	unsigned char startminute;
	unsigned char endhour;
	unsigned char endminute; 
};

struct protocol_cmdtype_identify{ 
	unsigned char endpoint;
};

struct protocol_cmdtype_identify_ieee{
	unsigned long long ieee;
	struct protocol_cmdtype_identify identify;
};

struct protocol_cmdtype_identify_ieee_cmd{
	unsigned long long cmdid;
	struct protocol_cmdtype_identify_ieee identify_ieee;
};

struct protocol_cmdtype_warning{
	unsigned char endpoint;
	zclWDStartWarning_t start_warning;
};

struct protocol_cmdtype_warning_ieee{
	unsigned long long ieee;
	struct protocol_cmdtype_warning warning;
};

struct protocol_cmdtype_warning_ieee_cmd{
	unsigned long long cmdid;
	struct protocol_cmdtype_warning_ieee warning_ieee;
};
#endif
