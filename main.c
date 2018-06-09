/*
* CS 261 Data Structures
* Assignment 5
* Name: Anne Harris
* Date: 11/18/17
*/

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    const char* fileName = "input1.txt";
    if (argc > 1)
    {
        fileName = argv[1];
    }
    printf("Opening file: %s\n", fileName);
    
    clock_t timer = clock();
    
    HashMap* map = hashMapNew(10);
    
    // --- Concordance code begins here ---
    // Be sure to free the word after you are done with it here.
	/* get next word with nextWord
		if the word is already in the map increment its number of occurrences
		otherwise put the work int the map
		free the word*/
	//open the file
	FILE * theFile = fopen(fileName, "r");
	assert(theFile != NULL);

	//save the word
	char* word = nextWord(theFile);
	//step through each word in file
	while (word != NULL)
	{
		char*temp = word;
		if (hashMapContainsKey(map, word) == 1)
		{
			//get the current number of occurances
			int* occurances = hashMapGet(map, word);
			//increase that by 1
			*occurances += 1;
			//advance the word
			temp = word;
			word = nextWord(theFile);
			free(temp);
				
		}
		else
		{
			//put the word in the hash map with count of 1
			hashMapPut(map, word, 1);
			//advance the word
			temp = word;
			word = nextWord(theFile);
			free(temp);
		}
	}

	//free the word
	free(word);

	//close the file
	fclose(theFile);

	//print the word count
	for (int i = 0; i < map->capacity; i++)
	{
		HashLink* link = map->table[i];
		if (link != NULL)
		{
			while (link != NULL)
			{
				printf("%s: %d\n", link->key, link->value);
				link = link->next;
			}
		}
	}
	printf("\n");

    // --- Concordance code ends here ---
    
    hashMapPrint(map);
    
    timer = clock() - timer;
    printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n", hashMapTableLoad(map));
    
    hashMapDelete(map);
    return 0;
}