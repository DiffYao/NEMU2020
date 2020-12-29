#include "cpu/exec/template-start.h"
#define instr mov


static void do_execute() {

	printf("val = 0x%x, su is %d\n", op_src->val, DATA_BYTE);
	printf("str = %s ", op_src->str);
	printf("type = %d ", op_src->type);
	printf("size = %lu ", op_src->size);
	printf("sreg = %d \n", op_src->sreg);

	printf("str = %s ", op_dest->str);
	printf("type = %d ", op_dest->type);
	printf("size = %lu ", op_dest->size);
	printf("sreg = %d \n", op_dest->sreg);
	OPERAND_W(op_dest, op_src->val);
	uint32_t ad = 0xc012183e;
	
	printf("value = 0x%x \n", swaddr_read(ad, 4, 3));
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

//make_helper(mov_r2rm)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX), 3);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, 3);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE == 4

make_helper(mov_cr2r){
	int r_num = instr_fetch(eip + 1, 1);;
	if (((r_num >> 3) & 0x7) == 0){
		reg_l(r_num & 0x7) = cpu.cr0.val;
		print_asm("mov %%cr0,%s", REG_NAME(r_num & 0x7));
	}
	else
	{
		reg_l(r_num & 0x7) = cpu.cr0.val;
		print_asm("mov %%cr0,%s", REG_NAME(r_num & 0x7));
	}
	
	return 2;
}

make_helper(mov_r2cr){
	int r_num = instr_fetch(eip + 1, 1);
	if (((r_num >> 3) & 0x7) == 0){
		cpu.cr0.val = reg_l(r_num & 0x7);
		print_asm("mov %s,%%cr0", REG_NAME(r_num & 0x7));
	}
	else
	{
		cpu.cr3.val = reg_l(r_num & 0x7);
		init_TLB();
		print_asm("mov %s,%%cr3", REG_NAME(r_num & 0x7));
	}
	return 2;
}

#endif

#if DATA_BYTE == 2
make_helper(mov_rm2s) {
	int len = decode_r2rm_w(eip + 1);
	cpu.sreg[op_src->reg].selector = op_dest->val & 0xffff;
	cpu.sreg[op_src->reg].cache.base_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[op_src->reg].INDEX + 2, 2);
	cpu.sreg[op_src->reg].cache.base_23_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[op_src->reg].INDEX + 4, 1);
	cpu.sreg[op_src->reg].cache.base_31_24 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[op_src->reg].INDEX + 7, 1);
	cpu.sreg[op_src->reg].cache.limit_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[op_src->reg].INDEX, 2);
	cpu.sreg[op_src->reg].cache.limit_19_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[op_src->reg].INDEX + 6, 1) & 0xf;
	switch (op_src->reg) {
		case 0: print_asm("mov %s,%%es", op_dest->str); break;
		case 1: print_asm("mov %s,%%cs", op_dest->str); break;
		case 2: print_asm("mov %s,%%ss", op_dest->str); break;
		case 3: print_asm("mov %s,%%ds", op_dest->str); break;
		default: assert(0);
	}
	return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
