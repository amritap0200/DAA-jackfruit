/*
 * nqueens.h
 * ---------
 * Public interface for the N-Queens Secure Vault Placement solver.
 *
 * Technique : Backtracking with O(1) conflict detection
 * Structures: Three boolean arrays track column and diagonal occupancy.
 *             A 2D blocked[][] array marks pre-blocked cells.
 *
 * All core algorithm functions are declared here.
 * See src/nqueens.c for implementations.
 */

#ifndef NQUEENS_H
#define NQUEENS_H

#include <stdbool.h>

/* ---------------------------------------------------------------
 * LIMITS
 * --------------------------------------------------------------- */
#define MAX_N         20    /* maximum board size supported         */
#define MAX_SOLUTIONS 5000  /* upper bound on stored solutions      */

/* ---------------------------------------------------------------
 * BOARD STATE
 * All conflict-tracking arrays are indexed directly by their key.
 *
 * cols[c]           = true  →  column c is occupied
 * diag1[r - c + N]  = true  →  "\" diagonal through (r,c) occupied
 *                              offset by N to keep index non-negative
 * diag2[r + c]      = true  →  "/" diagonal through (r,c) occupied
 * blocked[r][c]     = true  →  cell (r,c) cannot hold a queen
 * placement[r]      = c     →  queen in row r is at column c
 * --------------------------------------------------------------- */
typedef struct {
    int  n;                              /* board dimension N          */
    bool cols   [MAX_N];                 /* occupied columns           */
    bool diag1  [2 * MAX_N];             /* "\" diagonals (offset by N)*/
    bool diag2  [2 * MAX_N];             /* "/" diagonals              */
    bool blocked[MAX_N][MAX_N];          /* pre-blocked cells          */
    int  placement[MAX_N];               /* current queen positions    */
} Board;

/* ---------------------------------------------------------------
 * SOLUTION STORE
 * --------------------------------------------------------------- */
typedef struct {
    int  data [MAX_SOLUTIONS][MAX_N];   /* each row: queen col per row */
    int  count;                          /* number of solutions found   */
} SolutionStore;

/* ---------------------------------------------------------------
 * CORE ALGORITHM FUNCTIONS  (implemented in src/nqueens.c)
 * --------------------------------------------------------------- */

/*
 * board_init
 * ----------
 * Initialises all arrays in Board to their zero/false state.
 * Must be called before solve().
 *
 * Parameters:
 *   b  — pointer to a Board struct to initialise
 *   n  — board dimension
 */
void board_init(Board *b, int n);

/*
 * board_add_blocked
 * -----------------
 * Marks cell (row, col) as blocked (cannot hold a queen).
 * Call this after board_init() and before solve().
 */
void board_add_blocked(Board *b, int row, int col);

/*
 * is_safe
 * -------
 * Returns true if placing a queen at (row, col) violates no constraint.
 * Checks: column occupancy, both diagonal occupancies.
 * Does NOT check whether the cell is blocked — caller must do that.
 *
 * Time complexity: O(1)
 */
bool is_safe(const Board *b, int row, int col);

/*
 * backtrack
 * ---------
 * Core recursive backtracking function.
 * Tries every column in `row`, prunes on conflict or blocked cell,
 * recurses to row+1 on a valid placement, then restores state.
 *
 * Parameters:
 *   b     — current board state (modified in place, restored on return)
 *   row   — current row being processed (0-indexed)
 *   store — accumulates valid complete placements
 *
 * Time complexity: O(N!) worst case, much less in practice due to pruning.
 */
void backtrack(Board *b, int row, SolutionStore *store);

/*
 * solve
 * -----
 * Entry point. Calls backtrack(b, 0, store).
 * Populates store with all valid configurations.
 *
 * Parameters:
 *   b     — fully initialised Board (with any blocked cells pre-set)
 *   store — output store; initialised inside this function
 */
void solve(Board *b, SolutionStore *store);

/*
 * count_solutions
 * ---------------
 * Memory-efficient variant: counts solutions without storing boards.
 * Useful for large N where MAX_SOLUTIONS would be exceeded.
 *
 * Returns: total number of valid configurations.
 */
int count_solutions(Board *b);

/* ---------------------------------------------------------------
 * UTILITY FUNCTIONS  (implemented in src/utils.c)
 * --------------------------------------------------------------- */
void print_board (const int *placement, int n);
void print_all   (const SolutionStore *store, int n);

#endif /* NQUEENS_H */
