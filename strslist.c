#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp

// User structure type definition
typedef struct
{
	char	*token;
	int		freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*link;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
return	head node pointer
NULL if overflow
*/
LIST *createList(void)
{
	LIST * list = (LIST *)malloc(sizeof(LIST));
	if (list == NULL)
	{
		printf("overflow");
		return NULL;
	}
	else
	{
		list->count = 0;
		list->pos = NULL;
		list->head = NULL;
		list->rear = NULL;
	}
	return list;
};

/* Deletes all data in list and recycles memory
return	NULL head pointer
*/
LIST *destroyList(LIST *pList)
{
	NODE * temp = pList->head;
	while (temp != NULL)
	{
		NODE * ptemp = temp;
		temp = temp->link;
		free(ptemp->dataPtr->token);
		free(ptemp->dataPtr);
		free(ptemp);
	}
	free(pList);
	return pList;
};

/* Inserts data into list
return	-1 if overflow
0 if successful
1 if duplicated key

*/
int addNode(LIST *pList, tTOKEN *dataInPtr)
{
	NODE * temp = pList->head;
	NODE * ptemp = NULL;
	while (temp != NULL && strcmp(temp->dataPtr->token, dataInPtr->token) <= 0)
	{
		ptemp = temp;
		temp = temp->link;
	}

	if (ptemp != NULL && strcmp(ptemp->dataPtr->token, dataInPtr->token) == 0)
	{
		(ptemp->dataPtr->freq)++;
		return 1;
	}

	NODE * node = (NODE *)malloc(sizeof(NODE));
	if (node == NULL)
	{
		return -1;
	}
	node->dataPtr = dataInPtr;

	if (ptemp == NULL)
	{
		node->link = NULL;
		pList->head = node;
	}
	else
	{
		node->link = temp;
		ptemp->link = node;
	}

};


/* Removes data from list
return	0 not found
1 deleted
*/
int removeNode(LIST *pList, char *keyPtr, tTOKEN **dataOut);

/* interface to search function
Argu	key being sought
dataOut	contains found data
return	1 successful
0 not found
*/
int searchList(LIST *pList, char *pArgu, tTOKEN **pDataOut);

/* returns number of nodes in list
*/
int listCount(LIST *pList);

/* returns	1 empty
0 list has data
*/
int emptyList(LIST *pList);

//int fullList( LIST *pList);

/* prints data from list
*/
void printList(LIST *pList)
{
	NODE * temp = pList->head;
	while (temp != NULL)
	{
		printf("%s	%d\n", temp->dataPtr->token, temp->dataPtr->freq);
		temp = temp->link;
	}
};

/* internal insert function
inserts data into a new node
return	1 if successful
0 if memory overflow
*/
static int _insert(LIST *pList, NODE *pPre, tTOKEN *dataInPtr);

/* internal delete function
deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);

/* internal search function
searches list and passes back address of node
containing target and its logical predecessor
return	1 found
0 not found
*/
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
return	token structure pointer
NULL if overflow
*/
tTOKEN *createToken(char *str)
{
	tTOKEN * tok = (tTOKEN *)malloc(sizeof(tTOKEN));
	if (tok == NULL)
	{
		return NULL;
	}
	else
	{
		tok->token = strdup(str);
		tok->freq = 1;
		return tok;
	}
};

/* Deletes all data in token structure and recycles memory
return	NULL head pointer
*/
tTOKEN *destroyToken(tTOKEN *pToken)
{
	free(pToken->token);
	free(pToken);
	return pToken;
};



////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;

	// creates a null list
	list = createList();
	if (!list)
	{
		printf("Cannot create list\n");
		return 100;
	}

	while (scanf("%s", str) == 1)
	{
		pToken = createToken( str);

		// insert function call
		ret = addNode( list, pToken);

		if (ret == 1) // duplicated 
		destroyToken( pToken);
	}
	// print function call
	printList( list);

	destroyList( list);

	return 0;
}
