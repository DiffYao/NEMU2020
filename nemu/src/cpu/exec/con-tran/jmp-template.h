#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	if (op_src->type == OP_TYPE_MEM){
		cpu.eip = displacement - concat(decode_rm_, SUFFIX)(cpu.eip+1) -1;
		
	}
	else {
		print_asm("jmp %x", cpu.eip + displacement + 1);
		cpu.eip += displacement;
	}
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
