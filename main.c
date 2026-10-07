#include "chip.h"

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s <path_to_ROM>\n", argv[0]);
		return 1;
	}
	CPU chip;
	initCpu(&chip);
	if(loadRom(&chip, argv[1]) == EXIT_SUCCESS){
		printf("ROM successfully loaded into memory starting at 0x%03X.\n", START_ADR);
		printf("First 8 bytes of ROM in RAM: ");
		for(int i = 0; i < 8; i++){
			printf("%02X ", chip.mem[START_ADR + i]);
		}
		printf("\n");
	} else{
		printf("Failed to load ROM. ");
	}
}
