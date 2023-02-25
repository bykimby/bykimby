#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_YEAR_DURATION	10	
typedef struct {
	char	name[20];		
	char	sex;			
	int		freq[MAX_YEAR_DURATION]; 
} tName;

typedef struct {
	int		len;		
	int		capacity;	
	tName	*data;		
} tNames;

void load_names( FILE *fp, int start_year, tNames *names);
void print_names( tNames *names, int num_year);
int compare(const void *n1, const void *n2);

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스
int binary_search(const void* key, const void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*));

void load_names(FILE* fp, int start_year, tNames* names)
{
	char name[20];
	char sex;
	int year;
	int freq;
	char temp[40];
	tName temp_name;
	tName* key;
	int index;
	while (fgets(temp, sizeof(temp), fp) != NULL)
	{
		if (names->len == names->capacity)
		{
			names->data = (tName*)realloc(names->data, sizeof(tName) * 2 * (names->capacity));
			(names->capacity) *= 2;
		}
		sscanf(temp, "%d\t%s\t%c\t%d",&year, name, &sex, &freq);
		strcpy(temp_name.name, name);
		temp_name.sex = sex;
		key = bsearch(&temp_name, &(names->data[0]), names->len, sizeof(tName), compare);
		if (key)
		{
			key->freq[year - start_year] = freq;
		}
		else
		{
			index = binary_search(&temp_name, &(names->data[0]), names->len, sizeof(tName), compare);
			memmove(&(names->data[index+1]),&(names->data[index]),sizeof(tName)*(names->len-index));
			memset(names->data[index].freq, 0, 10 * sizeof(int));
			strcpy(names->data[index].name, name);
			names->data[index].sex = sex;
			names->data[index].freq[year-start_year] = freq;
			(names->len)++;
		}
	}
}
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc( sizeof(tNames));
	
	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}
void print_names(tNames* names, int num_year)
{
	for (int i = 0; i < (names->len); i++)
	{
		printf("%s\t%c", names->data[i].name, names->data[i].sex);
		for (int j = 0; j < num_year; j++)
		{
			printf("\t%d", names->data[i].freq[j]);
		}
		printf("\n");
	}
}
int compare(const void* n1, const void* n2)
{
	tName* a = (tName*)n1;
	tName* b = (tName*)n2;
	if (strcmp(a->name, b->name) < 0)
		return -1;
	else if (strcmp(a->name, b->name) > 0)
		return 1;
	else if (strcmp(a->name, b->name) == 0)
	{
		if (a->sex == b->sex)
			return 0;
		else
			return (a->sex) - b->sex;
	}
}

int binary_search(const void* key, const void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*))
{
	int start = 0;
	int end = nmemb - 1;
	int mid;
	int num;
	while (start <= end)
	{
		mid = (start + end) / 2;
		num = compare(key, base + size * mid);
		if (num == 0)
			break;
		else if (num > 0)
			start = mid + 1;
		else
			end = mid - 1;
	}
	return start;
}


int main(int argc, char **argv)
{
	tNames *names;
	FILE *fp;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n\n", argv[0]);
		return 1;
	}

	
	names = create_names();
	
	fp = fopen( argv[1], "r");
	if (!fp)
	{
		fprintf( stderr, "cannot open file : %s\n", argv[1]);
		return 1;
	}

	fprintf( stderr, "Processing [%s]..\n", argv[1]);
	load_names( fp, 2009, names);
	fclose( fp);
	print_names( names, MAX_YEAR_DURATION);
	destroy_names( names);
	
	return 0;
}