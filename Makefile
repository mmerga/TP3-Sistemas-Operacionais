all: TP3FAT

TP3FAT: main.o shell.o fat.o
	gcc -o TP3FAT main.o fat.o shell.o

TP3FAT.o: main.c shell.c fat.c
	gcc -c main.c shell.c fat.c

clean:
	rm -f *.o *~ 

run: 
	./TP3FAT