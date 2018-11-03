// Brijesh Patel br168467	
#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"


 int count_nodes(node *root)
 {
 	int total = 0;

 	if(root == NULL)
 		return 0;

	total = total + count_nodes(root->left);
	total++;
	total = total + count_nodes(root->right);

	return total;
 }

int isReflection(node *a, node *b)
{
	if(a == NULL && b == NULL)
		return 1;

	if(a == NULL && b != NULL || b == NULL && a != NULL)
		return 0;

	if(a->left == NULL && b->right != NULL || a->left != NULL && b->right == NULL)
		return 0;

	if(a->data != b->data)
		return 0;
	// branch left on one tree, and right on the other, while values are equal.
	return (isReflection(a->left, b->right) && isReflection(a->right, b->left));

}

node *makeReflection(node *root)
{
	node *reflectionTree = NULL; 

	if(root == NULL)
		return NULL;

	// Create root of reflected tree(Beginning). Otherwise create left and right nodes
	reflectionTree = malloc(sizeof(node));
	reflectionTree->data = root->data;
	reflectionTree->left = reflectionTree->right = NULL;

	// Go to  left subtree if any, reflected tree should branch right
	reflectionTree->left = makeReflection(root->right);
	// Go to right subtree reflection should branch left
	reflectionTree->right = makeReflection(root->left);
	
	return reflectionTree; 

}

void preOrderTraversal(node *root, int *a, int *i)
{
	int temp = *i;

	if(root == NULL)
		return;
	// Value of index(i) is passed by reference to avoid resetting value when returning from recursive calls
   	 a[temp] = root->data;
   	 *i = *i + 1;

	 preOrderTraversal(root->left, a, i);
	 preOrderTraversal(root->right, a, i);
	
}


void postOrderTraversal(node *root, int *b, int *i)
{
	int temp;

	if(root == NULL)
		return;

	 postOrderTraversal(root->left, b, i);
	 postOrderTraversal(root->right, b, i);

	// Value of index(i) is passed by reference to avoid resetting value when returning from recursive calls
	temp = *i;
	b[temp] = root->data;
	*i = *i + 1;
}



int kindredSpirits(node *a, node *b)
{
	int  numNodes, i = 0, count1, count2, *ar = NULL,  *br = NULL, counter = 0, kindred = 0;
	
	if(a == NULL && b == NULL)
		return 1;

	if(a == NULL && b != NULL || b == NULL && a != NULL)
		return 0;

	// If both trees are the same tree
	if(a == b)
		return 0;

    count1 = count_nodes(a);
    count2 = count_nodes(b);

    // If the trees have diffrent number of nodes(counts),they are not kindred spirits, just return to main
   	if(count1 != count2)
   		return 0;

   	// Create preorder and post order aux arrays
    ar = malloc(sizeof(node) * count1);
    br = malloc(sizeof(node) * count2);

    preOrderTraversal(a, ar, &counter);
    counter = 0;
    postOrderTraversal(b, br, &counter);

     while(ar[i] == br[i]) 
   	 {
   	 	// At this point the counts will be the same, resulting in an O(n) operation.
   	 	// If both trees return to root, and the preorder matches postorder don't reset boolean, just return
    	if(i >= count1 - 1 && i >= count2 - 1)
    		{
    			free(ar);
				free(br);
    			return kindred = 1;

    		}

    	kindred = 0;
    	i++;
   	 }
   	      
    i = 0;
    counter = 0;

    // Check Reversed Trees
    postOrderTraversal(a, ar, &counter);
    counter = 0;
    preOrderTraversal(b, br, &counter);

     while(ar[i] == br[i]) 
   	 {
   	 	// At this point the counts  will be the same, resulting in an O(n) operation.
   	 	// If both trees return to root, and  the preorder matches postorder don't reset boolean, just return
    	if(i >= count1 - 1 && i >= count2 - 1)
    		{
    			free(ar);
				free(br);
    			return kindred = 1;
    		}

    	kindred = 0;		
    	i++;
   	 }

   	
	free(ar);
	free(br);	

	return kindred;
}


double difficultyRating(void)
{
	return 2.0;
}

double hoursSpent(void)
{
	return 7.0;
}


