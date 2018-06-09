/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Anne Harris
 * Date: 11/12/17
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    // FIXME: implement
	for (int i = 0; i < map->capacity; i++)
	{
		HashLink * tempLink = map->table[i];

		while (tempLink != NULL) {
			HashLink * nextTemp = tempLink->next;
			hashLinkDelete(tempLink);
			tempLink = nextTemp;
			map->size--;
		}
	}
	free(map->table);
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    // FIXME: implement
	int hashIndex = HASH_FUNCTION(key) % map->capacity;
	if (hashIndex < 0)
	{
		hashIndex += map->capacity;
	}

	HashLink *link = map->table[hashIndex];
	//search list
	while (link != NULL)
	{
		//if keys match
		if (strcmp(link->key, key) ==0)
		{
			//return the address of the link's value
			return &(link->value);
		}
		link = link->next;
	}
	//if link is null, there is no match
	return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity. After allocating the new table, all of the links need to be
 * rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    // FIXME: implement
	
	//store old map table and capacity
	HashLink ** oldMap = map->table;
	int oldCapacity = map->capacity;

	//initialize the current map to the new capacity
	hashMapInit(map, capacity);

	HashLink * current;
	//loop through each linked list in oldMap, adding lements to new hash
	for (int i = 0; i < oldCapacity; i++)
	{
		current = oldMap[i];
		while (current != NULL)
		{
			//add the value from old map to new map
			hashMapPut(map, current->key, current->value);
			HashLink * temp = current;
			current = current->next;
			//free link in old map
			hashLinkDelete(temp);
		}
	}
	free(oldMap);
	
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // FIXME: implement
	//find the index
	int hashIndex = HASH_FUNCTION(key) % (map->capacity);
	if (hashIndex < 0)
	{
		hashIndex += map->capacity;
	}

	HashLink * link = map->table[hashIndex]; 
	//table at index is empty
	if (link == NULL) {
		//create new link
		HashLink * newLink = hashLinkNew(key, value, NULL);
		assert(newLink);
		//put link in hash index
		map->table[hashIndex] = newLink;
		//increment size
		map->size++;
	}
	//table at index is not empty
	else {
		//if the key is there
		if(hashMapContainsKey(map, key) == 1) {
			while (link != NULL) {
				if (strcmp(link->key, key) == 0) {
					int *preValue = hashMapGet(map, key);
					*preValue = value;

				}
				link = link->next;
			}
		}
		//key was not found
		else 
		{
			HashLink * newLink = hashLinkNew(key, value, map->table[hashIndex]);
			map->table[hashIndex] = newLink;
			map->size++;
		}
	}

	//check for resize
	float ratio = hashMapTableLoad(map);
	if (ratio >= MAX_TABLE_LOAD)
	{
		resizeTable(map, ((map->capacity) * 2));
	}
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXME: implement
	int hashIndex = HASH_FUNCTION(key) % (map->capacity);
	if (hashIndex < 0)
	{
		hashIndex += map->capacity;
	}

	HashLink *cur = map->table[hashIndex];
	HashLink *prev = map->table[hashIndex];
	if (cur != 0) //bucket has a link
	{
		//check for first link match
		if (strcmp((cur->key), key) == 0)
		{
			map->table[hashIndex] = cur->next;
			hashLinkDelete(cur);
			map->size--;
		}
		else //first link already checked
		{
			cur = cur->next;
			//check all links after first
			while (cur != 0) {
				if (strcmp((cur->key), key) == 0)
				{
					prev->next = cur->next;
					hashLinkDelete(cur);
					map->size--;
					return;
				}
				prev = cur;
				cur = cur->next;
			}
		}

	}
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // FIXME: implement
	int hashIndex = HASH_FUNCTION(key) % (map->capacity);
	if (hashIndex < 0)
	{
		hashIndex += map->capacity;
	}

	//check for key
	HashLink * link = map->table[hashIndex];
	while (link != NULL){
		if (strcmp((link->key), key) == 0){
			return 1;
		}
		link = link->next;
	}
	//key not found
	return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXME: implement
	return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXME: implement
	return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    // FIXME: implement
	//check each bucket for null
	int emptyCount = 0;

	for (int i = 0; i < map->capacity; i++)
	{
		HashLink * temp = map->table[i];
		if (temp == NULL)
		{
			emptyCount++;
		}
	}
    return emptyCount;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    // FIXME: implement
	float ratio = ((float)hashMapSize(map)) / ((float)hashMapCapacity(map));
    return ratio;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    for (int i = 0; i < map->capacity; i++)
    {
        HashLink* link = map->table[i];
        if (link != NULL)
        {
            printf("\nBucket %i ->", i);
            while (link != NULL)
            {
                printf(" (%s, %d) ->", link->key, link->value);
                link = link->next;
            }
        }
    }
    printf("\n");
}
