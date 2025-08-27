#ifndef PGE_H
#define PGE_H

#include "vector.h"
#include "rawmatrix.h"
#include "binary_tree.h"
#include "list.h"
#include "matrix_reductions.h"
#include "statistics.h"
#include "SVR.h"
#include <math.h>


typedef unsigned int* Positions; // used to represent the selected columns in PGE algorithm

unsigned long int binomial(unsigned int n,unsigned int k);
void init_const_weight_sequence(unsigned int* vector,unsigned int weight);
void next_vector_in_const_weight_sequence(unsigned int* vector,unsigned int n,unsigned int weight,int* reachedEnd);

void transpose_sub_matrix(Matrix mat,unsigned int startingRow,unsigned int startingCol,RawMatrix* result);

/*returns a vector*/
Vector find_parity_vector_PGE(Bits** receivedWords,unsigned int numberReceived,unsigned int numberWordsToUse,double* errorRate,unsigned int n,unsigned int t,unsigned int p,unsigned int iterationLimit);
/*returns a list of vector data (Bits type)*/
List find_parity_check_vectors_PGE(Bits** receivedWords,unsigned int numberReceived,unsigned int numberOfWordsToUse,double* errorRate,unsigned int n,unsigned int t,unsigned int p,unsigned int collisionListMaxSize,unsigned int numIter,unsigned int maxNumVectorsFind);

RawMatrix QCLDPC_reconstruction_PGE(Bits** receivedWords,unsigned int numberReceived,unsigned int numberOfWordsToUse,double* errorRate,unsigned int n,unsigned int numberOfIndependentVectorToFind,unsigned int circulantBlockSize,unsigned int t,unsigned int p,unsigned int N_iter,unsigned int itLimit);


#endif