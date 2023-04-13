#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

typedef struct {
    char show;       // _ = unreveled, * = mine, . = empty, x = mark, 0-8 = number
    int mine_count;  // number of mines around
    int marked;      // 1 = marked, 0 = not marked
    int revealed;    // 1 = revealed, 0 = not revealed
    int mine;        // 1 = mine, 0 = not mine
    matrix* m;
    int row;
    int col;
} cell;

cell* cell_new() {
    cell* c = malloc(sizeof(cell));
    c->show = '_';
    c->mine_count = 0;
    c->marked = 0;
    c->revealed = 0;
    c->mine = 0;
    cell* result;
}

void cell_setposition(cell* c, matrix* m, int row, int col) {
    c->m = m;
    c->row = row;
    c->col = col;
    matrix_set(m, row, col, c);
}

cell* cell_create(matrix* m, int row, int col) {
    cell* c = cell_new();
    cell_setposition(c, m, row, col);
    return c;
}

void cell_free(cell* c) { free(c); }

int cell_isrevealed(cell* c) { return c->revealed; }
int cell_ismarked(cell* c) { return c->marked; }
int cell_ismine(cell* c) { return c->mine; }
int cell_minescount(cell* c) { return c->mine_count; }

int cell_setmine(cell* c) {
    if (c->mine == 0) {
        c->mine = 1;
    }
    return c->mine;
}



void cell_update(cell* c) {
    if (cell_isrevealed(c)) {
        if (cell_ismine(c)) {
            c->show = '*';
        } else if (c->mine_count > 0) {
            c->show = c->mine_count + '0';
        } else {
            c->show = '.';
        }
    } else {
        if (cell_ismarked(c)) {
            c->show = 'x';
        } else {
            c->show = '_';            
        }
    }
}

int cell_setmarked(cell* c) {
    if (c->marked == 0) {
        c->marked = 1;
    } else {
        c->marked = 0;
    }
    cell_update(c);
    return c->marked;
}

void cell_update_mine_count(cell* c) {
    if (c->m != NULL) {
        int from_row = (c->row > 0) ? c->row - 1 : 0;
        int from_col = (c->col > 0) ? c->col - 1 : 0;
        int to_row = (c->row < matrix_rows(c->m) - 1) ? c->row + 1
                                                      : matrix_rows(c->m) - 1;
        int to_col = (c->col < matrix_columns(c->m) - 1)
                         ? c->col + 1
                         : matrix_columns(c->m) - 1;

        for (int i = from_row; i <= to_row; i++) {
            for (int j = from_col; j <= to_col; j++) {
                if (((cell*)matrix_get(c->m, i, j))->mine == 1) {
                    c->mine_count++;
                }
            }
        }
    }
}

void cell_print(void* c) { printf("%c ", ((cell*)c)->show); }

// cell revel neighborhood
int cell_setrevealed(cell* c);
void cell_revel_neighborhood(cell* c) {
    int from_row = (c->row > 0) ? c->row - 1 : 0;
    int from_col = (c->col > 0) ? c->col - 1 : 0;
    int to_row =
        (c->row < matrix_rows(c->m) - 1) ? c->row + 1 : matrix_rows(c->m) - 1;
    int to_col = (c->col < matrix_columns(c->m) - 1) ? c->col + 1
                                                     : matrix_columns(c->m) - 1;

    for (int i = from_row; i <= to_row; i++) {
        for (int j = from_col; j <= to_col; j++) {
            if (i != c->row || j != c->col) {
                cell* c_neighbor = (cell*)matrix_get(c->m, i, j);
                cell_setrevealed(c_neighbor);
            }
        }
    }
}

int cell_setrevealed(cell* c) {
    if (c->revealed == 0) {
        c->revealed = 1;
        if (c->mine_count == 0) {
            cell_revel_neighborhood(c);
        }
    }
    cell_update(c);
    return c->revealed;
}
