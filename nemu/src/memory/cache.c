#include "common.h"
#include <stdlib.h>

#define BLOCK_BIT 6
#define BLOCK_SIZE 64
#define GROUP_BIT 7
#define GROUP_NUM 128
#define WAY_NUM 8
#define TAG_BIT (32 - BLOCK_BIT - GROUP_BIT)
#define BLOCK_BIT_2 6
#define BLOCK_SIZE_2 64
#define GROUP_BIT_2 12
#define GROUP_NUM_2 4096
#define WAY_NUM_2 16
#define TAG_BIT_2 (32 - BLOCK_BIT_2 - GROUP_BIT_2)


uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* DECLARATION------------------------------------------------------------------------  */
uint32_t Cache_1_read(hwaddr_t, size_t);
void Cache_1_write(hwaddr_t, size_t, uint32_t);
static uint32_t Cache_2_read(hwaddr_t, size_t);
static void Cache_2_write(hwaddr_t, size_t, uint32_t);

/* DEFINITION-------------------------------------------------------------------------  */
//The cache struct is not the simpliest. It contains extra contents, offset and group.
typedef struct {
	bool valid;
	union {
		struct {
			uint32_t offset : BLOCK_BIT;
			uint32_t group : GROUP_BIT;
			uint32_t tag : TAG_BIT;
		};
		uint32_t addr;
	};
	uint8_t data[BLOCK_SIZE];
} Cache_1;

typedef struct {
	Cache_1 cache[WAY_NUM];
} Cache_1_group;

typedef struct {
	bool valid;
	bool dirty;
	union {
		struct {
			uint32_t offset : BLOCK_BIT_2;
			uint32_t group : GROUP_BIT_2;
			uint32_t tag : TAG_BIT_2;
		};
		uint32_t addr;
	};
	uint8_t data[BLOCK_SIZE_2];
} Cache_2;

typedef struct {
	Cache_2 cache[WAY_NUM_2];
} Cache_2_group;
/* ----------------------------------------------------------------------------------  */

Cache_1_group L1[GROUP_NUM];
Cache_2_group L2[GROUP_NUM_2];

/* ----------------------------------------------------------------------------------  */
static void init_cache1()
{
	int i, j;
	memset(L1, 0, sizeof L1);
	for (i = 0; i < GROUP_NUM; i++)
		for (j = 0; j < WAY_NUM; j++)
			L1[i].cache[j].group = (uint32_t)j;
}

static void init_cache2()
{
	int i, j;
	memset(L2, 0, sizeof L2);
	for (i = 0; i < GROUP_NUM_2; i++)
		for (j = 0; j < WAY_NUM_2; j++)
			L2[i].cache[j].group = (uint32_t)j;
}

void init_cache()
{
	init_cache1();
	init_cache2();
}
/* ---------------------------------------------------------------------------------- */

inline static void memcpy_cache (void *dest, void *src, size_t len)
{
	int i;
	for (i = 0; i < len; i++)
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
}


/* ---------------------------------------------------------------------------------- */

uint32_t Cache_1_read(hwaddr_t addr, size_t len) 
{
	Cache_1 mirror;
	mirror.addr = addr;
	uint32_t offset = mirror.offset;
	uint32_t group = mirror.group;
	uint32_t tag = mirror.tag;
	hwaddr_t addr_block = addr & (~0u << BLOCK_BIT);

	uint8_t temp[2 * BLOCK_SIZE];

	int Hit = 0, x = 0;
	int i = 0;
	for (i = 0; i < WAY_NUM; i++)
		if (L1[group].cache[i].valid && L1[group].cache[i].tag == tag)
		{
			Hit = 1;
			x = i;
			break;
		}

	/* If it doesn't hit, then we should allocate a new cache. The default value is 0. */
	if (!Hit)
	{
		for (i = 0; i < WAY_NUM; i++)
			if (!L1[group].cache[i].valid)
			{
				x = i;
				break;
			}
		if (i == WAY_NUM) {
			x = rand() % WAY_NUM;
		}
		L1[group].cache[x].valid = true;
		L1[group].cache[x].tag = tag;
		for (i = 0; i < BLOCK_SIZE; i++) 
			L1[group].cache[x].data[i] = Cache_2_read(addr_block + i, 1) & (~0u >> ((4 - 1) << 3));	//read into cache one byte by one
	}

	/* To generate the output and deal with the situation when it crosses caches. */
	memcpy_cache (temp, L1[group].cache[x].data, BLOCK_SIZE);
	if (offset + len > BLOCK_SIZE) *(uint32_t*)(temp + BLOCK_SIZE) = Cache_1_read(addr_block + BLOCK_SIZE, len);

	return unalign_rw(temp + offset, 4);
}



