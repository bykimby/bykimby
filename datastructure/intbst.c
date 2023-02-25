#include <stdlib.h> 
#include <stdio.h>
#include <assert.h>
#include <time.h> 

#define RANDOM_INPUT	1
#define FILE_INPUT		2

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
static void _destroy( NODE *root);
static void _insert( NODE *root, NODE *newPtr);
static NODE *_delete( NODE *root, int dltKey, int *success);
static NODE *_retrieve( NODE *root, int key);
static void _traverse( NODE *root);
static void _inorder_print( NODE *root, int level);
NODE *_makeNode( int data);
TREE *BST_Create( void)
{
	TREE* bst;
	bst=(TREE*)malloc(sizeof(TREE));
	bst->root=NULL;
	return bst;
}

void BST_Destroy( TREE *pTree)
{
	_destroy(pTree->root);
	free(pTree);
}

static void _destroy( NODE *root)
{
	if(root==NULL)
		return;
	_destroy(root->left);
	_destroy(root->right);
	free(root);
	return;
}

int BST_Insert( TREE *pTree, int data)
{
	NODE* temp;
	temp=_makeNode(data);
	if(temp==NULL)
		return 0;
	if(pTree->root==NULL)
	{
		pTree->root=temp;
		return 1;
	}
	else
	{
		_insert(pTree->root,temp);
		return 1;
	}
}

static void _insert( NODE *root, NODE *newPtr)
{
	
	if(newPtr->data>=root->data)
	{
		if(root->right==NULL)
		{
			root->right=newPtr;
			return;
		}
		else
			_insert(root->right,newPtr);
	}
	else if(newPtr->data<root->data)
	{
		if(root->left==NULL)
		{
			root->left=newPtr;
			return;
		}
		else
			_insert(root->left,newPtr);
	}
}

NODE *_makeNode( int data)
{
	NODE* temp;
	temp=(NODE*)malloc(sizeof(NODE));
	(temp->data)=data;
	temp->left=NULL;
	temp->right=NULL;
	return temp;
}

int BST_Delete( TREE *pTree, int dltKey)
{
	int success;
	NODE* newRoot;
	newRoot = _delete(pTree->root, dltKey, &success);
	if(success) 
	{
    	pTree->root = newRoot;
	}
	return success;
}

static NODE *_delete( NODE *root, int dltKey, int *success)
{
	NODE* dlt;
	NODE* newRoot;
	NODE* exch;
	int temp;
  
	if(root==NULL) {
    	(*success) = 0;
    	return NULL;
  	}
  
  	if(root->data<dltKey)
    	root->right = _delete(root->right, dltKey, success);
  	else if(root->data>dltKey)
    	root->left = _delete(root->left, dltKey, success);
  	else 
	{
    	dlt= root;
    	if(root->left==NULL&&root->right==NULL)
		{
			free(dlt);
			(*success)=1;
			return NULL;
		}
		else if(root->left==NULL) {
      		newRoot = root->right;
      		free(dlt);
      		(*success) =1;
      		return newRoot;
    	}
    	else if(root->right==NULL)
		{
        	newRoot = root->left;
        	free(dlt);
        	(*success) = 1;
        	return newRoot;
      	}
      	else 
		{
        	exch = root->right;
        	while(exch->left)
        		exch = exch->left;
        	temp = root->data;
        	root->data = exch->data;
        	exch->data = temp;
        	root->right = _delete(root->right, exch->data, success);
		}
    }
  	return root;
}

int *BST_Retrieve( TREE *pTree, int key)
{
	NODE* temp;
	int* num;
	temp=_retrieve(pTree->root,key);
	num=&(temp->data);
	return num;
}


static NODE *_retrieve( NODE *root, int key)
{
	NODE* temp;
	if(root==NULL)
		return root;
	if(key==root->data)
		return root;
	else if(key>root->data)
	{
		if(root->right==NULL)
			return NULL;
		else
			root=_retrieve(root->right,key);
	}
	else
	{
		if(root->left==NULL)
			return NULL;
		else
			root=_retrieve(root->left,key);
	}
}

void BST_Traverse( TREE *pTree)
{
	_traverse(pTree->root);
}

static void _traverse( NODE *root)
{
	if(root->left==NULL&&root->right==NULL)
		printf("%d ",root->data);
	else if(root->left==NULL)
	{
		printf("%d ",root->data);
		_traverse(root->right);
	}
	else if(root->right==NULL)
	{
		_traverse(root->left);
		printf("%d ",root->data);
	}
	else
	{
		_traverse(root->left);
		printf("%d ",root->data);
		_traverse(root->right);
	}
}


void printTree( TREE *pTree)
{
	int level=0;
	_inorder_print(pTree->root,level);
}
static void _inorder_print( NODE *root, int level)
{
	if(root==NULL)
	{
		return;
	}
	else if(root->left==NULL&&root->right==NULL)
	{
		for(int i=0;i<level;i++)
			printf("\t");
		printf("%d\n",root->data);
	}
	else if(root->right==NULL)
	{
		for(int i=0;i<level;i++)
			printf("\t");
		printf("%d\n",root->data);
		_inorder_print(root->left,level+1);
	}
	else if(root->left==NULL)
	{
		_inorder_print(root->right,level+1);
		for(int i=0;i<level;i++)
			printf("\t");
		printf("%d\n",root->data);
	}
	else
	{
		_inorder_print(root->right,level+1);
		for(int i=0;i<level;i++)
			printf("\t");
		printf("%d\n",root->data);
		_inorder_print(root->left,level+1);
	}
	
}

int BST_Empty( TREE *pTree)
{
	if(pTree->root==NULL)
		return 1;
	else
		return 0;
} 


int main( int argc, char **argv)
{
	int mode; 
	TREE *tree;
	int data;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else mode = FILE_INPUT;
	
	
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert( numbers > 0); 

		fprintf( stdout, "Inserting: ");
		
		srand( time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers*3) + 1; 
			fprintf( stdout, "%d ", data);
			
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}