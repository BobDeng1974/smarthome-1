#ifndef ZCL_H
#define ZCL_H
#include "mtAf.h"
#include "mtSys.h"

/*********************************************************************
 * CONSTANTS
 */

// General Clusters
#define ZCL_CLUSTER_ID_GEN_BASIC                             0x0000
#define ZCL_CLUSTER_ID_GEN_POWER_CFG                         0x0001
#define ZCL_CLUSTER_ID_GEN_DEVICE_TEMP_CONFIG                0x0002
#define ZCL_CLUSTER_ID_GEN_IDENTIFY                          0x0003
#define ZCL_CLUSTER_ID_GEN_GROUPS                            0x0004
#define ZCL_CLUSTER_ID_GEN_SCENES                            0x0005
#define ZCL_CLUSTER_ID_GEN_ON_OFF                            0x0006
#define ZCL_CLUSTER_ID_GEN_ON_OFF_SWITCH_CONFIG              0x0007
#define ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL                     0x0008
#define ZCL_CLUSTER_ID_GEN_ALARMS                            0x0009
#define ZCL_CLUSTER_ID_GEN_TIME                              0x000A
#define ZCL_CLUSTER_ID_GEN_LOCATION                          0x000B
#define ZCL_CLUSTER_ID_GEN_ANALOG_INPUT_BASIC                0x000C
#define ZCL_CLUSTER_ID_GEN_ANALOG_OUTPUT_BASIC               0x000D
#define ZCL_CLUSTER_ID_GEN_ANALOG_VALUE_BASIC                0x000E
#define ZCL_CLUSTER_ID_GEN_BINARY_INPUT_BASIC                0x000F
#define ZCL_CLUSTER_ID_GEN_BINARY_OUTPUT_BASIC               0x0010
#define ZCL_CLUSTER_ID_GEN_BINARY_VALUE_BASIC                0x0011
#define ZCL_CLUSTER_ID_GEN_MULTISTATE_INPUT_BASIC            0x0012
#define ZCL_CLUSTER_ID_GEN_MULTISTATE_OUTPUT_BASIC           0x0013
#define ZCL_CLUSTER_ID_GEN_MULTISTATE_VALUE_BASIC            0x0014
#define ZCL_CLUSTER_ID_GEN_COMMISSIONING                     0x0015
#define ZCL_CLUSTER_ID_GEN_PARTITION                         0x0016

#define ZCL_CLUSTER_ID_OTA                                   0x0019

#define ZCL_CLUSTER_ID_GEN_POWER_PROFILE                     0x001A
#define ZCL_CLUSTER_ID_GEN_APPLIANCE_CONTROL                 0x001B

#define ZCL_CLUSTER_ID_GEN_POLL_CONTROL                      0x0020

#define ZCL_CLUSTER_ID_GREEN_POWER_PROXY                     0x0021

// Closures Clusters
#define ZCL_CLUSTER_ID_CLOSURES_SHADE_CONFIG                 0x0100
#define ZCL_CLUSTER_ID_CLOSURES_DOOR_LOCK                    0x0101
#define ZCL_CLUSTER_ID_CLOSURES_WINDOW_COVERING              0x0102

// HVAC Clusters
#define ZCL_CLUSTER_ID_HVAC_PUMP_CONFIG_CONTROL              0x0200
#define ZCL_CLUSTER_ID_HVAC_THERMOSTAT                       0x0201
#define ZCL_CLUSTER_ID_HVAC_FAN_CONTROL                      0x0202
#define ZCL_CLUSTER_ID_HVAC_DIHUMIDIFICATION_CONTROL         0x0203
#define ZCL_CLUSTER_ID_HVAC_USER_INTERFACE_CONFIG            0x0204

// Lighting Clusters
#define ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL                0x0300
#define ZCL_CLUSTER_ID_LIGHTING_BALLAST_CONFIG               0x0301

// Measurement and Sensing Clusters
#define ZCL_CLUSTER_ID_MS_ILLUMINANCE_MEASUREMENT            0x0400
#define ZCL_CLUSTER_ID_MS_ILLUMINANCE_LEVEL_SENSING_CONFIG   0x0401
#define ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT            0x0402
#define ZCL_CLUSTER_ID_MS_PRESSURE_MEASUREMENT               0x0403
#define ZCL_CLUSTER_ID_MS_FLOW_MEASUREMENT                   0x0404
#define ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY                  0x0405
#define ZCL_CLUSTER_ID_MS_OCCUPANCY_SENSING                  0x0406

