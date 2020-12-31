#include "cpu/exec/template-start.h"

#define instr setcc

make_helper(setcc_a) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.CF == 0 && cpu.ZF == 0) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("seta %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_ae) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.CF == 0) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setae %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_b) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.CF == 1) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setb %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_be) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.CF == 1 || cpu.ZF == 1) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setbe %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_e) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.ZF == 1) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("sete %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_g) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.ZF == 0 && cpu.SF == cpu.OF) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setg %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_ge) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.SF == cpu.OF) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setge %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_l) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.SF != cpu.OF) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setl %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_le) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.ZF == 1 || cpu.SF != cpu.OF) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setle %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_ne) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.ZF == 0) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setne %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_no) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.OF == 0) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setno %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_np) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.PF == 0) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setnp %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_ns) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.SF == 0) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setns %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_o) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.OF == 1) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("seto %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_p) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.PF == 1) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("setp %s", op_dest->str);
	return 1 + len;
}

make_helper(setcc_s) {
	int len = concat(decode_r2rm_, SUFFIX)(eip + 1);
	if (cpu.SF == 1) OPERAND_W(op_dest, 1);
	else OPERAND_W(op_dest, 0);
	print_asm("sets %s", op_dest->str);
	return 1 + len;
}

#include "cpu/exec/template-end.h"
