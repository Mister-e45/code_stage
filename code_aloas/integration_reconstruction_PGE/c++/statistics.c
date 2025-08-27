#include "statistics.h"


double parity_check_fail_probability(double errorRate,unsigned int parityVectorWeight){
    return ( 1.0-pow(1.0-2*errorRate,parityVectorWeight) )/2.0;
}

int is_likely_parity(Bits** codeWords,Vector candidate,unsigned int numberCodeWords,double errorRate){
    unsigned int ones=0;
    for(unsigned int k=0;k<numberCodeWords;++k){
        Vector word=bits_to_vector(codeWords[k],candidate.size);
        Bit scal= scalar_product(word,candidate);
        if(scal){
            ++ones;
        }
             
    }
    unsigned int w=HW(candidate);
    double probabilityOfOne=parity_check_fail_probability(errorRate,w);
    /*if(numberCodeWords*probabilityOfOne<5 || (1-probabilityOfOne)*numberCodeWords<5){
        printf("might not be enough code words to have a good assesment of likelihood of vector being a partiy vector\n");
    }*/

    int expectancy=probabilityOfOne*numberCodeWords;
    double avg= (double)ones/(double)numberCodeWords;
    double param=probabilityOfOne*(1-probabilityOfOne)/numberCodeWords;
    double intervalHalfSize=1.96*sqrt(param);
    double upperBound=probabilityOfOne+intervalHalfSize;
    double lowerBound=probabilityOfOne-intervalHalfSize;
    if(avg<=upperBound && avg>=lowerBound){
        return 1;
    }
    else{
        return 0;
    }

}