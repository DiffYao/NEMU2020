#include "cpu/exec/template-start.h"

#define instr jns

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	
	if (cpu.SF == 1) cpu.eip += displacement;
	print_asm("js %x", cpu.eip + displacement + 1);

}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
