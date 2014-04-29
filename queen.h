// queen.h

#ifndef QUEEN_H
#define QUEEN_H

/* print solution */
void printSolution(int **board, int N);

/* check if a queen can be placed on board[row][col]*/
int isSafe(int** board, int row, int col, int N);

/* A recursive function to solve N Queen problem */
int solveNQ(int **board, int firstrow, int row, int col, int* pNum, int N, int ifPrint);

#endif
