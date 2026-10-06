#include <stdio.h>
#include "chip.h"

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s <path_to_ROM>\n", argv[0]);
		return 1;
	}
	CPU chip;
	initCpu(&chip);
	loadRom(&chip, argv[1]);
}
