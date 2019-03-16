#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define CAPACITY 100
#define BUFFER_SIZE 1000

typedef struct english {
	char *word;
	char *part;
	char *meaning;
}English;

English **directory;

int capacity;
int n;

void reallocate();
void read();
void add(char *word, char *part, char *meaning);
int find(English **directory, char *target, int begin, int end);

void init() {
	directory = (English**)malloc(CAPACITY * sizeof(English*));
	capacity = CAPACITY;
	n = 0;
}

int main() {

	init();
	char command[CAPACITY];

	while (1) {
		printf("$ ");
		scanf("%s", command);
		if (strcmp(command, "read") == 0)
			read();
		else if (strcmp(command, "size") == 0)
			printf("%d\n", n);
		else if (strcmp(command, "find") == 0)
		{
			int result;
			char words[CAPACITY];
			scanf("%s", words);

			result = find(directory, words, 0, n - 1);

			if (stricmp(words, directory[result]->word) == 0)
			{
				int item[BUFFER_SIZE];
				int count = 1;
				int k = 0;
				item[k] = result;
				k++;

				for (int i = result - 1; i > 0; i--)
				{
					if (stricmp(words, directory[i]->word) == 0)
					{
						count++;
						item[k] = i;
						k++;
					}
					else
						break;

				}

				for (int i = result + 1; i < n - 1; i++)
				{
					if (stricmp(words, directory[i]->word) == 0)
					{
						count++;
						item[k] = i;
						k++;
					}
					else
						break;

				}

				printf("Found %d items.\n", count);

				for (int i = 0; i < count; i++)
				{
					printf("%s (%s) %s\n", directory[item[i]]->word
						, directory[item[i]]->part, directory[item[i]]->meaning);
				}


			}
			else
			{
				printf("Not found.\n");
				printf("%s (%s)\n", directory[result]->word,
					directory[result]->part);
				printf("- - -\n");
				printf("%s (%s)\n", directory[result + 1]->word,
					directory[result + 1]->part);
			}
		}

		else if (strcmp(command, "exit") == 0)
			break;

	}

}

void add(char *word, char *part, char *meaning)
{
	if (n >= capacity)
		reallocate();

	directory[n] = (English*)malloc(sizeof(English));

	char word2[BUFFER_SIZE];
	int i;

	for (i = 0; i < strlen(word) - 1; i++)
	{
		word2[i] = word[i];
	}
	word2[i] = NULL;
	
	directory[n]->word = strdup(word2);
	directory[n]->part = strdup(part);
	directory[n]->meaning = strdup(meaning);


	n++;
}

void reallocate() {

	capacity *= 2;
	English**tmp = (English**)malloc((capacity * sizeof(English*)));
	for (int i = 0; i<n; i++)
		tmp[i] = directory[i];
	free(directory);
	directory = tmp;
}


void read() {

	char fileName[CAPACITY];
	char buffer[BUFFER_SIZE];
	char *word, *part, *meaning;

	scanf("%s", fileName);

	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) {
		printf("Open failed.\n");
		return;
	}

	while (!feof(fp)) 
	{
		fgets(buffer, BUFFER_SIZE, fp);

		if (strlen(buffer) > 1)
		{
			word = strtok(buffer, "(");
			part = strtok(NULL, ")");
			meaning = strtok(NULL, "\n");
			add(word, part, meaning);
		}

	}
	fclose(fp);
}

int find(English **directory, char *target, int begin, int end) {
	if (begin > end)
	{
		if (end >= 0 && end < n)
			return end;
		else
			return -1;
	}
	else {
		int middle = (begin + end) / 2;

		if (stricmp(directory[middle]->word, target) < 0)
			return  find(directory, target, middle + 1, end);
		
		else if (stricmp(directory[middle]->word, target) > 0)
			return find(directory, target, begin, middle-1);
		
		else
			return middle;
	}
}