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

lab6: $(OBJDIR)/lab6.o
	gcc -g -o lab6 $(LIBDIR)/matrix_lib.c $(LIBDIR)/initShader.c $(LIBDIR)/transformations.c lab6.c $(LIBFLAGS)

lab7: $(OBJDIR)/lab7.o
	gcc -g -o lab7 $(LIBDIR)/matrix_lib.c $(LIBDIR)/initShader.c $(LIBDIR)/transformations.c lab7.c $(LIBFLAGS)

proj1: $(OBJDIR)/project1.o
	gcc -g -o project1 $(LIBDIR)/matrix_lib.c $(LIBDIR)/initShader.c $(LIBDIR)/transformations.c project1.c $(LIBFLAGS)

proj2: $(OBJDIR)/project2.o
	gcc -g -o project2 $(LIBDIR)/matrix_lib.c $(LIBDIR)/initShader.c $(LIBDIR)/transformations.c $(LIBDIR)/maze_helper.c $(LIBDIR)/view.c project2.c $(LIBFLAGS)