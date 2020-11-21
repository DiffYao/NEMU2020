#include "cpu/exec/intr.h"

extern jmp_buf jbuf;

void raise_intr(uint8_t NO) {

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eflags, 2);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.sreg[1].selector, 2);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip, 2);

	uint16_t idt_selector = lnaddr_read(cpu.idtr.base + 8 * NO + 2, 2);
	cpu.sreg[1].selector = idt_selector;
	cpu.sreg[1].cache.base_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 2, 2);
	cpu.sreg[1].cache.base_23_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 4, 1);
	cpu.sreg[1].cache.base_31_24 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 7, 1);
	cpu.sreg[1].cache.limit_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX, 2);
	cpu.sreg[1].cache.limit_19_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 6, 1) & 0xf;

	uint32_t base_addr = (cpu.sreg[1].cache.base_31_24 << 24) | (cpu.sreg[1].cache.base_23_16 << 16) | cpu.sreg[1].cache.base_15_0;
	uint16_t offset1 = lnaddr_read(cpu.idtr.base + 8 * NO, 2);
	uint16_t offset2 = lnaddr_read(cpu.idtr.base + 8 * NO + 6, 2);
	uint32_t offset_total = (offset2 << 16) | offset1;

	cpu.eip = base_addr + offset_total;

	longjmp(jbuf, 1);
}