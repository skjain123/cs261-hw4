#ifndef LTE
#define LTE(a, b) (a <= b)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "avl.h"


int FindMinPath(struct AVLTree *tree, TYPE *path);
void printBreadthFirstTree(struct AVLTree *tree);

/* helper functions written by Sunil Jain */
int findMinCostPathHelper(struct AVLnode *node, int parent_val, int *path, int index);
void printBreadthFirstTreeHelper(struct AVLnode* current, int height);

/* -----------------------
The main function
  param: argv = pointer to the name (and path) of a file that the program reads for adding elements to the AVL tree
*/
int main(int argc, char** argv) {

	FILE *file;
	int len, i;
	TYPE num; /* value to add to the tree from a file */
	struct timeval stop, start; /* variables for measuring execution time */
	int pathArray[10000];  /* static array with values of nodes along the min-cost path of the AVL tree. The means that the depth of the AVL tree cannot be greater than 100 which is  sufficient for our purposes*/

	struct AVLTree *tree;
	
	tree = newAVLTree(); /*initialize and return an empty tree */
	
	file = fopen(argv[1], "r"); 	/* filename is passed in argv[1] */
	assert(file != 0);

	/* Read input file and add numbers to the AVL tree */
	while((fscanf(file, "%i", &num)) != EOF){
		addAVLTree(tree, num);		
	}
	/* Close the file  */
	fclose(file);

        printf("\nThe AVL tree has %d nodes.\n",tree->cnt);
	
	printf("\nPrinting the AVL tree breadth-first : \n");
	printBreadthFirstTree(tree);

	gettimeofday(&start, NULL);

	/* Find the minimum-cost path in the AVL tree*/
	len = FindMinPath(tree, pathArray);
	
	gettimeofday(&stop, NULL);

	/* Print out all numbers on the minimum-cost path */
	printf("\n\nThe minimum-cost path has %d nodes printed top-down from the root to the leaf: \n", len);
	for(i = 0; i < len; i++)
		printf("%d ", pathArray[i]);
	printf("\n");

	printf("\nYour execution time to find the mincost path is %f microseconds\n", (double) (stop.tv_usec - start.tv_usec));

        /* Free memory allocated to the tree */
	deleteAVLTree(tree); 
	
	return 0;
}

/* --------------------
Finds the minimum-cost path in an AVL tree
   Input arguments: 
        node = pointer to a node containing a subtree
		parent_val = integer containing the current node's parent value to compare
        path = pointer to array that stores values of nodes along the min-cost path, 
		index = integer holding the position to insert a new node's value into the path.
   Output: returns the total cost but is mainly used for recursion 

   pre: assume that
       path is already allocated sufficient memory space 
       tree exists and is not NULL
*/
int findMinCostPathHelper(struct AVLnode *node, int parent_val, int *path, int index) {
    int node_cost;
	int left_cost;
	int right_cost;

	if (node == NULL)
        return 0;
    
    node_cost = abs(node->val - parent_val);

    path[index] = node->val;

    /* use recursion to find the min-cost path for both possible subtrees*/
    left_cost = findMinCostPathHelper(node->left, node->val, path, index + 1);
    right_cost = findMinCostPathHelper(node->right, node->val, path, index + 1);

    /* return the sum of the current node's cost and its potential subtrees */
	if (left_cost < right_cost) {
		return node_cost + left_cost;
	} else if (left_cost == right_cost) {
		return node_cost + left_cost;
	} else {
		return node_cost + right_cost;
	}
}

/* --------------------
Finds the minimum-cost path in an AVL tree
   Input arguments: 
        tree = pointer to the tree,
        path = pointer to array that stores values of nodes along the min-cost path, 
   Output: return the min-cost path length 

   pre: assume that
       path is already allocated sufficient memory space 
       tree exists and is not NULL
*/
int FindMinPath(struct AVLTree *tree, TYPE *path)
{
	/* FIX ME */

	int length = 0;
	
	/* modify path */
	findMinCostPathHelper(tree->root, 0, path, 0);

	/* get the proper length since the path implementation is a simple dynarr */
	while (path[length]) {
		length++;
	}

    return length;

}

void printBreadthFirstTreeHelper(struct AVLnode* current, int height)
{
    if (current == NULL)
        return;
    if (height == 1) { /* if the height is at the */
        printf(" %d ", current->val);
    } else if (height > 1) { /* use recursion to print (height-1) is offsetting so that it can be printed per level */
        printBreadthFirstTreeHelper(current->left, height - 1);
        printBreadthFirstTreeHelper(current->right, height - 1);
    }
}


/* -----------------------
Printing the contents of an AVL tree in breadth-first fashion
  param: pointer to a tree
  pre: assume that tree was initialized well before calling this function
*/
void printBreadthFirstTree(struct AVLTree *tree)
{
   
    /* FIX ME */

	int i;
    int treeHeight = h(tree->root) + 1; /* converting to 0 to n-1 -> 1 to n */
    
	printf("Total tree height = %d \n", treeHeight);

    for (i = 1; i <= treeHeight; i++) {
        printf("Height: %d : ", i); /* display the height we are currently printing at */
        printBreadthFirstTreeHelper(tree->root, i); /* display the numbers at that height */
        printf("\n");
    }

}