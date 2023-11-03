#pragma once

#include <stdbool.h>

typedef struct binary_tree {
    struct binary_tree* left;
    struct binary_tree* right;
    void* value;
} binary_tree_t;

binary_tree_t* consBinaryTree();
bool isBinaryTreeEmpty(binary_tree_t* tree);

void setBinaryTreeValue(binary_tree_t* tree, void* value);
void* getBinaryTreeValue(binary_tree_t* tree);

void setBinaryTreeLeft(binary_tree_t* tree, binary_tree_t* left);
void setBinaryTreeRight(binary_tree_t* tree, binary_tree_t* right);

bool hasBinaryTreeLeft(binary_tree_t* tree);
bool hasBinaryTreeRight(binary_tree_t* right);

binary_tree_t* getBinaryTreeLeft(binary_tree_t* tree);
binary_tree_t* getBinaryTreeRight(binary_tree_t* tree);

int getBinaryTreeHeight(binary_tree_t* tree);
int getBinaryTreeSize(binary_tree_t* tree);

void freeBinaryTree(binary_tree_t** tree,void (*freeValue)());

void printBinaryTree(binary_tree_t* tree);
void saveBinaryTree(binary_tree_t* tree);
