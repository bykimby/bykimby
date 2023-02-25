#include <stdlib.h> 
#include <stdio.h>
#include <assert.h> 
#include <time.h> 

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;
float distance( float a, float b, float c, t_point p);
void separate_points( t_point *points, int num_point, t_point from, t_point to, t_point *s1, t_point *s2, int *n1, int *n2);
t_line *upper_hull( t_point *points, int num_point, t_point p1, t_point pn, t_line *lines, int *num_line, int *capacity)
{
	t_point maxp;
	float max=0;
	int n1,n2;
	if((*capacity)==(*num_line))
	{
		(*capacity)+=10;
		lines=(t_line*)realloc(lines,sizeof(t_line)*(*capacity));
	}
	if(!num_point)
	{
		lines[(*num_line)].from=p1;
		lines[(*num_line)].to=pn;
		(*num_line)++;
		return lines;
	}
	for(int i=0;i<num_point;i++)
	{
		if(distance(pn.y-p1.y,p1.x-pn.x,p1.x*pn.y-pn.x*p1.y,points[i])>max)
		{
			max=distance(pn.y-p1.y,p1.x-pn.x,p1.x*pn.y-pn.x*p1.y,points[i]);
			maxp=points[i];
		}
	}
	t_point *s1 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s1 != NULL);
	t_point *s2 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s2 != NULL);
	separate_points( points, num_point, p1, maxp, s1, s2, &n1, &n2);
	lines=upper_hull( s1, n1, p1, maxp, lines, num_line, capacity);
	separate_points( points, num_point, maxp, pn, s1, s2, &n1, &n2);
	lines=upper_hull( s1, n1, maxp, pn, lines, num_line, capacity);
	free(s1);
	free(s2);
	return lines;
}	


float distance( float a, float b, float c, t_point p)
{
	float dist;
	dist=a*(p.x)+b*(p.y)-c;
	if(dist<0)
		return -dist;
	else
		return dist;
}

void separate_points( t_point *points, int num_point, t_point from, t_point to, t_point *s1, t_point *s2, int *n1, int *n2)
{
	
	int post=0;
	(*n1)=0;
	(*n2)=0;
	for(int i=0;i<num_point;i++)
	{
		post=(to.y-from.y)*points[i].x+(from.x-to.x)*points[i].y-(from.x*to.y-from.y*to.x);
		if(post<0)
		{
			s1[(*n1)]=points[i];
			(*n1)++;
		}
		else if(post>0)
		{
			s2[(*n2)]=points[i];
			(*n2)++;
		}
	}
}

void print_header(char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}

void print_footer(void)
{
	printf( "dev.off()\n");
}

int cmp_x( const void *p1, const void *p2)
{
	t_point *p = (t_point *)p1;
	t_point *q = (t_point *)p2;
	
	return p->x - q->x;
}

void print_points( t_point *points, int num_point)
{
	int i;
	printf( "\n#points\n");
	
	for (i = 0; i < num_point; i++)
		printf( "points(%d,%d)\n", points[i].x, points[i].y);
}


void print_line_segments( t_line *lines, int num_line)
{
	int i;

	printf( "\n#line segments\n");
	
	for (i = 0; i < num_line; i++)
		printf( "segments(%d,%d,%d,%d)\n", lines[i].from.x, lines[i].from.y, lines[i].to.x, lines[i].to.y);
}


t_line *convex_hull( t_point *points, int num_point, int *num_line)
{
	int capacity = 10;
	t_line *lines = (t_line *) malloc( capacity * sizeof(t_line));
	(*num_line) = 0;

	t_point *s1 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s1 != NULL);

	t_point *s2 = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( s2 != NULL);
	int n1, n2;
	separate_points( points, num_point, points[0], points[num_point-1], s1, s2, &n1, &n2);
	lines = upper_hull( s1, n1, points[0], points[num_point-1], lines, num_line, &capacity);
	lines = upper_hull( s2, n2, points[num_point-1], points[0], lines, num_line, &capacity);
	free( s1);
	free( s2);
	return lines;
}


int main( int argc, char **argv)
{
	float x, y;
	int num_point; 
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}

	num_point = atoi( argv[1]);
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	t_point *points;
	points = (t_point *)malloc( sizeof(t_point) * num_point);
	assert( points != NULL);
	
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; 
		y = rand() % RANGE + 1;
	
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);
	
	qsort( points, num_point, sizeof(t_point), cmp_x);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
	int num_line;
	t_line *lines = convex_hull( points, num_point, &num_line);
	
	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
	
	print_footer();
	
	free( points);
	free( lines);
	
	return 0;
}