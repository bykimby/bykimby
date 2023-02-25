#include <stdio.h>
#include <stdlib.h>
#define PEASANT 0x08 
#define WOLF	0x04 
#define GOAT	0x02
#define CABBAGE	0x01

static int changePC( int state);
static int changeP( int state);
static int changePW( int state);
static int changePG( int state);
static void get_pwgc( int state, int *p, int *w, int *g, int *c);
static void print_statename( FILE *fp, int state)
{
	int p,w,g,c;
	get_pwgc(state,&p,&w,&g,&c);
	fprintf(fp,"\"<%d%d%d%d>\"",p,w,g,c);
}

static void get_pwgc( int state, int *p, int *w, int *g, int *c)
{
	int mark;
	mark=state%2;
	(*c)=mark;
	state/=2;
	mark=state%2;
	(*g)=mark;
	state/=2;
	mark=state%2;
	(*w)=mark;
	state/=2;
	mark=state%2;
	(*p)=mark;
	state/=2;
}

static int is_dead_end( int state)
{
	if(state==3)
		return 1;
	else if(state==6)
		return 1;
	else if(state==7)
		return 1;
	else if(state==8)
		return 1;
	else if(state==9)
		return 1;
	else if(state==12)
		return 1;
	else
		return 0;
}
static int is_possible_transition( int state1,	int state2)
{
	if(is_dead_end(state1)==1||is_dead_end(state2)==1)
		return 0;
	else if(state2==changeP(state1))
		return 1;
	else if(state2==changePW(state1))
		return 1;
	else if(state2==changePC(state1))
		return 1;
	else if(state2==changePG(state1))
		return 1;
	else
		return 0;
}

static int changeP( int state)
{
	int newstate;
	if(state>=8)
		newstate=state-8;
	else
		newstate=state+8;
	return newstate;
}

static int changePW( int state)
{
	int p,w,g,c;
	get_pwgc(state,&p,&w,&g,&c);
	if(p!=w)
		return -1;
	int newstate;
	if(state<8)
		newstate=state+12;
	else
		newstate=state-12;
	return newstate;
}

static int changePG( int state)
{
	int p,w,g,c;
	get_pwgc(state,&p,&w,&g,&c);
	if(p!=g)
		return -1;
	int newstate;
	if(state<8)
		newstate=state+10;
	else
		newstate=state-10;
	return newstate;
}

static int changePC( int state)
{
	int p,w,g,c;
	get_pwgc(state,&p,&w,&g,&c);
	if(p!=c)
		return -1;
	int newstate;
	if(state<8)
		newstate=state+9;
	else
		newstate=state-9;
	return newstate;

}

static int is_visited( int visited[], int depth, int state)
{
	for(int i=0;i<=depth;i++)
	{
		if(visited[i]==state)
			return 1;
	}
	return 0;
}

static void print_path( int visited[],int depth)
{
	int p,w,g,c;
	for(int i=0;i<=depth;i++)
	{
		get_pwgc(visited[i], &p, &w, &g, &c);
		printf("<%d%d%d%d>\n",p,w,g,c);
	}
}

static void dfs_main( int initial_state, int goal_state, int depth, int visited[])
{
	int p,w,g,c;
	int next[4];
	int cur=visited[depth];
	int check=0;
	if(depth==0&&is_visited(visited,depth,goal_state))
		return;
	get_pwgc(cur,&p,&w,&g,&c);
	printf("current state is <%d%d%d%d> (depth %d)\n",p,w,g,c,depth);
	if(cur==goal_state)
	{
		printf("Goal-state found\n");
		print_path(visited,depth);
		printf("\n");
		return;
	}
	next[0]=changeP(cur);
	next[1]=changePW(cur);
	next[2]=changePG(cur);
	next[3]=changePC(cur);
	
	for(int i=0;i<4;i++)
	{
		get_pwgc(next[i],&p,&w,&g,&c);
		if(next[i]==-1)
			continue;
		else if(is_dead_end(next[i]))
		{
			printf("\tnext state <%d%d%d%d> is dead-end\n",p,w,g,c);
		}
		else if(is_visited(visited,depth,next[i]))
		{
			printf("\tnext state <%d%d%d%d> has been visited\n",p,w,g,c);
		}
		else
		{
			depth++;
			visited[depth]=next[i];
			dfs_main(initial_state,goal_state,depth,visited);
			depth--;
			get_pwgc(cur,&p,&w,&g,&c);
			printf("back to <%d%d%d%d> (depth %d)\n",p,w,g,c,depth);
		}
	}
	
}

void make_adjacency_matrix( int graph[][16]) 
{
	int check;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<16;j++)
		{
			check=is_possible_transition(i, j);
			if(check==1)
				graph[i][j]=1;
			else
				graph[i][j]=0;
		}
	}
}

void print_graph( int graph[][16], int num) 
{
	for(int i=0;i<num;i++)
	{
		for(int j=0;j<16;j++)
			printf("%d ",graph[i][j]);
		printf("\n");
	}
}

void save_graph( char *filename, int graph[][16], int num)
{
	FILE *fp=fopen(filename,"w");
	fprintf(fp,"*Vertices %d\n",16);
	for(int i=0;i<16;i++)
	{
		fprintf(fp,"%d ",i+1);
		print_statename(fp,i);
		fprintf(fp,"\n");
	}
	fprintf(fp,"*Edges\n");
	for(int i=0;i<8;i++)
		for(int j=0;j<16;j++)
		{
			if(graph[i][j]==1)
			{
				fprintf(fp,"%d %d\n",i+1,j+1);
			}
		}
	fclose(fp);
}

void depth_first_search( int initial_state, int goal_state)
{
	int depth = 0;
	int visited[16] = {0,}; 
	dfs_main( initial_state, goal_state, depth, visited);
}

int main( int argc, char **argv)
{
	int graph[16][16] = {0,};
	
	make_adjacency_matrix( graph);

	//print_graph( graph, 16);
	
	save_graph( "pwgc.net", graph, 16);

	depth_first_search( 0, 15);
	
	return 0;
}
