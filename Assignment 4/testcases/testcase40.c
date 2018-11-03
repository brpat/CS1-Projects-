// Sean Szumlanski
// COP 3502, Spring 2018

// =============================
// Kindred Spirits: testcase40.c
// =============================
// A basic test of the kindredSpirits() function.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "KindredSpirits.h"

// A pretty standard create_node() function. Creates a new binary tree node,
// initializes its members, and returns a pointer to the new node.
node *create_node(int data)
{
    node *n = malloc(sizeof(node));
    
    n->data = data;
    n->left = n->right = NULL;
    
    return n;
}

// Free all the nodes in a binary tree.
node *forest_fire(node *root)
{
    if (root == NULL)
        return NULL;
    
    forest_fire(root->left);
    forest_fire(root->right);
    free(root);
    
    return NULL;
}

// Recursive BST insertion.
node *BST_insert(node *root, int data)
{
    if (root == NULL)
        return create_node(data);
    
    if (data < root->data)
    {
        // Left subtree insertion.
        root->left = BST_insert(root->left, data);
    }
    else if (data > root->data)
    {
        // Right subtree insertion.
        root->right = BST_insert(root->right, data);
    }
    else
    {
        // This empty statement is super cheeky. It's my way of pointing out
        // explicitly that I'm not inserting duplicate values into this BST.
        ;
    }
    
    return root;
}

// code used online to create a bst with a given traversal
void constructSubBT(node* root, int post[], int start, int end)
{
    if (start >= end)
        return;
    
    int i;
    for (i = start; i < end; ++i)
        if (post[i] > root->data)
            break;
    
    // left
    if (i > start)
    {
        root->left = create_node(post[i - 1]);
        constructSubBT(root->left, post, start, i - 1);
    }
    
    // right
    if (i >= start) {
        root->right = create_node(post[end - 1]);
        constructSubBT(root->right, post, i, end - 1);
    }
}

node* constructTree(int post[], int size)
{
    node* root = create_node(post[size - 1]);
    
    constructSubBT(root, post, 0, size - 1);
    
    return root;
}
int main(void)
{
    int i;
    node *root1 = NULL;
    node *root2 = NULL;
    
    // Test case #1: 10,000 nodes for each binary tree. They are kindred
    // if you do pre vs post
    for (i = 0; i <= 10000; i++)
        root1 = BST_insert(root1, i);
    for (i = 10000; i >= 0; i--)
        root2 = BST_insert(root2, i);
    printf("%s\n", kindredSpirits(root1, root2) ? "It's a match!" : "Not a match :(");
    forest_fire(root1);
    forest_fire(root2);
    
    // Test case #2: 10,000 nodes for each binary tree. They are kindred
    // if you do post vs pre
    node *root3 = NULL;
    node *root4 = NULL;
    
    for (i = 10000; i >= 1; i--)
        root3 = BST_insert(root3, i);
    for (i = 1; i <= 10000; i++)
        root4 = BST_insert(root4, i);
    printf("%s\n", kindredSpirits(root3, root4) ? "It's a match!" : "Not a match :(");
    
    forest_fire(root3);
    forest_fire(root4);
    
    // Test case #3: using a predefined preorder, create an identical postorder
    // and compare. MUST PASS
    node *root5 = NULL;
    root5 = create_node(40);
    root5->left = create_node(18);
    root5->right = create_node(89);
    root5->right->left = create_node(82);
    root5->right->right = create_node(101);
    root5->right->left->right = create_node(84);
    root5->right->right->left = create_node(100);
    
    // post == 40 18 89 82 84 101 100
    int post[] = {40, 18, 89, 82, 84, 101, 100};
    int size = sizeof(post) / sizeof(post[0]);
    node *root6 = constructTree(post, size);
    printf("%s\n", kindredSpirits(root5, root6) ? "You passed!" : "You suck. :(");
    
    forest_fire(root5);
    forest_fire(root6);
    
    // Test case #4: passes a non NULL tree and a NULL tree
    node *root7 = NULL;
    node *root8 = create_node(5);
    
    printf("%s\n", !kindredSpirits(root7, root8) ? "You passed!" : "You suck. :(");
    
    forest_fire(root7);
    forest_fire(root8);
    
    // Test case #5: passes an empty string as the nodes. MUST PASS
    node *root9 = create_node('\0');
    node *root10 = create_node('\0');
    printf("%s\n", kindredSpirits(root9, root10) ? "You passed!" : "You suck. :(");
    
    forest_fire(root9);
    forest_fire(root10);
    
    // Test case #6: Now I'm just being a bitch and testing your computer now
    node *root11 = NULL;
    node *root12 = NULL;
    
    for (i = 100000; i >= 1; i--)
        root11 = BST_insert(root11, i);
    for (i = 1; i <= 100000; i++)
        root12 = BST_insert(root12, i);
    printf("%s\n", kindredSpirits(root11, root12) ? "It's a match!" : "Not a match :(");
    
    forest_fire(root11);
    forest_fire(root12);
    
    return 0;
}

