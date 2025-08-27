#include "vector.h"

const unsigned int Bits_size=sizeof(Bits)*8;

const Bits bitmasks[64]={0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000,0x10000,0x20000,0x40000,0x80000,0x100000,0x200000,0x400000,0x800000,0x1000000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000,0x80000000,0x100000000,0x200000000,0x400000000,0x800000000,0x1000000000,0x2000000000,0x4000000000,0x8000000000,0x10000000000,0x20000000000,0x40000000000,0x80000000000,0x100000000000,0x200000000000,0x400000000000,0x800000000000,0x1000000000000,0x2000000000000,0x4000000000000,0x8000000000000,0x10000000000000,0x20000000000000,0x40000000000000,0x80000000000000,0x100000000000000,0x200000000000000,0x400000000000000,0x800000000000000,0x1000000000000000,0x2000000000000000,0x4000000000000000,0x8000000000000000};

#ifdef COMPLEXITY
unsigned long long int complexity; // used to measure algorithmic complexity

void reset_complexity(){
    complexity=0;
}

unsigned long long int get_complexity(){
    return complexity;
}

void increment_complexity(unsigned int increment){
    complexity+=increment;
}
#endif

unsigned int bitToBits(unsigned int n){ // donne le nombre d'octets nécéssaire pour stocker le nombre de bits passé entrée
    return n%Bits_size==0 ? n/Bits_size : (n/Bits_size)+1;
}

int not_in(unsigned int* array,unsigned int arraySize,unsigned int element){
    for(unsigned int i=0;i<arraySize;++i){
        if(element==array[i]){
            return 0;
        }
    }
    return 1;
}

unsigned int* sample_random_positive_integers(unsigned int max,unsigned int sampleSize){
    unsigned int* sample=calloc(sampleSize,sizeof(unsigned int));
    if(sample==0){
        printf("could not allocate memory for positive integers sampling\n");
        return sample;
    }
    else{
        int zeroFound=0;
        unsigned int counter=0;
        while(counter<sampleSize){
            unsigned int r=rand()%max;
            if(r==0 && !zeroFound){
                zeroFound=1;
                ++counter;
            }
            else if(not_in(sample,sampleSize,r)){
                sample[counter]=r;
                ++counter;
            }
            
        }
    }
    return sample;

}

Vector create_vector(unsigned int n){
    int integerBlocks= bitToBits(n); // on  cree suffisament de uint pour contenir n bits
    Bits* p=calloc(integerBlocks,Bits_size);
    Vector v;
    if(p==NULL){
        printf("error while creating vector\n");
        v.bits=NULL;
        v.size=0;
        return v;
    }
    
    v.bits=p;
    v.size=n;
    return v;
}


Vector vector_from_array(Bit* array,unsigned int n){
    unsigned int BitsBlocks= bitToBits(n); // on  cree suffisament de uint pour contenir n bits
    Vector v;
    Bits* p=calloc(BitsBlocks,Bits_size);
    if(p==NULL){
        printf("error while creating vector\n");
        v.bits=0;
        return v;
    }
    v.bits=p;
    v.size=n;
    for(unsigned int i=0;i<BitsBlocks-1;++i){
        for(unsigned int k=0;k<Bits_size;++k){
            if(array[i*Bits_size+k]){
                v.bits[i]^=bitmasks[k];
            }
        }
    }
    for(unsigned int k=0;k<n%Bits_size;++k){
        if(array[(BitsBlocks-1)*Bits_size+k]){
            v.bits[BitsBlocks-1]^=bitmasks[k];
        }
    }
    return v;
}


Vector vector_from_string(char* array,unsigned int n){
    unsigned int BitsBlocks= bitToBits(n); // on  cree suffisament de uint pour contenir n bits
    Bits* p=calloc(BitsBlocks,Bits_size);
    if(p==NULL){
        printf("error while creating vector\n");
    }
    Vector v;
    v.bits=p;
    v.size=n;
    for(unsigned int i=0;i<BitsBlocks-1;++i){
        for(unsigned int k=0;k<Bits_size;++k){
            if(array[i*Bits_size+k]=='1'){
                v.bits[i]^=bitmasks[k];
            }
        }
    }
    for(unsigned int k=0;k<n%Bits_size;++k){
        if(array[(BitsBlocks-1)*Bits_size+k]=='1'){
            v.bits[BitsBlocks-1]^=bitmasks[k];
        }
    }
    return v;
}

