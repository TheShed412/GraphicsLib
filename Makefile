OBJDIR=obj
LIBDIR=lib
LIBFLAGS=-lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL

$(OBJDIR)/%.o: %.c
	gcc -g -c -o $@ $<

maintest: $(OBJDIR)/main_test.o
	gcc -g -o main_test $(LIBDIR)/matrix_lib.c $(LIBDIR)/transformations.c main_test.c

lab1: $(OBJDIR)/lab1.o
	gcc -g -o lab1 $(LIBDIR)/matrix_lib.c lab1.c

lab3: $(OBJDIR)/lab3.o
	gcc -g -o lab3 $(LIBDIR)/matrix_lib.c $(LIBDIR)/initShader.c $(LIBDIR)/transformations.c lab3.c $(LIBFLAGS)

lab4: $(OBJDIR)/lab4.o
	gcc -g -o lab4 $(LIBDIR)/matrix_lib.c $(LIBDIR)/initShader.c $(LIBDIR)/transformations.c lab4.c $(LIBFLAGS)

lab5: $(OBJDIR)/lab5.o
	gcc -g -o lab5 $(LIBDIR)/matrix_lib.c $(LIBDIR)/initShader.c $(LIBDIR)/transformations.c lab5.c $(LIBFLAGS)