#pragma once
#include <stdint.h>

typedef struct cpu_s {
	uint16_t pc;
	uint8_t	acc, x, y;
	uint8_t sp;

	union {
		uint8_t status;

		struct {
			uint8_t carry_flag: 1;
			uint8_t zero_flag: 1;
			uint8_t interrupt_disable: 1;
			uint8_t decimal_mode: 1;
			uint8_t break_command: 1;
			uint8_t overflow_flag: 1;
			uint8_t negative_flag: 1;
		};
	};

	uint8_t *mem;
} cpu_t;

extern cpu_t *cpu_new(void);
extern void cpu_free(cpu_t *cpu);

extern void cpu_exec(cpu_t *cpu, uint16_t base, void *opcds, size_t len);

