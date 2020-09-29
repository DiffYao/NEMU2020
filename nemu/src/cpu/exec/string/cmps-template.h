#include "cpu/exec/template-start.h"

#define instr cmps

static void do_execute() {
	op_src->type = op_dest->type = OP_TYPE_REG;
	op_src->reg = R_ESI; op_dest->reg = R_EDI;

	snprintf(op_src->str, 11, "(%%edi)");
	snprintf(op_dest->str, 11, "(%%esi)");
	op_src->val = swaddr_read(cpu.edi, DATA_BYTE);
	op_dest->val = swaddr_read(cpu.esi, DATA_BYTE);

	DATA_TYPE result = op_dest->val - op_src->val;
	
	if(MSB(op_dest->val) != MSB(op_src->val) && MSB(result) != MSB(op_dest->val))
		cpu.OF = 1; else cpu.OF = 0;

	cpu.CF = op_dest->val < op_src->val;
	cpu.SF = MSB(result);
	cpu.ZF = !result;

	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	
	cpu.edi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	cpu.esi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
