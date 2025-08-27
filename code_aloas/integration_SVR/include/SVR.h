#ifndef SVR_H
#define SVR_H

#include "matrix_reductions.h"
#include "rawmatrix.h"
#include "list.h"
#include "linearcode.h"
#include "sparse_vector.h"



List SVR(Matrix H_b,const unsigned int d_s,const unsigned int w_thres,const unsigned int N_iter); // return a list of vectors

/*find_sparse_vector_SVR samples some of the received words and samples some coordinates then applies SVR to the resulting matrix
the function stops immediately after a non zero sparse vector is found. It returns a zero vector if it doesn't find any before [iterationLimit] sampling and SVR number of iterations */
Vector find_sparse_vector_SVR(Bits** receivedWords,unsigned int numReceived,const unsigned int n,unsigned int k,const unsigned int ns,const unsigned int ms,const unsigned int d_s,const unsigned int w_thres,const unsigned int N_iter, unsigned int iterationLimit);

/*same as SVR but single_SVR stops immediately after a non zero sparse vector is found. It returns a zero vector if it doesn't find any before N_iter iterations*/
Vector single_SVR(RawMatrix H_b,const unsigned int d_s,const unsigned int w_thres,const unsigned int N_iter,int* found);

RawMatrix reconstruction_SVR(Bits** receivedWords,unsigned int numReceived,const unsigned int n,unsigned int k,const unsigned int ns,const unsigned int ms,const unsigned int d_s,const unsigned int w_thres,const unsigned int N_iter,const unsigned int circulantBlockSize,const unsigned int limSamplingIterations);
void bit_flipping_correction(Vector vector,SparseVector* sparseParityVectors,unsigned int numberParityVectors,unsigned int maxIteration);
RawMatrix partial_corrected(RawMatrix receivedWords,SparseVector* sparseParityVectors,unsigned int numParityVecs,unsigned int maxIter);

#endif