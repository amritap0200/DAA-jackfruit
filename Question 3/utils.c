/*
 * utils.c
 * -------
 * Display utilities for the N-Queens solver.
 * Not part of the core algorithm — purely for output formatting.
 */

#include "nqueens.h"
#include <stdio.h>

/*
 * print_board
 * -----------
 * Prints a single placement as an N×N grid.
 * 'Q' marks a queen, '.' marks an empty cell.
 *
 * Example output for N=4, placement=[1,3,0,2]:
 *   . Q . .
 *   . . . Q
 *   Q . . .
 *   . . Q .
 */
void print_board(const int *placement, int n)
{
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            printf("%c ", placement[row] == col ? 'Q' : '.');
        }
        printf("\n");
    }
}

/*
 * print_all
 * ---------
 * Prints every solution in the SolutionStore, numbered.
 */
void print_all(const SolutionStore *store, int n)
{
    printf("Total solutions found: %d\n\n", store->count);
    for (int i = 0; i < store->count; i++) {
        printf("Solution %d:\n", i + 1);
        print_board(store->data[i], n);
        printf("\n");
    }
}
