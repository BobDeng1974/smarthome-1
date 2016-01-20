#include <stdlib.h>
#include <stdio.h>
#include "znp_device.h"

static struct rb_root znp_deviceroot = {NULL};

struct znp_device * znp_device_get(unsigned short shortaddr){
	struct rb_root * root = &znp_deviceroot;
	struct rb_node * node = root->rb_node;

	struct znp_device * device;
	while(node){
		device = rb_entry(node, struct znp_device, node);
		if( shortaddr > device->shortaddr){
			node = node->rb_right;
		}else if(shortaddr < device->shortaddr){ 
			node = node->rb_left;
		}else{
			return device;
		}
	}

	return NULL;
}

struct znp_device * _znp_device_insert(struct znp_device * d){
	struct rb_node ** newnode = &znp_deviceroot.rb_node, *parent = NULL;

	struct znp_device * device;

	while(*newnode){
		parent = *newnode;
		device = rb_entry(parent, struct znp_device, node);

		if(d->shortaddr < device->shortaddr){
			newnode = &((*newnode)->rb_left);
		}else if(d->shortaddr > device->shortaddr){
			newnode = &((*newnode)->rb_right);
		}else{
			fprintf(stdout, "!!!(((rbtree alread has the shortaddr %d the conn ieee is %016llX\n %01611X\n", device->shortaddr, device->ieeeaddr, d->ieeeaddr);

			return device;
		}
	}

	rb_link_node(&d->node, parent, newnode);
}

void znp_device_insert(struct znp_device * d){
	struct znp_device * ret;

	if((ret = _znp_device_insert(d))){
		return;
	}

	rb_insert_color(&d->node, &znp_deviceroot);
}

void znp_device_del(unsigned short shortaddr){
	struct znp_device * d = znp_device_get(shortaddr);
	if(d){
		rb_erase(&d->node, &znp_deviceroot);
		free(d);
	}
}