// Security and Safety (SS) Clusters
#define ZCL_CLUSTER_ID_SS_IAS_ZONE                           0x0500
#define ZCL_CLUSTER_ID_SS_IAS_ACE                            0x0501
#define ZCL_CLUSTER_ID_SS_IAS_WD                             0x0502

// Protocol Interfaces
#define ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL                     0x0600
#define ZCL_CLUSTER_ID_PI_BACNET_PROTOCOL_TUNNEL             0x0601
#define ZCL_CLUSTER_ID_PI_ANALOG_INPUT_BACNET_REG            0x0602
#define ZCL_CLUSTER_ID_PI_ANALOG_INPUT_BACNET_EXT            0x0603
#define ZCL_CLUSTER_ID_PI_ANALOG_OUTPUT_BACNET_REG           0x0604
#define ZCL_CLUSTER_ID_PI_ANALOG_OUTPUT_BACNET_EXT           0x0605
#define ZCL_CLUSTER_ID_PI_ANALOG_VALUE_BACNET_REG            0x0606
#define ZCL_CLUSTER_ID_PI_ANALOG_VALUE_BACNET_EXT            0x0607
#define ZCL_CLUSTER_ID_PI_BINARY_INPUT_BACNET_REG            0x0608
#define ZCL_CLUSTER_ID_PI_BINARY_INPUT_BACNET_EXT            0x0609
#define ZCL_CLUSTER_ID_PI_BINARY_OUTPUT_BACNET_REG           0x060A
#define ZCL_CLUSTER_ID_PI_BINARY_OUTPUT_BACNET_EXT           0x060B
#define ZCL_CLUSTER_ID_PI_BINARY_VALUE_BACNET_REG            0x060C
#define ZCL_CLUSTER_ID_PI_BINARY_VALUE_BACNET_EXT            0x060D
#define ZCL_CLUSTER_ID_PI_MULTISTATE_INPUT_BACNET_REG        0x060E
#define ZCL_CLUSTER_ID_PI_MULTISTATE_INPUT_BACNET_EXT        0x060F
#define ZCL_CLUSTER_ID_PI_MULTISTATE_OUTPUT_BACNET_REG       0x0610
#define ZCL_CLUSTER_ID_PI_MULTISTATE_OUTPUT_BACNET_EXT       0x0611
#define ZCL_CLUSTER_ID_PI_MULTISTATE_VALUE_BACNET_REG        0x0612
#define ZCL_CLUSTER_ID_PI_MULTISTATE_VALUE_BACNET_EXT        0x0613
#define ZCL_CLUSTER_ID_PI_11073_PROTOCOL_TUNNEL              0x0614

// Smart Energy (SE) Clusters
#define ZCL_CLUSTER_ID_SE_PRICE                              0x0700
#define ZCL_CLUSTER_ID_SE_DRLC                               0x0701
#define ZCL_CLUSTER_ID_SE_METERING                           0x0702
#define ZCL_CLUSTER_ID_SE_MESSAGING                          0x0703
#define ZCL_CLUSTER_ID_SE_TUNNELING                          0x0704
#define ZCL_CLUSTER_ID_SE_PREPAYMENT                         0x0705
#define ZCL_CLUSTER_ID_SE_ENERGY_MGMT                        0x0706
#define ZCL_CLUSTER_ID_SE_CALENDAR                           0x0707
#define ZCL_CLUSTER_ID_SE_DEVICE_MGMT                        0x0708
#define ZCL_CLUSTER_ID_SE_EVENTS                             0x0709
#define ZCL_CLUSTER_ID_SE_MDU_PAIRING                        0x070A
#define ZCL_CLUSTER_ID_SE_KEY_ESTABLISHMENT                  0x0800

#define ZCL_CLUSTER_ID_HA_APPLIANCE_IDENTIFICATION           0x0B00
#define ZCL_CLUSTER_ID_HA_METER_IDENTIFICATION               0x0B01
#define ZCL_CLUSTER_ID_HA_APPLIANCE_EVENTS_ALERTS            0x0B02
#define ZCL_CLUSTER_ID_HA_APPLIANCE_STATISTICS               0x0B03
#define ZCL_CLUSTER_ID_HA_ELECTRICAL_MEASUREMENT             0x0B04
#define ZCL_CLUSTER_ID_HA_DIAGNOSTIC                         0x0B05

