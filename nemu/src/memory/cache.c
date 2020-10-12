#include "common.h"
#include <stdlib.h>

/*Cache L1*/
#define way 8
#define BLOCK_SIZE 64
#define STORE_SIZE_L1 64*1024
#define COUNT_CACHE_L1 1024
#define BLOCK_BIT 6
#define GROUP_BIT 7
#define TAG_BIT (32 - BLOCK_BIT - GROUP_BIT) //19

/*Lache L2*/
#define way_2 16
#define STORE_SIZE_L2 4*1024*1024
#define COUNT_CACHE_L2 64*1024
#define BLOCK_BIT_2 6
//group num = 1024*64 / 16 = 4096
#define GROUP_BIT_2 12
#define TAG_BIT_2 (32 - BLOCK_BIT_2 - GROUP_BIT_2) //14


/* Memory accessing interfaces */
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);


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

struct Cache2
{
	bool valid;
	bool dirty;
	union 
	{
		struct 
		{
			uint32_t offset : BLOCK_BIT_2;
			uint32_t group  : GROUP_BIT_2;
			uint32_t tag 	: TAG_BIT_2;
		};
		uint32_t addr;
	};

	uint8_t data[BLOCK_SIZE];

}cache2[COUNT_CACHE_L2];


void init_cache( ){
	int i;
	for (i = 0; i < COUNT_CACHE_L1; i++)
	{
		cache1[i].valid = false;
		cache1[i].addr = 0;
		memset(cache1[i].data, 0, BLOCK_SIZE);
	}

	for (i = 0; i < COUNT_CACHE_L2; i++)
	{
		cache2[i].valid = false;
		cache2[i].dirty = false;
		cache2[i].addr = 0;
		memset(cache2[i].data, 0, BLOCK_SIZE);
	}

}

inline static void memcpy_cache(void *dest, void *src, size_t len){
	int i;
	for (i = 0; i < len; i++)
	{
		((uint8_t*) dest)[i] = ((uint8_t*) src)[i];
	}
	
}


uint32_t cache1_read(hwaddr_t addr, size_t len){
	uint32_t group_num = (addr >> 6) & 0x7f;
	uint32_t addr_block = (addr >> BLOCK_BIT << BLOCK_BIT);
	uint32_t offset = addr & 0x3f;
	int i;
	bool is = false;
	uint8_t temp[BLOCK_SIZE*2];
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