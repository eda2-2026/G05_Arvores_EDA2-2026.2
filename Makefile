CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude
SRC = src/main.c src/medicamento.c src/avl.c src/estoque.c src/csv.c src/relatorios.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean test
all: sistema

sistema: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

test_avl: tests/test_avl.c src/medicamento.c src/avl.c src/estoque.c
	$(CC) $(CFLAGS) -o $@ tests/test_avl.c src/medicamento.c src/avl.c src/estoque.c

test: test_avl
	./test_avl

clean:
	rm -f $(OBJ) sistema test_avl
