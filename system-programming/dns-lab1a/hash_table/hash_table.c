#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"

HashTable* InitHashTable()
{
    HashTable* hashTable = (HashTable*)malloc(sizeof(hashTable));
    if (hashTable != NULL)
    {
        hashTable->size = hashTableSize;

        hashTable->table = (HashTableItem**)calloc(hashTable->size, sizeof(HashTableItem*));
        if (hashTable->table != NULL)
        {
            return hashTable;
        }

        free(hashTable);
    }

    return INVALID_HASH_TABLE;
}

void DeleteHashTable(HashTable* hashTable)
{
    for (unsigned int i = 0; i < hashTable->size; i++)
    {
        if (hashTable->table[i] != NULL)
        {
            HashTableItem* temp1 = hashTable->table[i];
            while (temp1 != NULL)
            {
                HashTableItem* temp2 = temp1->next;
                free(temp1->key);
                free(temp1);
                temp1 = temp2;
            }
        }
    }

    free(hashTable);
}

void HashTableInsert(HashTable* hashTable, const char* key, unsigned int value)
{
    HashTableItem* hashTableItem = MakeItem(key, value);
    if (hashTableItem != INVALID_HASH_TABLE_ITEM)
    {
        unsigned int hash = ToHash(key);

        if (hashTable->table[hash] != NULL)
        {
            HashTableItem* temp = hashTable->table[hash];
            while (temp->next != NULL)
            {
                temp = temp->next;
            }

            temp->next = hashTableItem;
        }
        else
        {
            hashTable->table[hash] = hashTableItem;
        }
    }

}

unsigned int HashTableSearch(HashTable* hashTable, const char* key)
{
    unsigned int hash = ToHash(key);

    HashTableItem* hashTableItem = hashTable->table[hash];
    if (hashTableItem != NULL)
    {
        if (strcmp(hashTableItem->key, key) == 0)
            return hashTableItem->value;

        HashTableItem* temp = hashTableItem->next;
        while (temp != NULL)
        {
            if (strcmp(temp->key,key) == 0)
                return temp->value;

            temp = temp->next;
        }
    }

    return INVALID_VALUE;
}

unsigned int ToHash(const char* str)
{
    unsigned long hash = 5381;
    int length = strlen(str);

    for (int i = 0; i < length; i++)
    {
        int c = str[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % hashTableSize;
}

static HashTableItem* MakeItem(const char* key, unsigned int value)
{
    HashTableItem* hashTableItem = (HashTableItem*)malloc(sizeof(HashTableItem));

    if (hashTableItem != NULL)
    {
        hashTableItem->key = strdup(key);
        if (hashTableItem->key != NULL)
        {
            hashTableItem->value = value;
            hashTableItem->next = NULL;

            return hashTableItem;
        }

        free(hashTableItem);
    }

    return INVALID_HASH_TABLE_ITEM;
}

void SetSize(unsigned int size)
{
    hashTableSize = size;
}