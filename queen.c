// queen.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queen.h"

#define true 1
#define false 0

/* print solution */
void printSolution(int **board, int N)
{
    int i,j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            printf(" %d ", board[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* check if a queen can be placed on board[row][col]*/
int isSafe(int** board, int row, int col, int N)
{
    int i, j;
    /* Check this row on left side */
    for (i = 0; i < col; i++)
    {
        if (board[row][i])
            return false;
    }
    /* Check upper diagonal on left side */
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j])
            return false;
    }
    /* Check lower diagonal on left side */
    for (i = row, j = col; j >= 0 && i < N; i++, j--)
    {
        if (board[i][j])
            return false;
    }
    return true;
}

/* A recursive function to solve N Queen problem */
int solveNQ(int **board, int firstrow, int row, int col, int* pNum, int N, int ifPrint)
{
    // first column, decided by JobSize
    if(col == 1)
        board[firstrow][0] = 1;

    if (col >= N)
    {
        // find one solution
        (*pNum)++;
        if(ifPrint)
            printSolution(board,N);
        return true;
    }

    int i;

	/* col is fixed in each call, check row by row  */	
    for (i = row; i < N; i++)
    {
        /* Check if queen can be placed on board[i][col] */
        if ( isSafe(board, i, col, N) )
        {
            board[i][col] = 1; /* put the queen*/

            /* recur to check next col of the queens */
            if ( solveNQ(board, firstrow, 0, col + 1, pNum, N, ifPrint) == true )
                if (col < N-1)
                    return true;

            /* can not find place in next col, go back move to next row in this col*/
            /* col==N-1: or reset the queen in the last col after finding a solution  */
            board[i][col] = 0; /* BACKTRACK */
        }
        // if back to second col, finish solutions of this row, return to next subjob
        // clear the first column to 0, which is a clear board again
        if (col == 1) // 1 is the JobSize
        {
            board[firstrow][0]=0;
            return true;
        }
    }
    // can not find place in this col, back track
    return false;
}
 

