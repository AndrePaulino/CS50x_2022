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

// Big prime number, big enough to bring down lookup time, but not enough to slow load and unload.
enum
{
    N = 14293
};
unsigned int dict_size = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    for (node *n = table[hash(word)]; n != NULL; n = n->next)
    {
        if (strcmp(word, n->word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Polynomial rolling hash function.
    unsigned int hash_value = 0, sum = 0, p_pow = 1;
    //Prime number close to the number of letters in the alphabet.
    int p = 31;

    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]) * p_pow;
        p_pow = (p_pow * p);
    }
    hash_value = sum % N;

    return hash_value;
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
    node *cursor;
    node *tmp;

    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            cursor = table[i];

            while (cursor != NULL)
            {
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
            }
        }
    }

    return true;
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
