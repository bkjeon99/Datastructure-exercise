#define DEBUG 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <ctype.h>

// 토큰-문서 구조체
typedef struct {
	char	*token;	// 토큰
	int		docid;	// 문서번호(document ID)
} tTokenDoc;

typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// 문서 빈도(document frequency)
} tHEADER;

////////////////////////////////////////////////////////////////////////////////
// 토큰 구조체로부터 역색인 파일을 생성한다.
void invertedIndex( tTokenDoc *tokens, int num_tokens, char *dicfilename, char *headerfilename, char *postingfilename)
{
	FILE* fp = fopen(postingfilename, "w");
	int posting[num_tokens];
	int i = 0;
	int j = 0;
	int header_size = 1;
	int index= 0, df = 1;
	
	char * ptoken = NULL;
	for(i=0; i<num_tokens; i++)
	{
		posting[i] = tokens[i].docid;
	}
	fwrite(posting, sizeof(int), num_tokens, fp);
	fclose(fp);
	
	
	FILE * fp2 = fopen(headerfilename, "w");
	tHEADER* header = (tHEADER*)malloc(sizeof(tHEADER)*65000);
	for(i=0; i<num_tokens; i++)
	{
		df = 1;
		index = i;
		while(strcmp(tokens[i+1].token, tokens[i].token) == 0)
		{
			df++;
			i++;
		}
		header[j].index = index;
		header[j].df = df;

		j++;
	}
	fwrite(header, sizeof(tHEADER), j, fp2);

	fclose(fp2);
	
	
	
	ptoken = NULL;
	FILE * fp3 = fopen(dicfilename, "w");
	i = 0;
	fputs(tokens[i].token, fp3);	
	fprintf(fp3, "\n");
	for(i=1; i<num_tokens; i++)
	{
		while(strcmp(tokens[i-1].token, tokens[i].token) == 0)
		{
			i++;
		}
		fputs(tokens[i].token, fp3);	
		fprintf(fp3, "\n");
	}
	fclose(fp3);

	
};

// 입력 파일로부터 토큰-문서 구조체를 생성한다.
tTokenDoc *get_tokens(char *filename, int *num_tokens)
{
	tTokenDoc* tokendoc;
	tokendoc = (tTokenDoc*)malloc(sizeof(tTokenDoc) * 10000);
	char* ptoken;
	char temp[5000];
	int i = 0; 
	int j;
	int docid = 1;
	int tokendocsize = 1;
	FILE * fp = fopen(filename, "r");
	
	
	while(1)
	{
		fgets(temp, sizeof(temp), fp);
		if(feof(fp)) break;
		
		ptoken = strtok(temp, " ");
	

		while(ptoken != NULL)
		{
			if(i >= tokendocsize * 10000 -1)
			{
				tokendocsize++;
				tokendoc = realloc(tokendoc, sizeof(tTokenDoc) * tokendocsize * 10000); 
			}
			
			for(j=0; j<sizeof(ptoken); j++)
			{
				if(ptoken[j] >= 'A' && ptoken[j] <= 'Z')
				ptoken[j] = tolower(ptoken[j]);
			}
			tokendoc[i].token = strdup(ptoken);
			tokendoc[i].docid = docid;	
			
			i++;
			ptoken = strtok(NULL, " ");
		}
		free(tokendoc[i].token);
		i--;
		docid = docid + 1;
		
	}
	
	*num_tokens = i;
	fclose(fp);
	return tokendoc;
};

// qsort를 위한 비교함수 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
static int _compare(const void *n1, const void *n2)
{
	tTokenDoc a = *(tTokenDoc*)n1;
	tTokenDoc b = *(tTokenDoc*)n2;
	
	if(strcmp(a.token, b.token)<0) return -1;
	
	if(strcmp(a.token, b.token)>0) return 1;
	
	else
	{
		if(a.docid < b.docid) return -1;
		if(a.docid > b.docid) return 1;
		else return 0;
	}
	
};

////////////////////////////////////////////////////////////////////////////////
static void print_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		printf( "%s\t%d\n", tokens[i].token, tokens[i].docid);
	}
}

////////////////////////////////////////////////////////////////////////////////
void destroy_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		free( tokens[i].token);
	}
	free( tokens);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tTokenDoc *tokens;
	int num_tokens = 0;

	if (argc != 2)
	{
		printf( "Usage: %s FILE\n", argv[0]);
		return 2;
	}

	tokens = get_tokens( argv[1], &num_tokens);
	
	assert( tokens != NULL && num_tokens > 0);

#if DEBUG	
	//print_tokens( tokens, num_tokens);
#endif

	// 정렬 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
	qsort( tokens, num_tokens, sizeof( tTokenDoc), _compare);
	
	invertedIndex( tokens, num_tokens, "dic.txt", "header.idx", "posting.idx");

#if DEBUG	
	print_tokens( tokens, num_tokens);
#endif	

	destroy_tokens( tokens, num_tokens);
	
	return 0;
}

