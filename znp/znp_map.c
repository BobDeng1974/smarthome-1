#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "znp_map.h"

static struct rb_root znp_map_shortaddr_to_ieee = {NULL};
static struct rb_root znp_map_ieee_to_shortaddr = {NULL};

struct znp_map * znp_map_get_shortaddr(unsigned long long ieee){ 
	struct rb_root * root = &znp_map_ieee_to_shortaddr;
	struct rb_node * node = root->rb_node;

	struct znp_map * map;
	while(node){
		map = rb_entry(node, struct znp_map, node);
		if(ieee > map->ieee){
			node = node->rb_right;
		}else if(ieee < map->ieee){
			node = node->rb_left;
		}else{
			return map;
		}
	}

	return NULL;
}


struct znp_map * znp_map_get_ieee(unsigned short shortaddr){
	struct rb_root * root = &znp_map_shortaddr_to_ieee;
	struct rb_node * node = root->rb_node;

	struct znp_map * map;
	while(node){
		map = rb_entry(node, struct znp_map, node);
		if(shortaddr > map->shortaddr){
			node = node->rb_right;
		}else if(shortaddr < map->shortaddr){
			node = node->rb_left;
		}else{
			return map;
		}
	}

	return NULL;
}

struct znp_map * _znp_map_insert_ieee(struct znp_map * map){ 
	struct rb_node ** newnode = &znp_map_ieee_to_shortaddr.rb_node, *parent = NULL;

	struct znp_map * m;

	while(*newnode){
		parent = *newnode;
		m = rb_entry(parent, struct znp_map, node);

		if(map->ieee< m->ieee){
			newnode = &((*newnode)->rb_left);
		}else if(map->ieee> m->ieee){
			newnode = &((*newnode)->rb_right);
		}else{
	//		fprintf(stdout, "!!!(((rbtree alread has the shortaddr %d the tree ieee is %016llX the new %016llX\n", device->shortaddr, device->ieeeaddr, d->ieeeaddr);
			m->shortaddr = map->shortaddr;

			return m;
		}
	}

	rb_link_node(&map->node, parent, newnode);

	return NULL;
	
}

void _znp_map_ieee_dump(){
	struct rb_node * node = rb_first(&znp_map_ieee_to_shortaddr);
	struct znp_map * m = rb_entry(node, struct znp_map, node);
	fprintf(stdout, "e2aieee 0x%016llX addr 0x%04X \n",m->ieee, m->shortaddr);
	struct rb_node * n = rb_next(&m->node);
	while(n){
		struct znp_map *m = rb_entry(n, struct znp_map, node);
		fprintf(stdout, "e2aieee 0x%016llX addr 0x%04X \n",m->ieee, m->shortaddr);
		n=rb_next(&m->node);
	}
}


void znp_map_insert_ieee(struct znp_map * map){
	struct znp_map * ret;
	
	if((ret = _znp_map_insert_ieee(map))){
		free(map);
		return;
	}

	rb_insert_color(&map->node, &znp_map_ieee_to_shortaddr);
}


struct znp_map * _znp_map_insert_shortaddr(struct znp_map * map){ 
	struct rb_node ** newnode = &znp_map_shortaddr_to_ieee.rb_node, *parent = NULL;

	struct znp_map * m;

	while(*newnode){
		parent = *newnode;
		m = rb_entry(parent, struct znp_map, node);

		if(map->shortaddr< m->shortaddr){
			newnode = &((*newnode)->rb_left);
		}else if(map->shortaddr > m->shortaddr){
			newnode = &((*newnode)->rb_right);
		}else{
	//		fprintf(stdout, "!!!(((rbtree alread has the shortaddr %d the tree ieee is %016llX the new %016llX\n", device->shortaddr, device->ieeeaddr, d->ieeeaddr);
			m->ieee= map->ieee;

			return m;
		}
	}

	rb_link_node(&map->node, parent, newnode);

	return NULL;
	
}

void _znp_map_shortaddr_dump(){
	struct rb_node * node = rb_first(&znp_map_shortaddr_to_ieee);
	struct znp_map * m = rb_entry(node, struct znp_map, node);
	fprintf(stdout, "a2eieee 0x%016llX addr 0x%04X \n",m->ieee, m->shortaddr);
	struct rb_node * n = rb_next(&m->node);
	while(n){
		struct znp_map *m = rb_entry(n, struct znp_map, node);
		fprintf(stdout, "a2eieee 0x%016llX addr 0x%04X \n",m->ieee, m->shortaddr);
		n=rb_next(&m->node);
	}
}

void _znp_map_dump(){
	_znp_map_ieee_dump();
	fprintf(stdout, "------\n");
	_znp_map_shortaddr_dump();
}

void znp_map_insert_shortaddr(struct znp_map * map){
	struct znp_map * ret;

	if((ret = _znp_map_insert_shortaddr(map))){
		free(map);
		return;
	}

	rb_insert_color(&map->node, &znp_map_shortaddr_to_ieee);
}

void znp_map_insert(unsigned short shortaddr, unsigned long long ieee){ 
	struct znp_map * addr = znp_map_get_shortaddr(ieee);
	if(addr == NULL){ 
		struct znp_map * map = (struct znp_map *)malloc(sizeof(struct znp_map));
		memset(map, 0, sizeof(struct znp_map)); 
		map->shortaddr = shortaddr;
		map->ieee = ieee;
		znp_map_insert_ieee(map);
	}else{ 
		addr->shortaddr = shortaddr;
	}

	struct znp_map * ieeeaddr = znp_map_get_ieee(shortaddr);
	if(ieeeaddr == NULL){
		struct znp_map * map = (struct znp_map *)malloc(sizeof(struct znp_map));
		memset(map, 0, sizeof(struct znp_map)); 
		map->shortaddr = shortaddr;
		map->ieee = ieee;
		znp_map_insert_shortaddr(map);
	}

	_znp_map_dump();
}

void znp_map_del_ieee(unsigned long long ieee){
	struct znp_map * map = znp_map_get_ieee(ieee);
	if(map){
		rb_erase(&map->node, &znp_map_ieee_to_shortaddr);
		struct znp_map * map2 = znp_map_get_shortaddr(map->shortaddr);
		if(map2){
			rb_erase(&map2->node, &znp_map_shortaddr_to_ieee);
			free(map2);
		}
		free(map);
	}
	_znp_map_dump();

}

void znp_map_del_shortaddr(unsigned short shortaddr){
	struct znp_map * map = znp_map_get_shortaddr(shortaddr);
	if(map){
		rb_erase(&map->node, &znp_map_shortaddr_to_ieee);
		struct znp_map * map2 = znp_map_get_ieee(map->ieee);
		if(map2){
			rb_erase(&map2->node, &znp_map_ieee_to_shortaddr);
			free(map2);
		}
		free(map);
	}
}
