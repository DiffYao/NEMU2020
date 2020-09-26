#include "cpu/exec/template-start.h"

#define instr jna

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	
	if (cpu.CF == 1 || cpu.ZF == 1) cpu.eip += displacement;
	print_asm("jne %x", cpu.eip + displacement + 1);

}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
