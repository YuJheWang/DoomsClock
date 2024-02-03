all : glad.lib

glad.lib : glad.o
	ar crf libglad.a glad.o

glad.o : src\glad.c
	gcc -c src\glad.c -I include