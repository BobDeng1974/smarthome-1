#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#include "bytebuffer.h"

void toolkit_printbytes(unsigned char* buf, unsigned int len){
	unsigned int i;
	for (i = 0; i < len; i++)
	{
		fprintf(stdout,"%02X", buf[i]);
	}
	fprintf(stdout,"\n");
}

void
toolkit_backtrace(void)
{
	int j, nptrs;
#define SIZE 100
	void *buffer[100];
	char **strings;

	nptrs = backtrace(buffer, SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);

	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
	 *        would produce similar output to the following: */

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	for (j = 0; j < nptrs; j++)
		printf("%s\n", strings[j]);

	free(strings);
}

unsigned long long toolkit_getmac(){
	struct ifreq ifr;
	struct ifconf ifc;
	char buf[1024];
	int success = 0;

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sock == -1) { 
		return 0;
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { return 0; }

	struct ifreq* it = ifc.ifc_req;
	const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

	for (; it != end; ++it) {
		strcpy(ifr.ifr_name, it->ifr_name);
		if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
			if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
				if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
					success = 1;
					break;
				}
			}
		}
		else { /* handle error */ }
	}

	unsigned char mac_address[8] = {0};

	if (success) memcpy(&mac_address[2], ifr.ifr_hwaddr.sa_data, 6);
	
	return bytebuffer_getquadword(mac_address);
}

unsigned char toolkit_in_period(unsigned char starthour, unsigned char startminute, unsigned char endhour, unsigned char endminute, unsigned char targethour, unsigned char targetminute){ 
	short start_minutes = starthour * 60 + startminute; 
	short end_minutes = endhour * 60 + endminute;
	short target_minutes = targethour * 60 + targetminute; 

	short end_minutes_ = end_minutes - start_minutes;
	if(end_minutes_ < 0){
		end_minutes_ += 24 * 60;
	}
	short target_minutes_ = target_minutes - start_minutes;
	if(target_minutes_ < 0 ){
		target_minutes_ += 24 * 60;
	}

	if(target_minutes_ > end_minutes_){
		return 0;
	}

	return 1;
}
