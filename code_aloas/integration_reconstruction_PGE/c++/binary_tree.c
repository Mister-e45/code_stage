#include "binary_tree.h"


BinaryTree create_BinaryTree(unsigned int h){
    BinaryTree tree;
    tree.top=(Node*)malloc(sizeof(Node));
    if(tree.top==0){
        printf("could not create binary tree\n");
        tree.height=0;
        return tree;
    }
    tree.top->left=0;
    tree.top->right=0;
    tree.height=h;
    return tree;
}

Node* create_node(){
    Node* pointer=(Node*)malloc(sizeof(Node));
    if(pointer==0){
        printf("could not create node\n");
    }
    else{
        pointer->right=0;
        pointer->left=0;
    }
    return pointer;
}


void* binarytree_get(BinaryTree tree,Vector vec){
    if(vec.size!=tree.height){
        printf("cannot search in binary tree if vector is not of same length as height\n");
        return 0;
    }
    else{
        Node* pointer=tree.top;
        for(unsigned int d=0;d<tree.height;++d){
            if(get_coordinate(vec,d)){
                pointer= (Node*)(pointer->right);
            }
            else{
                pointer= (Node*)(pointer->left);
            }
            if(pointer==0){ //the corresponding element doesn't exist
                return NULL;
            }
        }
        return pointer->left; // should be the same if "pointer->right" was written
    }
}

Node* move_to_left_node(Node* pointer){
    Node* p=(Node*)(pointer->left);
    if(p==0){
        pointer->left = create_node();
        if(pointer->left==0){
            printf("could not create left node when writing in binary tree\n");
        }
        else{
            p=(Node*)(pointer->left);
        }
    }
    else{
        return p;
    }
    return p;
}

Node* move_to_right_node(Node* pointer){
        Node* p=(Node*)(pointer->right);
        if(p==0){
            pointer->right = create_node();
            if(pointer->right==0){
                printf("could not create right node when writing in binary tree\n");
            }
            else{
                p=(Node*)(pointer->right);
            }
        }
        else{
            return p;
        }
        return p;
}

void binarytree_put_in(BinaryTree tree,Vector key,void* val){
    if(key.size!=tree.height){
        printf("cannot write in binary tree if vector is not of same length as height\n");
    }
    else{
        Node* pointer=tree.top;
        for(unsigned int d=0;d<tree.height;++d){
            if(get_coordinate(key,d)){
                pointer= move_to_right_node(pointer);
            }
            else{
                pointer= move_to_left_node(pointer);
            }
        }
        pointer->left = val;
        pointer->right = val;
    }
}

void destroy_BinaryTree(BinaryTree tree,void(*item_destruction_function)(void*)){
    Node* pointer=tree.top;
    while(tree.top->left!=0 || tree.top->right!=0){
        Node** parentNodeLink=0;
        while (pointer->left!=pointer->right){ // while we have not reach a leaf (ie a node without sons)
            if(pointer->left!=0){
                parentNodeLink=&(pointer->left);
                pointer=(Node*)pointer->left;
            }
            else{
                parentNodeLink=&(pointer->right);
                pointer=(Node*)pointer->right;
            }
        } // at the end of this loop the node on which pointer is pointing has both it's left and right pointers pointing to the same address
        if(pointer->left==0){ // if this is true then pointer->right==0 too
            free(pointer); // we don't have to destroy an item
            *parentNodeLink=0;
        }
        else{
            item_destruction_function(pointer->left); // we call the function able to free the structure type of the items
            pointer->left=0;
            pointer->right=0; // the current node becomes an itemless leaf and will be destroyed in the next iteration;
        }
        pointer=tree.top;
    }
    free(pointer);
    pointer=0;
}