// Light Link cluster
#define ZCL_CLUSTER_ID_LIGHT_LINK                           0x1000

/*** Frame Control bit mask ***/
#define ZCL_FRAME_CONTROL_TYPE                          0x03
#define ZCL_FRAME_CONTROL_MANU_SPECIFIC                 0x04
#define ZCL_FRAME_CONTROL_DIRECTION                     0x08
#define ZCL_FRAME_CONTROL_DISABLE_DEFAULT_RSP           0x10

/*** Frame Types ***/
#define ZCL_FRAME_TYPE_PROFILE_CMD                      0x00
#define ZCL_FRAME_TYPE_SPECIFIC_CMD                     0x01

/*** Frame Client/Server Directions ***/
#define ZCL_FRAME_CLIENT_SERVER_DIR                     0x00
#define ZCL_FRAME_SERVER_CLIENT_DIR                     0x01

/*** Chipcon Manufacturer Code ***/
#define CC_MANUFACTURER_CODE                            0x1001

/*** Foundation Command IDs ***/
#define ZCL_CMD_READ                                    0x00
#define ZCL_CMD_READ_RSP                                0x01
#define ZCL_CMD_WRITE                                   0x02
#define ZCL_CMD_WRITE_UNDIVIDED                         0x03
#define ZCL_CMD_WRITE_RSP                               0x04
#define ZCL_CMD_WRITE_NO_RSP                            0x05
#define ZCL_CMD_CONFIG_REPORT                           0x06
#define ZCL_CMD_CONFIG_REPORT_RSP                       0x07
#define ZCL_CMD_READ_REPORT_CFG                         0x08
#define ZCL_CMD_READ_REPORT_CFG_RSP                     0x09
#define ZCL_CMD_REPORT                                  0x0a
#define ZCL_CMD_DEFAULT_RSP                             0x0b
#define ZCL_CMD_DISCOVER_ATTRS                          0x0c
#define ZCL_CMD_DISCOVER_ATTRS_RSP                      0x0d
#define ZCL_CMD_DISCOVER_CMDS_RECEIVED                  0x11
#define ZCL_CMD_DISCOVER_CMDS_RECEIVED_RSP              0x12
#define ZCL_CMD_DISCOVER_CMDS_GEN                       0x13
#define ZCL_CMD_DISCOVER_CMDS_GEN_RSP                   0x14
#define ZCL_CMD_DISCOVER_ATTRS_EXT                      0x15
#define ZCL_CMD_DISCOVER_ATTRS_EXT_RSP                  0x16

#define ZCL_CMD_MAX           ZCL_CMD_DISCOVER_ATTRS_EXT_RSP

// define reporting constant
#define ZCL_REPORTING_OFF     0xFFFF  // turn off reporting (maxReportInt)

// define command direction flag masks
#define CMD_DIR_SERVER_GENERATED          0x01
#define CMD_DIR_CLIENT_GENERATED          0x02
#define CMD_DIR_SERVER_RECEIVED           0x04
#define CMD_DIR_CLIENT_RECEIVED           0x08

