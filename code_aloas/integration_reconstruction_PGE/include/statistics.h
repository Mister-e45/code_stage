#ifndef STATISTICS_H
#define STATISTICS_H

#include "vector.h"
#include <math.h>


double parity_check_fail_probability(double errorRate,unsigned int parityVectorWeight);
int is_likely_parity(Bits** codeWords,Vector candidate,unsigned int numberCodeWords,double errorRate);
#endif