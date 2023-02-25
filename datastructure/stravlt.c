#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#define max(x, y)	(((x) > (y)) ? (x) : (y))

typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;
} AVL_TREE;

static void _destroy( NODE *root);
static NODE *_insert( NODE *root, NODE *newPtr);
static NODE *_makeNode( char *data);
static NODE *_retrieve( NODE *root, char *key);
static void _traverse( NODE *root);
static void _infix_print( NODE *root, int level);
static int getHeight( NODE *root);
static NODE *rotateRight( NODE *root);
static NODE *rotateLeft( NODE *root);
static int getHeight( NODE *root);
AVL_TREE *AVL_Create( void)
{
	AVL_TREE* temp;
	temp=(AVL_TREE*)malloc(sizeof(AVL_TREE));
	temp->root=NULL;
	temp->count=0;
	return temp;
}

void AVL_Destroy( AVL_TREE *pTree)
{
	if(pTree)
		_destroy(pTree->root);
	free(pTree);
	return;
}

static void _destroy( NODE *root)
{
	if(root)
	{
		_destroy(root->left);
		free(root->data);
		_destroy(root->right);
		free(root);
	}
	return;
}

int AVL_Insert( AVL_TREE *pTree, char *data)
{
	NODE* temp;
	temp=_makeNode(data);
	if(temp==NULL)
		return 0;
	pTree->root=_insert(pTree->root,temp);
	(pTree->count)++;
	return 1;
}

static NODE *_insert( NODE *root, NODE *newPtr)
{
	int l,r,bal;
	if(root==NULL)
	{
		root=newPtr;
		return root;
	}
	if(strcmp(newPtr->data, root->data) < 0) 
	{
    	root->left = _insert(root->left, newPtr);
		root->height=1+max(getHeight(root->left),getHeight(root->right));
		l=getHeight(root->left);
		r=getHeight(root->right);
		bal=l-r;
		if (bal> 1 && strcmp(newPtr->data, root->left->data) < 0)
			return rotateRight(root);

		if (bal< -1 && strcmp(newPtr->data, root->right->data) > 0)
			return rotateLeft(root);

		if (bal> 1 && strcmp(newPtr->data, root->left->data) > 0) 
		{
			root->left = rotateLeft(root->left);
			return rotateRight(root);
		}

		if (bal< -1 &&strcmp(newPtr->data, root->right->data) < 0) 
		{
			root->right = rotateRight(root->right);
			return rotateLeft(root);
		}
    	return root;
  	}
  	else 
	{
    	root->right = _insert(root->right, newPtr);
		root->height=1+max(getHeight(root->left),getHeight(root->right));
		l=getHeight(root->left);
		r=getHeight(root->right);
		bal=l-r;
		if (bal> 1 && strcmp(newPtr->data, root->left->data) < 0)
			return rotateRight(root);

		if (bal< -1 && strcmp(newPtr->data, root->right->data) > 0)
			return rotateLeft(root);

		if (bal> 1 && strcmp(newPtr->data, root->left->data) > 0) 
		{
			root->left = rotateLeft(root->left);
			return rotateRight(root);
		}

		if (bal< -1 &&strcmp(newPtr->data, root->right->data) < 0) 
		{
			root->right = rotateRight(root->right);
			return rotateLeft(root);
		}
    	return root;
	}
}

static NODE *_makeNode( char *data)
{
	NODE* temp;
	temp=(NODE*)malloc(sizeof(NODE));
	temp->data=strdup(data);
	temp->height=0;
	temp->left=NULL;
	temp->right=NULL;
	return temp;
}

char *AVL_Retrieve( AVL_TREE *pTree, char *key)
{
	NODE* temp;
	temp=_retrieve(pTree->root,key);
	if(temp==NULL)
		return NULL;
	else
		return temp->data;
}
static NODE *_retrieve( NODE *root, char *key)
{
	NODE* temp;
	if(root==NULL)
		return root;
	if(strcmp(key,root->data)==0)
		return root;
	else if(strcmp(key,root->data)>0)
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

void AVL_Traverse( AVL_TREE *pTree)
{
	_traverse(pTree->root);
}
static void _traverse( NODE *root)
{
	if(root->left==NULL&&root->right==NULL)
		printf("%s ",root->data);
	else if(root->left==NULL)
	{
		printf("%s ",root->data);
		_traverse(root->right);
	}
	else if(root->right==NULL)
	{
		_traverse(root->left);
		printf("%s ",root->data);
	}
	else
	{
		_traverse(root->left);
		printf("%s ",root->data);
		_traverse(root->right);
	}
}

void printTree( AVL_TREE *pTree)
{
	int level=0;
	_infix_print(pTree->root,level);
}
static void _infix_print( NODE *root, int level)
{
	if(root==NULL)
	{
		return;
	}
	_infix_print(root->right,level+1);
	for(int i=0;i<level;i++)
		printf("\t");
	printf("%s\n",root->data);
	_infix_print(root->left,level+1);
}

static int getHeight( NODE *root)
{
	if (root==NULL)
        return 0;
    return 1+max(getHeight(root->left), getHeight(root->right));
}

static NODE *rotateRight( NODE *root)
{
	NODE* node1 = root->left;
	NODE* node2 = node1->right;
	node1->right = root;
	root->left = node2;
	root->height=getHeight(root);
	node1->height=getHeight(node1);
	return node1;
}
static NODE *rotateLeft( NODE *root)
{

	NODE* node1= root->right;
	NODE* node2 = node1->left;
	node1->left = root;
	root->right = node2;
	root->height=getHeight(root);
	node1->height=getHeight(node1);
	return node1;
}

int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	

	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	
	AVL_Destroy( tree);

	return 0;
}