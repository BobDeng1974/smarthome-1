#ifndef __ENCODE_PROTOCL_H_H_
#define __ENCODE_PROTOCL_H_H_
struct zclzonechangenotification;

unsigned int encode_alarm(unsigned char *buf, struct zclzonechangenotification * notification);
unsigned int encode_heart(struct gateway *gw, unsigned char *buf);

unsigned int encode_adddeldevice(unsigned char * buf, unsigned long long ieeeaddr, unsigned char add);

#endif
