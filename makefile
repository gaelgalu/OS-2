
all: release

release:
	gcc main.c functions.c -o laboratorio1 -lm -Wall

run:
	./laboratorio1 -n 5 -L 10 -X 4 -Y 6 -d 1.5 -b
