OBJDIR=obj
LIBDIR=lib

$(OBJDIR)/%.o: %.c
	gcc -g -c -o $@ $<

maintest: $(OBJDIR)/main_test.o
	gcc -g -o main_test $(LIBDIR)/matrix_lib.c main_test.c

lab1: $(OBJDIR)/lab1.o
	gcc -g -o lab1 $(LIBDIR)/matrix_lib.c lab1.c