Vector vector_copy(Vector v){
    Vector copy=create_vector(v.size);
    unsigned int numBlocks=bitToBits(v.size);
    for(unsigned int i=0;i<numBlocks;++i){
        copy.bits[i]=v.bits[i];
    }
    return copy;
}


void destroy_vector(struct Vector v){
    if(v.size!=0){
        free(v.bits);
        v.bits=0;
        v.size=0;
    }

}

Vector vector_from_bits_array(Bits* array,unsigned int vectorSize){
    Vector v;
    v.size=vectorSize;
    unsigned int blocks=bitToBits(vectorSize);
    Bits* coords=calloc(blocks,Bits_size);
    if(coords==NULL){
        printf("could not allocate memory to create vector from bits\n");
        v.bits=0;
        return v;
    }
    else{
        for(unsigned int k=0;k<blocks;++k){
            coords[k]=array[k];
        }
        v.bits=coords;
        return v;
    }

}

Bit get_coordinate(struct Vector v,unsigned int index){
    int block=index/Bits_size;
    int bitInBits=index%Bits_size;
    return (v.bits[block] & bitmasks[bitInBits])? 1:0 ;
}

void put_coordinate(struct Vector v,unsigned int index,Bit val){
    int block=index/Bits_size;
    int bitInBits=index%Bits_size;
    if(val){
        v.bits[block]|=bitmasks[bitInBits];
    }
    else{
        v.bits[block]&= (~bitmasks[bitInBits]);
    }
        
}

void print_coordinates(Vector v){ 
    for(unsigned int i=0;i<v.size-1;++i){
        Bits val= get_coordinate(v,i);
        val ? printf("1 "):printf("0 ");
    }
    Bits val= get_coordinate(v,v.size-1);
    val ? printf("1"):printf("0");
}

void print_vector(Vector v){
    printf("[");
    for(unsigned int i=0;i<v.size-1;++i){
        Bits val= get_coordinate(v,i);
        val ? printf("1,"):printf("0,");
    }
    Bits val= get_coordinate(v,v.size-1);
    val ? printf("1]\n"):printf("0]\n"); 
}

void print_vector_from_pointer(void* p){
    print_vector(*((Vector*)p));
}



Bit scalar_product(struct Vector v1,struct Vector v2){
    if(v1.size!= v2.size){
        printf("error vector sizes do not match for scalar product\n");
    }
    else{
        unsigned int numBitss =  bitToBits(v1.size);
        Bits sumBits=0;
        for(unsigned int i=0;i<numBitss;++i){
            sumBits ^= v1.bits[i] & v2.bits[i]; // on fait le produit bit a bit sur chaque octet puis on somme partiellement
        }
        int count=0; // pour compter ne nombre de 1
        for(unsigned int i=0;i<Bits_size;++i){ // on effectue la somme totale
            if(bitmasks[i] & sumBits){
                ++count;
            }

        }
        return count%2;
    }
    return 3;
}



int HW(Vector v){
    unsigned int numBitss= bitToBits(v.size);
    int weight=0;
    for(unsigned int i=0;i<numBitss;++i){
        Bits b=v.bits[i];
        for(unsigned int k=0;k<Bits_size;++k){
            weight+= bitmasks[0] & b;
            b=b>>1;
        }
    }
    return weight;
}




void vector_equal(Vector* v1,Vector v2){ // the function makes it that v1 takes the value of v2
    v1->size=v2.size;
    int numBlocks=bitToBits(v2.size);
    for(int k=0;k<numBlocks;++k){
        v1->bits[k]=v2.bits[k];
    }
}

