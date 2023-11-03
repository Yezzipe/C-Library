#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"
#include "hash.h"
#include "tools.h"

#define TREEIFY_THRESHOLD 8
#define MIN_TREEIFY_CAPACITY 64
#define UNTREEIFY_TRESHOLD 6
#define LOAD_FACTOR 0.75
#define DEFAULT_BUCKETS 16

typedef struct hashmap_list_node {
    void* key;
    void* value;
    struct hashmap_list_node* next;
} hashmap_list_node_t;

typedef struct hashmap_tree_node {
    bool black;
    void* key;
    void* value;
    struct hashmap_tree_node* left;
    struct hashmap_tree_node* right;
    struct hashmap_tree_node* parent;
} hashmap_tree_node_t;

typedef struct hashmap_tree {
    hashmap_tree_node_t* root;
    int size;
} hashmap_tree_t;

void leftRotate(hashmap_tree_t* tree, hashmap_tree_node_t* root)
{
    hashmap_tree_node_t* pivot = root->right;
    root->right = pivot->left;
    
    if (pivot->left != NULL)
        pivot->left->parent = root;
    pivot->parent = root->parent;
    
    if (root->parent == NULL)
        tree->root = pivot;
    else if (root == root->parent->right) 
        root->parent->right = pivot;
    else 
        root->parent->left = pivot;
    pivot->left = root;
    root->parent = pivot;
}

void rightRotate(hashmap_tree_t* tree, hashmap_tree_node_t* root) {
    hashmap_tree_node_t* pivot = root->left;
    root->left = pivot->right;
    if (pivot->right != NULL)
        pivot->right->parent = root;
    pivot->parent = root->parent;
    
    if (root->parent == NULL)
        tree->root = pivot;
    else if (root == root->parent->left)
        root->parent->left = pivot;
    else 
        root->parent->right = pivot;
    pivot->right = root;
    root->parent = pivot;
}

void insertFix(hashmap_tree_t* tree, hashmap_tree_node_t* node) {
    if (node->parent == NULL) {
        node->black = true;
    } else if (!node->parent->black) {
        hashmap_tree_node_t* grandparent = node->parent->parent;
        hashmap_tree_node_t* uncle;
        if (node->parent == grandparent->left) 
            uncle = grandparent->right;
        else 
            uncle = grandparent->left;
        if (uncle == NULL || uncle->black) {
            if (node == node->parent->left && uncle == grandparent->left) {
                node = node->parent;
                rightRotate(tree, node);
                insertFix(tree,node);
            } else if (node == node->parent->right && uncle == grandparent->right) {
                node = node->parent;
                leftRotate(tree, node);
                insertFix(tree,node);
            } else if (node == node->parent->right && uncle == grandparent->left) {
                leftRotate(tree, grandparent);
                grandparent->black = false;
                node->parent->black = true;
                insertFix(tree,node->parent);
            } else {
                rightRotate(tree, grandparent);
                grandparent->black = false;
                node->parent->black = true;
                insertFix(tree,node->parent);
            }
        } else {
            grandparent->black = false;
            uncle->black = true;
            node->parent->black = true;
            insertFix(tree, grandparent);
        }
    }
    
}

void insert(hashmap_tree_t* tree, void* key, void* value, unsigned long long int key_size) {
    hashmap_tree_node_t* y = NULL;
    hashmap_tree_node_t* x = tree->root;
    while (x != NULL) {
        y = x;
        int c = memcmp_fast(x->key, key, key_size);
        if (c == 0) {
            x->value = value;
            return;
        } else if (c > 0)
            x = x->left;
        else
            x = x->right;
    }
    hashmap_tree_node_t* node = malloc(sizeof(hashmap_tree_node_t));
    node->key = key;
    node->value = value;
    node->parent = y;
    if (y == NULL)
        tree->root = node;
    else if (memcmp_fast(y->key, key, key_size) > 0)
        y->left = node;
    else 
        y->right = node;
    node->left = NULL;
    node->right = NULL;
    node->black = false;
    tree->size++;
    insertFix(tree, node);
}

