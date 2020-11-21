#include "cpu/exec/template-start.h"

#define instr popa

make_helper(concat(popa_, SUFFIX)) {
	
	REG(7) = MEM_R(cpu.esp, 2);
    cpu.esp += DATA_BYTE;
    REG(6) = MEM_R(cpu.esp, 2);
    cpu.esp += DATA_BYTE;
	REG(5) = MEM_R(cpu.esp, 2);
    cpu.esp += DATA_BYTE;
    REG(3) = MEM_R(cpu.esp, 2);
    cpu.esp += DATA_BYTE;
    REG(2) = MEM_R(cpu.esp, 2);
    cpu.esp += DATA_BYTE;
    REG(1) = MEM_R(cpu.esp, 2);
    cpu.esp += DATA_BYTE;
    REG(0) = MEM_R(cpu.esp, 2);
    cpu.esp += DATA_BYTE;
    print_asm("pusha");
	
	return 1;
}

#include "cpu/exec/template-end.h"