Bit vector_is_equal(Vector v1,Vector v2){ // return 1 if both vectors are equal
    
    if(v1.size!=v2.size){
        return 0;
    }
    else{
        int numBlocks=bitToBits(v1.size);
        for(int k=0;k<numBlocks;++k){
            if(v1.bits[k]!=v2.bits[k]){
                return 0;
            }
        }
        return 1;
    }
    
}

void vector_add(Vector v1,Vector v2,Vector result){
    if(v1.size!=v2.size){
        printf("cannot sum vector of size %i and size %i",v1.size,v2.size);
    }
    else{
        unsigned int numBlocks=bitToBits(v1.size);
        for(unsigned int i=0;i<numBlocks;++i){
            result.bits[i]=v1.bits[i] ^ v2.bits[i];
        }
#ifdef COMPELXITY
    increment_complexity(v1.size);
#endif
    }

}

void star_product(Vector v1,Vector v2,Vector result){
    int numBlocks=bitToBits(v1.size);
    if(v1.size!=v2.size){
        printf("non matching size in size product sizes: %i and %i \n",v1.size,v2.size);
    }
    else{
        for(int k=0;k<numBlocks;++k){
            result.bits[k]=v1.bits[k] & v2.bits[k];
        }
#ifdef COMPELXITY
    increment_complexity(v1.size);
#endif
    }

}


Vector random_vector(unsigned int size){
    Vector out=create_vector(size);
    unsigned int numBlocks=bitToBits(size);
    for(unsigned int k=0;k<numBlocks;++k){
        srand(rand()); //for better quality randomness
        if(Bits_size>=sizeof(int)*8){
            const unsigned int rounds=Bits_size/(sizeof(int)*8);
            for(unsigned int l=0;l<rounds;++l){
                out.bits[k]<<=(l*sizeof(int)*8);
                int r=rand();
                out.bits[k]^=r;
                if(rand()%2){ // rand() only gives positive values so to make the most significant bit follow a 1/2 bernouilli law we make it negative with 1/2 probability
                    out.bits[k]^=0x80000000UL;
                }
            }
        }

        else{
            int r=rand();
            out.bits[k]=(Bits)r;
        }
    }
    unsigned int remainingBits=size%(Bits_size);
    if(remainingBits){
        out.bits[numBlocks-1]>>=(Bits_size - remainingBits); // this insures the correctness of the functions checking if vector is null or if 2 vectors are equal
    }
    return out;
}

void write_random_bits_in_vector(Vector v){
    unsigned int numBlocks=bitToBits(v.size);
    for(unsigned int k=0;k<numBlocks;++k){
        srand(rand()); //for better quality randomness
        v.bits[k]=0;
        if(Bits_size>=sizeof(int)*8){
            const unsigned int rounds=Bits_size/(sizeof(int)*8);
            for(unsigned int l=0;l<rounds;++l){
                v.bits[k]<<=(l*sizeof(int)*8);
                int r=rand();
                v.bits[k]^=r;
                if(rand()%2){ // rand() only gives positive values so to make the most significant bit follow a 1/2 bernouilli law we make it negative with 1/2 probability
                    v.bits[k]^=0x80000000UL;
                }
            }
        }

        else{
            int r=rand();
            v.bits[k]=(Bits)r;
        }
    }
}

Vector error_vector(unsigned int size,double prob){
    Vector out=create_vector(size);
    for(unsigned int i=0;i<size;++i){
        double trial=( (double)rand() )/RAND_MAX;
        if(trial<=prob){
            put_coordinate(out,i,1);
        }
    }
    return out;
}

void write_error_positions_in_vector(Vector v,double* prob){
    vector_set_to_zero(v);
    for(unsigned int i=0;i<v.size;++i){
        double trial=( (double)rand() )/RAND_MAX;
        if(trial<=*prob){
            put_coordinate(v,i,1);
        }
    }
}

