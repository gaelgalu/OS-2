
all: release

release:
	gcc main.c functions.c -o laboratorio1 -lm -Wall

run:
	./laboratorio1 -n 10 -L 10 -X 10 -Y 6 -d 1.5 -b
