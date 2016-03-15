#ifndef __ENCODE_PROTOCL_H_H_
#define __ENCODE_PROTOCL_H_H_
struct zclzonechangenotification;
struct gateway;

unsigned int encode_heart(struct gateway *gw, unsigned char *buf);

unsigned int encode_login(struct gateway *gw, unsigned char *buf);

#endif
