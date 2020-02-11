#include <stdlib.h> // malloc
#include <stdio.h>
#include <ctype.h> // toupper
#pragma warning(disable:4996)

#define QUIT	1
#define INSERT	2
#define DELETE	3
#define PRINT	4
#define SEARCH	5

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	int			data;
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
	//LIST * list = malloc(sizeof(LIST));
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
	NODE * temp = pList->head->link;
	while (temp != NULL)
	{
		NODE * ptemp = temp;
		temp = temp->link;
		free(ptemp);
	}
	free(pList);
	return pList;
	
	/*
	NODE * temp = pList->head;
	while (temp != NULL)
	{
		NODE * ptemp = temp;
		temp = temp->link;
		free(ptemp);
	}
	free(pList);
	return pList;
	*/
};

/* Inserts data into list
return	-1 if overflow
0 if successful
1 if dupe key
*/
int addNode(LIST *pList, int dataIn)
{
	NODE * temp = (NODE *)malloc(sizeof(NODE));

	if (temp == NULL) return -1;

	else
	{
		temp->data = dataIn;
		temp->link = NULL;
		(pList->count)++;
		
		NODE * findnode = pList->head;
		while (findnode != NULL)
		{
			if (findnode->data == dataIn)
			{
				free(temp);
				return 1;
			}
			findnode = findnode->link;
		}
		
		if (pList->head == NULL)
		{
			pList->head = temp;
			return 0;
		}
		else
		{
			NODE * prenode;
			prenode = pList->head;
			if (dataIn <= pList->head->data)
			{
				
				temp->link = pList->head;
				pList->head = temp;
				return 0;
			}
			else
			{
				while (prenode->link != NULL && dataIn > prenode->link->data)
				{
					prenode = prenode->link;
				}

				temp->link = prenode->link;
				prenode->link = temp;
			}
		}
		return 0;
	}
};

/* Removes data from list
return	0 not found
1 deleted
*/
int removeNode(LIST *pList, int Key, int *dataOut)
{
	NODE * temp = pList->head;
	NODE * pretemp = NULL;
	while (temp->link != NULL && temp->data < Key)
	{
		pretemp = temp;
		temp = temp->link;
	}
	if (temp->data == Key)
	{
		if (pretemp != NULL)
		{
			pretemp->link = temp->link;
			temp->link = NULL;
			free(temp);
			return 1;
		}
		else
		{
			pList->head = temp->link;
			free(temp);
			return 1;
		}
	}
	return 0;
};

/* interface to search function
Argu	key being sought
dataOut	contains found data
return	1 successful
0 not found
*/
int searchList(LIST *pList, int Argu, int *dataOut)
{
	if (pList->head == NULL) return 0;
	pList->pos = pList->head;

	while (pList->pos != NULL)
	{
		if (pList->pos->data == Argu)
		{
			*dataOut = Argu;
			return 1;
		}
		pList->pos = pList->pos->link;
	}
	return 0;
};

/* returns number of nodes in list
*/
int listCount(LIST *pList);

/* returns	1 empty
0 list has data
*/
int emptyList(LIST *pList)
{
	if (pList->count == 0)
	{
		return 1;
	}
	else return 0;
};

//int fullList( LIST *pList);

/* prints data from list
*/
void printList(LIST *pList)
{
	NODE * temp = pList->head;
	while (temp != NULL) {
		fprintf(stdout, "%d ->", temp->data);
		temp = temp->link;
	}
	fprintf(stdout, "NULL\n");
}
;

/* internal insert function
inserts data into a new node
return	1 if successful
0 if memory overflow
*/
static int _insert(LIST *pList, NODE *pPre, int dataIn)
{
	pList->pos = (NODE *)malloc(sizeof(NODE));
	if (pList->pos == NULL)
	{
		return 0;
	}
	pList->pos->data = dataIn;

	return 1;
};

/* internal delete function
deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, int *dataOut);

/* internal search function
searches list and passes back address of node
containing target and its logical predecessor
return	1 found
0 not found
*/
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, int argu);


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
	case 'P':
		return PRINT;
	case 'I':
		return INSERT;
	case 'D':
		return DELETE;
	case 'S':
		return SEARCH;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	int num;
	LIST *list;
	int data;

	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}

	fprintf(stdout, "Select Q)uit, P)rint, I)nsert, D)elete, or S)earch: ");

	while (1)
	{
		int action = get_action();

		switch (action)
		{
		case QUIT:
			destroyList(list);
			return 0;

		case PRINT:
			//print function call
			printList(list);
			break;

		case INSERT:
			fprintf(stdout, "Enter a number to insert: ");
			fscanf(stdin, "%d", &num);

			// insert function call
			addNode(list, num);

			// print function call
			printList(list);
			break;

		case DELETE:
			fprintf(stdout, "Enter a number to delete: ");
			fscanf(stdin, "%d", &num);

			// delete function call
			removeNode( list, num, &data);

			// print function call
			printList( list);
			break;

		case SEARCH:
			fprintf(stdout, "Enter a number to retrieve: ");
			fscanf(stdin, "%d", &num);

			// search function call
			int found;
			found = searchList( list, num, &data);
			if (found) fprintf( stdout, "Found: %d\n", data);
			else fprintf( stdout, "Not found: %d\n", num);

			break;
		}
		if (action) fprintf(stdout, "Select Q)uit, P)rint, I)nsert, D)elete, or S)earch: ");

	}

	return 0;
}