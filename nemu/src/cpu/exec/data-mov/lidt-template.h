#include "cpu/exec/template-start.h"
#include <stdio.h>
#define instr lidt

make_helper(concat(lidt_, SUFFIX)) {
	int len = decode_rm2r_l(eip + 1);
	cpu.idtr.limit = lnaddr_read(op_src->addr, 2);
	if (DATA_BYTE == 2) 
		cpu.idtr.base = lnaddr_read(op_src->addr + 2, 3);
	else 
		cpu.idtr.base = lnaddr_read(op_src->addr + 2, 4);

	print_asm("lidt 0x%hx 0x%x", cpu.idtr.limit, cpu.idtr.base);
	return len + 1;
}

#include "cpu/exec/template-end.h"