void transplant(hashmap_tree_t* tree, hashmap_tree_node_t* a, hashmap_tree_node_t* b) {
    if (a->parent == NULL) {
        tree->root = b;
    } else if (a == a->parent->left) {
        a->parent->left = b;
    } else {
        a->parent->right = b;
    }
    if (b != NULL)
        b->parent = a->parent;
}

hashmap_tree_node_t* minimum(hashmap_tree_node_t* a) {
    while (a->left != NULL) {
        a = a->left;
    }
    return a;
}

hashmap_tree_node_t* search(hashmap_tree_t* tree,void* key, long long unsigned int key_size) {
    hashmap_tree_node_t* a = tree->root;
    while (a != NULL) {
        int c = memcmp_fast(a->key, key, key_size);
        if (c == 0) break;
        a = c > 0 ? a->left : a->right;
    }
    return a;
} 

hashmap_tree_node_t* searchValue(hashmap_tree_node_t* node, void* value, long long unsigned int value_size) {
    if (node == NULL) return NULL;
    if (memcmp_fast(node->value, value, value_size) == 0) return node;
    hashmap_tree_node_t* left = searchValue(node->left, value, value_size);
    if (left != NULL) return left;
    hashmap_tree_node_t* right = searchValue(node->right, value, value_size);
    if (right != NULL) return right;
    return NULL;
}

void deleteFix(hashmap_tree_t* tree, hashmap_tree_node_t* a) {
    if (a->parent == NULL || !a->black) {
        a->black = true;
        return;
    }
    hashmap_tree_node_t* s;
    if (a->parent->left == a)
        s = a->parent->right;
    else 
        s = a->parent->left;
    if (s->black) {
        if ((s->left == NULL || s->left->black) && (s->right == NULL || s->right->black)) {
            s->black = false;
            deleteFix(tree, a->parent);
            return;
        }
        if (a->parent->left == a) {
            if ((s->right == NULL || s->right->black)) {
                s->left->black = true;
                s->black = false;
                rightRotate(tree, s);
            }
            s->black = a->parent->black;
            a->parent->black = true;
            s->right->black = true;
            leftRotate(tree, a->parent);
            deleteFix(tree, tree->root);
        } else { 
            if ((s->left == NULL || s->left->black)) {
                s->right->black = true;
                s->black = false;
                leftRotate(tree, s);
            }
            s->black = a->parent->black;
            a->parent->black = true;
            s->left->black = true;
            rightRotate(tree, a->parent);
            deleteFix(tree, tree->root);
        }
    } else {
        s->black = true;
        a->parent->black = false;
        if (a->parent->left == a) {
            leftRotate(tree, a->parent);
        } else { 
            rightRotate(tree, a->parent);
        }
        deleteFix(tree, a);
    }
}

void delete(hashmap_tree_t* tree, hashmap_tree_node_t* a) {
    hashmap_tree_node_t* b = a;
    hashmap_tree_node_t* c;
    bool og_color = b->black;
    if (a->left == NULL) {
        c = a->right;
        transplant(tree,a,a->right);
    } else if (a->right == NULL) {
        c = a->left;
        transplant(tree,a,a->left);
    } else {
        b = minimum(a->right);
        og_color = b->black;
        c = b->right;
        
        if (b->parent == a) {
            if (c != NULL) c->parent = b;
            else c = b;
        } else {
            transplant(tree, b, b->right);
            b->right = a->right;
            b->right->parent = b;
        }
        transplant(tree, a, b);
        b->left = a->left;
        b->left->parent = b;
        b->black = a->black;
    }   
    if (og_color) {
        if (c != NULL) deleteFix(tree, c);
    }
    tree->size--;
    free(a);
}

