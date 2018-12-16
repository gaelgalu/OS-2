#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

typedef struct Grid {
    int** matrix;
    pthread_mutex_t** mutex;
    int rows;
    int cols;
    float delta;
} Grid;

typedef struct Photon {
    int id;
    int state;
    float L;
    float posX;
    float posY;
    Grid* grid;
} Photon;

Grid* gridInit(int X, int Y, float d);
Photon** photonArrayInit(Grid* grid, int n, float L);
void freeGrid(Grid* grid);
void freePhotonArray(Photon** photonArray, int n);
void* move(void* photon);
void init(int bflag, int n, float L, int X, int Y, float d);
