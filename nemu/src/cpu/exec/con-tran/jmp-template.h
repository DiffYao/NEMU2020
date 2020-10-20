#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	
	DATA_TYPE displacement = op_src->val;
	if (op_src->type == OP_TYPE_MEM || op_src->type == OP_TYPE_REG){
		cpu.eip = displacement - concat(decode_rm_, SUFFIX)(cpu.eip+1) -1;
		print_asm_template1();
	}
	else {
		cpu.eip += (DATA_TYPE_S)displacement;
		print_asm("jmp %x", cpu.eip + 1 + DATA_BYTE);
	}
	
}

make_instr_helper(i)
make_instr_helper(rm)


#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_l_, SUFFIX)) {

	swaddr_t offset = instr_fetch(eip + 1, DATA_BYTE);
	uint16_t cs_new = instr_fetch(eip + 5, 2);

	cpu.eip = offset - DATA_BYTE - 3;
#if DATA_BYTE == 2
	cpu.eip = cpu.eip & 0x0000ffff;
#endif
	cpu.sreg[1].selector = cs_new;
	cpu.sreg[1].cache.base_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 2, 2);
	cpu.sreg[1].cache.base_23_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 4, 1);
	cpu.sreg[1].cache.base_31_24 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 7, 1);
	cpu.sreg[1].cache.limit_15_0 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX, 2);
	cpu.sreg[1].cache.limit_19_16 = lnaddr_read(cpu.gdtr.base + 8 * cpu.sreg[1].INDEX + 6, 1) & 0xf;

	print_asm("ljmp $%hx,$%x", cs_new, offset);
	return DATA_BYTE + 3;

}
#endif

#include "cpu/exec/template-end.h"
