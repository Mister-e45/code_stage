#ifndef SPARCE_VECTOR_H
#define SPARCE_VECTOR_H


#include "vector.h"
#include "permutation.h"

struct SparseVector{
    unsigned int size;
    unsigned int numberOfOnes;
    unsigned int* entries;
};
typedef struct SparseVector SparseVector;

SparseVector create_SparseVector(unsigned int n);
SparseVector sparse_from_array(unsigned int* array,int n,int numberOfEntries);
SparseVector sparse_from_char_array(char* array,int n);
SparseVector sparse_from_vector(Vector vector);


void destroy_sparse(SparseVector* v);
int HW_sparse_vector(SparseVector v);

Bit scalar_product_sparse(SparseVector v1,SparseVector v2);
Bit scalar_product_sparse_and_vector(SparseVector sparse,Vector vector);


void sparse_vector_write_one(SparseVector* vector,unsigned int i);
void sparse_vector_write_zero(SparseVector* vector,unsigned int i);
void sparce_vector_write_in_sparse_vector(SparseVector* vector,unsigned int i,Bit value);



#endif