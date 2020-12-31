#include "cpu/exec/template-start.h"
#define instr bt

make_helper(concat(bt_i2rm_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	len += decode_i_b(eip + 1 + len);
	if (op_dest->type == OP_TYPE_REG)
	{
		uint32_t mask = 1;
		mask = mask << (op_src->val % (DATA_BYTE * 8));
		cpu.CF = !(!(op_dest->val & mask));
	}
	else
	{
		uint32_t addr = op_src->val / (DATA_BYTE * 8) + op_dest->addr;
		uint32_t offset = op_src->val % (DATA_BYTE * 8);
		cpu.CF = (swaddr_read(addr, DATA_BYTE, op_dest->sreg) >> offset) & 0x1;
	}
	return 1 + len;
}

make_helper(concat(bt_r2rm_, SUFFIX)) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (op_dest->type == OP_TYPE_REG)
	{
		uint32_t mask = 1;
		mask = mask << (op_src->val % (DATA_BYTE * 8));
		cpu.CF = !(!(op_dest->val & mask));
	}
	else
	{
		//In this case, the second operand indicates the offset in memory, not in the value of the first operand.
		uint32_t addr = op_src->val / (DATA_BYTE * 8) + op_dest->addr;
		uint32_t offset = op_src->val % (DATA_BYTE * 8);
		cpu.CF = (swaddr_read(addr, DATA_BYTE, op_dest->sreg) >> offset) & 0x1;
	}
	return 1 + len;
}

#include "cpu/exec/template-end.h"
