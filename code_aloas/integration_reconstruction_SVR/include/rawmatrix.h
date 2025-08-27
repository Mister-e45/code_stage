#ifndef RAWMATRIX_H
#define RAWMATRIX_H

#include "vector.h"
#include "sparse_vector.h"

extern const unsigned int Bits_size;


struct RawMatrix{
    unsigned int nrows;
    unsigned int ncols;
    Bits** rows;
};

struct SparseRawMatrix{
    int nrows;
    int ncols;
    struct SparseVector* rows;
    int* indexRows; // les entrées de ce tableau sont les indices des lignes non nulle de la matrice
    int numberOfOnes;
};

typedef struct RawMatrix RawMatrix;
typedef struct SparseRawMatrix SparseRawMatrix;

RawMatrix create_rawmatrix(unsigned int nrows,unsigned int ncols);
RawMatrix create_empty_rows_matrix(unsigned int nrows);
RawMatrix create_rawmatrix_from_bits_array(Bits** vectors,unsigned int numWords,unsigned int n); // does not copy "vectors"
void destroy_rawmatrix(RawMatrix mat);

RawMatrix random_rawmatrix(unsigned int nrows,unsigned int ncols);

Bit rawmatrix_get_value(RawMatrix mat,unsigned int i,unsigned int j);
Vector rawmatrix_get_row_vector(RawMatrix mat,unsigned int i); // carefull when using this, the vector IS the row not a copy
Vector rawmatrix_get_row_copy(RawMatrix mat,unsigned int i);
Vector rawmatrix_get_column_copy(RawMatrix mat,unsigned int j);
RawMatrix rawmatrix_copy(RawMatrix mat);

void rawmatrix_write(RawMatrix mat,unsigned int i,unsigned int j,Bit val);
RawMatrix identity_rawmatrix(unsigned int n);
RawMatrix rawmatrix_one_on_diagonal(unsigned int nrows,unsigned int ncols);

void print_rawmatrix(RawMatrix mat);
void print_rawmatrix_coordinates(RawMatrix mat); // no commas in the display

void rows_sum(RawMatrix mat,unsigned int* rows,unsigned int sumSize,Vector sum);//gives a vector containing the sum of the rows indexed in the argument "rows"
void sum_rows_from_vector(RawMatrix mat,Vector v,Vector result); // sum of rows according to non-zero coordinates in the vector v
Vector rawmatrix_sum_rows_from_positions(RawMatrix mat,unsigned int* positions,unsigned int sumSize);

void rawmatrix_apply_sumRow_to_row(RawMatrix mat,unsigned int targetRow,unsigned int* rowSumPositions,unsigned int sumSize);
void rawmatrix_add_row_to_row(RawMatrix mat,unsigned int target,unsigned int rowToAdd);

void rawmatrix_write_row(RawMatrix mat,unsigned int i,Vector v); 
void rawmatrix_replace_row(RawMatrix mat,unsigned int i,Vector v);// v is bound to mat
void rawmatrix_write_column(RawMatrix mat,unsigned int j,Vector v); // v is not bound to mat, its values are copied

RawMatrix rawmatrix_from_vector_array(Vector* array,unsigned int nrows);
RawMatrix rawmatrix_from_bits_array(Bits** array,unsigned int nrows,unsigned int ncols); // copies the content in array into a RawMatrix

RawMatrix transposed_rawmatrix(RawMatrix mat);

Vector left_multiply(Vector v,RawMatrix mat);
Vector right_multiply(RawMatrix mat,Vector v);

int rawmatrix_row_is_zero(RawMatrix mat,unsigned int rowIndex);
unsigned int rawmatrix_find_one_under_row_in_column(RawMatrix mat,unsigned int columnIndex,unsigned int rowIndex,int* isFound);
void rawmatrix_swap_rows(RawMatrix mat,unsigned int i,unsigned int j);

void rawmatrix_reduce_column(RawMatrix mat,unsigned int columnIndex,unsigned int pivotRow);

void rawmatrix_nullify_under_row(RawMatrix mat,unsigned int columnIndex,unsigned int pivotRow);
unsigned int row_echelonize_rawmatrix(RawMatrix mat);


void rawmatrix_apply_permutation_to_rows_addresses(RawMatrix* mat,Permutation sigma);
unsigned int row_reduce_rawmatrix(RawMatrix mat);
unsigned int row_reduce_rawmatrix_with_column_permutation(RawMatrix mat,Permutation intitialColumnPermutation);//returns the index of the first null row

Vector* rawmatrix_as_vector_array(RawMatrix mat);//the vector array returned contains the vector data in mat, destroying returned vectors would destroy the rows of mat

void print_permuted_rawmatrix(RawMatrix mat,Permutation columnPermutation);
#endif