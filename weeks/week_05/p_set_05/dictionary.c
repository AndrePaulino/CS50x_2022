// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

void insert(node *n, int hashCode);

// TODO: Choose number of buckets in hash table
const unsigned int N = 187751;
unsigned int dict_size = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    for (node *n = table[hash(word)]; n != NULL; n = n->next)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //By Dr. Rob Edwards from San Diego State University.
    unsigned int hash = 0;
    for (int i = 0; i <= strlen(word); i++)
    {
        hash = (31 * hash + tolower(word[i])) % N;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char tmpWord[LENGTH + 1];
    unsigned int hash_code;

    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    while (fscanf(dict, "%s", tmpWord) == 1)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(dict);
            return false;
        }

        strcpy(n->word, tmpWord);
        n->next = NULL;

        hash_code = hash(tmpWord);
        insert(n, hash_code);

        dict_size++;
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];

        while (n != NULL)
        {
            node *tmpNode = n;
            n = n->next;
            free(tmpNode);
        }

        if (i == N - 1 && n == NULL)
        {
            return true;
        }
    }
    return false;
}

// Insert node into hash table
void insert(node *n, int hashCode)
{
    if (table[hashCode] != NULL)
    {
        n->next = table[hashCode];
        table[hashCode] = n;
    }
    else
    {
        table[hashCode] = n;
    }
}