void treeToArrayUtil(hashmap_tree_node_t* node, hashmap_list_node_t** head) {
    hashmap_list_node_t* new_node = malloc(sizeof(hashmap_list_node_t));
    new_node->key = node->key;
    new_node->value = node->value;
    new_node->next = *head;
    *head = new_node;
    if (node->left != NULL) treeToArrayUtil(node->left, head);
    if (node->right != NULL) treeToArrayUtil(node->right, head);
    free(node);
}

hashmap_list_node_t* treeToArray(hashmap_tree_t* tree) {
    hashmap_list_node_t* head = NULL;
    treeToArrayUtil(tree->root, &head);
    return head;
}

hashmap_t* consHashMap(unsigned long long int key_size, unsigned long long int value_size, int init_size) {
    hashmap_t* hashmap = malloc(sizeof(hashmap_t));
    int b = 1;
    if (init_size != 0) {
        while (init_size >= b*LOAD_FACTOR) {
            b = b<<1;
        }
    }
    hashmap->init_size = b; 
    hashmap->bucket_nb = b == 1 ? DEFAULT_BUCKETS : b;
    hashmap->size = 0;
    hashmap->key_size = key_size;
    hashmap->value_size = value_size;
    hashmap->buckets = malloc(sizeof(hashmap_bucket_t)*hashmap->bucket_nb);
    for(int i = 0; i < hashmap->bucket_nb; ++i) {
        hashmap_bucket_t* bucket = &(hashmap->buckets[i]);
        bucket->tree = false;
        bucket->list = NULL;
        bucket->size = 0;
    }
    return hashmap;
}

void hashMapClear(hashmap_t* hashmap) {
    for (int i = 0; i < hashmap->bucket_nb; ++i) {
        hashmap_bucket_t* bucket = &(hashmap->buckets[i]);
        if (bucket->tree) {
            hashmap_tree_t* tree = bucket->list;
            hashmap_list_node_t* node = treeToArray(tree);
            while (node != NULL) {
                hashmap_list_node_t* old = node;
                node = node->next;
                free(old);
            }  
            free(tree);
        } else {
            hashmap_list_node_t* node = bucket->list;
            while (node != NULL) {
                hashmap_list_node_t* old = node;
                node = node->next;
                free(old);
            }
        }
    }
    free(hashmap->buckets);
    hashmap->bucket_nb = hashmap->init_size == 1 ? DEFAULT_BUCKETS : hashmap->init_size;
    hashmap->buckets = malloc(sizeof(hashmap_bucket_t)*hashmap->bucket_nb);
    for(int i = 0; i < hashmap->bucket_nb; ++i) {
        hashmap_bucket_t* bucket = &(hashmap->buckets[i]);
        bucket->tree = false;
        bucket->list = NULL;
        bucket->size = 0;
    }
}

bool hashMapContainsKey(hashmap_t* hashmap, void* key) {
    int hash = hashCode(key, hashmap->key_size);
    int index = hash & (hashmap->bucket_nb-1);
    hashmap_bucket_t* bucket = &(hashmap->buckets[index]);
    if (bucket->size == 0) return false;
    if (bucket->tree) {
        hashmap_tree_node_t* node = search(bucket->list, key, hashmap->key_size);
        return node != NULL;
    } else {
        hashmap_list_node_t* node = bucket->list;
        while (node != NULL && memcmp_fast(node->key, key, hashmap->key_size) != 0) {
            node = node->next;
        }
        return node != NULL;
    }
}

bool hashMapContainsValue(hashmap_t* hashmap, void* value) {
    for (int i = 0; i < hashmap->bucket_nb; ++i) {
        hashmap_bucket_t* bucket = &(hashmap->buckets[i]);
        if (bucket->size == 0) continue;
        if (bucket->tree) {
            hashmap_tree_t* tree = bucket->list;
            hashmap_tree_node_t* node = searchValue(tree->root, value, hashmap->value_size);
            if (node != NULL) return true;
        } else {
            hashmap_list_node_t* node = bucket->list;
            while (node != NULL) {
                if (memcmp_fast(node->value, value, hashmap->value_size) == 0) return true;
                node = node->next;
            }
        }
    }
    return false;
}

