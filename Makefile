%.o: %.c
	gcc -g -c -o $@ $<

matlibtest: matrix_lib.o
	gcc -Wall -Werror -g -o matrix_lib matrix_lib.c