/*** Data Types ***/
#define ZCL_DATATYPE_NO_DATA                            0x00
#define ZCL_DATATYPE_DATA8                              0x08
#define ZCL_DATATYPE_DATA16                             0x09
#define ZCL_DATATYPE_DATA24                             0x0a
#define ZCL_DATATYPE_DATA32                             0x0b
#define ZCL_DATATYPE_DATA40                             0x0c
#define ZCL_DATATYPE_DATA48                             0x0d
#define ZCL_DATATYPE_DATA56                             0x0e
#define ZCL_DATATYPE_DATA64                             0x0f
#define ZCL_DATATYPE_BOOLEAN                            0x10
#define ZCL_DATATYPE_BITMAP8                            0x18
#define ZCL_DATATYPE_BITMAP16                           0x19
#define ZCL_DATATYPE_BITMAP24                           0x1a
#define ZCL_DATATYPE_BITMAP32                           0x1b
#define ZCL_DATATYPE_BITMAP40                           0x1c
#define ZCL_DATATYPE_BITMAP48                           0x1d
#define ZCL_DATATYPE_BITMAP56                           0x1e
#define ZCL_DATATYPE_BITMAP64                           0x1f
#define ZCL_DATATYPE_UINT8                              0x20
#define ZCL_DATATYPE_UINT16                             0x21
#define ZCL_DATATYPE_UINT24                             0x22
#define ZCL_DATATYPE_UINT32                             0x23
#define ZCL_DATATYPE_UINT40                             0x24
#define ZCL_DATATYPE_UINT48                             0x25
#define ZCL_DATATYPE_UINT56                             0x26
#define ZCL_DATATYPE_UINT64                             0x27
#define ZCL_DATATYPE_INT8                               0x28
#define ZCL_DATATYPE_INT16                              0x29
#define ZCL_DATATYPE_INT24                              0x2a
#define ZCL_DATATYPE_INT32                              0x2b
#define ZCL_DATATYPE_INT40                              0x2c
#define ZCL_DATATYPE_INT48                              0x2d
#define ZCL_DATATYPE_INT56                              0x2e
#define ZCL_DATATYPE_INT64                              0x2f
#define ZCL_DATATYPE_ENUM8                              0x30
#define ZCL_DATATYPE_ENUM16                             0x31
#define ZCL_DATATYPE_SEMI_PREC                          0x38
#define ZCL_DATATYPE_SINGLE_PREC                        0x39
#define ZCL_DATATYPE_DOUBLE_PREC                        0x3a
#define ZCL_DATATYPE_OCTET_STR                          0x41
#define ZCL_DATATYPE_CHAR_STR                           0x42
#define ZCL_DATATYPE_LONG_OCTET_STR                     0x43
#define ZCL_DATATYPE_LONG_CHAR_STR                      0x44
#define ZCL_DATATYPE_ARRAY                              0x48
#define ZCL_DATATYPE_STRUCT                             0x4c
#define ZCL_DATATYPE_SET                                0x50
#define ZCL_DATATYPE_BAG                                0x51
#define ZCL_DATATYPE_TOD                                0xe0
#define ZCL_DATATYPE_DATE                               0xe1
#define ZCL_DATATYPE_UTC                                0xe2
#define ZCL_DATATYPE_CLUSTER_ID                         0xe8
#define ZCL_DATATYPE_ATTR_ID                            0xe9
#define ZCL_DATATYPE_BAC_OID                            0xea
#define ZCL_DATATYPE_IEEE_ADDR                          0xf0
#define ZCL_DATATYPE_128_BIT_SEC_KEY                    0xf1
#define ZCL_DATATYPE_UNKNOWN                            0xff

/*** Error Status Codes ***/
#define ZCL_STATUS_SUCCESS                              0x00
#define ZCL_STATUS_FAILURE                              0x01
// 0x02-0x7D are reserved.
#define ZCL_STATUS_NOT_AUTHORIZED                       0x7E
#define ZCL_STATUS_MALFORMED_COMMAND                    0x80
#define ZCL_STATUS_UNSUP_CLUSTER_COMMAND                0x81
#define ZCL_STATUS_UNSUP_GENERAL_COMMAND                0x82
#define ZCL_STATUS_UNSUP_MANU_CLUSTER_COMMAND           0x83
#define ZCL_STATUS_UNSUP_MANU_GENERAL_COMMAND           0x84
#define ZCL_STATUS_INVALID_FIELD                        0x85
#define ZCL_STATUS_UNSUPPORTED_ATTRIBUTE                0x86
#define ZCL_STATUS_INVALID_VALUE                        0x87
#define ZCL_STATUS_READ_ONLY                            0x88
#define ZCL_STATUS_INSUFFICIENT_SPACE                   0x89
#define ZCL_STATUS_DUPLICATE_EXISTS                     0x8a
#define ZCL_STATUS_NOT_FOUND                            0x8b
#define ZCL_STATUS_UNREPORTABLE_ATTRIBUTE               0x8c
#define ZCL_STATUS_INVALID_DATA_TYPE                    0x8d
#define ZCL_STATUS_INVALID_SELECTOR                     0x8e
#define ZCL_STATUS_WRITE_ONLY                           0x8f
#define ZCL_STATUS_INCONSISTENT_STARTUP_STATE           0x90
#define ZCL_STATUS_DEFINED_OUT_OF_BAND                  0x91
#define ZCL_STATUS_INCONSISTENT                         0x92
#define ZCL_STATUS_ACTION_DENIED                        0x93
#define ZCL_STATUS_TIMEOUT                              0x94
#define ZCL_STATUS_ABORT                                0x95
#define ZCL_STATUS_INVALID_IMAGE                        0x96
#define ZCL_STATUS_WAIT_FOR_DATA                        0x97
#define ZCL_STATUS_NO_IMAGE_AVAILABLE                   0x98
#define ZCL_STATUS_REQUIRE_MORE_IMAGE                   0x99

