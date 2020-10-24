#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

static PTE table[NR_PTE] align_to_page;

/* Use the function to get the start address of user page directory. */
PDE* get_updir();

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	//panic("please implement me");
	memset(table, 0, NR_PTE * sizeof(PTE));
	PDE* updir = get_updir();
	updir[(VMEM_ADDR >> 22) & 0x3ff].val = make_pde(va_to_pa(table));
	int start = (VMEM_ADDR >> 12) & 0x3ff;
	int i = 0;
	for (; i < SCR_SIZE / PAGE_SIZE + 1; i++) {
		table[start + i].val = make_pte(VMEM_ADDR + PAGE_SIZE * i);
	}


}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

