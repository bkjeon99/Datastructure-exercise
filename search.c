//#define DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "trie.h"

// 역색인 헤더 정보에 대한 구조체
typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// document frequency
} tHEADER;

////////////////////////////////////////////////////////////////////////////////
// 헤더 정보가 저장된 파일(예) "header.idx")을 읽어 메모리에 저장한다.
// 헤더 구조체 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
tHEADER *load_header( char *filename)
{
	FILE * fp = fopen(filename, "r");
	tHEADER* header = (tHEADER*)malloc(sizeof(tHEADER) * 65000);
	fread(header, sizeof(tHEADER), 65000, fp);
	if(header == NULL) return NULL;
	else return header;
};

// 포스팅 리스트가 저장된 파일(예) "posting.idx")를 읽어 메모리에 저장한다.
// 포스팅 리스트(int arrary) 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
int *load_posting( char *filename)
{
	FILE * fp = fopen(filename, "r");
	if(fp == NULL) return NULL;
	
	int * array = (int*)malloc(sizeof(int) * 65000);
	int i = 0;
	fread(array, sizeof(int), 65000, fp);
	if(array == NULL) return NULL;
	/*
	while(!feof(fp))
	{
		if(feof(fp)) break;
		fscanf(fp, "%d", &array[i]);
		if(&array[i] == NULL) break;
		i++;
	}
	*/
	return array;
};

// 문서 집합을 화면에 출력한다.
void showDocuments( int *docs, int numdocs)
{
	int i = 0;
	printf("%d", numdocs);
	for(i=0; i<numdocs ; i++)
	{
		printf("%d ->", docs[i]);
	}
};

// 두 문서 집합의 교집합을 구한다.
// 교집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 교집합의 문서 수는 newnumdocs에 저장한다.
int *intersectDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs)
{
	int i = 0,j = 0, count = 0;
	int * newdocs;
	if(numdocs > numdocs2) newdocs = (int*)malloc(sizeof(int) * numdocs);
	else newdocs = (int*)malloc(sizeof(int)*numdocs2);
	while(1)
	{
		if(docs[i] == docs2[j])
		{
			newnumdocs[count] = docs[i];
			i++;
			j++;
			count++;
		}
		else if(docs[i] < docs2[j])
		{
			i++;
			if( i >= numdocs) break;
		}
		else
		{
			j++;
			if(j >= numdocs2) break;
		}
	}
	*newnumdocs = count;
	return newdocs;

};

// 두 문서 집합의 합집합을 구한다.
// 합집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 합집합의 문서 수는 newnumdocs에 저장한다.
int *unionDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs)
{
	int i = 0, j = 0, count = 0, c;
	int * newdocs = (int*)malloc(sizeof(int) *(numdocs + numdocs2));
	while(1)
	{
		if(docs[i] == docs2[j])
		{
			newnumdocs[count] = docs[i];
			i++;
			j++;
			count++;
		}
		else if(docs[i] < docs2[j])
		{
			newnumdocs[count] = docs[i];
			i++;
			count++;
			if( i >= numdocs)
			{
				for(c=j; c<numdocs2; c++)
				{
					newnumdocs[count] = docs2[c];
					count ++;
				}
				break;
			}
		}
		else
		{
			newnumdocs[count] = docs2[j];
			j++;
			count++;
			if(j >= numdocs2)
			{
				for(c=i; c<numdocs; c++)
				{
					newnumdocs[count] = docs[c];
					count ++;
				}
				break;
			}
		}
	}
	*newnumdocs = count;
	return newdocs;
};

// 단일 텀(single term)을 검색하여 문서를 찾는다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *getDocuments( tHEADER *header, int *posting, TRIE *trie, char *term, int *numdocs)
{
	int index = trieSearch( trie, term);
	if(index <0) return NULL;

	
	int i =0;
	int * docs = (int*)malloc(sizeof(int)*header[index].df);
	int docloc = header[index].index;
	for(i=0; i<header[index].df; i++)
	{
		docs[i] = posting[docloc];
		docloc++;
	}
	
	*numdocs = header[index].df;
	return docs;
};

// 질의(query)를 검색하여 문서를 찾는다.
// 질의는 단일 텀 또는 하나 이상의 불린 연산자('&' 또는 '|')를 포함한 질의가 될 수 있다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *searchDocuments( tHEADER *header, int *posting, TRIE *trie, char *query, int *numdocs)
{
	int length = strlen(query);
	int i, j=0 , c, k = 0;
	int* docs;
	int* pdocs;
	int docsnum;
	int pdocsnum;
	char temp[100];
	char ptemp[100];
	char dump;
	
	for (i=length -2 ; i>= 0; i--)
	{
		if(query[i] == '|' || query[i] == '&')
		{
			for(k=0; k<j/2; k++);
			{
				dump = temp[k];
				temp[k] = temp[j-k];
				temp[j-k] = dump;
			}
			temp[j] = '\0';
			docs = getDocuments( header, posting, trie, temp, &docsnum);
			
			for(c=0; c<length-j; c++)
			{
				ptemp[c] = query[c];
			}
			c++;
			ptemp[c] = '\0';
			pdocs = searchDocuments(header,posting,trie,ptemp ,&pdocsnum);
			
			if(query[i] == '|') docs = unionDocuments( docs, docsnum, pdocs, pdocsnum, &docsnum);
			else docs = intersectDocuments( docs, docsnum, pdocs, pdocsnum, &docsnum);
			*numdocs = docsnum;
			return docs;
		}
		else
		{
			temp[j] = query[i];
			j++;
		}
	}
	for(k=0; k<j/2; k++);
	{
		dump = temp[k];
		temp[k] = temp[j-k];
		temp[j-k] = dump;
	}
	temp[j] = '\0';
	docs = getDocuments( header, posting, trie, temp, &docsnum);
	*numdocs = docsnum;
	return docs;
};

////////////////////////////////////////////////////////////////////////////////
static char *rtrim( char *str)
{
	char *p = str + strlen( str) - 1;
	
	while (p >= str)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') *p = 0;
		else return str;
		p--;
	}
}

static char *ltrim( char *str)
{
	char *p = str;
	
	while (*p)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') p++;
		else 
		{
			return p;
		}
	}
	return NULL;
}

static char *trim( char *str)
{
	if( str == NULL || *str == 0) return str;
	
	return rtrim(ltrim(str));
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tHEADER *header;
	int *posting;
	TRIE *trie;
	char query[100];
	int index;
	
	header = load_header( "header.idx");
	if (header == NULL) return 1;
	
	posting = load_posting( "posting.idx");
	if (posting == NULL) return 1;
		
	trie = dic2trie( "dic.txt");
	
	printf( "\nQuery: ");
	while (fgets( query, 100, stdin) != NULL)
	{
		int numdocs;
		int *docs = searchDocuments( header, posting, trie, query, &numdocs);

		if (docs == NULL) printf( "not found!\n");
		else 
		{
			
			showDocuments( docs, numdocs);
			free( docs);
		}
		printf( "\nQuery: ");
	}
	
	free( header);
	free( posting);
	//trieDestroy( trie);
	
	return 0;
}
