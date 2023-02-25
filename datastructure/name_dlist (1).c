#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <string.h> 
#include <ctype.h> 

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

typedef struct 
{
	char	*name;
	char	sex;
	int		freq;
} tName;

typedef struct node
{
	tName		*dataPtr;
	struct node	*llink;
	struct node	*rlink;
} NODE;

typedef struct
{
	int		count;
	NODE	*head;
	NODE	*rear;
} LIST;

LIST* createList(void);
void destroyList(LIST* pList);
int addNode(LIST* pList, tName* dataInPtr);
int removeNode(LIST* pList, tName* keyPtr, tName** dataOutPtr);
int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr);
int countList(LIST* pList);
int emptyList(LIST* pList);
void traverseList( LIST *pList, void (*callback)(const tName *));
void traverseListR( LIST *pList, void (*callback)(const tName *));
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr);
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr);
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu);
void destroyName( tName *pNode);
int get_action();
int cmpName( const tName *pName1, const tName *pName2);
void print_name(const tName *dataPtr);
void increase_freq(tName *dataOutPtr, const tName *dataInPtr);
void split_name_sex(char *str, char *sex);

LIST* createList(void)//check
{
	LIST* list;
	list = (LIST*)malloc(sizeof(LIST));
	list->count = 0;
	list->head = NULL;
	list->rear = NULL;
	return list;
}

void destroyList(LIST* pList)//check
{
	NODE* delete;
	if(pList)
	{
		while(pList->count>0)
		{
			delete=pList->head;
			pList->head=pList->head->rlink;
			pList->count-=1;
			destroyName(delete->dataPtr);
			free(delete);
		}
		free(pList);
	}
}

int addNode(LIST* pList, tName* dataInPtr)
{
	NODE* pPre=NULL;
	NODE* pLoc=NULL;
	int num,check;
	num=_search(pList, &pPre, &pLoc, dataInPtr);
	if(num==0)
	{
		check=_insert(pList,pPre,dataInPtr);
		if(check==0)
			return 0;
		else
			return 1;
	}
	else if(num==1)
	{
		increase_freq(pLoc->dataPtr,dataInPtr);
		return 2;
	}	
}

int removeNode(LIST* pList, tName* keyPtr, tName** dataOutPtr)
{
	NODE* pPre=NULL;
	NODE* pLoc=NULL;
	int num;
	num=_search(pList, &pPre, &pLoc, keyPtr);
	if(num==1)
	{
		_delete(pList,pPre,pLoc,dataOutPtr);
		return 1;
	}
	return 0;
}

int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr) //check
{
	NODE* pPre=NULL;
	NODE* pLoc=NULL;
	int num;
	num=_search(pList, &pPre, &pLoc, pArgu);
	if(num==1)
	{
		*dataOutPtr=pLoc->dataPtr;
		return 1;
	}
	return 0;
}
int countList(LIST* pList) //check
{
	return pList->count;
}

int emptyList(LIST* pList) //check
{
	if (pList->count == 0)
		return 1;
	else
		return 0;
}

void traverseList( LIST *pList, void (*callback)(const tName *))  //check
{
	NODE* pnt;
	pnt=pList->head;
	for(int i=0;i<pList->count;i++)
	{
		callback(pnt->dataPtr);
		pnt=pnt->rlink;
	}
}

void traverseListR( LIST *pList, void (*callback)(const tName *)) //check
{
	NODE* pnt;
	pnt=pList->rear;
	for(int i=0;i<pList->count;i++)
	{
		callback(pnt->dataPtr);
		pnt=pnt->llink;
	}
}

