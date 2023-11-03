#include "binary_tree.h"
#include "tools.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

binary_tree_t* consBinaryTree() {
    binary_tree_t* tree = malloc(sizeof(binary_tree_t));
    tree->left = NULL;
    tree->right = NULL;
    tree->value = NULL;
    return tree;
}

void setBinaryTreeValue(binary_tree_t* tree, void* value) {
    tree->value = value;
}

void* getBinaryTreeValue(binary_tree_t* tree) {
    return tree->value;
}

bool isBinaryTreeEmpty(binary_tree_t* tree) {
    return tree->value == NULL;
}

void setBinaryTreeLeft(binary_tree_t* tree, binary_tree_t* left) {
    tree->left = left;
}

void setBinaryTreeRight(binary_tree_t* tree, binary_tree_t* right) {
    tree->right = right;
}

bool hasBinaryTreeLeft(binary_tree_t* tree) {
    return tree->left != NULL;
}

bool hasBinaryTreeRight(binary_tree_t* tree) {
    return tree->right != NULL;
}

binary_tree_t* getBinaryTreeLeft(binary_tree_t* tree) {
    return tree->left;
}

binary_tree_t* getBinaryTreeRight(binary_tree_t* tree) {
    return tree->right;
}

int getBinaryTreeHeight(binary_tree_t* tree) {
    if (isBinaryTreeEmpty(tree)) return 0;
    if (hasBinaryTreeLeft(tree) && hasBinaryTreeRight(tree))
        return 1+maxInt(getBinaryTreeHeight(getBinaryTreeLeft(tree)), getBinaryTreeHeight(getBinaryTreeRight(tree)));
    else if (hasBinaryTreeLeft(tree))
        return 1+getBinaryTreeHeight(getBinaryTreeLeft(tree));
    else if (hasBinaryTreeRight(tree))
        return 1+getBinaryTreeHeight(getBinaryTreeRight(tree));
    else 
        return 1;
}

int getBinaryTreeSize(binary_tree_t* tree) {
    if (isBinaryTreeEmpty(tree)) return 0;
    int left = hasBinaryTreeLeft(tree) ? getBinaryTreeSize(getBinaryTreeLeft(tree)) : 0;
    int right = hasBinaryTreeRight(tree) ? getBinaryTreeSize(getBinaryTreeRight(tree)) : 0;
    return 1+left+right;
}

void freeBinaryTree(binary_tree_t** tree, void (*freeValue)()) {
    if(hasBinaryTreeLeft(*tree)) {
        binary_tree_t* tempLeft = getBinaryTreeLeft(*tree);
        freeBinaryTree(&tempLeft, freeValue);
    }
    if(hasBinaryTreeRight(*tree)) {
        binary_tree_t* tempRight = getBinaryTreeRight(*tree);
        freeBinaryTree(&tempRight, freeValue);
    }
    if(!isBinaryTreeEmpty(*tree) && freeValue != NULL) freeValue(&((*tree)->value));
    free(*tree);
    *tree = NULL;
}
