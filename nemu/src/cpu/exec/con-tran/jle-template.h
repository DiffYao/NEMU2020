#include "cpu/exec/template-start.h"

#define instr jle

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	
	if (cpu.ZF == 1|| cpu.SF != cpu.OF) cpu.eip += displacement;
	print_asm("jle %x", cpu.eip + displacement + 1);

}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