// 0xbd-bf are reserved.
#define ZCL_STATUS_HARDWARE_FAILURE                     0xc0
#define ZCL_STATUS_SOFTWARE_FAILURE                     0xc1
#define ZCL_STATUS_CALIBRATION_ERROR                    0xc2
// 0xc3-0xff are reserved.
#define ZCL_STATUS_CMD_HAS_RSP                          0xFF // Non-standard status (used for Default Rsp)

/*** Attribute Access Control - bit masks ***/
#define ACCESS_CONTROL_READ                             0x01  // attribute can be read
#define ACCESS_CONTROL_WRITE                            0x02  // attribute can be written
#define ACCESS_REPORTABLE                               0x04  // indicate attribute is reportable
#define ACCESS_CONTROL_COMMAND                          0x08
#define ACCESS_CONTROL_AUTH_READ                        0x10
#define ACCESS_CONTROL_AUTH_WRITE                       0x20
#define ACCESS_CLIENT                                   0x80  // TI unique, indicate client side attribute

// Access Control as reported OTA via DiscoveryAttributesExtended
#define ACCESS_CONTROLEXT_MASK                          0x07  // read/write/reportable bits same as above

#define ZCL_ATTR_ID_MAX                                 0xFFFF

// Used by Configure Reporting Command
#define ZCL_SEND_ATTR_REPORTS                           0x00
#define ZCL_EXPECT_ATTR_REPORTS                         0x01

// Predefined Maximum String Length
#define MAX_UTF8_STRING_LEN                             50

// Used by zclReadWriteCB_t callback function
#define ZCL_OPER_LEN                                    0x00 // Get length of attribute value to be read
#define ZCL_OPER_READ                                   0x01 // Read attribute value
#define ZCL_OPER_WRITE                                  0x02 // Write new attribute value

/*********************************************************************
 * MACROS
 */
#define zcl_ProfileCmd( a )         ( (a) == ZCL_FRAME_TYPE_PROFILE_CMD )
#define zcl_ClusterCmd( a )         ( (a) == ZCL_FRAME_TYPE_SPECIFIC_CMD )

#define zcl_ServerCmd( a )          ( (a) == ZCL_FRAME_CLIENT_SERVER_DIR )
#define zcl_ClientCmd( a )          ( (a) == ZCL_FRAME_SERVER_CLIENT_DIR )

#define UNICAST_MSG( msg )          ( (msg)->wasBroadcast == false && (msg)->groupId == 0 )

// Padding needed if buffer has odd number of octects in length
#define PADDING_NEEDED( bufLen )    ( (bufLen) % 2 )

// Check for Cluster IDs
#define ZCL_CLUSTER_ID_GEN( id )      ( /* (id) >= ZCL_CLUSTER_ID_GEN_BASIC &&*/ \
                                        (id) <= ZCL_CLUSTER_ID_GEN_COMMISSIONING )
#define ZCL_CLUSTER_ID_CLOSURES( id ) ( (id) >= ZCL_CLUSTER_ID_CLOSURES_SHADE_CONFIG && \
                                        (id) <= ZCL_CLUSTER_ID_CLOSURES_WINDOW_COVERING )
#define ZCL_CLUSTER_ID_HVAC( id )     ( (id) >= ZCL_CLUSTER_ID_HVAC_PUMP_CONFIG_CONTROL && \
                                        (id) <= ZCL_CLUSTER_ID_HVAC_USER_INTERFACE_CONFIG )
#define ZCL_CLUSTER_ID_LIGHTING( id ) ( (id) >= ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL && \
                                        (id) <= ZCL_CLUSTER_ID_LIGHTING_BALLAST_CONFIG )
#define ZCL_CLUSTER_ID_MS( id )       ( (id) >= ZCL_CLUSTER_ID_MS_ILLUMINANCE_MEASUREMENT && \
                                        (id) <= ZCL_CLUSTER_ID_MS_OCCUPANCY_SENSING )
