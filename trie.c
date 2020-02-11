#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	char 			*entry;
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
return	node pointer
NULL if overflow
*/
TRIE *trieCreateNode(void)
{
	int i = 0;
	
	TRIE * trie = (TRIE *)malloc(sizeof(TRIE));
	trie -> entry = NULL;
	for(i = 0; i< MAX_DEGREE; i++)
	{
		trie->subtrees[i] = NULL;
	}
	return trie;
};

/* Deletes all data in trie and recycles memory
*/
void trieDestroy(TRIE *root)
{
	
	int i;
	if(root == NULL) return;
	else
	{
		for(i=0;i<MAX_DEGREE;i++)
		{
			if(root->subtrees[i] != NULL) 
			{
				if(root->subtrees[i] -> entry != NULL) free(root->subtrees[i]->entry);
				trieDestroy(root->subtrees[i]);
			}
		}
		free(root);
	}
	
};

/* Inserts new entry into the trie
return	1 success
0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert(TRIE *root, char *str)
{
	int length = strlen(str);
	int i = 0, index;
	TRIE * temp = root;
	for (i = 0; i < length; i++)
	{
		if(str[i] == '$') index = MAX_DEGREE-1;
		else
		{
			if (isupper(str[i]))
			{
				str[i] = tolower(str[i]);
			}
			index = (int)getIndex(str[i]);
		}
		
		if (index < 0 || 26 < index) return 0;

		if (temp->subtrees[index] == NULL)
		{
			temp->subtrees[index] = trieCreateNode();
		}
		temp = temp->subtrees[index];
	}
	if(temp->entry == NULL) temp->entry = str;
	return 1;
};

/* Retrieve trie for the requested key
return	1 key found
0 key not found
*/
int trieSearch(TRIE *root, char *str)
{
	int length = strlen(str), i;
	TRIE * temp = root;
	for(i=0; i<length ; i++)
	{
		int index = (int)getIndex(str[i]);
		if (index < 0 || 26 < index) return 0;
		
		temp = temp->subtrees[index];
		if(temp == NULL) return 0;
	}		
	if(temp->entry != NULL) return 1;
	return 0;
};

/* prints all entries in trie using preorder traversal
*/
void trieList(TRIE *root)
{
	int i = 0;
	if(root != NULL)
	{
		if(root->entry != NULL) printf("%s\n",root->entry);
		for(i=0;i<MAX_DEGREE;i++)
		{
			if(root->subtrees[i] != NULL) 
			{
				trieList(root->subtrees[i]);
			}
		}
		return;
	}
	else return;
};

/* prints all entries starting with str (as prefix) in trie
ex) "abb" -> "abbas", "abbasid", "abbess", ...
using trieList function
*/
void triePrefixList(TRIE *root, char *str)
{
	
	int length = strlen(str);
	int i;
	TRIE * temp = root;
	for(i=0; i<length-1; i++)
	{
		int index = (int)getIndex(str[i]);
		
		if(temp != NULL) temp = temp->subtrees[index];
		
		else return;
	}
	trieList(temp);
};

/* makes permuterms for given str
ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
return	number of permuterms
*/
int make_permuterms(char *str, char *permuterms[])
{
	int length = strlen(str);
	int i = 0, j = 0, count = 0, loc;
	char * temp = (char *)malloc(sizeof(char)*(length+1));
	char * forcpy = (char *)malloc(sizeof(char)*(length+2));
	
	
	for (i = 0; i < length; i++)
	{
		temp[i] = str[i];
	}
	temp[length] = '$';
	//temp[length +1] = '\0';
	
	for (i = 0; i <= length; i++)
	{
		permuterms[i] = (char *)malloc(sizeof(char)*(length+1));

		for (j = 0; j <= length; j++)
		{
			loc = j - i;
			if(loc < 0) loc = loc + length+1;
			forcpy[loc] = temp[j];		
		}
		//forcpy[length + 1] = '\0';
		permuterms[i] = strdup(forcpy);
	}	
	free(temp);
	free(forcpy);
	return (length + 1);
};

/* recycles memory for permuterms
*/
void clear_permuterms(char *permuterms[], int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		free(permuterms[i]);
	}
};

/* wildcard search
ex) "ab*", "*ab", "a*b", "*ab*"
using triePrefixList function
*/
void trieSearchWildcard(TRIE *root, char *str)
{

	int length = strlen(str);
	int wildcardloc = 0 , i, temploc;
	
	for(i=0;i<length;i++)
	{
		if(str[i] == '*') break;
		i = i + 1;
	}
	wildcardloc = i;
	
	char * ptemp = (char *)malloc(sizeof(char)*(length+2)); // free 해야함
	for (i = 0; i < length; i++)
	{
		ptemp[i] = str[i];
	}
	ptemp[length] = '$';
	ptemp[length + 1] = '\0';
	
	while(ptemp[length] != '*')
	{
		ptemp[length + 1] = ptemp[0];
		for(i=1;i< length+1; i++)
		{
			ptemp[i-1] = ptemp[i];
		}
		ptemp[length] = ptemp[length + 1];
		ptemp[length + 1] = '\0';
	}
	
	triePrefixList(root, ptemp);
	
	free(ptemp);
};

int main(int argc, char **argv)
{
	TRIE *trie;
	TRIE *permute_trie;
	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}

	fp = fopen(argv[1], "rt");
	if (fp == NULL)
	{
		fprintf(stderr, "File open error: %s\n", argv[1]);
		return 1;
	}

	trie = trieCreateNode(); // original trie
	
	permute_trie = trieCreateNode(); // trie for permuterm index

	printf("Inserting to trie...\t");
	while (fscanf(fp, "%s", str) == 1) // words file
	{
		ret = trieInsert( trie, str);
		
		if (ret)
		{
			num_p = make_permuterms( str, permuterms);

			for (int i = 0; i < num_p; i++)
			trieInsert( permute_trie, permuterms[i]);

			clear_permuterms( permuterms, num_p);
		}
		
	}

	printf("[done]\n"); // Inserting to trie

	fclose(fp);

	printf("\nQuery: ");
	while (fscanf(stdin, "%s", str) == 1)
	{
		if (strchr(str, '*')) // wildcard search term
		{
			trieSearchWildcard( permute_trie, str);
		}
		else // search term
		{
			ret = trieSearch( trie, str);
			printf( "[%s]%s found!\n", str, ret ? "": " not");
		}
		printf("\nQuery: ");
	}

	trieDestroy( trie);
	trieDestroy( permute_trie);

	return 0;
}