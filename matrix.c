#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

/**
 * @brief Crea la matriz con el tamaño apropiado según las dimensiones establecidas.
 * 
 * @param row filas 
 * @param col columnaumns 
 * @return matrix* 
 */
matrix* matrix_new(int rows, int columns){
    matrix* result = (matrix*)malloc(sizeof(matrix));
    result->a = (void**)malloc(sizeof(void*) * rows * columns);
    result->rows = rows;
    result->columns = columns;
    return result;
}

/**
 * @brief Elimina la matriz
 * 
 * @param m puntero a la matriz 
 */
void matrix_free(matrix* m){
    free(m);
}

/**
 * @brief Permite obtener la cantidad de filas
 * 
 * @param m puntero a la matriz 
 * @return int la cantidad de filas
 */
int matrix_rows(matrix* m){
    return m->rows;
}

/**
 * @brief Permite obtener la cantidad de columnas
 * 
 * @param m puntero a la matriz 
 * @return int la cantidad de columnas
 */
int matrix_columns(matrix* m){
    return m->columns;
}

/**
 * @brief Permite obtener el valor de una posición de la matriz
 * 
 * @param m puntero a la matriz 
 * @param row fila 
 * @param col columna 
 * @return void* 
 */
void* matrix_get(matrix* m, int row, int col){
    void* result = NULL;
    if (row >= 0 && col >= 0 && row < m->rows && col < m->columns){
        result = m->a[row * m->columns + col];
    }
    return result; 
}

/**
 * @brief Permite reemplazar o asignar un elemento a la matriz
 * 
 * @param m puntero a la matriz 
 * @param row fila
 * @param col columna
 * @param value valor a asignar
 */
void matrix_set(matrix* m, int row, int col, void* value){
    m->a[row * m->columns + col] = value;
}

/**
 * @brief Permite imprimir una matriz por consola
 * 
 * @param m puntero a la matriz 
 * @param prt función que permite imprimir un valor de la matriz
 */
void matrix_print(matrix* m, void prt(void*), int show_labels){
    if (show_labels){
        printf("     ");
        for (int i = 0; i < m->columns; i++){
            printf("%d ", i);
        }
        printf("\n");
    }
    for(int i = 0; i < m->rows; i++){
       if (show_labels) {
            printf("%4d ", i);
       }
       for(int j = 0; j < m->columns; j++){
          prt(matrix_get(m, i, j));
       }
       printf("\n");
    }
}