Vector permuted_vector(Permutation sigma,Vector v){
    Vector res;
    if(v.size!=sigma.size){
        printf("cannot apply permutation on vector with wrong length: permuation length is %i and vector length is %i\n",sigma.size,v.size);
        res.size=0;
        res.bits=0;
        return res;
    }
    else{
        res=create_vector(v.size);
        for(unsigned int k=0;k<res.size;++k){
            Bit val=get_coordinate(v,sigma.permutation[k]);
            if(val)
                put_coordinate(res,k,val);
        }
        return res;
    }
}

void permute_vector(Permutation sigma,Vector* v){ // avoid use is you are using Vector struture as a temporary container for Bits
    Vector res=permuted_vector(sigma,*v);
    free(v->bits);
    v->bits=res.bits;
}

int vector_is_zero(Vector v){
    unsigned int blocks=bitToBits(v.size);
    for(unsigned int k=0;k<blocks;++k){
        if(v.bits[k]!=0){
            return 0;
        }
    }
    return 1;
}

Vector projection(Vector vec, unsigned int* keptPosition,unsigned int numberOfPosition){
    Vector projected=create_vector(numberOfPosition);
    for(unsigned int k=0;k<numberOfPosition;++k){
        Bit val=get_coordinate(vec,keptPosition[k]);
        if(val){
            put_coordinate(projected,k,1);
        }
        
    }
    return projected;
}

Vector extension(Vector vec,unsigned int* positions,unsigned int finalSize){
    Vector extended=create_vector(finalSize);
    for(unsigned int k=0;k<vec.size;++k){
        Bit val=get_coordinate(vec,k);
        if(val){
            put_coordinate(extended,positions[k],1);
        }
    }
    return extended;
}

void cyclic_right_shift(Vector* v,unsigned int shift){
    unsigned int count=0;
    unsigned int initialPosition=0;
    unsigned int currentPosition=0;
    Bit tempVal=get_coordinate(*v,currentPosition%v->size);
    while(count<v->size){
        unsigned int writingPosition=(currentPosition+shift)%v->size;
        if(tempVal){
                tempVal=get_coordinate(*v,writingPosition);
                put_coordinate(*v,writingPosition,1);
        }
        else{
            tempVal=get_coordinate(*v,writingPosition);
            put_coordinate(*v,writingPosition,0);
        }
        ++count;
        if(writingPosition!=initialPosition){
            currentPosition=writingPosition;
        }
        else{
            currentPosition=(initialPosition+1)%v->size;
            tempVal=get_coordinate(*v,currentPosition%v->size);
            initialPosition=currentPosition;
        }
    }
}

Vector const_weight_random_vector(unsigned int n,unsigned int weight){
    unsigned int* pos=sample_random_positive_integers(n,weight);
    Vector v=create_vector(n);
    for(unsigned int k=0;k<weight;++k){
        put_coordinate(v,pos[k],1);
    }
    free(pos);
    return v;
}

Vector cyclic_right_shift_vector(Vector v,unsigned int shift){
    Vector res=vector_copy(v);
    cyclic_right_shift(&res,shift);
    return res;
}

void free_vector(void* p){
    destroy_vector(*((Vector*)p));
}

void change_vector_using_string(Vector* vec, char* string){ // must be a null terminted sequence!!
    char b=string[0];
    unsigned int count=0;
    unsigned int size=strlen(string);
    if(vec->size!=size){
        destroy_vector(*vec);
        create_vector(size);
    }
    while(b!=0){
        if(b=='1'){
            put_coordinate(*vec,count,1);
        }
        else{
            put_coordinate(*vec,count,0);
        }
        ++count;
        b=string[count];
    }
}

void vector_set_to_zero(Vector v){
    const unsigned int blocks=bitToBits(v.size);
    for(unsigned k=0;k<blocks;++k){
        v.bits[k]=0;
    }
}

Vector bits_to_vector(Bits* b,unsigned int vectorLength){
    Vector v;
    v.size=vectorLength;
    v.bits=b;
    return v;
}

Vector vector_from_positions_array(unsigned int* positions,unsigned int n,unsigned int weight){
    Vector v=create_vector(n);
    for(unsigned int i=0;i<weight;++i){
        put_coordinate(v,positions[i],1);
    }
    return v;
}