OBJDIR=obj

$(OBJDIR)/%.o: %.c
	gcc -g -c -o $@ $<

matlibtest: $(OBJDIR)/matrix_lib.o
	gcc -Wall -Werror -g -o matrix_lib matrix_lib.c