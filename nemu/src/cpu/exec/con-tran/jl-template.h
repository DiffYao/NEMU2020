#include "cpu/exec/template-start.h"

#define instr jl

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	
	if (cpu.SF != cpu.OF) cpu.eip += displacement;
	print_asm("je %x", cpu.eip + displacement + 1);

}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
