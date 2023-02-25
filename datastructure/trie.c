#include <stdio.h>
#include <stdlib.h>	
#include <string.h>	
#include <ctype.h>	
#define MAX_DEGREE	27 
#define EOW			'$'	
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

typedef struct trieNode {
	int 			index; 
	struct trieNode	*subtrees[MAX_DEGREE]; 
} TRIE;


TRIE *trieCreateNode(void)
{
	TRIE* temp;
	temp=(TRIE*)malloc(sizeof(TRIE));
	if(temp==NULL)
		return temp;
	temp->index=-1;
	for (int i = 0; i < MAX_DEGREE; i++)
		temp->subtrees[i] = NULL;
	return temp;
}

void trieDestroy( TRIE *root)
{
	for(int i=0;i<MAX_DEGREE;i++)
	{
		if(!root->subtrees[i])
			continue;
		else 
			trieDestroy(root->subtrees[i]);
	}
	free(root);
	return;
}


int trieInsert( TRIE *root, char *str, int dic_index)
{
	int len=strlen(str);
    int num;
    TRIE* temp=root;
    for (int i=0; i<len; i++)
    {
		if(isupper(str[i]))
			str[i]=tolower(str[i]);
        num = getIndex(str[i]);
		if(num<0||num>MAX_DEGREE-1)
			return 0;
        if (!temp->subtrees[num])
            temp->subtrees[num] = trieCreateNode();
        temp = temp->subtrees[num];
    }
	temp->index=dic_index;
	return 1;
}

int trieSearch( TRIE *root, char *str)
{
	int len=strlen(str);
	TRIE* temp=root;
	int num;
	for(int i=0;i<len;i++)
	{
		num=getIndex(str[i]);
		if(!temp->subtrees[num])
			return -1;
		temp=temp->subtrees[num];
	}
	return temp->index;
}


void trieList( TRIE *root, char *dic[])//루트를 받아 루트로부터 조회할 수 있는 모든 문자열 출력
{
	if(root==NULL)
		return;
	if(root->index!=-1)
	{
		printf("%s\n",dic[root->index]);
	}
	for(int i=0;i<MAX_DEGREE;i++)
	{
		if(root->subtrees[i]==NULL)
			continue;
		else
			trieList(root->subtrees[i],dic);
	}
	return;
}
void triePrefixList( TRIE *root, char *str, char *dic[])
{
	int len=strlen(str);
	TRIE* temp=root;
	int num;
	for(int i=0;i<len;i++)
	{
		num=getIndex(str[i]);
		if(root->subtrees[num]==NULL)
			return;
		temp=temp->subtrees[num];
	}
	trieList(temp,dic);
}

int main(int argc, char **argv)
{
	TRIE *trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	int index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode();
	
	while (fscanf( fp, "%s", str) != EOF)
	{
		ret = trieInsert( trie, str, index);

		if (ret) 
			dic[index++] = strdup( str);
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		if (str[strlen(str)-1] == '*')
		{
			str[strlen(str)-1] = 0;
			triePrefixList( trie, str, dic);
		}
		else
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		
		printf( "\nQuery: ");
	}
	
	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	
	return 0;
}
