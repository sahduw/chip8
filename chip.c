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
				memset(cpu->display, 0, sizeof(cpu->display));
			} // 0x00EE: return from a subroutine
			else if(opcode == 0x00EE){
				cpu->PC = cpu->stack[--cpu->SP];
			}
			break;
		case 0x1:
			// 1NNN: jump to address nnn
			cpu->PC = nnn;
			break;	
		case 0x2:
			// 2NNN: calls subroutine
			cpu->stack[cpu->SP++] = cpu->PC;
			cpu->PC = nnn;
			break;
		case 0x3:
			// 3XNN: skip if VX = NN
			if(VX == nn){
				cpu->PC += 2;
			}
			break;
		case 0x4:
			// 4XNN: skip if VX != NN
			if(VX != nn){
				cpu->PC += 2;
			}
			break;
		case 0x5:
			// 5XY0: skip if VX = VY
			if(VX == VY){
				cpu->PC += 2;
			}
			break;
		case 0x6:
			// 6XNN: sets VX to NN
			VX = nn;
			break;
		case 0x7:
			// 7XNN: adds NN to VX (overflow not accounted)
			VX += nn;
			break;
		case 0x8:
			// 8XYN: N decides the operation
			switch(n){
				case 0: // 8XY0
					VX = VY;
					break;
				case 1: // 8XY1
					VX = VX | VY;
					break;
				case 2: // 8XY2
					VX = VX & VY;
					break;
				case 3: // 8XY3
					VX = VX ^ VY;
					break;
				case 4: // 8XY4
					u16 sum = VX + VY;
					if(sum > 255){
						VX = sum;
						cpu->V[F] = 1;
					} else{
						VX = sum;
						cpu->V[F] = 0;
					}
					break;
				case 5: // 8XY5
					if(VX >= VY){
						cpu->V[F] = 1;
						VX -= VY;
					} else{
						cpu->V[F] = 0;
						VX -= VY;
					}
					break;
				case 6: // 8XY6
					cpu->V[F] = VX & 1;
					VX >>= 1;
					break;
				case 7: //8XY7
					if(VY >= VX){
						cpu->[F] = 1;
						VY -= VX;
					} else{
						cpu->[F] = 0;
						VY -= VX;
					}
					break;
				case E: // 8XYE
					cpu->V[F] = (VX & 0x80) >> 7;
					VX <<= 1;
					break;
			}
		case 0x9:
			// 9XY0: opposite of 5XY0
			if(VX != VY){
				cpu->PC += 2;
			}
			break;
		case 0xA:
			// ANNN: set i pointer
			cpu->I = nnn;
			break;
		case 0xB:
			// BNNN: jump to nnn plus address in V0
			cpu->PC = nnn + cpu->V[0];
		case 0xC:
			// CXNN: random number
			VX = (rand() % 255) & nn;
			break;
	}
}