#define ZCL_CLUSTER_ID_SS( id )       ( (id) >= ZCL_CLUSTER_ID_SS_IAS_ZONE && \
                                        (id) <= ZCL_CLUSTER_ID_SS_IAS_WD )
#define ZCL_CLUSTER_ID_KEY( id )      ( (id) == ZCL_CLUSTER_ID_GEN_KEY_ESTABLISHMENT )
#define ZCL_CLUSTER_ID_SE( id )       ( (id) >= ZCL_CLUSTER_ID_SE_PRICE && \
                                        (id) <= ZCL_CLUSTER_ID_SE_MDU_PAIRING )
#define ZCL_CLUSTER_ID_PI( id )       ( (id) >= ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL && \
                                        (id) <= ZCL_CLUSTER_ID_PI_11073_PROTOCOL_TUNNEL )
#define ZCL_CLUSTER_ID_LL( id )       ( (id) == ZCL_CLUSTER_ID_LIGHT_LINK )
#define ZCL_CLUSTER_ID_PART( id )     ( (id) == ZCL_CLUSTER_ID_GEN_PARTITION )

#define ZCL_CLUSTER_ID_PC( id )       ( (id) == ZCL_CLUSTER_ID_GEN_POLL_CONTROL )

#define ZCL_CLUSTER_ID_EM( id )	      ( (id) == ZCL_CLUSTER_ID_HA_ELECTRICAL_MEASUREMENT )

#define ZCL_CLUSTER_ID_DIAG( id )     ( (id) == ZCL_CLUSTER_ID_HA_DIAGNOSTIC )

#define ZCL_CLUSTER_ID_MI( id )       ( (id) == ZCL_CLUSTER_ID_HA_METER_IDENTIFICATION )

#define ZCL_CLUSTER_ID_APPL_C( id )   ( (id) == ZCL_CLUSTER_ID_GEN_APPLIANCE_CONTROL )

#define ZCL_CLUSTER_ID_APPL_I( id )   ( (id) == ZCL_CLUSTER_ID_HA_APPLIANCE_IDENTIFICATION )

#define ZCL_CLUSTER_ID_APPL_STAT( id ) ( (id) == ZCL_CLUSTER_ID_HA_APPLIANCE_STATISTICS )

#define ZCL_CLUSTER_ID_APPL_E_A( id ) ( (id) == ZCL_CLUSTER_ID_HA_APPLIANCE_EVENTS_ALERTS )

#define ZCL_CLUSTER_ID_APPL_S( id )   ( (id) == ZCL_CLUSTER_ID_HA_APPLIANCE_STATISTICS )

#define ZCL_CLUSTER_ID_PP( id )       ( (id) == ZCL_CLUSTER_ID_GEN_POWER_PROFILE )

#define ZCL_CLUSTER_ID_DL( id )       ( (id) == ZCL_CLUSTER_ID_CLOSURES_DOOR_LOCK )

//**************

typedef unsigned char ZStatus_t;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

// Redefined Generic Status Return Values for code backwards compatibility
#define ZSuccess                    SUCCESS
#define ZFailure                    FAILURE
#define ZInvalidParameter           INVALIDPARAMETER

// ZStack status values must start at 0x10, after the generic status values (defined in comdef.h)
#define ZMemError                   0x10
#define ZBufferFull                 0x11
#define ZUnsupportedMode            0x12
#define ZMacMemError                0x13

#define ZSapiInProgress             0x20
#define ZSapiTimeout                0x21
#define ZSapiInit                   0x22

#define ZNotAuthorized              0x7E

#define ZMalformedCmd               0x80
#define ZUnsupClusterCmd            0x81

// ZCL header - frame control field
struct zclframecontrol {
	unsigned char type:2;
	unsigned char manuspecific:1;
	unsigned char direction:1;
	unsigned char disabledefaultrsp:1;
	unsigned char reserved:3;
};

// ZCL header
struct zclframehdr{
	struct zclframecontrol control; 
	short manucode;
	unsigned char transseqnum;
	unsigned char commandid;
}; 

// ZCL incoming message
struct zclincomingmsg{
	IncomingMsgFormat_t * message;
	struct zclframehdr zclframehdr; 
	unsigned char *data;
	unsigned short datalen;
};

/*
 *  Function for Sending a Command
 */
