#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "vector.h"

//binary tree specifically for storing ordered arrays of unsigned int 
// to be used in the PGE algorithm



typedef struct Node Node;
typedef struct BinaryTree BinaryTree;

struct Node{
    void* left;
    void* right;
};


struct BinaryTree{
    Node* top;
    unsigned int height;
};

BinaryTree create_BinaryTree(unsigned int h);
Node* create_node();
void* binarytree_get(BinaryTree tree,Vector vec); // gives a pointer to the stored element, casting to the correct type is up to the user
void binarytree_put_in(BinaryTree tree,Vector key,void* val);
void destroy_BinaryTree(BinaryTree tree,void(*item_destruction_function)(void*));

#endif