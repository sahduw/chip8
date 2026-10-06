
#ifndef CHIP_H
#define CHIP_H 

#include <stdint.h>
#define u8 uint8_t
#define u16 uint16_t

extern const u8 fontset[80];

typedef struct{
	u8 V[16];			// 16 registers from V0 to VF
	u16 PC;				// program counter
	u8 mem[4096];		// 4096 bytes of memory
	u16 I;				// address pointer
	u16 stack[16];
	u8 SP;				// stack pointer
	u8 delayReg;		// delay timer
	u8 soundReg;		// sound timer

	u8 display[64*32];
	u8 keypads[16];
} CPU;

void initCpu(CPU *cpu);

// load the binary game file to the memory
int loadRom(CPU *cpu, const char* filename);

#endif
