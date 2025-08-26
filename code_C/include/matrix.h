#ifndef MATRIX_H
#define MATRIX_H


#include "rawmatrix.h"
#include "permutation.h"



struct Matrix{
    RawMatrix matrix;
    Permutation sigma_col;
};

typedef struct Matrix Matrix;

Matrix create_matrix(unsigned int nrows,unsigned int ncols);
Matrix create_matrix_from_rawmatrix(RawMatrix mat); // makes a copy of "mat"
void destroy_matrix(Matrix mat);

Matrix matrix_copy(Matrix mat);

Matrix random_matrix(unsigned int nrows,unsigned int ncols);
Matrix identity_matrix(unsigned int n);

void matrix_write(Matrix mat,unsigned int i,unsigned int j,Bit val);


Bit matrix_get_coordinate_value(Matrix mat,unsigned int i,unsigned int j);
Vector matrix_get_row_copy(Matrix mat,unsigned int i);
Vector matrix_get_column_copy(Matrix mat,unsigned int j);

void transpose_matrix(Matrix* mat);
Matrix transposed_matrix(Matrix mat);

void matrix_swap_rows(Matrix mat,unsigned int i,unsigned int j);
void matrix_swap_columns(Matrix mat,unsigned int i,unsigned int j);
void matrix_apply_permutation_to_rows(Matrix* mat,Permutation sigma);
void matrix_apply_permutation_to_columns(Matrix* mat,Permutation sigma);

Vector matrix_row_sums_from_positions(Matrix mat,unsigned int* rowIndexes,unsigned int sumSize);
Vector matrix_sum_columns_from_positions(Matrix mat,unsigned int* columnIndexes,unsigned int sumSize);
Vector matrix_sum_rows_from_vector(Matrix mat,Vector support);
Vector matrix_sum_columns_from_vector(Matrix mat,Vector support);

void matrix_add_row_to_row(Matrix mat,unsigned int target,unsigned int rowToAdd);
void matrix_add_column_to_column(Matrix mat,unsigned int target,unsigned int columnToAdd);

void matrix_write_row(Matrix mat,unsigned int targetRow,Vector val);//binds val to mat
void matrix_write_column(Matrix mat,int targetColumn,Vector val);
void matrix_apply_sumRow_to_row(Matrix mat,unsigned int targetRow,unsigned int* rowSumPositions,unsigned int sumSize);

Vector matrix_left_multiply(Vector v,Matrix mat);
Vector matrix_right_mutiply(Matrix mat, Vector v);


int matrix_row_is_zero(Matrix mat,unsigned int index);
int matrix_column_is_zero(Matrix mat,unsigned int index);

unsigned int row_reduce_matrix_with_column_permutation(Matrix mat);

void matrix_reduce_column(Matrix mat,unsigned int columnIndex,unsigned int pivotRow);

Matrix rawmatrix_as_matrix(RawMatrix mat);

void print_matrix(Matrix mat);

#endif