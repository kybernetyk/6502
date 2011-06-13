/*
 * cpu.c
 * our virtual 8bit cpu (heavily inspired by 6502)
 */
#include <stdlib.h>
#include <memory.h>
#include "cpu.h"
#include <stdio.h>

cpu_t *cpu_new(void)
{
	cpu_t *c = malloc(sizeof(struct cpu_s));
	c->mem = malloc(sizeof(uint8_t) * 0xffff);	//64kb
	memset(c->mem, 0x00, sizeof(uint8_t) * 0xffff);
	c->x = c->y = c->acc = 0;
	c->ip = 0;
	c->sp = 0x01ff;
	return c;
}

void cpu_free(cpu_t *cpu)
{
	free(cpu->mem);
	free(cpu);
}

void cpu_exec(cpu_t *cpu, uint16_t base, void *opcds, size_t len)
{
	void **octbl[] = {
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 00h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 10h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 20h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 30h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	&&jmpabs,	NULL,		NULL,	NULL,		// 40h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 50h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	&&adcimm,	NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 60h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 70h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// 80h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		&&staabx,	NULL,	NULL,		// 90h
		NULL,	NULL,	&&ldximm,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	&&ldaimm,	NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// A0h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		&&ldaabx,	NULL,	NULL,		// B0h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// C0h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// D0h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	NULL,		// E0h
		NULL,	NULL,	NULL,		NULL,	NULL,	NULL,	NULL,	NULL,	NULL,	NULL,		NULL,	NULL,	NULL,		NULL,		NULL,	&&dmpcpu// F0h
	};

	memcpy((cpu->mem+base), opcds, len);

	uint8_t oc;
	uint8_t  *tmp8;
	uint16_t *tmp16;
	cpu->ip = base;
loop:
	if (cpu->ip >= len+base)
		goto end;
	oc = cpu->mem[cpu->ip];
	printf("\t[0x%x]\t", cpu->ip);
	cpu->ip++;

	//skip invalid opcodes
	if (octbl[oc] == 0x00) {
		goto loop;
	}

	goto *octbl[oc];

jmpabs: //jmp absolute to $(op1op2)
	printf("jmpabs ");
	tmp16 = cpu->mem+cpu->ip;
	cpu->ip = *tmp16;
	printf("[-> 0x%x]\n", cpu->ip);

	goto loop;

ldaimm: //load op1 into acc
	printf("ldaimm\n");
	cpu->acc = cpu->mem[cpu->ip];
	cpu->ip += 1;
	goto loop;

ldximm: //load op1 into x
	printf("ldximm\n");
	cpu->x = cpu->mem[cpu->ip];
	cpu->ip += 1;
	goto loop;

staabx: //store acc into $(op1op2) + x
	printf("staabx ");
	tmp16 = (uint16_t *)(cpu->mem+cpu->ip);
	cpu->mem[ *tmp16 + cpu->x ] = cpu->acc;

	printf("[0x%x + 0x%x <- %i]\n", *tmp16, cpu->x, cpu->acc);

	cpu->ip += 2;
	goto loop;

ldaabx: //load acc from $(op1op2) + x
	printf("ldaabx ");
	tmp16 = (uint16_t *)(cpu->mem+cpu->ip);
	cpu->acc = cpu->mem[ *tmp16 + cpu->x ];

	printf("[0x%x + 0x%x -> %i]\n", *tmp16, cpu->x, cpu->acc);


	cpu->ip += 2;
	goto loop;

adcimm: //add op1 to acc
	printf("adc ");
	printf("[%x] ", cpu->mem[cpu->ip]);
	cpu->acc += cpu->mem[cpu->ip];
	printf("-> [acc = %i]\n", cpu->acc);
	//mem[cpu->ip+1] = cpu->acc;
	cpu->ip += 1;
	goto loop;

dmpcpu: //dump cpu info
	printf("opcode 0xff dmpcpu:");
	printf("\n===========================================\n");
	printf("\tip: %.4xh\n", cpu->ip);
	printf("\tsp: %.2xh\n", cpu->sp);
	printf("\tstatus: %.2xh\n", cpu->status);
	printf("\n");
	printf("\tacc: %.2xh\n", cpu->acc);
	printf("\tx: %.2xh\n", cpu->x);
	printf("\ty: %.2xh\n", cpu->y);
	printf("===========================================\n");
	goto loop;

end:
	return;
}

