#include "cpu/exec/template-start.h"

#define instr pusha

make_helper(concat(pusha_, SUFFIX)) {
	DATA_TYPE temp = (DATA_TYPE)cpu.esp;
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(0), 2);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(1), 2);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(2), 2);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(3), 2);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, temp, 2);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(5), 2);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(6), 2);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, REG(7), 2);
	print_asm("pusha");
	
	return 1;
}

#include "cpu/exec/template-end.h"