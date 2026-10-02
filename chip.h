
#ifndef CHIP_H
#define CHIP_H 

#include <stdint.h>
#define u8 uint8_t
#define u16 uint16_t

extern const u8 fontset[80];

typedef struct{
	// 4096 bytes of memory
	u8 memory[4096];

	// 16 8bytes registers
	u8 V[16];

	u16 stack[16];
	u16 PC; // program counter
	u16 I; // address pointer
	u8 SP; // stack pointer
	u8 delayReg; // delay timer
	u8 soundReg; // sound timer
	u8 display[64*32]; // 64 by 32 display
	u8 keypads[16];
} CPU;

// set the initial state of pc, font, etc
void initCpu(CPU *cpu);
// load the binary game file to the memory/cpu
int loadRom(CPU *cpu, const char* filename);

#endif
