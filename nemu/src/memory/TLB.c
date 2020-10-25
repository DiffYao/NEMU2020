#include "common.h"
#include <stdlib.h>

#define PAGE_BIT 12
#define TAG_BIT 20
#define	COUNT_ITEM 64

struct TLB
{
	bool valid;
	union
	{
		struct 
		{
			uint32_t offset : PAGE_BIT;
			uint32_t tag 	: TAG_BIT;
		};
		uint32_t addr;
		
	};
	uint32_t result;
	
}tlb[COUNT_ITEM];

void init_TLB(){

	int i;
	for (i = 0; i < COUNT_ITEM; i++)
	{
		tlb[i].valid = 0;
		tlb[i].addr = 0;
		tlb[i].result = 0;
	}
	
}

uint32_t TLB_translate(lnaddr_t addr){

	uint32_t tag = addr >> 12;
	int HIT = 0, i;
	for (i = 0; i < COUNT_ITEM; i++)
	{
		if (tlb[i].valid && tlb[i].tag == tag)
		{
			HIT = 1;
			break;
		}

	}
	if (HIT) return tlb[i].result;
	else 	return 0xffffffff;
}

void TLB_update(lnaddr_t addr, hwaddr_t hwaddr){

	
	int i;
	for (i = 0; i < 64; i++)
		if (!tlb[i].valid)
		{
			break;
		}

	srand(0);
	if (i == 64) i = rand() % 64;
	tlb[i].valid = 1;
	tlb[i].addr = addr;
	tlb[i].result = hwaddr;
}