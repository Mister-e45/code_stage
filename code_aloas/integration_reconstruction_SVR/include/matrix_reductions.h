#ifndef MATRIX_REDUCTIONS_H
#define MATRIX_REDUCTIONS_H

#include "rawmatrix.h"
#include "matrix.h"

unsigned int left_block_row_reduce_rawmatrix(RawMatrix mat,Permutation initial);
Vector rawmatrix_row_PGE(RawMatrix mat,unsigned int t,Permutation col_sigma);


unsigned int left_block_row_reduce_matrix(Matrix mat);
Vector matrix_row_PGE(Matrix mat,unsigned int t);
#endif