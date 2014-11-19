/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include "string.h"
#include "ctype.h"

#define HASHSIZE 10000
typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
}
node;

node* hash_table[HASHSIZE];
static int word;

int hash(char* buffer, int len)
{
    int hash_num = 0;
    for (int i = 0; i < LENGTH; i++)
    {
        if( buffer[i] == '\0')
            break;
        hash_num += (int)buffer[i];
    }

    if (hash_num%10 == 1)
        hash_num = (hash_num%100)*buffer[0];
    else if (hash_num%10 == 2)
        hash_num = hash_num/2;
    else if (hash_num%10 == 3)
        hash_num = hash_num*5000;
    else if (hash_num%10 == 4)
        hash_num = hash_num/2 + 2017;
    else if (hash_num%10 == 5)
        hash_num = hash_num*105;
   else if (hash_num%10 == 6)
        hash_num = hash_num*hash_num;
    else if (hash_num%10 == 7)
        hash_num = hash_num+50000;
    else if (hash_num%10 == 8)
        hash_num += hash_num/2;
    else if (hash_num%10 == 9)
        hash_num = hash_num*7;
    
    hash_num = hash_num%len;
    
return hash_num;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    
    char buffer[LENGTH+1] = {0};
    
    // make word lower case:
    for(int h = 0; word[h];h++)
    {
        buffer[h] = tolower(word[h]);
    }

    int word_hash = hash(buffer,HASHSIZE);
    node* test_node = hash_table[word_hash];
    
    while(true)
    {
     
        if (strcmp(test_node->word,buffer) == 0)
            return true;
        else if (test_node->next == NULL)
            break;
        else
            {
            test_node = test_node->next;
            }
    }
    return false;
}

/*
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    char buffer[LENGTH+1];
    int hash_num = 0;
    word = 0;
    int keep_reading = 1;
    
    
    
    // try to open dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open file.\n");
        unload();
        return false;
    }
    
    for (int qbert = 0;qbert < HASHSIZE; qbert++)
    {
    node* node_setup = (node*)malloc(sizeof(node));
    strcpy(node_setup->word,"\0");
    node_setup->next = NULL;
    hash_table[qbert] = node_setup;
    }
while(keep_reading == 1)
{    
    // Read a line from the dictionary
    fscanf(fp, "%s",buffer);

    if (feof(fp))
        break;
           
    // make word lower case:
    for(int h = 0; buffer[h];h++)
    {
        buffer[h] = tolower(buffer[h]);
    }
    
    hash_num = hash(buffer,HASHSIZE);
    word +=1;
    node* nody = malloc(sizeof(node));
    strcpy(nody->word,buffer);
    nody->next = NULL;
    node* test_node = hash_table[hash_num];

    while(true)
    {
    if (strcmp(test_node->word,"\0") == 0)
        {
        free(hash_table[hash_num]);
        hash_table[hash_num] = nody;
        break;
        }
    else if (test_node->next == NULL)
        {
            test_node->next = nody;
            break;
         }
    else
        test_node = test_node->next;
    }
    
    
    if (feof(fp))
            keep_reading = 0;

} 


/*     // Hash printing routine
     for (int aaa = 0; aaa < HASHSIZE; aaa++)
     {
        node* holder = hash_table[aaa];
        while(holder->next != NULL)
        {
            printf("%s ---->",holder->word);
            holder = holder->next;
        }
        printf("%s ---->\n",holder->word);
     }
*/     
     
     // close dict
    fclose(fp);       
            
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    return word;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int o = 0; o < HASHSIZE; o++)
    {
        node* free1 = hash_table[o];
        node* free2;
        
        while (free1->next != NULL)
            {
                free2 = free1;
                free1 = free1->next;
                free(free2);
            }
         free(free1); 
     }   
    return true;
}
