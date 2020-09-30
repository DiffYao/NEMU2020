#include "cpu/exec/template-start.h"

#define instr jne

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	print_asm("jne %x", cpu.eip + displacement + DATA_BYTE + 1);
	if (cpu.ZF == 0) cpu.eip += displacement;
	

}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
