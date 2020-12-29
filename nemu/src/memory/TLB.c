#include "common.h"
#include <stdlib.h>

typedef struct {
	bool valid;
	union {
		struct {
			uint32_t offset : 12;
			uint32_t tag : 20;
		};
		uint32_t addr;
	};
	uint32_t hwaddr_result;
} Tlb;

Tlb TLB[64];

void TLB_flush() 
{
	memset(TLB, 0, sizeof TLB);
}

hwaddr_t TLB_translate(lnaddr_t addr)
{
	uint32_t tag = addr >> 12;
	int Hit = 0, x = 0, i;
	for (i = 0; i < 64; i++)
		if (TLB[i].valid && TLB[i].tag == tag)
		{
			Hit = 1;
			x = i;
			break;
		}
	if (Hit) return TLB[x].hwaddr_result;
	else return 0xffffffff;
}

void TLB_update(lnaddr_t addr, hwaddr_t hwaddr)
{
	uint32_t tag = addr >> 12;
	int x = 0, i;
	for (i = 0; i < 64; i++)
		if (!TLB[i].valid)
		{
			x = i;
			break;
		}
	if (i == 64) x = rand() % 64; 
	TLB[x].tag = tag;
	TLB[x].valid = 1;
	TLB[x].hwaddr_result = hwaddr;
}
