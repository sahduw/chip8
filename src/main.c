#include <time.h>
#include <raylib.h>
#include "chip.h"

const int scale = 10;
const int clockspeed = 700 / 60;

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: %s <path_to_ROM>\n", argv[0]);
		return 1;
	}

	srand(time(NULL));
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

	InitWindow(64*scale, 32*scale, "Chip 8 maybe");
	SetTargetFPS(60);
	while(!WindowShouldClose()){
		for(int i = 0; i < clockspeed; i++){
			cycle(&chip);
		}
		if(chip.delayReg > 0){
			chip.delayReg--;
		}
		if(chip.soundReg > 0){
			printf("\a");
			chip.soundReg--;
		}

		BeginDrawing();

		ClearBackground(BLACK);

		// grid logic
		for(int y = 0; y < 32; y++){
			for(int x = 0; x < 64; x++){
				if(chip.display[y*64+x] == 1){
					DrawRectangle(x*scale, y*scale, scale, scale, WHITE);
				}
			}
		}

		EndDrawing();
	}
	CloseWindow();
}
