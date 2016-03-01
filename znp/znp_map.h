#ifndef _ZNP_MAP_H_H
#define _ZNP_MAP_H_H
#include "rbtree.h"

struct znp_map{
	struct rb_node node;
	unsigned short shortaddr;
	unsigned long long ieee;
};

struct znp_map * znp_map_get_shortaddr(unsigned long long ieee);
struct znp_map * znp_map_get_ieee(unsigned short shortaddr);
void znp_map_insert(unsigned short shortaddr, unsigned long long ieee);
void znp_map_del_ieee(unsigned long long ieee);
void znp_map_del_shortaddr(unsigned short shortaddr);

#endif
