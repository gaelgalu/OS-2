#include "functions.h"

Grid* gridInit(int X, int Y, float d) {
	Grid* grid = (Grid*)calloc(1,sizeof(Grid));

	grid->cols  = X;
	grid->rows  = Y;
	grid->delta = d;

	grid->matrix = (int**)calloc(X, sizeof(int*));
	grid->mutex  = (pthread_mutex_t**)calloc(X, sizeof(pthread_mutex_t*));

	for (int i = 0; i < X; i++) {
		grid->matrix[i] = (int*)calloc(Y, sizeof(int));
		grid->mutex[i]  = (pthread_mutex_t*)calloc(Y, sizeof(pthread_mutex_t));
		for (int j = 0; j < Y; j++) {
			grid->matrix[i][j] = 0;
			pthread_mutex_init(&grid->mutex[i][j], NULL);
		}
	}

	return grid;
}

Photon** photonArrayInit(Grid* grid, int n, int L) {
	Photon** photonArray = (Photon**)calloc(n, sizeof(Photon*));
	for (int i = 0; i < n; i++) {
		photonArray[i] = (Photon*)calloc(1,sizeof(Photon));

		photonArray[i]->id = i+1;
		photonArray[i]->state = 1;
		photonArray[i]->L = L;
		photonArray[i]->posX = grid->cols/2.0;
		photonArray[i]->posY = grid->rows/2.0;
		photonArray[i]->grid = grid;
	}

	return photonArray;
}

void freeGrid(Grid* grid) {

	for (int i = 0; i < grid->cols; i++) {
		free(grid->matrix[i]);
		free(grid->mutex[i]);
	}
	free(grid->matrix);
	free(grid->mutex);
	free(grid);
}

void freePhotonArray(Photon** photonArray, int n) {
	for (int i = 0; i < n; i++) {
		free(photonArray[i]);
	}
	free(photonArray);
}

void* move(void* photon) {
	Photon* p = (Photon*) photon;

	int col = 0;
	int row = 0;

	while (p->state) {

		/* Obtain the distance to move */
		int random = rand() % 1000;
		double distance = log(1-(random/1000.0)) * (-1);

		/* Randomly generate the vector */
		int direction[2];
		direction[0] = rand() % p->grid->cols;
		direction[1] = rand() % p->grid->rows;

		/*
		// Remove block-comments to avoid vector = 0i + 0j
		while (mu[0] == mu[1] == 0) {
			mu[0] = rand() % p->grid->cols;
			mu[1] = rand() % p->grid->rows;
		}
		*/

		/* Select the signs randomly */
		if (rand() % 2) {
			direction[0] = direction[0] * (-1);
		}
		if (rand() % 2) {
			direction[1] = direction[1] * (-1);
		}

		/* Obtain a new position for the photon */
		float newPosX = p->posX + (distance * direction[0]);
		float newPosY = p->posY + (distance * direction[1]);

		/* CASE 1: The photon ends out of the grid */
		if ((newPosX > p->grid->cols || newPosY > p->grid->rows) || (newPosX < 0 || newPosY < 0)) {
			p->state = 0;
		}

		/* CASE 2: The photon ends between two cells */
		else if ((newPosX - (int)newPosX) == 0 || (newPosY - (int)newPosY) == 0 ) {

			/* Move the photon to the correct cell*/
			if (newPosX - (int)newPosX == 0) {
				col = ((int)newPosX/p->grid->delta) + 1;
			}
			if (newPosY - (int)newPosY == 0) {
				row = ((int)newPosY/p->grid->delta) + 1;
			}

			/* The cell is inside the matrix */
			if (col <= p->grid->cols && row <= p->grid->rows) {
				/* Absorption's events (50% chance) */
				if (rand()%2) {
					pthread_mutex_lock(&(p->grid->mutex[row][col]));
					p->grid->matrix[row][col] ++;
					pthread_mutex_unlock(&(p->grid->mutex[row][col]));
				}
			}
			/* The cell is outside the matrix */
			else {
				p->state = 0;
			}
		}
		/* CASE 3: The photon ends into a cell directly */
		else {
			/* Absorption's events (50% chance) */
			if (rand()%2) {
				/* Obtain the cell to absorb the energy */
				col = (int)newPosX/p->grid->delta;
				row = (int)newPosY/p->grid->delta;

				/* Mutual exclusion */
				pthread_mutex_lock(&(p->grid->mutex[row][col]));
				p->grid->matrix[row][col] ++;
				pthread_mutex_unlock(&(p->grid->mutex[row][col]));
			}
		}
		p->posX = newPosX;
		p->posY = newPosY;

	}
	return NULL;
}

void init(int bflag, int n, int L, int X, int Y, float d) {

	/* Generate the grid, photons and the threads*/
	Grid* grid = gridInit(X,Y, d);
	Photon** photonArray = photonArrayInit(grid, n, L);
	pthread_t* threadArray = (pthread_t*)calloc(n, sizeof(pthread_t));

	/* Seed for rand()*/
	srand(time(NULL));

	int i,j;

	/* Initialize the theads*/
	for (i = 0; i < n; i++) {
		pthread_create(&threadArray[i], NULL, move, (void*)photonArray[i]);
	}

	/* Join the threads*/
	for (i = 0; i < n; i++) {
		pthread_join(threadArray[i], NULL);
	}

	/* Show the final matrix (bflag needed)*/
	if (bflag) {
		for (i = 0; i < Y; i++) {
			for (j = 0; j < X; j++) {
				printf("<%d [%d][%d]>\n", grid->matrix[j][i], i, j);
			}
		}
	}

	/* Free the memory allocated*/
	freeGrid(grid);
	freePhotonArray(photonArray, n);
	free(threadArray);

}
