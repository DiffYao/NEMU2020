#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	
	DATA_TYPE_S displacement = op_src->val;
	print_asm("jmp %x", cpu.eip + displacement + 1);
	cpu.eip += displacement;

}

make_instr_helper(i);
make_instr_helper(rm);

#if DATA_BYTE != 1
make_helper(concat(jmp_rmm_, SUFFIX)){

	int len = concat(decode_i_, SUFFIX)(eip+1);
	swaddr_t temp_addr = op_src->val;
	DATA_TYPE_S displacement = MEM_R(temp_addr);	
	print_asm("jmp %x", cpu.eip + displacement + 1);
	return len;

}

#endif

#include "cpu/exec/template-end.h"
