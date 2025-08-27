#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "permutation.h"



#ifdef COMPLEXITY
extern unsigned long long int complexity;
unsigned long long int get_complexity();
void increment_complexity(unsigned int increment);
void reset_complexity();
#endif

// the base field if GF2 
typedef unsigned long int Bits;
typedef unsigned char Bit;

extern const Bits bitmasks[64];
extern const unsigned int Bits_size;

struct Vector {
    unsigned int size;
    Bits* bits;
};




typedef struct Vector Vector;

unsigned int bitToBits(unsigned int n);

unsigned int bitToBits(unsigned int bits);
int not_in(unsigned int* array,unsigned int arraySize,unsigned int element);
unsigned int* sample_random_positive_integers(unsigned int max,unsigned int sampleSize);

Vector create_vector(unsigned int n);

Vector vector_from_bits_array(Bits* array,unsigned int vectorSize);// copies the content into the returned vector
Vector vector_from_positions_array(unsigned int* positions,unsigned int n,unsigned int weight);
void vector_set_positions_to_one(Vector vector,unsigned int* positions,unsigned int positonsCount,unsigned int offset);

Vector random_vector(unsigned int size);
Vector const_weight_random_vector(unsigned int n,unsigned int weight);
void write_random_bits_in_vector(Vector v);

Vector vector_from_array(Bit* array,unsigned int n);
Vector vector_from_string(char* array,unsigned int n);
void change_vector_using_string(Vector* vec, char* string); //string must be a null terminted sequence!!
void vector_set_to_zero(Vector v);

Vector vector_copy(Vector v);
void vector_equal(Vector* v1,Vector v2); // v1 takes the value of v2

Vector bits_to_vector(Bits* b,unsigned int vectorLength);//used to turn bits data into a vector which holds that SAME data

void destroy_vector(struct Vector v);
void free_vector(void* p);


Bit get_coordinate(struct Vector v,unsigned int index);
void put_coordinate(struct Vector v,unsigned int index,Bit val);

void print_coordinates(Vector v); // prints only the coordinates separated by one space
void print_vector(struct Vector v); // prints a formated version of the vector with brackets and commas
void print_vector_from_pointer(void* p);//the same thing but in a standardized form to allow printing from data structures

Bit vector_is_equal(Vector v1,Vector v2);
int vector_is_zero(Vector v);

Bit scalar_product(struct Vector v1,struct Vector v2);
void vector_add(Vector v1,Vector v2,Vector result);
void star_product(Vector v1,Vector v2,Vector result);//returns a vector whose coordinates are the pairwise product of v1 and v2

Vector error_vector(unsigned int size,double prob);
void write_error_positions_in_vector(Vector v,double* prob);

Vector permuted_vector(Permutation sigma,Vector v);
void permute_vector(Permutation sigma,Vector* v);

int HW(Vector v);

Vector projection(Vector vec, unsigned int* keptPosition,unsigned int numberOfPosition); // keptPositinos must be sorted
Vector extension(Vector vec,unsigned int* positions,unsigned int finalSize);

void cyclic_right_shift(Vector* v,unsigned int shift);
Vector cyclic_right_shift_vector(Vector v,unsigned int shift);

#endif