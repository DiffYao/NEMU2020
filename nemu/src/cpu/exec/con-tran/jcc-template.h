#include "cpu/exec/template-start.h"

#define instr jcc

#define for_2_bit (DATA_BYTE!=1)

#if DATA_BYTE == 1 || DATA_BYTE == 4
swaddr_t concat(next_addr_,SUFFIX)(swaddr_t eip) {
	concat(decode_si_, SUFFIX)(eip + 1);
	return (swaddr_t)(eip + 1 + DATA_BYTE + op_src->simm);
}
#else
swaddr_t concat(next_addr_,SUFFIX)(swaddr_t eip) {
	return (eip + 1 + DATA_BYTE + instr_fetch(eip + 1, DATA_BYTE)) & 0x0000ffff;
}
#endif


make_helper(concat(jcc_e_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("je %x", cur_addr);
	if (cpu.ZF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_a_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("ja %x", cur_addr);
	if (cpu.CF == 0 && cpu.ZF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_ae_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jae %x", cur_addr);
	if (cpu.CF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_b_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jb %x", cur_addr);
	if (cpu.CF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_be_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jbe %x", cur_addr);
	if (cpu.ZF == 1 || cpu.CF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_ecxz_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jecxz %x", cur_addr);
	if (cpu.gpr[1]._32 == 0) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_g_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jg %x", cur_addr);
	if (cpu.ZF == 0 && cpu.SF == cpu.OF) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_ge_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jge %x", cur_addr);
	if (cpu.SF == cpu.OF) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_l_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jl %x", cur_addr);
	if (cpu.SF != cpu.OF) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_le_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jle %x", cur_addr);
	if (cpu.ZF == 1 || cpu.SF != cpu.OF) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_ne_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jne %x", cur_addr);
	if (cpu.ZF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_no_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jno %x", cur_addr);
	if (cpu.OF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_np_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jnp %x", cur_addr);
	if (cpu.PF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_ns_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jns %x", cur_addr);
	if (cpu.SF == 0) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_o_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jo %x", cur_addr);
	if (cpu.OF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_p_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("jp %x", cur_addr);
	if (cpu.PF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

make_helper(concat(jcc_s_,SUFFIX)) {
	swaddr_t cur_addr = concat(next_addr_,SUFFIX)(eip);
	print_asm("js %x", cur_addr);
	if (cpu.SF == 1) cpu.eip = cur_addr - (1 + DATA_BYTE) - for_2_bit;
	return DATA_BYTE + 1;	
}

#include "cpu/exec/template-end.h"
