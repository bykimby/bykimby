#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	
#define LINEAR_SEARCH 0
#define BINARY_SEARCH 1

typedef struct {
	char	name[20];		
	char	sex;			
	int		freq[MAX_YEAR_DURATION];
} tName;

typedef struct {
	int		len;		
	int		capacity;	
	tName* data;		
} tNames;

void load_names_lsearch(FILE* fp, int year_index, tNames* names);
void load_names_bsearch(FILE* fp, int year_index, tNames* names);
void print_names(tNames* names, int num_year);
int compare(const void* n1, const void* n2);

void load_names_lsearch(FILE* fp, int year_index, tNames* names) 
{
	char name[20];
	char sex;
	int freq;
	char temp[30];
	int mark;
	if (year_index == 0)
	{
		while (fgets(temp, sizeof(temp), fp) != NULL)
		{
			if (names->len == names->capacity)
			{
				names->data = (tName*)realloc(names->data, sizeof(tName) * 2*(names->capacity));
				(names->capacity) *= 2;
				for (int j = names->len; j < names->capacity; j++)
				{
					memset(names->data[j].freq, 0, 10 * sizeof(int));
				}
			}
			sscanf(temp, "%[^','],%[^','],%d", name, &sex, &freq);
			strcpy(names->data[names->len].name, name);
			names->data[names->len].sex = sex;
			names->data[names->len].freq[year_index] = freq;
			(names->len)++;
		}
	}
	else
	{
		while (fgets(temp, sizeof(temp), fp) != NULL)
		{
			if (names->len == names->capacity)
			{
				names->data = (tName*)realloc(names->data, sizeof(tName) * 2 * (names->capacity));
				(names->capacity) *= 2;
				for (int i = names->len; i < names->capacity; i++)
				{
					memset(names->data[i].freq, 0, MAX_YEAR_DURATION * sizeof(int));
				}
			}
			sscanf(temp, "%[^','],%[^','],%d", name, &sex, &freq);
			mark = 0;
			for (int i = 0; i < names->len; i++)
			{
				if (strcmp(name, names->data[i].name)==0&& names->data[i].sex == sex)
				{	
						names->data[i].freq[year_index] = freq;
						mark=1;
						break;
				}
			}
			if (mark == 0)
			{
				strcpy(names->data[names->len].name,name);
				names->data[names->len].sex = sex;
				names->data[names->len].freq[year_index] =freq;
				(names->len)++;
			}
		}
	}
}


void load_names_bsearch(FILE* fp, int year_index, tNames* names)
{
	char name[20];
	char sex;
	int freq;
	char temp[30];
	tName *mark;
	tName temp_name;
	int len;
	if (year_index == 0)
	{
		while (fgets(temp, sizeof(temp), fp) != NULL)
		{
			if (names->len == names->capacity)
			{
				names->data = (tName*)realloc(names->data, sizeof(tName) * 2 * (names->capacity));
				(names->capacity) *= 2;
				for (int j = names->len; j < names->capacity; j++)
				{
					memset(names->data[j].freq, 0, 10 * sizeof(int));
				}
			}
			sscanf(temp, "%[^','],%[^','],%d", name, &sex, &freq);
			strcpy(names->data[names->len].name, name);
			names->data[names->len].sex = sex;
			names->data[names->len].freq[year_index] = freq;
			(names->len)++;
		}
	}
	else
	{
		len = names->len;
		while (fgets(temp, sizeof(temp), fp) != NULL)
		{
			if (names->len == names->capacity)
			{
				names->data = (tName*)realloc(names->data, sizeof(tName) * 2 * (names->capacity));
				(names->capacity) *= 2;
				for (int i = names->len; i < names->capacity; i++)
				{
					memset(names->data[i].freq, 0, MAX_YEAR_DURATION * sizeof(int));
				}
			}
			sscanf(temp, "%[^','],%[^','],%d", name, &sex, &freq);
			strcpy(temp_name.name, name);
			temp_name.sex = sex;
			temp_name.freq[0] = freq;
			mark=bsearch(&temp_name,&(names->data[0]),len,sizeof(tName), compare);
			if (mark)
			{
				mark->freq[year_index] = freq;
			}
			else
			{
				strcpy(names->data[names->len].name, name);
				names->data[names->len].sex = sex;
				names->data[names->len].freq[year_index] = freq;
				(names->len)++;
			}

		}
	}
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
	if (strcmp(a->name,b->name)<0)
		return -1;
	else if (strcmp(a->name , b->name)>0)
		return 1;
	else if (strcmp(a->name, b->name) == 0)
	{
		if (a->sex == b->sex)
			return 0;
		else
			return (a->sex)-b->sex;
	}
}

tNames* create_names(void)
{
	tNames* pnames = (tNames*)malloc(sizeof(tNames));

	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName*)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

void destroy_names(tNames* pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}

int main(int argc, char** argv)
{
	tNames* names;
	int option;

	FILE* fp;
	int num_year = 0;

	if (argc <= 2)
	{
		fprintf(stderr, "Usage: %s option FILE...\n\n", argv[0]);
		fprintf(stderr, "option\n\t-l\n\t\twith linear search\n\t-b\n\t\twith binary search\n");
		return 1;
	}

	if (strcmp(argv[1], "-l") == 0) option = LINEAR_SEARCH;
	else if (strcmp(argv[1], "-b") == 0) option = BINARY_SEARCH;
	else {
		fprintf(stderr, "unknown option : %s\n", argv[1]);
		return 1;
	}

	names = create_names();

	int start_year = atoi(&argv[2][strlen(argv[2]) - 8]);

	for (int i = 2; i < argc; i++)
	{
		num_year++;
		fp = fopen(argv[i], "r");
		if (!fp) {
			fprintf(stderr, "cannot open file : %s\n", argv[i]);
			return 1;
		}

		int year = atoi(&argv[i][strlen(argv[i]) - 8]); 

		fprintf(stderr, "Processing [%s]..\n", argv[i]);

		if (option == LINEAR_SEARCH)
		{
			load_names_lsearch( fp, year-start_year, names);
		}
		else
		{
			load_names_bsearch( fp, year-start_year, names);
			qsort(names->data, names->len, sizeof(tName), compare);
		}
		fclose(fp);

	}

	qsort(names->data, names->len, sizeof(tName), compare);
	print_names( names, num_year);
	destroy_names(names);
	return 0;
}