void* hashMapGet(hashmap_t* hashmap, void* key) {
    int hash = hashCode(key, hashmap->key_size);
    int index = hash & (hashmap->bucket_nb-1);
    hashmap_bucket_t* bucket = &(hashmap->buckets[index]);
    if (bucket->size == 0) return NULL;
    if (bucket->tree) {
        
        hashmap_tree_node_t* node = search(bucket->list, key, hashmap->key_size);
        if (node == NULL) return NULL;
        return node->value;
    } else {
        
        hashmap_list_node_t* node = bucket->list;
        while (node != NULL && memcmp_fast(node->key, key, hashmap->key_size) != 0) {
            node = node->next;
        }
        if (node == NULL) return NULL;
        return node->value;
    }
}

void bucketPutNoFilter(hashmap_t* hashmap, hashmap_bucket_t* bucket, void* key, void* value) {
    hashmap_list_node_t* node = bucket->list;
    if (node == NULL) {
        hashmap_list_node_t* new_node = malloc(sizeof(hashmap_list_node_t));
        new_node->key = key;
        new_node->value = value;
        new_node->next = NULL;
        bucket->list = new_node;
        bucket->size++;
    } else {
        while (node->next != NULL) {
            node = node->next;
        };
        hashmap_list_node_t* new_node = malloc(sizeof(hashmap_list_node_t));
        new_node->key = key;
        new_node->value = value;
        new_node->next = NULL;
        node->next = new_node;
        bucket->size++;
    }
}

void bucketPut(hashmap_t* hashmap, hashmap_bucket_t* bucket, void* key, void* value) {
    if (bucket->tree) {
        hashmap_tree_t* tree = bucket->list;
        hashmap_tree_node_t* node = search(tree, key, hashmap->key_size);
        if (node != NULL) {
            node->value = value;
        } else {
            insert(tree, key, value, hashmap->key_size);
            bucket->size++;
            hashmap->size++;
        }
    } else {
        hashmap_list_node_t* node = bucket->list;
        if (node == NULL) {
            hashmap_list_node_t* new_node = malloc(sizeof(hashmap_list_node_t));
            new_node->key = key;
            new_node->value = value;
            new_node->next = NULL;
            bucket->list = new_node;
            bucket->size++;
            hashmap->size++;
        } else {
            bool identical = memcmp_fast(node->key, key, hashmap->key_size) == 0;
            while (node->next != NULL && !identical) {
                node = node->next;
                identical = memcmp_fast(node->key, key, hashmap->key_size) == 0;
            };
            if (identical) {
                node->value = value;
            } else {
                hashmap_list_node_t* new_node = malloc(sizeof(hashmap_list_node_t));
                new_node->key = key;
                new_node->value = value;
                new_node->next = NULL;
                node->next = new_node;
                bucket->size++;
                hashmap->size++;
            }
        }
    }
}

