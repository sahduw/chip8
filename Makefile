CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lraylib -lm

SRC = $(wildcard src/*.c)
OUT = chip8

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
