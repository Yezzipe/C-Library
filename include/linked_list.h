#pragma once

#include <stdbool.h>

typedef struct linked_list_node {
    void* value;
    struct linked_list_node* next_node;
    struct linked_list_node* prev_node;
} linked_list_node_t;


typedef struct {
	linked_list_node_t* first;
	linked_list_node_t* last;
	int size;
} linked_list_t;

linked_list_t* consLinkedList();

bool linkedListIsEmpty(linked_list_t* linked_list);

void linkedListAddFirst(linked_list_t* linked_list, void* value);
void linkedListAddLast(linked_list_t* linked_list, void* value);

void* linkedListRemoveFirst(linked_list_t* linked_list);
void* linkedListRemoveLast(linked_list_t* linked_list);

void* linkedListGetFirst(linked_list_t* linked_list);
void* linkedListGetLast(linked_list_t* linked_list);

int linkedListSize(linked_list_t* linked_list);

void linkedListFree(linked_list_t** linked_list, void (*freeValue)());