
#ifndef CHIP_H
#define CHIP_H 

#include <stdint.h>
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

extern const u8 fontset[80];

typedef struct{
	u8 memory[4096];
	u8 display[64*32];
	u16 PC;
	u16 I;
	u16 stack[16];
	u8 SP;
	u8 delay;
	u8 sound;
	u8 registers[16];
	u8 keypads[16];
} CPU;

void init_cpu(CPU *cpu);

#endif
