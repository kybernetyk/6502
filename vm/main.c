#include <stdio.h>
#include "cpu.h"

int main(int argc, char **argv)
{
	cpu_t *mycpu = cpu_new();

	uint8_t program[] = {0xa9, 0x00,							//lda #$00
	                     0xa2, 0x1f,							//ldx #$1f
	                     0x9d, 0x00, 0x30,				//sta $3000,x
	                     0xbd, 0x00, 0x30,				//lda $3000,x   <-- $2007
	                     0x69, 0x01,							//adc #$01
	                     0x9d, 0x00, 0x30,				//sta $3000,x
	                     0xff,										//cpudmp
	                     0x4c, 0x07, 0x20,				//jmp $2007
	                    };

	cpu_exec(mycpu, 0x2000, program, sizeof(program));

	cpu_free(mycpu);


	return 0;
}
