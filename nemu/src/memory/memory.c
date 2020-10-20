#include "common.h"
#include "cpu/reg.h"
#include <stdlib.h>

/* Cache accessing interfaces */
uint32_t cache1_read(hwaddr_t, size_t);
void cache1_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);


uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	//return cache1_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	return dram_write(addr, len, data);
	//return cache1_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
	assert(sreg == 0 || sreg == 1 || sreg == 2 || sreg == 3);
	lnaddr_t ret_addr = addr;
	if (cpu.PE == 1) {
		ret_addr += (cpu.sreg[sreg].cache.base_15_0 | (cpu.sreg[sreg].cache.base_23_16 << 16) | (cpu.sreg[sreg].cache.base_31_24 << 24));
	}
	return ret_addr;	
}



uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {

#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

