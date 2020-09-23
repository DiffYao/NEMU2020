#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	
	if (cpu.ZF == 1|| cpu.CF == 1) cpu.eip += displacement;
	print_asm("jbe %x", cpu.eip + displacement + 1);

}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
