#ifndef LTE
#define LTE(a, b) (a <= b)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "avl.h"


/* helper functions made by Sunil Jain */
int calculateHeight(struct AVLnode *node);
struct AVLnode * _single_balance(struct AVLnode * current);

/* Alocate and initialize AVL tree structure. */
struct AVLTree * newAVLTree()
{
	struct AVLTree *tree = (struct AVLTree *)malloc(sizeof(struct AVLTree));
	assert(tree != 0);
	
	initAVLTree(tree);
	return tree;
}

/* Initialize AVL tree structure. */
void initAVLTree(struct AVLTree *tree)
{
	tree->cnt = 0;
	tree->root = 0;

}

void _freeAVL(struct AVLnode *node)
{
	if (node != 0) {
		_freeAVL(node->left);
		_freeAVL(node->right);		
		free(node);
	}
}

/* Deallocate nodes in AVL tree. */
void clearAVLTree(struct AVLTree *tree)
{
	_freeAVL(tree->root);
	tree->root = 0;
	tree->cnt  = 0;
}

/* Deallocate nodes in AVL tree and deallocate the AVL tree structure. */
void deleteAVLTree(struct AVLTree *tree)
{
	clearAVLTree(tree);
	free(tree);
}


/* return height of current node */
int h(struct AVLnode *current)
{
	if (current == 0)
		return -1;
	return current->height;
}

/* set height for current node */
void setHeight (struct AVLnode * current)
{
	int lch = h(current->left);
	int rch = h(current->right);
	if (lch < rch)
		current->height = 1 + rch;
	else
		current->height = 1 + lch;
}

/* return balance factor value */
int bf(struct AVLnode * current)
{
	return h(current->right) - h(current->left);
}

/* Function to calculate the height of a node */
int calculateHeight(struct AVLnode *node) {
	int leftHeight;
	int rightHeight;

    if (node == NULL)
        return -1;
	
	/* set new heights */
    leftHeight = h(node->left);
    rightHeight = h(node->right);

	/* return the smaller height with an offset */
	if (leftHeight > rightHeight) {
		return 1 + leftHeight;
	} else {
		return 1 + rightHeight;
	}
}

/* left-rotate subtree of current node */
struct AVLnode * rotateLeft(struct AVLnode * current)
{
	struct AVLnode *temp = current->right;

	/* swap */
    current->right = temp->left;
    temp->left = current;

	/* set new heights */
    current->height = calculateHeight(current);
    temp->height = calculateHeight(temp);

    return temp;
}

/* right-rotate subtree of current node */
struct AVLnode * rotateRight(struct AVLnode * current)
{
	struct AVLnode *temp = current->left;

	/* swap */
    current->left = temp->right;
    temp->right = current;

	/* set new heights */
    current->height = calculateHeight(current);
    temp->height = calculateHeight(temp);

    return temp;
}

/* balance subtree of current node */
struct AVLnode * _single_balance(struct AVLnode * current)
{
	/* from the slides -- balance a single node*/

	int rotate;

	if (current == NULL)
        return NULL;

    rotate = h(current->right) - h(current->left);

    if (rotate < -1) {
        if (current->left != NULL && h(current->left->right) > h(current->left->left))
            current->left = rotateLeft(current->left);
        return rotateRight(current);
    } else if (rotate > 1) {
        if (current->right != NULL && h(current->right->left) > h(current->right->right))
            current->right = rotateRight(current->right);
        return rotateLeft(current);
    }

    current->height = calculateHeight(current);
    return current;
}

struct AVLnode * _balance(struct AVLnode * current)
{
	/* balance whole tree instead of a single node */
	
    if(current) {

        current->left = _balance(current->left);
        current->right = _balance(current->right);
        return _single_balance(current);

    } else {
        return NULL;
    }
}

/* add newValue to subtree of current node */
struct AVLnode * AVLnodeAdd(struct	AVLnode * current, TYPE newValue)
{
	/* search for leaf */
	if (current == NULL) { /* stop recursion */
		/* create new node and fill it and set it as a leaf */
		struct AVLnode* new = (struct AVLnode*) malloc (sizeof(struct AVLnode));
		new->val = newValue;
		new->left = NULL;
		new->right = NULL;
		new->height = 0;

		return new;
	}

	/* if the val is less than the current put it on the left, if it is not, put it on the right */
	if (LTE(newValue, current->val)) {
		current->left = AVLnodeAdd(current->left, newValue);
	} else {
		current->right = AVLnodeAdd(current->right, newValue);
	}

	/* balance the node properly as it recurses back */
	return _single_balance(current);

}

/* add val to AVL tree */
void addAVLTree(struct AVLTree *tree, TYPE val)
{
	tree->root = AVLnodeAdd(tree->root, val);	
	tree->cnt++;
}

/* determine if val is contained in the AVL tree */
int containsAVLTree(struct AVLTree *tree, TYPE val)
{
	struct AVLnode* cur = tree->root;

	while(cur != 0){
		if (EQ(cur->val, val))	
			return 1;
		else if (LTE(val, cur->val))
			cur = cur->left;
		else
			cur = cur->right;
	}

	return 0; 
}

/* find leftmost value from subtree of current node */
TYPE _leftMost(struct AVLnode *cur)
{
	while(cur->left != 0)
		cur = cur->left;
	return cur->val;
}

/* remove leftmost node from subtree of current node */
struct AVLnode * _removeLeftmost(struct AVLnode * cur)
{
	struct AVLnode * temp;

	if(cur->left != 0){
		cur->left = _removeLeftmost(cur->left);
		return _balance(cur);
	}

	temp = cur->right;
	free(cur);
	return temp;
}

/* remove val from subtree of cur node */
struct AVLnode * _removeNode(struct AVLnode * cur, TYPE val)
{
	struct AVLnode * temp;

	if(EQ(val, cur->val)){
		if(cur->right != 0){
			cur->val = _leftMost(cur->right);
			cur->right = _removeLeftmost(cur->right);
			return _balance(cur);
		} else {
			temp = cur->left;
			free(cur);
			return temp;
		}
	} else if (LTE(val, cur->val))
		cur->left = _removeNode(cur->left, val);
	else cur->right = _removeNode(cur->right, val);

	return _balance(cur);
}

/* remove val from AVL tree */
void removeAVLTree(struct AVLTree * tree, TYPE val)
{
	if(containsAVLTree(tree, val)){
		tree->root = _removeNode(tree->root, val);
		tree->cnt--;
	}
}

/* remove val from AVL tree */
void removeAllAVLTree(struct AVLTree * tree, TYPE val)
{
	if(containsAVLTree(tree, val))
		tree->root = _removeAllNodes(tree, tree->root, val);
}


struct AVLnode * _removeAllNodes(struct AVLTree * tree, struct AVLnode * cur, TYPE val){
   struct AVLnode * temp = NULL;
   while(EQ(val, cur->val)){
      if(cur->right != 0){
         cur->val = _leftMost(cur->right);
         cur->right = _removeLeftmost(cur->right);
         tree->cnt--;
      } else {
         temp = cur->left;
         free(cur);
         tree->cnt--;
         cur = temp;
      }
   }
   if (cur){  
      if (LTE(val, cur->val))
         cur->left = _removeAllNodes(tree,cur->left, val);
      else 
         cur->right = _removeAllNodes(tree,cur->right, val);
   }
   return _balance(cur);
  
}


