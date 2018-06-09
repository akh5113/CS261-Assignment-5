/*
* CS 261 Data Structures
* Assignment 5
* Name: Anne Harris
* Date: 11/18/17
*/

#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
* Calculates the levenshtein distance for the word,
* @param char
* @param char
* @param int
* @return int
* Source: https://rosettacode.org/wiki/Levenshtein_distance#C */
int levenshtein(const char *s, int ls, const char *t, int lt)
{
	int a, b, c;

	/* if either string is empty, difference is inserting all chars
	* from the other
	*/
	if (!ls) return lt;
	if (!lt) return ls;

	/* if last letters are the same, the difference is whatever is
	* required to edit the rest of the strings
	*/
	if (s[ls] == t[ls])
		return levenshtein(s, ls - 1, t, lt - 1);

	/* else try:
	*      changing last letter of s to that of t; or
	*      remove last letter of s; or
	*      remove last letter of t,
	* any of which is 1 edit plus editing the rest of the strings
	*/
	a = levenshtein(s, ls - 1, t, lt - 1);
	b = levenshtein(s, ls, t, lt - 1);
	c = levenshtein(s, ls - 1, t, lt);

	if (a > b) a = b;
	if (a > c) a = c;

	return a + 1;
}



/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXME: implement
	char* word = nextWord(file);

	while (word != NULL)
	{
		hashMapPut(map, word, 1);
		char *temp = word;
		word = nextWord(file);
		free(temp);
	}
	free(word);
}

int getSuggestedWords(HashMap* map, const char* inputWord)
{
	//array to hold suggested keys
	char * suggs[6];
	//count for number of suggested words in array
	int count = 0;
	//loop through dynamic array
	for (int i = 0; i < map->capacity; i++)
	{
		HashLink *current = map->table[i];
		//loop through linked list while the suggested array isn't full
		while (current != NULL && count < 6) {
			//calculate levenshtein distance
			int levDist = levenshtein(current->key, strlen(current->key), inputWord, strlen(inputWord));
			if (levDist < 2)
			{
				//add to suggestion array
				suggs[count] = (current->key);
				count++;
			}
			//advance through the list
			current = current->next;
		}
	}

	//PRINT SUGGESTIONS
	int answer = 2;
	//loop through suggestion array
	for (int j = 0; j < 6; j++)
	{
		//if there is a suggestion
		if (suggs[j] != NULL)
		{
			//print the suggestion
			printf("Did you mean: %s\n", suggs[j]);
			printf("Enter 1 for YES or 2 for NO\n");
			scanf("%d", &answer);
			if (answer == 1) //yes
			{
				printf("Done!");
				return 1;
			}
		}
		else
		{
			printf("No more suggestions\n");
			return 0;
		}

	}
	return 0;
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
    HashMap* map = hashMapNew(1000);
    
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        
        // Implement the spell checker code here..
		//check to see if word is in dictionary
		if (hashMapContainsKey(map, inputBuffer) == 0) //not found
		{
			//if the word is not found, call function to get suggestions
			int found = getSuggestedWords(map, inputBuffer);
			//if the word was found, display found it message
			if (found == 1)
			{
				printf("Found it!\n");
				quit = 1;
			}
			//word was not found, display sorry message
			else
			{
				printf("Sorry, word not found\n");
				quit = 1;
			}
		}

        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
    }
    
    hashMapDelete(map);
    return 0;
}