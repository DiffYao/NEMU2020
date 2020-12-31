#include "cpu/exec/helper.h"

void raise_intr(uint8_t NO);

make_helper(int_i) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	cpu.eip += 2;
	print_asm("int $0x%x", imm);
	raise_intr(imm);
	return 0;
}