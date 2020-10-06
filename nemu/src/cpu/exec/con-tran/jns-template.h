#include "cpu/exec/template-start.h"

#define instr jns

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	print_asm("jns %x", cpu.eip + displacement + 1 + DATA_BYTE);
	if (cpu.SF == 0) cpu.eip += displacement;
	

}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
