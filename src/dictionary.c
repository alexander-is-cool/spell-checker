// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../header/dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;
unsigned int num_words = 0;

// Frees memory being used by a trie
bool unload_trie(node *cursor);

// Hashes a letter to a number between 0 and 26, inclusive, based on its first letter
unsigned int hash(const char letter)
{
    if (letter == '\'')
    {
        return 26;
    }

    return tolower(letter) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
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

    // Bucket that each letter in word belongs in
    int bucket;

    // Cursor for traversing the tree
    node *cursor = NULL;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        cursor = root;
        // Iterates over each character in the trie
        for (int i = 0, len = strlen(word); i < len; i++)
        {
            // Bucket that the letter in word belongs in
            bucket = hash(word[i]);

            //printf("bucket: %i, NumWord: %i, Word: %s, Character: %c\n", bucket, num_words, word, word[i]);

            // Checks if the bucket is empty
            if (cursor -> children[bucket] == NULL)
            {
                // Allocates memory for new node in the trie
                cursor -> children[bucket] = malloc(sizeof(node));
                if (cursor -> children[bucket] == NULL)
                {
                    return false;
                }

                // Initializes new node
                cursor -> children[bucket] -> is_word = false;
                for (int j = 0; j < N; j++)
                {
                    cursor -> children[bucket] -> children[j] = NULL;
                }
            }

            // Move down the tree
            cursor = cursor -> children[bucket];
        }
        // Sets is word to true
        cursor -> is_word = true;

        // Increase the number of words in the list
        num_words ++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return num_words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Bucket that each letter in word belongs in
    int bucket;

    // Cursor for traversing the tree
    node *cursor = root;

    // Iterates over each character in word
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        // Bucket that the letter in word belongs in
        bucket = hash(word[i]);

        // Checks if the bucket is empty
        if (cursor -> children[bucket] == NULL)
        {
            return false;
        }

        // Move down the tree
        cursor = cursor -> children[bucket];
    }

    // Checks if the word is infact a word
    if (cursor -> is_word == true)
    {
        return true;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    return unload_trie(root);
}

// Frees memory being used by a trie
bool unload_trie(node *cursor)
{
    // Iterates over nodes of the children of the cursor
    for (int i = 0; i < N; i++)
    {
        // Checks if there is a child
        if (cursor -> children[i] != NULL)
        {
            // Goes to child
            unload_trie(cursor -> children[i]);
        }
    }

    // Frees cursor
    free(cursor);

    return true;
}