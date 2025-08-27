#include "sparse_vector.h"


unsigned int* create_entries_array(unsigned int size){
    unsigned int* entries=calloc(size,sizeof(unsigned int));
    if(entries==0){
        printf("could not allocate memory to create entries of sparse vector\n");
        return entries;
    }
    return entries;
}

unsigned int find_in_sorted_array(unsigned int* array,unsigned int arraySize,unsigned int value,unsigned int defaultValue){
    unsigned int read=arraySize/2;
    unsigned int searchIntervalSize=arraySize;
    while(searchIntervalSize!=0){
        unsigned int readVal=array[read];
        if(readVal==value){
            return read;
        }
        else{
            searchIntervalSize=searchIntervalSize/2;
            if(readVal>value){
                read-=searchIntervalSize;
            }
            else{
                read+=searchIntervalSize;
            }
        }
    }
    return defaultValue;
}


SparseVector create_SparseVector(unsigned int n){
    SparseVector vector;
    vector.size=n;
    vector.entries=NULL;
    vector.numberOfOnes=0;
    return vector;
}

SparseVector sparse_from_array(unsigned int* array,int n,int numberOfEntries){
    SparseVector vector;
    vector.size=n;
    vector.entries=create_entries_array(numberOfEntries);
    vector.numberOfOnes=numberOfEntries;
    for(unsigned int k=0;k<numberOfEntries;++k){
        vector.entries[k]=array[k];
    }
    return vector;
}

void append_to_entries_array(SparseVector* sparseVec,unsigned int entry){
    unsigned int* newArray=calloc(sparseVec->numberOfOnes+1,sizeof(unsigned int));
    if(newArray==NULL){
        printf("could not allocate memory to append entries of sparse vector\n");
    }
    for(unsigned int k=0;k<sparseVec->numberOfOnes;++k){
        newArray[k]=sparseVec->entries[k];
    }
    newArray[sparseVec->numberOfOnes]=entry;
    sparseVec->numberOfOnes+=1;

    free(sparseVec->entries);
    sparseVec->entries=newArray;

}

SparseVector sparse_from_vector(Vector vector){
    unsigned int numBlocks=bitToBits(vector.size);
    SparseVector sparse=create_SparseVector(vector.size);
    for(unsigned int k=0;k<numBlocks;++k){
        if(vector.bits[k]!=0){
            for(unsigned int l=0;l<numBlocks;++l){
                if(vector.bits[k] && bitmasks[l]){
                    append_to_entries_array(&sparse,Bits_size*k+l);
                }
            }
        }
    }
    return sparse;
}

Bit scalar_product_sparse_and_vector(SparseVector sparse,Vector vector){
    unsigned int res=0;
    for(unsigned int k=0;k<sparse.numberOfOnes;++k){
        if(get_coordinate(vector,sparse.entries[k])){
            ++res;
        }
    }
    return res%2;
}

SparseVector random_sparse_vector(unsigned int n,unsigned int weight){
    SparseVector vec;
    vec.size=n;
    vec.numberOfOnes=weight;
    Permutation p=fast_random_permutation(n);
    vec.entries=create_entries_array(weight);
    for(unsigned int k=0;k<weight;++k){
        vec.entries[k]=p.permutation[k];
    }
    return vec;
}

SparseVector sparse_from_char_array(char* array,int n){
    SparseVector vector;
    vector.size=n;
    vector.numberOfOnes=0;
    for(unsigned int k=0;k<n;++k){
        if(array[k]=='1'){
            ++vector.numberOfOnes;
        }
    }
    vector.entries=create_entries_array(vector.numberOfOnes);
    for(unsigned int k=0;k<n;++k){
        if(array[k]=='1'){
            vector.entries[k]=k;
        }
    }

    return vector;
}

void destroy_sparse(SparseVector* v){
    free(v->entries);
    v->numberOfOnes=0;
    v->size=0;
}

int HW_sparse_vector(SparseVector v){
    return v.numberOfOnes;
}

Bit scalar_product_sparse(SparseVector v1,SparseVector v2){
    unsigned int ones=0;
    for(unsigned int i=0;i<v1.numberOfOnes;){
        for(unsigned int j=0;j<v2.numberOfOnes;){
            unsigned int v1Coord=v1.entries[i];
            unsigned int v2Coord=v2.entries[j];
            if(v1Coord==v2Coord){
                ++ones;
                ++i;
                ++j;
            }
            else if(v1Coord>v2Coord){
                ++j;
            }
            else{
                ++i;
            }
        }
    }
    return ones%2;
}

void sparse_vector_write_one(SparseVector* vector,unsigned int i){
    unsigned int index=find_in_sorted_array(vector->entries,vector->numberOfOnes,i,vector->size);
    if(index==vector->size){
        vector->numberOfOnes+=1;
        unsigned int* newEntries=create_entries_array(vector->numberOfOnes);
        int placed=0;
        for(unsigned int k=0;k<vector->size;++k){
            unsigned int entry=vector->entries[k];
            if(!placed){
                if(i<entry){
                    newEntries[k]=i;
                    placed=1;
                    --k;
                }
                else{
                    newEntries[k]=entry;
                }
            }
            else{
                newEntries[k+1]=entry;
            }
        }
        free(vector->entries);
        vector->entries=newEntries;
    }
}

void sparse_vector_write_zero(SparseVector* vector,unsigned int i){
    unsigned int index=find_in_sorted_array(vector->entries,vector->numberOfOnes,i,vector->size);
    if(index!=vector->size){
        vector->numberOfOnes-=1;
        unsigned int* newEntries=create_entries_array(vector->numberOfOnes);
        for(unsigned int k=0;k<index;++k){
            newEntries[k]=vector->entries[k];
        }
        for(unsigned int k=index+1;k<vector->numberOfOnes;++k){
            newEntries[k]=vector->entries[k];
        }
        free(vector->entries);
        vector->entries=newEntries;
    }
}

void sparce_vector_write_in_sparse_vector(SparseVector* vector,unsigned int i,Bit value){
    if(value){
       sparse_vector_write_one(vector,i);
    }
    
    else{
        sparse_vector_write_zero(vector,i);
    }
}


