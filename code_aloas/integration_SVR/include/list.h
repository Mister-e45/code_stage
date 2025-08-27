#ifndef LIST_H
#define LIST_H

typedef struct ListElement ListElement;
typedef struct List List;

#include <stdlib.h>
#include <stdio.h>

struct ListElement{
    ListElement* nextElement;
    ListElement* previousElement;
    void* value;
};

struct List{
    ListElement* firstElement;
    ListElement* lastElement;
    unsigned int listSize;
};

List create_List();
ListElement create_ListElement(void* value);
void* list_pop_last_element(List* list);
void* list_pop_first_element(List* list);
void destroy_List(List* list,void(*destroy_element_function)(void*));
ListElement* next_ListElement(ListElement* pos);
void* list_get_value(List list,unsigned int i);
void append_list(List* list,void* val);
void list_append_list(List* list1,List* list2);

void for_each(List list,void(function)(void*));

void print_list(List list,void(*element_print_function)(void*));

#endif