void Cache_1_write(hwaddr_t addr, size_t len, uint32_t data) 
{
	Cache_1 mirror;
	mirror.addr = addr;
	uint32_t offset = mirror.offset;
	uint32_t group = mirror.group;
	uint32_t tag = mirror.tag;
	hwaddr_t addr_block = addr & (~0u << BLOCK_BIT);

	if (offset + len - 1 > 0x3f) {
		int front = 0x3f - offset + 1;
		int back = len - front;
		uint32_t high = data >> (8 * front);
		uint32_t low = data & ((1 << (front * 8)) - 1);
		Cache_1_write(addr, front, low);
		Cache_1_write(addr + front, back, high);
		return;
	}

	int Hit = 0, x = 0;
	int i = 0;
	for (i = 0; i < WAY_NUM; i++)
		if (L1[group].cache[i].valid && L1[group].cache[i].tag == tag)
		{
			Hit = 1;
			x = i;
			break;
		}
	


	Cache_2_write(addr, len, data);	//update the memory
	if (Hit)
		for (i = 0; i < BLOCK_SIZE; i++)
			L1[group].cache[x].data[i] = Cache_2_read(addr_block + i, 1) & (~0u >> ((4 - 1) << 3));	//read into cache one byte by one

}


/* ---------------------------------------------------------------------------------- */

static uint32_t Cache_2_read(hwaddr_t addr, size_t len) 
{
	Cache_2 mirror;
	mirror.addr = addr;
	uint32_t offset = mirror.offset;
	uint32_t group = mirror.group;
	uint32_t tag = mirror.tag;
	hwaddr_t addr_block = addr & (~0u << BLOCK_BIT_2);
	uint8_t temp[2 * BLOCK_SIZE_2];

	int Hit = 0, x = 0;
	int i = 0;
	for (i = 0; i < WAY_NUM_2; i++)
		if (L2[group].cache[i].valid && L2[group].cache[i].tag == tag)
		{
			Hit = 1;
			x = i;
			break;
		}

	//if (addr == 0x15d47e) printf("%x, %d, %d\n", addr, len, Hit);	
	
	if (!Hit)
	{
		for (i = 0; i < WAY_NUM_2; i++)
			if (!L2[group].cache[i].valid)
			{
				x = i;
				break;
			}
		if (i == WAY_NUM_2) {
			x = rand() % WAY_NUM_2;
		}
		if (L2[group].cache[x].dirty)	//If the substituted cache is dirty, then we should update the memory one byte by one.
			for (i = 0; i < BLOCK_SIZE_2; i++)
				dram_write(addr_block + i, 1, L1[group].cache[x].data[i]);
		L2[group].cache[x].valid = true;
		L2[group].cache[x].tag = tag;
		L2[group].cache[x].dirty = false;
		for (i = 0; i < BLOCK_SIZE_2; i++) 
			L2[group].cache[x].data[i] = dram_read(addr_block + i, 1) & (~0u >> ((4 - 1) << 3));
	}
	
	memcpy_cache (temp, L2[group].cache[x].data, BLOCK_SIZE_2);
	if (offset + len > BLOCK_SIZE_2) *(uint32_t*)(temp + BLOCK_SIZE_2) = Cache_2_read(addr_block + BLOCK_SIZE_2, len);

	return unalign_rw(temp + offset, 4);
}


static void Cache_2_write(hwaddr_t addr, size_t len, uint32_t data) 
{
	Cache_2 mirror;
	mirror.addr = addr;
	uint32_t offset = mirror.offset;
	uint32_t group = mirror.group;
	uint32_t tag = mirror.tag;
	hwaddr_t addr_block = addr & (~0u << BLOCK_BIT_2);

	if (offset + len - 1 > 0x3f) {
		int front = 0x3f - offset + 1;
		int back = len - front;
		uint32_t high = data >> (8 * front);
		uint32_t low = data & ((1 << (front * 8)) - 1);
		Cache_2_write(addr, front, low);
		Cache_2_write(addr + front, back, high);
		return;
	}

	int Hit = 0, x = 0;
	int i = 0;
	for (i = 0; i < WAY_NUM_2; i++)
		if (L2[group].cache[i].valid && L2[group].cache[i].tag == tag)
		{
			Hit = 1;
			x = i;
			break;
		}
		
	if (Hit)
	{
		uint8_t* datap = (uint8_t *)(&data);	//If it hits, then only the cache is updated, and the dirty byte is set.
		for (i = 0; i < len; i++)
			L2[group].cache[x].data[offset + i] = *(datap + i);
		L2[group].cache[x].dirty = true;
		/*if (addr == 0x15d47e) 
		{
			for (i = 0; i < len; i++)
			printf("%hx\n", L2[group].cache[x].data[offset+i]);
		}*/
	}
	else
	{
		dram_write(addr, len, data);	//Otherwise, the memory is updated first and then a cache is allocated for it.
		for (i = 0; i < WAY_NUM_2; i++)
			if (!L2[group].cache[i].valid)
			{
				x = i;
				break;
			}
		if (i == WAY_NUM_2) {
			x = rand() % WAY_NUM_2;
		}
		if (L2[group].cache[x].dirty)
			for (i = 0; i < BLOCK_SIZE_2; i++)
				dram_write(addr_block + i, 1, L1[group].cache[x].data[i]);
		L2[group].cache[x].valid = true;
		L2[group].cache[x].tag = tag;
		L2[group].cache[x].dirty = false;
		for (i = 0; i < BLOCK_SIZE_2; i++) 
			L2[group].cache[x].data[i] = dram_read(addr_block + i, 1) & (~0u >> ((4 - 1) << 3));
	}
}


