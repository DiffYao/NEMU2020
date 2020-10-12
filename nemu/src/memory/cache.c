#include "common.h"
#include <stdlib.h>

/*Cache L1*/
#define way 8
#define BLOCK_SIZE 64
#define STORE_SIZE_L1 64*1024
#define COUNT_CACHE_L1 1024
#define BLOCK_BIT 6
#define GROUP_BIT 7
#define TAG_BIT (32 - BLOCK_BIT - GROUP_BIT)

/*Lache L2*/




uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
/* Memory accessing interfaces */

struct Cache1
{
	bool valid;
	union 
	{
		struct 
		{
			uint32_t offset : BLOCK_BIT;
			uint32_t group  : GROUP_BIT;
			uint32_t tag 	: TAG_BIT;
		};
		uint32_t addr;
	};

	uint8_t data[BLOCK_SIZE];

}cache1[COUNT_CACHE_L1];

void init_cache( ){
	int i;
	for (i = 0; i < COUNT_CACHE_L1; i++)
	{
		cache1[i].valid = false;
		cache1[i].addr = 0;
		memset(cache1[i].data, 0, BLOCK_SIZE);
	}

}

inline static void memcpy_cache(void *dest, void *src, size_t len){
	int i;
	for (i = 0; i < len; i++)
	{
		((uint8_t*) dest)[i] = ((uint8_t*) src)[i];
	}
	
}

void cache1_write(hwaddr_t addr, size_t len, uint32_t data){
	
	struct Cache1 mirror;
	mirror.addr = addr;
	uint32_t group_num = mirror.group;
	uint32_t tag = mirror.tag;
	uint32_t addr_block = (addr >> BLOCK_BIT << BLOCK_BIT);

	int i;
	bool is = false;
	for (i = group_num * way ; i < (group_num + 1) * way ; i++)
	{
		if (cache1[i].valid && cache1[i].tag == tag)
			{
				is = true;
				break;
			}
	}
	//update the memory
	dram_write(addr, len, data);
	if (is){
		int j;
		for(j = 0; j < BLOCK_SIZE; j++)
			cache1[i].data[j] = dram_read(addr_block+j, 1) & (~0u >> ((4 - len) << 3));		
	}
}
uint32_t cache1_read(hwaddr_t addr, size_t len){
	uint32_t group_num = (addr >> 6) & 0x7f;
	uint32_t addr_block = (addr >> BLOCK_BIT << BLOCK_BIT);
	uint32_t offset = addr & 0x3f;
	int i;
	bool is = false;
	uint8_t temp[BLOCK_SIZE*2];
	printf("hello world\n");
	for (i = group_num * way ; i < (group_num + 1) * way ; i++)
	{
		if (cache1[i].valid && cache1[i].tag == (addr >> 13))
			{
				is = true;
				break;
			}
	}
	if (!is){
		for (i = group_num * way ; i < (group_num + 1) * way ; i++)
		{
			if (!cache1[i].valid)	break;
		}
		if (i == (group_num+1) * way){
			srand(0);
			i = group_num * way + rand() % way;
		}
		cache1[i].valid = true;
		cache1[i].tag = addr >> 13;
		int j;
		for(j = 0; j < BLOCK_SIZE; j++)
			cache1[i].data[j] = dram_read(addr_block+j, 1) & (~0u >> ((4 - len) << 3));		
	}
	memcpy_cache(temp, cache1[i].data, BLOCK_SIZE);
	if (len + offset > BLOCK_SIZE) 
		 *(uint32_t*)(temp + BLOCK_SIZE) = cache1_read(addr_block + BLOCK_SIZE, len);
	
	return unalign_rw(temp + offset, 4);

}