#include "chip.h"
#include <string.h>

const size_t MAX_ROM_SIZE = 4096-512;

static const u8 fontset[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void initCpu(CPU *cpu){
	memset(cpu, 0, sizeof(CPU));
	cpu->PC = START_ADR;
	for(int i = 0; i < 80; i++){
		cpu->mem[0x050+i] = fontset[i];
	}	
}

int loadRom(CPU *cpu, const char* filename){
	FILE *fp = fopen(filename, "rb");
	if(fp == NULL){
		perror("Error opening file. ");
		return EXIT_FAILURE;
	}

	size_t r = fread(&cpu->mem[START_ADR], 1, MAX_ROM_SIZE, fp);
	if(r < MAX_ROM_SIZE){
		if(ferror(fp)){
			perror("Stream error occured during read.\n");
		} else if(feof(fp)){
			printf("End of file reached. Read %zu items.\n", r);
		}
	} else{
		printf("Successfully read all %zu items.\n", MAX_ROM_SIZE);
	}

	fclose(fp);
	return EXIT_SUCCESS;
}

void cycle(CPU *cpu){
	// fetch
	u16 opcode = (cpu->mem[cpu->PC] << 8) | cpu->mem[cpu->PC+1];
	cpu->PC += 2;

	// decode
	u8 fn = (opcode & 0xF000) >> 12;

	u8 x = (opcode & 0x0F00) >> 8;	// register VX
	u8 y = (opcode & 0x00F0) >> 4;	// register VY
	u8 n = opcode & 0x000F;			// 4 bit number
	u8 nn = opcode & 0x00FF;		// 8 bit number
	u16 nnn = opcode & 0x0FFF;		// 12 bit number

	// execute
	switch(fn){
		case 0x0:
			// 00E0: clear screen
			if(opcode == 0x00E0){
				memset(cpu->display, 0, sizeof(cpu->display))
			} else if(opcode == 0x00EE){
			}
			break;
		case 0x1:
			// 1NNN: jump to address nnn
			cpu->PC = nnn;
			break;	
		case 0x2:
			// 2NNN: calls subroutine

	}
}
