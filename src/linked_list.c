#include <stdbool.h>
#include <stdlib.h>
#include "linked_list.h"

linked_list_t* consLinkedList() {
    linked_list_t* linked_list = malloc(sizeof(linked_list_t));
    linked_list->first = NULL;
    linked_list->last = NULL;
    linked_list->size = 0;
    return linked_list;
}

bool linkedListIsEmpty(linked_list_t* linked_list) {
    return linked_list->size == 0;
}

void linkedListAddFirst(linked_list_t* linked_list, void* value) {
    linked_list_node_t* node = malloc(sizeof(linked_list_node_t));
    node->value = value;
    node->prev_node = NULL;
    if (!linkedListIsEmpty(linked_list)) {
        node->next_node = linked_list->first;
        linked_list->first->prev_node = node;
    } else {
        node->next_node = NULL;
        linked_list->last = node;
    }
    linked_list->first = node;
    linked_list->size++;
}


void linkedListAddLast(linked_list_t* linked_list, void* value) {
    linked_list_node_t* node = malloc(sizeof(linked_list_node_t));
    node->value = value;
    node->next_node = NULL;
    if (!linkedListIsEmpty(linked_list)) {
        node->prev_node = linked_list->last;
        linked_list->last->next_node = node;
    } else {      
        node->prev_node = NULL;
        linked_list->first = node;
    }
    linked_list->last = node;
    linked_list->size++;
}

void* linkedListRemoveFirst(linked_list_t* linked_list) {
    if (linkedListIsEmpty(linked_list)) return NULL;
    linked_list_node_t* node = linked_list->first;
    if (node->next_node != NULL) {
        linked_list->first = node->next_node;
        linked_list->first->prev_node = NULL;
    } else {
        linked_list->last = NULL;
        linked_list->first = NULL;
    }
    linked_list->size--;
    void* value = node->value;
    free(node);
    return value;
}

void* linkedListRemoveLast(linked_list_t* linked_list) {
    if (linkedListIsEmpty(linked_list)) return NULL;
    linked_list_node_t* node = linked_list->last;
    if (node->prev_node != NULL) {
        linked_list->last = node->prev_node;
        linked_list->last->next_node = NULL;
    } else {
        linked_list->last = NULL;
        linked_list->first = NULL;
    }
    linked_list->size--;
    void* value = node->value;
    free(node);
    return value;
}

void* linkedListGetFirst(linked_list_t* linked_list) {
    if (linkedListIsEmpty(linked_list)) return NULL;
    return linked_list->first->value;
}
void* linkedListGetLast(linked_list_t* linked_list) {
    if (linkedListIsEmpty(linked_list)) return NULL;
    return linked_list->last->value;
}

int linkedListSize(linked_list_t* linked_list) {
    return linked_list->size;
}

void linkedListFree(linked_list_t** linked_list, void (*freeValue)()) {
    if (linkedListIsEmpty(*linked_list)) {
        free(*linked_list);
        *linked_list = NULL;
        return;
    }
    linked_list_node_t* last = (*linked_list)->last;
    while (last != NULL) {
        linked_list_node_t* t = last;
        last = last->prev_node;
        if (freeValue != NULL) freeValue(t->value);
        free(t);
    }
    free(*linked_list);
    *linked_list = NULL;
}