static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr) //check
{	
	NODE* new;
	new=(NODE*)malloc(sizeof(NODE));
	if(new==NULL)
		return 0;
	new->dataPtr=dataInPtr;
	if(new->dataPtr==NULL)
		return 0;
	if(pList->count==0)
	{
		new->llink=NULL;
		new->rlink=NULL;
		pList->head=new;
		pList->rear=new;
	}	
	else if(pPre==NULL)
	{
		new->llink=NULL;
		new->rlink=pList->head;
		pList->head->llink=new;
		pList->head=new;
	}
	else if(pPre->rlink==NULL)
	{
		new->llink=pPre;
		new->rlink=NULL;
		pPre->rlink=new;
		pList->rear=new;
	}
	else
	{
		new->llink=pPre;
		new->rlink=pPre->rlink;
		pPre->rlink->llink=new;
		pPre->rlink=new;
	}
	(pList->count)++;
	return 1;
}

static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr)
{
	if(pPre==NULL)
	{
		(*dataOutPtr)=pLoc->dataPtr;
		pList->head=pLoc->rlink;
		pLoc->llink=NULL;
		if(pList->count==0)
			pList->rear=NULL;		
	}
	else if(pLoc->rlink==NULL)
	{
		(*dataOutPtr)=pLoc->dataPtr;
		pPre->rlink=pLoc->rlink;
		pList->rear=pPre;
	}
	else
	{
		(*dataOutPtr)=pLoc->dataPtr;
		pPre->rlink=pLoc->rlink;
		pLoc->rlink->llink=pPre;
	}
	free(pLoc);
	(pList->count)--;
}


static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu)
{
	(*pPre)=NULL;
	(*pLoc)=pList->head;
	while((*pLoc)!=NULL)
	{
		if(cmpName(pArgu,(*pLoc)->dataPtr)==0)
			return 1;
		else if(cmpName(pArgu,(*pLoc)->dataPtr)<0)
			return 0;
		else
		{
			(*pPre)=(*pLoc);
			(*pLoc)=(*pLoc)->rlink;
		}
	}
	return 0;
}
tName* createName(char* str, char sex, int freq)
{
	tName* new;
	int len;
	new = (tName*)malloc(sizeof(tName));
	len=strlen(str);
	new->name=(char*)malloc(sizeof(char)*(len+1));
	strcpy(new->name,str);
	new->sex = sex;
	new->freq = freq;
	return new;
}

void destroyName( tName *pNode)
{
	free(pNode->name);
	free(pNode);
}

int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'P':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; 
}

int cmpName( const tName *pName1, const tName *pName2)
{
	int ret = strcmp( pName1->name, pName2->name);
	if (ret == 0) return pName1->sex - pName2->sex;
	else return ret;
}


void print_name(const tName *dataPtr)
{
	printf( "%s\t%c\t%d\n", dataPtr->name, dataPtr->sex, dataPtr->freq);
}

void increase_freq(tName *dataOutPtr, const tName *dataInPtr)
{
	dataOutPtr->freq += dataInPtr->freq;
}

void split_name_sex(char *str, char *sex)
{
	char *p;
	
	p = strchr(str, '/');
	if (p == NULL) 
	{
		fprintf( stderr, "unexpected name/sex format\n");
		return;
	}
	else
	{
		*p++ = 0;
		*sex = *p;
	}
}
 
int main( int argc, char **argv)
{
	LIST *list;
	
	char str[1024];
	char name[100];
	char sex;
	int freq;
	
	tName *pName;
	int ret;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%*d\t%s\t%c\t%d", str, &sex, &freq) != EOF)
	{
		pName = createName( str, sex, freq);
		
		ret = addNode( list, pName);
		
		if (ret == 2) 
		{
			destroyName( pName);
		}
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		tName *p;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				traverseList( list, print_name);
				break;
			
			case BACKWARD_PRINT:
				traverseListR( list, print_name);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a name/sex to find: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				pName = createName( str, sex, 0);

				if (searchList( list, pName, &p)) 
					print_name( p);
				else 
					fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a name/sex to delete: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				pName = createName( str, sex, 0);

				if (removeNode( list, pName, &p))
				{
					fprintf( stdout, "(%s, %c, %d) deleted\n", p->name, p->sex, p->freq);
					destroyName( p);
				}
				else 
					fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
		}
		
		if (action) 
			fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}