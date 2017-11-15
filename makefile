FLAGS=-ggdb -c -o

all: main

main: output/main.o output/bmp_func.o output/sepia_func.o output/sepia_sse_func.o
	gcc -ggdb -o main output/main.o output/bmp_func.o output/sepia_func.o output/sepia_sse_func.o

output/main.o: main.c
	gcc $(FLAGS) output/main.o main.c

output/bmp_func.o: bmp_func.c
	gcc $(FLAGS) output/bmp_func.o bmp_func.c

output/sepia_func.o: sepia_func.c
	gcc $(FLAGS) output/sepia_func.o sepia_func.c

output/sepia_sse_func.o: sepia_sse_func.asm
	nasm -felf64 sepia_sse_func.asm -o output/sepia_sse_func.o

clean:
	rm -f main *.o ./images/rp.bmp
