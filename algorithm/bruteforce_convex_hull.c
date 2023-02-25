#include <stdlib.h> 
#include <stdio.h>
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

long long line_func(int x1, int y1, int x2, int y2, int x, int y);
void print_header(char* filename);
void print_footer(void);
void print_points(t_point* points, int num_point);
void print_line_segments(t_line* lines, int num_line);
t_line* convex_hull(t_point* points, int num_point, int* num_line);
long long line_func(int x1, int y1, int x2, int y2, int x, int y)
{
	long long result;
	result = (y2 - y1) * x + (x1 - x2) * y - (x1 * y2 - y1 * x2);
	return result;
}

void print_header( char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
void print_footer( void)
{
	printf( "dev.off()\n");
}

void print_points(t_point* points, int num_point)
{
	printf("#points\n");
	for (int i = 0; i < num_point; i++)
		printf("points(%d,%d)\n", points[i].x, points[i].y);
}

void print_line_segments(t_line* lines, int num_line)
{
	printf("#line segments\n");
	for (int i = 0; i < num_line; i++)
		printf("segments(%d,%d,%d,%d)\n", lines[i].from.x,lines[i].from.y,lines[i].to.x,lines[i].to.y);
}

t_line* convex_hull(t_point* points, int num_point, int* num_line)
{
	t_line* line;
	line = (t_line*)malloc(sizeof(t_line));
	long long num; 
	int plus,minus;
	for (int i = 0; i < num_point-1; i++)
	{
		for (int j = i+1; j < num_point; j++)
		{
			plus = 0;
			minus = 0;
			for (int p = 0; p < num_point; p++)
			{
				num=line_func(points[i].x, points[i].y, points[j].x, points[j].y, points[p].x, points[p].y);
				if (num > 0)
				{
					plus++;
				}
				else if (num < 0)
				{
					minus++;
				}
			}
			if (plus*minus==0)
			{
				(*num_line)++;
				line = (t_line*)realloc(line, sizeof(t_line)*(*num_line));
				line[(*num_line)-1].from.x = points[i].x;
				line[(*num_line)-1].from.y= points[i].y;
				line[(*num_line)-1].to.x= points[j].x;
				line[(*num_line)-1].to.y = points[j].y;
			}
		}
	}
	return line;
}

int main( int argc, char **argv)
{
	int x, y;
	int num_point; 
	int num_line=0; 
	
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

	t_point *points = (t_point *) malloc( num_point * sizeof( t_point));
		
	t_line *lines;

	
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1;
		y = rand() % RANGE + 1;
		
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
	lines = convex_hull( points, num_point, &num_line);

	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
	
	print_footer();
	
	free( points);
	free( lines);
	
	return 0;
}

