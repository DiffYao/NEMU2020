#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char str[32];
	uint32_t val;	
	/* TODO: Add more members if necessary */


} WP;
void init_wp_pool();
WP* new_wp();
void free_wp(WP* wp);
bool check_WP();
int delete_wp(int num);
void print_wp();
#endif
