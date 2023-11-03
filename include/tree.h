#pragma once

typedef struct tree {
    long long unsigned int* sub;
    void* value;
    int n;
} tree_t;

tree_t* consTree();
void setValue(tree_t* tree, void* value);
void* getValue(tree_t* tree);
int getSize(tree_t* tree);
void addSubTree(tree_t* tree, tree_t* subTree);
long long unsigned int* getSubTrees(tree_t* tree);
void printTree(tree_t* tree);
void freeTree(void** tree,void (*freeValue)(void**));
