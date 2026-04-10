/*
 * main.c
 * ------
 * Driver program for the N-Queens Secure Vault Placement solver.
 * Runs several test cases and prints results to stdout.
 *
 * Compile:  gcc -Wall -Iinclude src/nqueens.c src/utils.c src/main.c -o nqueens
 * Run:      ./nqueens
 */

#include "nqueens.h"
#include <stdio.h>

int main(void)
{
    Board         b;
    SolutionStore store;

    /* --------------------------------------------------------
     * TEST 1: Standard 4-Queens (no blocked cells)
     * Expected: 2 solutions
     * -------------------------------------------------------- */
    printf("=== Test 1: 4-Queens, no blocked cells ===\n");
    board_init(&b, 4);
    solve(&b, &store);
    print_all(&store, 4);

    /* --------------------------------------------------------
     * TEST 2: 4-Queens with one blocked cell at (0, 1)
     * Expected: 1 solution (the other 4-queen solution uses col 1 in row 0)
     * -------------------------------------------------------- */
    printf("=== Test 2: 4-Queens, blocked cell (0,1) ===\n");
    board_init(&b, 4);
    board_add_blocked(&b, 0, 1);
    solve(&b, &store);
    print_all(&store, 4);

    /* --------------------------------------------------------
     * TEST 3: 8-Queens (classic benchmark)
     * Expected: 92 solutions
     * -------------------------------------------------------- */
    printf("=== Test 3: 8-Queens, no blocked cells ===\n");
    board_init(&b, 8);
    solve(&b, &store);
    printf("Solutions found: %d  (expected: 92)\n\n", store.count);

    /* --------------------------------------------------------
     * TEST 4: Count-only mode for larger N
     * Verifies against OEIS sequence A000170
     * -------------------------------------------------------- */
    printf("=== Test 4: Count-only mode ===\n");
    int expected[] = {0, 1, 0, 0, 2, 10, 4, 40, 92, 352, 724};
    for (int n = 1; n <= 10; n++) {
        board_init(&b, n);
        int cnt = count_solutions(&b);
        printf("  %2d-Queens: %4d solutions  (expected: %4d) %s\n",
               n, cnt, expected[n],
               cnt == expected[n] ? "OK" : "MISMATCH");
    }
    printf("\n");

    /* --------------------------------------------------------
     * TEST 5: 5-Queens with multiple blocked cells
     * -------------------------------------------------------- */
    printf("=== Test 5: 5-Queens, blocked (0,0), (2,2), (4,4) ===\n");
    board_init(&b, 5);
    board_add_blocked(&b, 0, 0);
    board_add_blocked(&b, 2, 2);
    board_add_blocked(&b, 4, 4);
    solve(&b, &store);
    printf("Solutions found: %d\n\n", store.count);

    return 0;
}
