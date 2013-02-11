CC=gcc
FLAGS=-O0 -ggdb3 -Wall -Werror

all:
	$(CC) $(FLAGS) structs.c functions.c main.c -o 537sh

clean:
	rm -rf 537sh

