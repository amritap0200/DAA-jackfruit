/*
 * nqueens.c
 * ---------
 * Implementation of the N-Queens backtracking algorithm with:
 *   - O(1) conflict detection via auxiliary boolean arrays
 *   - Support for pre-blocked cells
 *   - Full solution enumeration and count-only mode
 *
 * Algorithm overview
 * ------------------
 * We solve one row at a time (top to bottom).
 * For each row we try every column. Before placing we check:
 *   1. Is the cell blocked?           → skip (O(1))
 *   2. Is the column occupied?        → skip (O(1))
 *   3. Is either diagonal occupied?   → skip (O(1))
 * If all checks pass we place the queen, recurse to the next row,
 * then UNDO the placement (backtrack) so the next column can be tried.
 *
 * Key insight for diagonal indexing
 * ----------------------------------
 * All cells on the same "\" diagonal share the same (row - col) value.
 * All cells on the same "/" diagonal share the same (row + col) value.
 * We store these as booleans in arrays, giving O(1) lookup.
 * Because (row - col) can be negative (e.g. row=0, col=3 → -3),
 * we offset it by N so all indices stay non-negative: key = row - col + N.
 */

#include "nqueens.h"
#include <string.h>  /* memset */

/* ---------------------------------------------------------------
 * board_init
 * --------------------------------------------------------------- */
void board_init(Board *b, int n)
{
    b->n = n;

    /* zero out all conflict-tracking arrays and placement */
    memset(b->cols,      0, sizeof(b->cols));
    memset(b->diag1,     0, sizeof(b->diag1));
    memset(b->diag2,     0, sizeof(b->diag2));
    memset(b->blocked,   0, sizeof(b->blocked));
    memset(b->placement, -1, sizeof(b->placement));
}

/* ---------------------------------------------------------------
 * board_add_blocked
 * --------------------------------------------------------------- */
void board_add_blocked(Board *b, int row, int col)
{
    b->blocked[row][col] = true;
}

/* ---------------------------------------------------------------
 * is_safe
 *
 * Returns true iff placing a queen at (row, col) does not conflict
 * with any already-placed queen.
 *
 * We check:
 *   cols[col]            — another queen in this column
 *   diag1[row-col+N]     — another queen on the "\" diagonal
 *   diag2[row+col]       — another queen on the "/" diagonal
 *
 * No row check needed — we place exactly one queen per row,
 * so two queens can never share a row by construction.
 * --------------------------------------------------------------- */
bool is_safe(const Board *b, int row, int col)
{
    int n = b->n;

    if (b->cols [col])              return false;
    if (b->diag1[row - col + n])    return false;
    if (b->diag2[row + col])        return false;

    return true;
}

/* ---------------------------------------------------------------
 * place_queen  (internal helper)
 * Marks column and both diagonals as occupied.
 * --------------------------------------------------------------- */
static void place_queen(Board *b, int row, int col)
{
    b->placement[row]            = col;
    b->cols [col]                = true;
    b->diag1[row - col + b->n]   = true;
    b->diag2[row + col]          = true;
}

/* ---------------------------------------------------------------
 * remove_queen  (internal helper)
 * Unmarks column and both diagonals — restores state for backtracking.
 * --------------------------------------------------------------- */
static void remove_queen(Board *b, int row, int col)
{
    b->placement[row]            = -1;
    b->cols [col]                = false;
    b->diag1[row - col + b->n]   = false;
    b->diag2[row + col]          = false;
}

/* ---------------------------------------------------------------
 * backtrack
 *
 * Recursive core of the algorithm.
 *
 * Base case  : row == n  →  all N queens placed, record solution.
 * Recursive  : try each column in the current row.
 *              Prune if blocked or unsafe.
 *              Place → recurse → remove  (classic backtrack pattern).
 * --------------------------------------------------------------- */
void backtrack(Board *b, int row, SolutionStore *store)
{
    int n = b->n;

    /* BASE CASE: every row has been filled → valid solution */
    if (row == n) {
        if (store->count < MAX_SOLUTIONS) {
            /* copy current placement into the solution store */
            memcpy(store->data[store->count], b->placement, n * sizeof(int));
            store->count++;
        }
        return;
    }

    /* Try placing a queen in each column of this row */
    for (int col = 0; col < n; col++) {

        /* PRUNE 1: cell is blocked — cannot place here */
        if (b->blocked[row][col])
            continue;

        /* PRUNE 2: placement would attack an existing queen */
        if (!is_safe(b, row, col))
            continue;

        /* PLACE the queen at (row, col) */
        place_queen(b, row, col);

        /* RECURSE to the next row */
        backtrack(b, row + 1, store);

        /* BACKTRACK: remove queen, restore state for next column */
        remove_queen(b, row, col);
    }
}

/* ---------------------------------------------------------------
 * solve
 * Entry point — initialises store, then triggers backtracking from row 0.
 * --------------------------------------------------------------- */
void solve(Board *b, SolutionStore *store)
{
    store->count = 0;
    backtrack(b, 0, store);
}

/* ---------------------------------------------------------------
 * count_solutions
 *
 * Memory-efficient variant.
 * Uses a local static counter instead of a SolutionStore.
 * Avoids storing O(N * MAX_SOLUTIONS) integers.
 * Useful when N is large and you only need the total count.
 * --------------------------------------------------------------- */

/* Internal recursive counter — mirrors backtrack() but only increments */
static void bt_count(Board *b, int row, int *count)
{
    int n = b->n;

    if (row == n) {
        (*count)++;
        return;
    }

    for (int col = 0; col < n; col++) {
        if (b->blocked[row][col]) continue;
        if (!is_safe(b, row, col)) continue;

        place_queen(b, row, col);
        bt_count(b, row + 1, count);
        remove_queen(b, row, col);
    }
}

int count_solutions(Board *b)
{
    int count = 0;
    bt_count(b, 0, &count);
    return count;
}
