// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];
unsigned int numberOfDictionaryEntries = 0;


// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        numberOfDictionaryEntries++;
        int bucketIndex = hash(word);

        node *newNode = malloc(sizeof(node));

        // check malloc success
        if (newNode == NULL)
        {
            unload();
            return false;
        }

        // put the new node in the head of the bucket
        strcpy(newNode->word, word);

        if (hashtable[bucketIndex] != NULL)
        {
            newNode->next = hashtable[bucketIndex];
        }
        else {
            newNode->next = NULL;
        }

        hashtable[bucketIndex] = newNode;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return numberOfDictionaryEntries;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lowercaseWord[LENGTH + 1];
    strcpy(lowercaseWord, word);

     for (int i = 0, len = strlen(word); i < len; i++)
     {
        lowercaseWord[i] = tolower(lowercaseWord[i]);
     }


    int bucketIndex = hash(word);
    node *currNode = hashtable[bucketIndex];

    while (currNode != NULL)
    {
        if (strcmp(currNode->word, lowercaseWord) == 0)
        {
            return true;
            break;
        }

        currNode = currNode->next;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}

