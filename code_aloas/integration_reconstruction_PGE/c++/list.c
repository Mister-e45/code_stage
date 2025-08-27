#include "list.h"

List create_List(){
    List list;
    list.firstElement=0;
    list.lastElement=0;
    list.listSize=0;
    return list;
}

ListElement create_ListElement(void* value){
    ListElement pos;
    pos.nextElement=0;
    pos.previousElement=0;
    pos.value=value;
    return pos;
}

void* list_pop_first_element(List* list){
    if(list->listSize!=0){
        void* v=list->firstElement->value;
        ListElement* el=list->firstElement;
        if(list->listSize>1){
            el->nextElement->previousElement=el->nextElement;
        }
        list->firstElement=el->nextElement;
        free(el);
        list->listSize-=1;
        return v;
    }
    else{
        return NULL;
    }
}

void* list_pop_last_element(List* list){ //gives the last element's value and removes it from list

    if(list->listSize!=0){
        void* v=list->lastElement->value;
        ListElement* el=list->lastElement;
        if(list->listSize!=1){
            el->previousElement->nextElement=0;
            list->lastElement=el->previousElement;
        }
        free(el);
        list->listSize-=1;
        return v;
    }
    return NULL;
}

void* list_get_value(List list,unsigned int i){
    ListElement* e= list.firstElement;
    for(unsigned int k=0;k<i;++k){
        e=next_ListElement(e);
    }
    return e->value;
}

void destroy_List(List* list,void(*destroy_element_function)(void*)){
    unsigned int size=list->listSize;
    for(unsigned int i=0;i<size;++i){
        void* val=list_pop_last_element(list);
        destroy_element_function(val);
    }
    list->firstElement=0;
    list->lastElement=0;
}

ListElement* next_ListElement(ListElement* pos){
    return pos->nextElement;
}

void append_list(List* list,void* val){
    ListElement* item =(ListElement*)malloc(sizeof(ListElement));
    if(list->listSize==0){
        list->firstElement=item;
        list->lastElement=item;
        item->nextElement=0;
        item->previousElement=item;
        item->value=val;
        list->listSize+=1;
    }
    else{
        item->previousElement=list->lastElement;
        list->lastElement->nextElement=item;
        item->nextElement=0;
        list->lastElement=item;
        item->value=val;
        list->listSize+=1;
    }
}

void list_append_list(List* list1,List* list2){ // list1 gets appended with list2
    if(list1->listSize!=0){
        list1->lastElement->nextElement=list2->firstElement;
        list2->firstElement->previousElement=list1->lastElement;
        list1->listSize+=list2->listSize;
    }
    else{
        list1->firstElement=list2->firstElement;
        list1->lastElement=list2->lastElement;
        list1->listSize=list2->listSize;
    }
    
}

void print_list(List list,void(*element_print_function)(void*)){
    printf("[list of length %i]\n",list.listSize);
    ListElement* el=list.firstElement;
    for(unsigned int k=0;k<list.listSize;++k){
        element_print_function(el->value);
        el=next_ListElement(el);
    }
    printf("[list end]\n\n");
}

void for_each(List list,void(function)(void*)){
    ListElement* element=list.firstElement;
    for(unsigned int k=0;k<list.listSize;++k){
        function(element->value);
        element=next_ListElement(element);
    }
}



