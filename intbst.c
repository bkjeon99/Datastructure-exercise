#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
return	head node pointer
NULL if overflow
*/
TREE *BST_Create(void);

/* Deletes all data in tree and recycles memory
return	NULL head pointer
*/
TREE *BST_Destroy(TREE *pTree);
static void _destroy(NODE *root);

/* Inserts new data into the tree
return	1 success
0 overflow
*/
int BST_Insert(TREE *pTree, int data);

static NODE *_makeNode(int data);

/* Deletes a node with dltKey from the tree
return	1 success
0 not found
*/
int BST_Delete(TREE *pTree, int dltKey);

/* internal function
success is 1 if deleted; 0 if not
return	pointer to root
*/
NODE *_delete(NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
return	address of data of the node containing the key
NULL not found
*/
int *BST_Retrieve(TREE *pTree, int key);

/* internal function
Retrieve node containing the requested key
return	address of the node containing the key
NULL not found
*/
NODE *_retrieve(NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse(TREE *pTree);
static void _traverse(NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE *pTree);
/* internal traversal function
*/
static void _infix_print(NODE *root, int level);


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TREE *tree;
	int data;

	// creates a null tree
	tree = BST_Create();
	printf("1");

	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}

	fprintf(stdout, "Inserting: ");

	srand(time(NULL));
	for (int i = 1; i < 20; i++)
	{
		data = rand() % 100 + 1; // 1 ~ 100 random number

		fprintf(stdout, "%d ", data);

		// insert funtion call
		BST_Insert( tree, data);
	}
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);

	int ret;
	do
	{
		fprintf(stdout, "Input a number to delete: ");
		int num;
		ret = scanf("%d", &num);
		if (ret != 1) break;

		ret = BST_Delete( tree, num);
		if (!ret) fprintf( stdout, "%d not found\n", num);

		// print tree with right-to-left infix traversal
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);

	} while (1);

	BST_Destroy( tree);

	return 0;
}


TREE *BST_Create(void)
{
	TREE * tree = (TREE *)malloc(sizeof(TREE));
	
	tree->root = NULL;
	return tree;
}

/* Deletes all data in tree and recycles memory
return	NULL head pointer
*/
TREE *BST_Destroy(TREE *pTree)
{
	if (pTree)
	{
		_destroy(pTree->root);
	}

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
int BST_Insert(TREE *pTree, int data)
{
	NODE * node = _makeNode(data);
	NODE * temp = pTree->root;
	NODE * ptemp = NULL;

	if (pTree->root == NULL)
	{
		pTree->root = node;
		return 1;
	}

	if (node != NULL)
	{
		while (temp != NULL)
		{
			ptemp = temp;
			if (temp->data <= node->data)
			{
				temp = temp->right;
				if (temp == NULL)
				{
					ptemp-> right = node;
					return 1;
				}
			}
			else
			{
				temp = temp->left;
				if (temp == NULL)
				{
					ptemp->left = node;
					return 1;
				}
			}
		}
	}
	else return 0;
}

static NODE *_makeNode(int data)
{
	NODE * temp = (NODE *)malloc(sizeof(NODE));
	temp->data = data;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

/* Deletes a node with dltKey from the tree
return	1 success
0 not found
*/
int BST_Delete(TREE *pTree, int dltKey)
{
	NODE * toDelete;
	NODE * dump;
	int i;
	toDelete = _retrieve(pTree->root, dltKey);
	
	printf("%d",toDelete->data);

	if (toDelete == NULL)
	{
		return 0;
	}
	else
	{
		dump=_delete(toDelete, dltKey, &i);
		return 1;
	}
}

/* internal function
success is 1 if deleted; 0 if not
return	pointer to root
*/
NODE *_delete(NODE *root, int dltKey, int *success)
{
	NODE * temp = NULL;
	NODE * ptemp = NULL;
	if (root->left == NULL) // 오른쪽만 있거나 양쪽 다 없는 경우
	{
		if (root->right == NULL)
		{
			free(root);
			return NULL;
		}
		else
		{
			temp = root->right;
			root->data = temp->data;
			root->left = temp->left;
			root->right = temp->right;
			free(temp);
			return NULL;
		}
	}
	else if (root->right == NULL)
	{
		temp = root->left;
		root->data = temp->data;
		root->left = temp->left;
		root->right = temp->right;
		free(temp);
		return NULL;
	}
	else
	{
		temp = root->left;
		ptemp = root;
		while (temp->right != NULL)
		{
			ptemp = temp;
			temp = temp->right;
		}
		root->data = temp->data;

		if (temp->left == NULL)
		{
			free(temp);
			return NULL;
		}
		else
		{
			ptemp->left = temp->left;
			free(temp);
			return NULL;
		}
	}
}

/* Retrieve tree for the node containing the requested key
return	address of data of the node containing the key
NULL not found
*/
int *BST_Retrieve(TREE *pTree, int key)
{

}

/* internal function
Retrieve node containing the requested key
return	address of the node containing the key
NULL not found
*/
NODE *_retrieve(NODE *root, int key)
{
	NODE * temp = root;
	NODE * ptemp;
	while (temp != NULL)
	{
		ptemp = temp;
		if (temp->data == key)
		{
			return temp;
		}
		else if (temp->data < key)
		{
			temp = temp->right;
		}
		else
		{
			temp = temp->left;
		}
	}
	return NULL;
}

/* prints tree using inorder traversal
*/
void BST_Traverse(TREE *pTree)
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

/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE *pTree)
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
