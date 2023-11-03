#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

tree_t* consTree() {
    tree_t* tree = malloc(sizeof(tree_t));
    tree->n = 0;
    tree->value = NULL;
    return tree;
}

void setValue(tree_t* tree, void* value) {
    assert(tree);
    tree->value = value;
}

void* getValue(tree_t* tree) {
    assert(tree);
    return tree->value;
}

int getSize(tree_t* tree) {
    assert(tree);
    return tree->n;
}

void addSubTree(tree_t* tree, tree_t* subTree) {
    assert(tree);
    assert(subTree);
    tree->n++;
    long long unsigned int* newArray = calloc(tree->n, sizeof(void*));
    for (int i = 0; i < tree->n-1; ++i) {
        newArray[i] = tree->sub[i];
    }
    newArray[tree->n-1] = (long long unsigned int) subTree;
    if (tree->n-1) free(tree->sub);
    tree->sub = newArray;
}

long long unsigned int* getSubTrees(tree_t* tree) {
    assert(tree);
    return tree->sub;
}

void printTree(tree_t* tree) {
    printf("print : %d\n", *(int*)tree->value);
}

void freeTree(void** pointer, void (*freeValue)(void**)) {
    tree_t** tree = (tree_t**) pointer;
    long long unsigned int* list = getSubTrees(*tree);
	for (int i = 0; i < getSize(*tree); ++i) {
		tree_t* tempTree = (tree_t*) list[i];
		freeTree(&tempTree, freeValue);
	}
    if ((*tree)->n) free((*tree)->sub);
    freeValue(&((*tree)->value));
    free(*tree);
    *tree = NULL;
}