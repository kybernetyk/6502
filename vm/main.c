#include <stdio.h>
#include "cpu.h"

int main(int argc, char **argv)
{
	cpu_t *mycpu = cpu_new();

	uint8_t program[] = {0xa9, 0x00,
	                     0xa2, 0x1f,
	                     0x9d, 0x00, 0x30,
	                     0xbd, 0x00, 0x30,
	                     0x69, 0x01,
	                     0x9d, 0x00, 0x30,
	                     0xff,
	                     0x4c, 0x07, 0x20,
	                    };

	cpu_exec(mycpu, 0x2000, program, sizeof(program));

	cpu_free(mycpu);


	return 0;
}