void hashMapFix(hashmap_t* hashmap, hashmap_bucket_t* bucket) {
    bool rebuild = (hashmap->size >= hashmap->bucket_nb*LOAD_FACTOR);
    if (rebuild) {
        int hash; 
        int index;
        int old_bucket_nb = hashmap->bucket_nb;
        hashmap_bucket_t* old_buckets = hashmap->buckets;
        hashmap->bucket_nb = old_bucket_nb << 1;
        hashmap_bucket_t* new_buckets = malloc(sizeof(hashmap_bucket_t)*hashmap->bucket_nb);
        hashmap->buckets = new_buckets;
        for(int i = 0; i < hashmap->bucket_nb; ++i) {
            hashmap_bucket_t* bucket = &(hashmap->buckets[i]);
            bucket->tree = false;
            bucket->list = NULL;
            bucket->size = 0;
        }    
        for (int i = 0; i < old_bucket_nb; ++i) {
            hashmap_bucket_t* bucket = &(old_buckets[i]);               
            if (bucket->size == 0) continue;
            if (!bucket->tree) {
                hashmap_list_node_t* node = bucket->list;
                while (node != NULL) {
                    hash = hashCode(node->key, hashmap->key_size);
                    index = hash & (hashmap->bucket_nb-1);
                    bucketPutNoFilter(hashmap, &(hashmap->buckets[index]), node->key, node->value);
                    hashmap_list_node_t* old = node;
                    node = node->next;
                    free(old);
                }                
            } else {
                hashmap_tree_t* tree = bucket->list;
                hashmap_list_node_t* node = treeToArray(tree);
                while (node != NULL) {
                    hash = hashCode(node->key, hashmap->key_size);
                    index = hash & (hashmap->bucket_nb-1);
                    bucketPutNoFilter(hashmap, &(hashmap->buckets[index]), node->key, node->value);
                    hashmap_list_node_t* old = node;
                    node = node->next;
                    free(old);
                }  
                free(tree);
            }
        }
        free(old_buckets);
    } else if (hashmap->bucket_nb >= MIN_TREEIFY_CAPACITY && bucket->size >= TREEIFY_THRESHOLD) {
        hashmap_tree_t* tree = malloc(sizeof(hashmap_tree_t));
        tree->root = NULL;
        tree->size = 0;
        hashmap_list_node_t* node = bucket->list;
        while (node != NULL) {
            insert(tree, node->key, node->value, hashmap->key_size);   
            hashmap_list_node_t* old = node;
            node = node->next;
            free(old);
        }
        bucket->list = tree;
        bucket->tree = true;
    }
}

void hashMapPut(hashmap_t* hashmap, void* key, void* value) {  
    int hash = hashCode(key, hashmap->key_size);
    int index = hash & (hashmap->bucket_nb-1);
    hashmap_bucket_t* bucket = &(hashmap->buckets[index]);
    bucketPut(hashmap, bucket, key, value);
    hashMapFix(hashmap, bucket);
}

void hashMapRemove(hashmap_t* hashmap, void* key) {
    int hash = hashCode(key, hashmap->key_size);
    int index = hash & (hashmap->bucket_nb-1);
    hashmap_bucket_t* bucket = &(hashmap->buckets[index]);
    if (bucket->size == 0) return;
    if (bucket->tree) {
        hashmap_tree_t* tree = bucket->list;
        hashmap_tree_node_t* node = search(tree, key, hashmap->key_size);
        if (node != NULL) {
            delete(tree, node);
            hashmap->size--;
        }
    } else {
        hashmap_list_node_t* prev_node = NULL;
        hashmap_list_node_t* node = bucket->list;
        while (node != NULL && memcmp_fast(node->key, key, hashmap->key_size) != 0) {
            prev_node = node;
            node = node->next;
        }
        if (node != NULL) {
            if (prev_node == NULL)
                bucket->list = node->next;
            else {
                prev_node->next = node->next;
            }
            free(node);
            hashmap->size--;
        }
    }
}

int hashMapSize(hashmap_t* hashmap) {
    return hashmap->size;
}

bool hashMapIsEmpty(hashmap_t* hashmap) {
   return hashMapSize(hashmap) == 0;
}

int hashMapBucketSize(hashmap_t* hashmap) {
    return hashmap->bucket_nb;
}

void hashMapFree(hashmap_t** hashmap) {
    for (int i = 0; i < (*hashmap)->bucket_nb; ++i) {
        hashmap_bucket_t* bucket = &((*hashmap)->buckets[i]);
        if (bucket->size == 0) continue;
        if (!bucket->tree) {
            hashmap_list_node_t* node = bucket->list;
            while (node != NULL) {
                hashmap_list_node_t* old = node;
                node = node->next;
                free(old);
            }
        } else {
            hashmap_tree_t* tree = bucket->list;
            hashmap_list_node_t* node = treeToArray(tree);
            while (node != NULL) {
                hashmap_list_node_t* old = node;
                node = node->next;
                free(old);
            }  
            free(tree);
        }
    }
    free((*hashmap)->buckets);
    free(*hashmap);
    *hashmap = NULL;
}