ZStatus_t zcl_sendcommand( uint8 srcEP, uint8 dstEp, uint16 dstaddr,
                                  uint16 clusterID, uint8 cmd, uint8 specific, uint8 direction,
                                  uint8 disableDefaultRsp, uint16 manuCode, uint8 seqNum,
                                  uint16 cmdFormatLen, uint8 *cmdFormat );


ZStatus_t zcl_SendCommand( uint8 srcEP, afAddrType_t *dstAddr,
                                  uint16 clusterID, uint8 cmd, uint8 specific, uint8 direction,
                                  uint8 disableDefaultRsp, uint16 manuCode, uint8 seqNum,
                                  uint16 cmdFormatLen, uint8 *cmdFormat );

// process incoming message
int zcl_proccessincomingmessage(IncomingMsgFormat_t * message); 

// ---- ZCL_READ
// Read Attribute Command format
typedef struct
{
  uint8  numAttr;            // number of attributes in the list
  uint16 attrID[];           // supported attributes list - this structure should
                             // be allocated with the appropriate number of attributes.
} zclReadCmd_t;

// Read Attribute Response Status record
typedef struct
{
  uint16 attrID;            // attribute ID
  uint8  status;            // should be ZCL_STATUS_SUCCESS or error
  uint8  dataType;          // attribute data type
  uint8  *data;             // this structure is allocated, so the data is HERE
                            // - the size depends on the attribute data type
} zclReadRspStatus_t;

// Read Attribute Response Command format
typedef struct
{
  uint8              numAttr;     // number of attributes in the list
  zclReadRspStatus_t attrList[];  // attribute status list
} zclReadRspCmd_t;
// ZCL_READ
//
// Attribute record
typedef struct
{
  uint16  attrId;         // Attribute ID
  uint8   dataType;       // Data Type - defined in AF.h
  uint8   accessControl;  // Read/write - bit field
  void    *dataPtr;       // Pointer to data field
} zclAttribute_t;

typedef struct
{
  uint16          clusterID;    // Real cluster ID
  zclAttribute_t  attr;
} zclAttrRec_t;

// Function pointer type to read/write attribute data.
//
//   clusterId - cluster that attribute belongs to
//   attrId - attribute to be read or written
//   oper - ZCL_OPER_LEN, ZCL_OPER_READ, or ZCL_OPER_WRITE
//   pValue - pointer to attribute (length) value
//   pLen - length of attribute value read
//
//   return  ZCL_STATUS_SUCCESS: Operation successful
//           ZCL Error Status: Operation not successful
typedef ZStatus_t (*zclReadWriteCB_t)( uint16 clusterId, uint16 attrId, uint8 oper,
                                       uint8 *pValue, uint16 *pLen );

// Callback function prototype to authorize a Read or Write operation
//   on a given attribute.
//
//   srcAddr - source Address
//   pAttr - pointer to attribute
//   oper - ZCL_OPER_READ, or ZCL_OPER_WRITE
//
//   return  ZCL_STATUS_SUCCESS: Operation authorized
//           ZCL_STATUS_NOT_AUTHORIZED: Operation not authorized
typedef ZStatus_t (*zclAuthorizeCB_t)( afAddrType_t *srcAddr, zclAttrRec_t *pAttr, uint8 oper );
// Attribute record list item
typedef struct zclAttrRecsList
{
  struct zclAttrRecsList *next;
  uint8                  endpoint;      // Used to link it into the endpoint descriptor
  zclReadWriteCB_t       pfnReadWriteCB;// Read or Write attribute value callback function
  zclAuthorizeCB_t       pfnAuthorizeCB;// Authorize Read or Write operation
  uint8                  numAttributes; // Number of the following records
  const zclAttrRec_t     *attrs;        // attribute records
} zclAttrRecsList;

ZStatus_t zcl_SendReadRsp( uint8 srcEP, uint8 dstEp, uint16 dstAddr,
		uint16 clusterID, zclReadRspCmd_t *readRspCmd,
		uint8 direction, uint8 disableDefaultRsp, uint8 seqNum );


ZStatus_t zcl_SendRead( uint8 srcEP, uint8 dstEp, uint16 dstAddr,
		uint16 clusterID, zclReadCmd_t *readCmd,
		uint8 direction, uint8 disableDefaultRsp, uint8 seqNum);
#define FALSE 0
#define TRUE 1

#endif
