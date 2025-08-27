#include "permutation.h"

void swap_values_in_array(unsigned int* array,unsigned int i,unsigned int j){
    if(i!=j){
        array[i]^=array[j];
        array[j]^=array[i];
        array[i]^=array[j];
    }
}

Permutation create_permutation(unsigned int n){
    Permutation sigma;
    sigma.size=n;
    sigma.permutation=calloc(n,sizeof(unsigned int));
    if(sigma.permutation==NULL){
        printf("could not allocate memory to create permutation\n");
        sigma.size=0;
        return sigma;
    }
    else{
        for(unsigned int i=0;i<sigma.size;++i){
            sigma.permutation[i]=i;
        }
        return sigma;
    }
} 

int is_in_array(unsigned int element,unsigned int* array,unsigned int arraySize){
    for(unsigned int k=0;k<arraySize;k++){
        if(element==array[k]){
            return 1;
        }
    }
    return 0;
}

Permutation random_transposition(unsigned int n){
    Permutation p=create_permutation(n);
    unsigned int i=rand()%n;
    unsigned int j=rand()%n;
    while(j==i){
        j=rand()%n;
    }
    p.permutation[i]=j;
    p.permutation[j]=i;
    return p;
}

Permutation fast_random_permutation(unsigned int size){// fisher-yatte algorithm
    Permutation p=create_permutation(size);
    for(unsigned int k=size;k>0;--k){
        unsigned int j=rand()%k;
        swap_values_in_array(p.permutation,k-1,j);
        print_permutation(p);
    }
    return p;
}

unsigned int* give_random_distinct_elements_in_range(unsigned int sampleSize,unsigned int max){ // fisher-yatte algorithm
    Permutation p=create_permutation(max);
    unsigned int limit=max-sampleSize;
    for(unsigned int k=max;k>limit;--k){
        unsigned int j=rand()%k;
        swap_values_in_array(p.permutation,k-1,j);
    }
    
    unsigned int* out=calloc(sampleSize,sizeof(unsigned int));
    if(out==0){
        printf("could not allocate memory to store random distincs elements\n");
    }
    for(unsigned int k=0;k<sampleSize;++k){
        out[k]=p.permutation[k+limit];
    }
    destroy_permutation(p);

    return out;
}

void random_distinct_elements_in_range(unsigned int sampleSize,unsigned int min,unsigned int max,unsigned int* array){ // fisher-yatte algorithm
    Permutation p=create_permutation(max);
    unsigned int limit=max-min-sampleSize;
#ifdef DEBUG
if(sampleSize>max){
    printf("in request for generating random distinct elements the sample size exceeds the range size\n");
    printf("requested sample size= %u and range size= %u\n",sampleSize,max);
}
#endif

    for(unsigned int k=max;k>limit;--k){
        unsigned int j=rand()%k;
        swap_values_in_array(p.permutation,j,k-1);
    }
    for(unsigned int k=0;k<sampleSize;++k){
        array[k]=p.permutation[k+limit];
    }
    destroy_permutation(p);
}



void compose_random_transposition(Permutation p){
    unsigned int i=rand()%p.size;
    unsigned int j=rand()%p.size;
    while(j==i){
        j=rand()%p.size;
    }
    p.permutation[i]^=p.permutation[j];
    p.permutation[j]^=p.permutation[i];
    p.permutation[i]^=p.permutation[j];
}

void compose_random_transposition_and_get(Permutation p,unsigned int* a,unsigned int* b){
    unsigned int i=rand()%p.size;
    unsigned int j=rand()%p.size;
    while(j==i){
        j=rand()%p.size;
    }
    *a=i;
    *b=j;
    p.permutation[i]^=p.permutation[j];
    p.permutation[j]^=p.permutation[i];
    p.permutation[i]^=p.permutation[j];
}

Permutation random_permutation(unsigned int n){
    unsigned int count=0;
    unsigned int s;
    int placedZero=0;
    Permutation p;
    p.permutation=calloc(n,sizeof(unsigned int));
    p.size=n;
    if(p.permutation==NULL){
        printf("could not allocate memoty to create random permutation\n");
        p.size=0;
        return p;
    }
    while(count<n){
        s=rand()%n;
        if(s==0 && !placedZero){
                count++;
                placedZero=1;
        }
        else if(!(is_in_array(s,p.permutation,n))){
            p.permutation[count]=s;
            count++;
        }
    }
        return p; 
}

void destroy_permutation(Permutation sigma){
    sigma.size=0;
    free(sigma.permutation);
    sigma.permutation=0;
}

Permutation permutation_composition(Permutation sigma1,Permutation sigma2){
    
    if(sigma1.size!=sigma2.size){
        printf("cannot compose permutations whith unequal lengths: length %i and %i",sigma1.size,sigma2.size);
        Permutation result=create_permutation(1);
        result.permutation=0;
        result.size=0;
        return result;
    }
    else{
        Permutation result=create_permutation(sigma1.size);
        for(unsigned int k=0;k<result.size;++k){
            result.permutation[k]=sigma2.permutation[sigma1.permutation[k]];
        }
        return result;
    }
}

Permutation inverse_permutation(Permutation sigma){
    Permutation inv=create_permutation(sigma.size);
    for(unsigned int k=0;k<inv.size;++k){
        inv.permutation[sigma.permutation[k]]=k;
    }
    return inv;
}

void get_inverse_permutation(Permutation sigma,Permutation invSigma){
    for(unsigned int k=0;k<invSigma.size;++k){
        invSigma.permutation[sigma.permutation[k]]=k;
    }
}

Permutation transposition(unsigned int i,unsigned int j,unsigned int n){
    Permutation tau=create_permutation(n);
    tau.permutation[i]=j;
    tau.permutation[j]=i;
    return tau;
}



void apply_transposition_to_permutation(Permutation sigma,unsigned int i,unsigned int j){
    sigma.permutation[i]^=sigma.permutation[j];
    sigma.permutation[j]^=sigma.permutation[i];
    sigma.permutation[i]^=sigma.permutation[j]; // echange les valeurs dans sigma[i] et sigma[j]
}

void print_permutation(Permutation p){
    for(unsigned int i=0;i<p.size;++i){
        printf("%u ",p.permutation[i]);
    }
    printf("\n");
}

Permutation permutation_copy(Permutation sigma){
    Permutation copy=create_permutation(sigma.size);
    for(unsigned int i=0;i<copy.size;++i){
        copy.permutation[i]=sigma.permutation[i];
    }
    return copy;
}