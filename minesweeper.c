#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "matrix.h"
#include "cells.c"

#define GAME_STATE_PLAYING 1
#define GAME_STATE_WIN 2
#define GAME_STATE_LOST 3
#define GAME_STATE_ABORT 4

typedef struct {
    char game_state;
    matrix* board;
} minesweeper;

minesweeper* minesweeper_new(int rows, int cols) {
    minesweeper* m = malloc(sizeof(minesweeper));
    m->game_state = GAME_STATE_PLAYING;
    matrix* board = matrix_new(rows, cols);
    m->board = board;

    srand(time(NULL));
    for (int i = 0; i < matrix_rows(board); i++) {
        for (int j = 0; j < matrix_columns(board); j++) {
            cell* c = cell_create(board, i, j);
            float r = rand() % (matrix_rows(board) * matrix_columns(board));
            if (r <= matrix_rows(board)) {
                cell_setmine(c);
            }
        }
    }

    for (int i = 0; i < matrix_rows(board); i++) {
        for (int j = 0; j < matrix_columns(board); j++) {
            cell* c = (cell*)matrix_get(board, i, j);
            cell_update_mine_count(c);
            cell_update(c);
        }
    }

    return m;
}

void minesweeper_destroy(minesweeper* m) {
    matrix_free(m->board);
    free(m);
}

int minesweeper_getstate(minesweeper* m) {
    return m->game_state;
}

void minesweeper_setstate(minesweeper* m, int state) {
    m->game_state = state;
}

int board_verify(minesweeper* m) {
    // recorrer toda la matriz y verificar que estén reveladas
    // todas las casillas que no contienen minas. Si no están,
    // devuelve 0, si están, devuelve 1.
    int result = 0;
    matrix* board = m->board;

    int nro = 0;
    int rows = matrix_rows(board);
    int cols = matrix_columns(board);
    int cells = rows * cols;
    int ok = 1;
    while (ok && nro < cells) {
        int i = nro / cols;
        int j = nro % cols;
        cell* c = (cell*)matrix_get(board, i, j);
        ok = (cell_isrevealed(c) || cell_ismine(c));        
        nro++;
    }
    if (nro >= cells) {
        result = 1;
    }
    return result;
}

void minesweeper_showmines(minesweeper* m) {
    matrix* board = m->board;
    // revelar todas las casillas que contienen minas.
    for (int i = 0; i < matrix_rows(board); i++) {
        for (int j = 0; j < matrix_columns(board); j++) {
            cell* act = (cell*)matrix_get(board, i, j);
            cell_setrevealed(act);
        }
    }
}

int minesweeper_move(minesweeper* m, int row, int col) {
    matrix* board = m->board;    
    cell* c = (cell*)matrix_get(board, row, col);
    if (c != NULL) {
        cell_setrevealed(c);
        if (cell_ismine(c)) {
            minesweeper_showmines(m);
            minesweeper_setstate(m, GAME_STATE_LOST);            
        } else {
            if (board_verify(m) != 0) {
                minesweeper_setstate(m, GAME_STATE_WIN);                
            }
        }
    }
    return minesweeper_getstate(m);
}

void minesweeper_print(minesweeper* m) {
    matrix* board = m->board;
    printf ("  0 1 2 3 4 5 6 7 8 9\n");
    for (int i = 0; i < matrix_rows(board); i++) {
        printf("%d ", i);
        for (int j = 0; j < matrix_columns(board); j++) {
            cell* c = (cell*)matrix_get(board, i, j);
            cell_print(c);
        }
        printf("\n");
    }
}


void main(){
    minesweeper* game = minesweeper_new(10, 10);

    while (minesweeper_getstate(game) == GAME_STATE_PLAYING) {
        int row, col;
        //system("cls");
        minesweeper_print(game);
        printf("Ingrese fila y columna: ");
        scanf("%d %d", &row, &col);        
        minesweeper_move(game, row, col);
    }

    //system("cls");
    minesweeper_print(game);
    if (minesweeper_getstate(game) == GAME_STATE_WIN) {
        printf("Ganaste!\n");
    } else {
        printf("Perdiste!\n");
    }

    minesweeper_destroy(game);

    system("pause");


}

