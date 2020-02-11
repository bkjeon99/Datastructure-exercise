#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time

#define MAX_ELEM 20
#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
return	head node pointer
NULL if overflow
*/
AVL_TREE *AVL_Create(void);

/* Deletes all data in tree and recycles memory
return	NULL head pointer
*/
AVL_TREE *AVL_Destroy(AVL_TREE *pTree);
static void _destroy(NODE *root);

/* Inserts new data into the tree
return	1 success
0 overflow
*/
int AVL_Insert(AVL_TREE *pTree, int data);

/* internal function
This function uses recursion to insert the new data into a leaf node
return	pointer to new root
*/
static NODE *_insert(NODE *root, NODE *newPtr);

static NODE *_makeNode(int data);

/* Retrieve tree for the node containing the requested key
return	address of data of the node containing the key
NULL not found
*/
int *AVL_Retrieve(AVL_TREE *pTree, int key);

/* internal function
Retrieve node containing the requested key
return	address of the node containing the key
NULL not found
*/
static NODE *_retrieve(NODE *root, int key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse(AVL_TREE *pTree);

static void _traverse(NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print(NODE *root, int level);

/* internal function
return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE *root);

/* internal function
Exchanges pointers to rotate the tree to the right
updates heights of the nodes
return	new root
*/
static NODE *rotateRight(NODE *root);

/* internal function
Exchanges pointers to rotate the tree to the left
updates heights of the nodes
return	new root
*/
static NODE *rotateLeft(NODE *root);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	AVL_TREE *tree;
	int data;

	// creates a null tree
	tree = AVL_Create();

	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}

	fprintf(stdout, "Inserting: ");

	srand(time(NULL));
	for (int i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // random number
											//data = i+1; // sequential number

		fprintf(stdout, "%d ", data);

		// insert function call
		AVL_Insert( tree, data);
	}
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);

	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);

	AVL_Destroy( tree);

	return 0;
}

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
return	head node pointer
NULL if overflow
*/
AVL_TREE *AVL_Create(void)
{
	AVL_TREE * avltree = (AVL_TREE *)malloc(sizeof(AVL_TREE));
	avltree->root = NULL;
	avltree->count = 0;
	return avltree;
}

/* Deletes all data in tree and recycles memory
return	NULL head pointer
*/
AVL_TREE *AVL_Destroy(AVL_TREE *pTree)
{
	
	_destroy(pTree->root);

	free(pTree);

	return NULL;
}
static void _destroy(NODE *root)
{
	if (root->left != NULL)
	{
		_destroy(root->left);
	}
	if (root->right != NULL)
	{
		_destroy(root->right);
	}
	free(root);
	return;
}
/* Inserts new data into the tree
return	1 success
0 overflow
*/
int AVL_Insert(AVL_TREE *pTree, int data)
{
	NODE * temp = _makeNode(data);

	pTree->root = _insert(pTree->root, temp);
	
	pTree->count++;
}

/* internal function
This function uses recursion to insert the new data into a leaf node
return	pointer to new root
*/
static NODE *_insert(NODE *root, NODE *newPtr)
{
	int left_height, right_height, a, b;

	if (root == NULL)
	{
		root = newPtr;
	}

	else
	{
		if (newPtr->data < root->data)
		{
			root->left = _insert(root->left, newPtr);

			root->height = getHeight(root);
			//check balance
			if (root->left == NULL) left_height = 0;
			else left_height = root->left->height;
			if (root->right == NULL) right_height = 0;
			else right_height = root->right->height;

			if (left_height - right_height > 1)
			{
				a = getHeight(root->left->left);
				b = getHeight(root->left->right);
				if (a > b)
				{
					root = rotateRight(root);
				}
				else
				{
					root->left = rotateLeft(root->left);
					root = rotateRight(root);
				}
			}
		}
		else
		{
			root->right = _insert(root->right, newPtr);

			root->height = getHeight(root);
			//check balance
			if (root->left == NULL) left_height = 0;
			else left_height = root->left->height;
			if (root->right == NULL) right_height = 0;
			else right_height = root->right->height;

			if (right_height - left_height > 1)
			{
				a = getHeight(root->right->left);
				b = getHeight(root->right->right);
				if (a < b)
				{
					root = rotateLeft(root);
				}
				else
				{
					root->right = rotateRight(root->right);
					root = rotateLeft(root);
				}
			}
		}
	}
	return root;
}

static NODE *_makeNode(int data)
{
	NODE * node = (NODE *)malloc(sizeof(NODE));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
}

/* Retrieve tree for the node containing the requested key
return	address of data of the node containing the key
NULL not found
*/
int *AVL_Retrieve(AVL_TREE *pTree, int key)
{

}

/* internal function
Retrieve node containing the requested key
return	address of the node containing the key
NULL not found
*/
static NODE *_retrieve(NODE *root, int key)
{

}

/* Prints tree using inorder traversal
*/
void AVL_Traverse(AVL_TREE *pTree)
{
	_traverse(pTree->root);
}

static void _traverse(NODE *root)
{
	if (root != NULL)
	{
		_traverse(root->left);
		printf(" %d", root->data);
		_traverse(root->right);
	}
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE *pTree)
{
	_infix_print(pTree->root, 0);
}
/* internal traversal function
*/
static void _infix_print(NODE *root, int level)
{
	int i;
	if (root != NULL)
	{
		_infix_print(root->right, level + 1);
		for (i = 0; i < level; i++)
		{
			printf("	");
		}
		printf("%d \n", root->data);
		_infix_print(root->left, level + 1);
	}
}

/* internal function
return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE *root)
{
	int L_height, R_height;
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		if (root->left == NULL) L_height = 0;
		else L_height = root->left->height;

		if (root->right == NULL) R_height = 0;
		else R_height = root->right->height;

		if (L_height > R_height) return L_height + 1;
		else return R_height + 1;
	}
}

/* internal function
Exchanges pointers to rotate the tree to the right
updates heights of the nodes
return	new root
*/
static NODE *rotateRight(NODE *root)
{
	NODE * temp;

	temp = root->left;
	root->left = temp->right;
	temp->right = root;

	root->height = getHeight(root);
	temp->height = getHeight(temp);

	return temp;
}

/* internal function
Exchanges pointers to rotate the tree to the left
updates heights of the nodes
return	new root
*/
static NODE *rotateLeft(NODE *root)
{
	NODE * temp;

	temp = root->right;
	root->right = temp->left;
	temp->left = root;

	root->height = getHeight(root);
	temp->height = getHeight(temp);

	return temp;
}