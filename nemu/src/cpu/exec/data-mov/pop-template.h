#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	
	swaddr_write(op_src->addr, DATA_BYTE, MEM_R(reg_l (R_ESP)));
	MEM_W(reg_l (R_ESP), 0);
	reg_l (R_ESP) += DATA_BYTE;
	print_asm_template1(); 
}

make_instr_helper(r)
#include "cpu/exec/template-end.h"
