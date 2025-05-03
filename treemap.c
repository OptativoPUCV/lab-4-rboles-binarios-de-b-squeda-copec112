#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2)
{
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) 
{
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *parent = NULL;
    TreeNode *current = tree->root;

    while (current != NULL) {
        parent = current;
        if (is_equal(tree, key, current->pair->key)) return; 

        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }

    TreeNode * newNode = createTreeNode(key, value);
    newNode->parent = parent;

    if (parent == NULL)
        tree->root = newNode;
    else if (tree->lower_than(key, parent->pair->key))
        parent->left = newNode;
    else
        parent->right = newNode;

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    TreeNode *replacement = NULL;

    if (node->left != NULL && node->right != NULL) 
    {
        TreeNode *successor = node->right;
        while (successor->left != NULL) 
        {
            successor = successor->left;
        }

        node->pair = successor->pair;
        node = successor; 
    }
    if (node->left != NULL)
        replacement = node->left;
    else if (node->right != NULL)
        replacement = node->right;

    if (replacement != NULL)
        replacement->parent = node->parent;

    if (node->parent == NULL) 
    {
        tree->root = replacement; 
    } 
    else if (node == node->parent->left) 
    {
        node->parent->left = replacement;
    } 
    else node->parent->right = replacment;
    free(node);
}

void eraseTreeMap(TreeMap * tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *current = tree->root;

    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return current->pair;
        }

        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }

    return NULL;
}



Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *current = tree->root;
    TreeNode *candidate = NULL;

    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return current->pair;
        }

        if (tree->lower_than(key, current->pair->key)) {
            candidate = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (candidate != NULL) {
        tree->current = candidate;
        return candidate->pair;
    }

    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
    if (tree->root == NULL) return NULL;
    TreeNode *min = minimum(tree->root);
    tree->current = min;
    return min->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    TreeNode *current = tree->current;

     if (current->right != NULL) 
    {
        TreeNode *min = minimum(current->right);
        tree->current = min;
        return min->pair;
    }
    TreeNode *parent = current->parent;
    while (parent != NULL && current == parent->right) 
    {
        current = parent;
        parent = parent->parent;
    }

    tree->current = parent;
    if (parent != NULL)  return parent->pair;
    return NULL;
}
