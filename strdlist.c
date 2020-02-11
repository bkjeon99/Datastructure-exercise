#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define DELETE			4

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
	struct node	*llink;
	struct node	*rlink;
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
	while(temp != NULL)
	{
		NODE * ptemp = temp;
		temp = temp->rlink;
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
	while (temp != NULL && strcmp(temp->dataPtr->token, dataInPtr->token) < 0)
	{
		ptemp = temp;
		temp = temp->rlink;
	}

	if (temp != NULL && strcmp(temp->dataPtr->token, dataInPtr->token) == 0)
	{
		(temp->dataPtr->freq)++;
		return 1;
	}
	NODE * node = (NODE *)malloc(sizeof(NODE));	
	if (node == NULL) return -1;

	node->dataPtr = dataInPtr;

	node->llink = ptemp;
	node->rlink = temp;

	if(ptemp != NULL)ptemp->rlink = node;
	else pList->head = node;
	if(temp != NULL) temp->llink = node;
	else pList->rear = node;
	return 0;
};

/* Removes data from list
return	0 not found
1 deleted
*/
int removeNode(LIST *pList, char *keyPtr, tTOKEN **dataOut)
{
	NODE * temp = pList->head;
	while (temp != NULL)
	{
		if (strcmp(temp->dataPtr->token, keyPtr) == 0)
		{
			*dataOut = temp->dataPtr;
			if(temp->llink != NULL)
			{
				temp->llink->rlink = temp->rlink;
				if(temp->rlink != NULL) temp->rlink->llink = temp->llink;
				else pList->rear = temp->llink;
			}
			else 
			{
				pList->head = temp->rlink;
				if(temp->rlink != NULL) temp->rlink->llink = temp->llink;
				pList->rear = temp->llink;
			}
			free(temp);
			return 1;
		}
		temp = temp->rlink;
	}
	return 0;
};

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

/* prints data from list (forward)
*/
void printList(LIST *pList)
{
	NODE * temp = pList->head;
	while (temp != NULL)
	{
		printf("%s	%d\n", temp->dataPtr->token, temp->dataPtr->freq);
		temp = temp->rlink;
	}
};

/* prints data from list (backward)
*/
void printListR(LIST *pList)
{
	NODE * temp = pList->rear;
	while (temp != NULL)
	{
		printf("%s	%d\n", temp->dataPtr->token, temp->dataPtr->freq);
		temp = temp->llink;
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

/* gets user's input
*/
int get_action()
{
	char ch;
	scanf("%c", &ch);
	ch = toupper(ch);
	switch (ch)
	{
	case 'Q':
		return QUIT;
	case 'F':
		return FORWARD_PRINT;
	case 'B':
		return BACKWARD_PRINT;
	case 'D':
		return DELETE;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	FILE *fp;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}

	fp = fopen(argv[1], "rt");
	if (!fp)
	{
		fprintf(stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}

	// creates a null list
	list = createList();
	if (!list)
	{
	printf( "Cannot create list\n");
	return 100;
	}

	while (fscanf(fp, "%s", str) == 1)
	{
		pToken = createToken( str);

		// insert function call
		ret = addNode( list, pToken);

		if (ret == 1) // duplicated 
		destroyToken( pToken);
	}

	fclose(fp);

	fprintf(stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");

	while (1)
	{
		int action = get_action();

		switch (action)
		{
		case QUIT:
			destroyList( list);
			return 0;

		case FORWARD_PRINT:
			printList( list);
			break;

		case BACKWARD_PRINT:
			printListR( list);
			break;

		case DELETE:
			fprintf(stdout, "Input a string to delete: ");
			fscanf(stdin, "%s", str);
			int ret = removeNode( list, str, &pToken);
			if (ret)
			{
			fprintf( stdout, "%s deleted\n", pToken->token);
			destroyToken( pToken);
			}
			else fprintf( stdout, "%s not found\n", str);
			break;
		}

		if (action) fprintf(stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
	}
	return 0;
}
