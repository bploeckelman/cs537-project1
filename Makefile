CC=gcc
FLAGS=-O0 -ggdb3 -Wall -Werror

all:
	$(CC) $(FLAGS) main.c -o 537sh

clean:
	rm -rf 537sh

