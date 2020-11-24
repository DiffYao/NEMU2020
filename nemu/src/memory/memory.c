#include "common.h"
#include "device/mmio.h"
#include "cpu/reg.h"
#include <stdlib.h>

/* Cache accessing interfaces */
uint32_t cache1_read(hwaddr_t, size_t);
void cache1_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* TLB accessing interfaces*/
uint32_t TLB_translate(lnaddr_t);
void TLB_update(lnaddr_t, hwaddr_t);

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {

	if (is_mmio(addr) == -1)
	{
		return cache1_read(addr, len) & (~0u >> ((4 - len) << 3));
	}
	else 
	{
		return mmio_read(addr, len, is_mmio(addr)) & (~0u >> ((4 - len) << 3));
	}
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	
	if (is_mmio(addr) == -1)
	{
		return cache1_write(addr, len, data);
	}
	else 
	{
		return mmio_write(addr, len, data, is_mmio(addr));
	}
		
}

hwaddr_t page_translate(lnaddr_t addr) {
	hwaddr_t hwaddr = addr;
	if (cpu.cr0.paging == 1 && cpu.cr0.protect_enable == 1) {
		hwaddr = TLB_translate(addr);
		if (!(~hwaddr))
		{
			uint32_t dir = addr >> 22;
			uint32_t page = (addr >> 12) & 0x000003ff;
			uint32_t offset = addr & 0xfff;
			hwaddr_t page_directory = cpu.cr3.page_directory_base;
			page_directory = (page_directory << 12) | (dir << 2);
			PDE pde;
			pde.val = hwaddr_read(page_directory, 4);
			//assert(pde.present == 1);
			hwaddr_t page_entry = pde.page_frame;
			page_entry = (page_entry << 12) | (page << 2);
			PTE pte;
			pte.val = hwaddr_read(page_entry, 4);
			//assert(pte.present == 1);
			hwaddr = offset | (pte.page_frame << 12);
			TLB_update(addr, hwaddr);
		}
		else
		{
			uint32_t offset = addr & 0xfff;
			hwaddr = (hwaddr & 0xfffff000) | offset;
		}
	}
	return hwaddr;
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {

	assert(len == 1 || len == 2 || len == 4);
	if ((addr & 0xfff) + len - 1 > 0xfff) {
		int front = 0xfff - (addr & 0xfff) + 1;
		int back = len - front;
		uint32_t low = hwaddr_read(page_translate(addr), front);
		uint32_t high = hwaddr_read(page_translate(addr + front), back);
		return (high << (front * 8)) | low;
	}
	else {
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {

	if ((addr & 0xfff) + len - 1 > 0xfff) {
		int front = 0xfff - (addr & 0xfff) + 1;
		int back = len - front;
		uint32_t high = data >> (8 * front);
		uint32_t low = data & ((1 << (front * 8)) - 1);
		hwaddr_write(page_translate(addr), front, low);
		hwaddr_write(page_translate(addr + front), back, high);
	}
	else {
		hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(hwaddr, len, data);
	}
	
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {

	assert(sreg == 0 || sreg == 1 || sreg == 2 || sreg == 3);
	lnaddr_t ret_addr = addr;
	if (cpu.cr0.protect_enable == 1) {
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

