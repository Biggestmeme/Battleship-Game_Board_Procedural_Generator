#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8
#define ATTEMPTS 10
//this means one ship of length 5
//one ship of length 3
//...
#define SHIP_DIRECTION "vh"
#define NUMBER_OF_SHIPS 3

short SHIP_CONFIG[NUMBER_OF_SHIPS] = { 5,4,2 };

short** generateBoard(short height, short width) {
	short** newboard = (short**)calloc(width, sizeof(short*));
	for (short i = 0; i < width; i++) 
		for (short j = 0; j < height; j++) 
			newboard[i] = (short*)calloc(1, sizeof(short));
		
	
	return newboard;
}

void freeBoard(short** matrix, short width) {
	for (short i = 0; i < width; i++) 
		free(matrix[i]);
	
	free(matrix);
}

void setBoardOnZero(short** matrix,short height,short width) {
	for (short i = 0; i < width; i++) 
		for (short j = 0; j < height; j++) 
			matrix[i][j] = 0;	
}

void insertShip(short** matrix,short shipLength, char direction, short corner[2]) {
	switch (direction) {
	case 'h':
		for (short i = 0; i < shipLength; i++)
			matrix[corner[0] + i][corner[1]] = 1;
		break;
	case 'v':
		for (short i = 0; i < shipLength; i++)
			matrix[corner[0]][corner[1] + i] = 1;
		break;
	}
}

char canFit(short** matrix, short shipLength,char direction,short corner[2]) {
	//WRITE cells()
	short* cellList = (short*)calloc(shipLength,sizeof(short));
	switch (direction) {
		case 'h':
			for (short i = 0; i < shipLength; i++)
				cellList[i] = matrix[corner[0] + i][corner[1]];
			break;
		case 'v':
			for (short i = 0; i < shipLength; i++)
				cellList[i] = matrix[corner[0]][corner[1] + i];
			break;
	}

	for (short i = 0; i < shipLength; i++) {
		if (cellList[i] == 1) {
			return 0;
		}
	}
	
	free(cellList);
	return 1;
}

char placeShip(short** matrix,short ship) {
	//TO DO -> remove rand and replace it with arduino equivalent
	char direction = SHIP_DIRECTION[rand() % 2];
	short corner[2];
	short max_col = BOARD_WIDTH;
	short max_row = BOARD_HEIGHT;

	switch (direction) {
		case 'v':
			max_row -= ship - 1;
			break;
		case 'h':
			max_col -= ship - 1;
			break;
	}

	if (max_row < 1 || max_col < 1)
		return 0;

	corner[0] = (short)floor(rand() % max_col);
	corner[1] = (short)floor(rand() % max_row);
	
	if (canFit(matrix, ship, direction, corner) == 1) {
		insertShip(matrix, ship, direction, corner);

		return 1;
	}
	return 0;
}

void placeShips(short** matrix,short attempts, short height, short width) {
	for (short i = 0; i < attempts; i++) {
		setBoardOnZero(matrix, height, width);
		for (short i = 0; i < NUMBER_OF_SHIPS; i++) {
			if (placeShip(matrix, SHIP_CONFIG[i]) == 0)
				break;
		}
	}
}

short* generateLedMatrix(short** board, short height, short width) {
	short* ledMatrix = (short*)calloc(width, sizeof(short));
	int mask = 1;
	int result = 0;
	for (int i = 0; i < width; i++) {
		result = 0;
		for (int j = 0; j < height; j++) {
			if (board[j][i] == 1) {
				result = result ^ (mask << j);
			}
		}
		ledMatrix[i] = result;

	}
	return ledMatrix;
}

int main() {

	srand(time(NULL));

	
	short** board = generateBoard(BOARD_HEIGHT, BOARD_WIDTH);
	setBoardOnZero(board, BOARD_HEIGHT, BOARD_WIDTH);
	generateBoard(BOARD_HEIGHT, BOARD_WIDTH);

	placeShips(board, ATTEMPTS, BOARD_HEIGHT, BOARD_WIDTH);
	for (int i = 0; i < 8; i++) {
		printf("%\n");
		for (int j = 0; j < 8; j++) {
			printf("%d ", board[i][j]);
		}
	}
	printf("%\n");

	short* ledMatrix = generateLedMatrix(board, BOARD_HEIGHT, BOARD_WIDTH);

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		printf("%d ", ledMatrix[i]);
	}
}