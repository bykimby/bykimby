#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define INSERT_OP      0x01 
#define DELETE_OP      0x02  
#define SUBSTITUTE_OP  0x04  
#define MATCH_OP       0x08  
#define TRANSPOSE_OP   0x10 

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1
static int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}
static int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m);
void print_alignment( char align_str[][8], int level);
static void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8])
{
	static int order=1;
	if(n==0&&m==0)
	{	
		printf("\n[%d]==============================\n",order);
		order++;
		print_alignment(align_str,level-1);
	}
	int state=op_matrix[n*col_size+m];
	if(state&SUBSTITUTE_OP||state&MATCH_OP)
	{
		align_str[level][0]=str1[n-1];
		align_str[level][1]=' ';
		align_str[level][2]='-';
		align_str[level][3]=' ';
		align_str[level][4]=str2[m-1];
		align_str[level][5]='\0';
		backtrace_main(op_matrix,col_size,str1,str2,n-1,m-1,level+1,align_str);
	}
	if(state&INSERT_OP)
	{
		align_str[level][0]='*';
		align_str[level][1]=' ';
		align_str[level][2]='-';
		align_str[level][3]=' ';
		align_str[level][4]=str2[m-1];
		align_str[level][5]='\0';
		backtrace_main(op_matrix,col_size,str1,str2,n,m-1,level+1,align_str);
	}
	if(state&DELETE_OP)
	{
		align_str[level][0]=str1[n-1];
		align_str[level][1]=' ';
		align_str[level][2]='-';
		align_str[level][3]=' ';
		align_str[level][4]='*';
		align_str[level][5]='\0';
		backtrace_main(op_matrix,col_size,str1,str2,n-1,m,level+1,align_str);
	}
	if(state&TRANSPOSE_OP)
	{
		align_str[level][0]=str1[n-2];
		align_str[level][1]=str1[n-1];
		align_str[level][2]=' ';
		align_str[level][3]='-';
		align_str[level][4]=' ';
		align_str[level][5]=str2[m-2];
		align_str[level][6]=str2[m-1];
		align_str[level][7]='\0';
		backtrace_main(op_matrix,col_size,str1,str2,n-2,m-2,level+1,align_str);
	}
	if(level==0)
		order=1;
}

void print_matrix( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	for(int i=n;i>0;i--)
	{
		printf("%c\t",str1[i-1]);
		for(int j=1;j<=m;j++)
		{
			int state=op_matrix[i*col_size+j];
			if(state&MATCH_OP)
				printf("M");
			if(state&SUBSTITUTE_OP)
				printf("S");
			if(state&INSERT_OP)
				printf("I");
			if(state&DELETE_OP)
				printf("D");
			if(state&TRANSPOSE_OP)
				printf("T");
			printf("\t");
		}
		printf("\n");
	}
	printf(" \t");
	for(int i=0;i<m;i++)
		printf("%c\t",str2[i]);
	printf("\n");
}

int min_editdistance( char *str1, char *str2)
{
	int n,m;
	int col_size,result;
	n=strlen(str1);
	m=strlen(str2);
	col_size=m+1;
	int num_matrix[(n+1)*(m+1)];
	int op_matrix[(n+1)*(m+1)];
	int choice1,choice2,choice3,choice4;
	for(int i=0;i<=n;i++)
		num_matrix[i*col_size]=DELETE_COST*i;
	for(int i=1;i<=m;i++)
		num_matrix[0*col_size+i]=INSERT_COST*i;
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
		{
			if(str1[i-1]==str2[j-1])//일치
			{
				choice1=num_matrix[i*col_size+j-1]+INSERT_COST;
				choice2=num_matrix[(i-1)*col_size+j]+DELETE_COST;
				choice3=num_matrix[(i-1)*col_size+j-1];
				num_matrix[i*col_size+j]=__GetMin3(choice1,choice2,choice3);
			}
			else
			{
				choice1=num_matrix[i*col_size+j-1]+INSERT_COST;
				choice2=num_matrix[(i-1)*col_size+j]+DELETE_COST;
				choice3=num_matrix[(i-1)*col_size+j-1]+SUBSTITUTE_COST;
				num_matrix[i*col_size+j]=__GetMin3(choice1,choice2,choice3);
				if(str1[i-1]==str2[j-2]&&str1[i-2]==str2[j-1]&&i>1&&j>1)
				{
					choice4=num_matrix[(i-2)*col_size+j-2]+TRANSPOSE_COST;
					num_matrix[i*col_size+j]=__GetMin4(choice1,choice2,choice3,choice4);
				}
			}
		}
	result=num_matrix[n*col_size+m];
	for(int i = 0; i < (n+1)*(m+1); i++) 
		op_matrix[i] = 0;
	for (int i = 1; i <= n ; i++)
		op_matrix[i * col_size] = DELETE_OP*i;
	for (int i = 1; i <= m ; i++)
		op_matrix[i] = INSERT_OP*i;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			int state=num_matrix[i*col_size+j];
			if(str1[i-1]==str2[j-1])
			{
				if(state == num_matrix[i*col_size+j-1] + INSERT_COST) 
					op_matrix[i * col_size + j] |= INSERT_OP;
				if(state == num_matrix[(i-1)*col_size+j]+ DELETE_COST) 
					op_matrix[i * col_size + j] |= DELETE_OP;
				if(state == num_matrix[(i-1)*col_size+j-1]) 
					op_matrix[i * col_size + j] |= MATCH_OP;
			}
			else
			{
				if(num_matrix[i*col_size+j]==num_matrix[(i-1)*col_size+j-1]+SUBSTITUTE_COST)
					op_matrix[i*col_size+j]|=SUBSTITUTE_OP;
				if(num_matrix[i*col_size+j]==num_matrix[i*col_size+j-1]+INSERT_COST)
					op_matrix[i*col_size+j]|=INSERT_OP;
				if(num_matrix[i*col_size+j]==num_matrix[(i-1)*col_size+j]+DELETE_COST)
					op_matrix[i*col_size+j]|=DELETE_OP;
				if(num_matrix[i*col_size+j]==num_matrix[(i-2)*col_size+j-2]+TRANSPOSE_COST)
					if(str1[i-1]==str2[j-2]&&str1[i-2]==str2[j-1])
						op_matrix[i*col_size+j]|=TRANSPOSE_OP;
			}
		}
	}
	print_matrix(op_matrix, col_size, str1, str2, n, m);
	backtrace(op_matrix,col_size,str1,str2,n,m);
	return result;
}

void print_alignment( char align_str[][8], int level)
{
	int i;
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8];
	backtrace_main(op_matrix, col_size, str1, str2, n, m, 0, align_str);
}

int main()
{
	char str1[30];
	char str2[30];
	
	int distance;
	
	fprintf( stderr, "INSERT_COST = %d\n", INSERT_COST);
	fprintf( stderr, "DELETE_COST = %d\n", DELETE_COST);
	fprintf( stderr, "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	fprintf( stderr, "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}