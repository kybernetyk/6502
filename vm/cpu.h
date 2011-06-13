#pragma once
#include <stdint.h>

typedef struct cpu_s {
	uint8_t	acc, x, y, sp;
	uint8_t status;
	uint16_t ip;

	uint8_t *mem;
} cpu_t;

extern cpu_t *cpu_new(void);
extern void cpu_free(cpu_t *cpu);

extern void cpu_exec(cpu_t *cpu, uint16_t base, void *opcds, size_t len);

