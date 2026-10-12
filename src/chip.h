
#ifndef CHIP_H
#define CHIP_H 

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#define u8 uint8_t
#define u16 uint16_t
#define START_ADR 0x200
#define VX cpu->V[x]
#define VY cpu->V[y]

typedef struct{
	u8 V[16];		// 16 registers from V0 to VF
	u16 PC;			// program counter (or instruction pointer)
	u8 mem[4096];	// 4096 bytes of memory
	u16 I;			// address pointer
	u16 stack[16];
	u8 SP;			// stack pointer
	u8 delayReg;	// delay timer
	u8 soundReg;	// sound timer

	u8 display[64*32];
	u8 keypads[16];
} CPU;

void initCpu(CPU *cpu);

// load the game file into the memory
int loadRom(CPU *cpu, const char* filename);

// fetch/decode/execute cycle
// this is a bit tricky when i was first learning about this
// tldr: after extracting each nibble (aka decoding it),
// if the first nibble is 1, it is a jump instruction, and 1NNN will be its format
void cycle(CPU *cpu);

#endif
