#ifndef SOURCE_HASH_TABLE_H
#define SOURCE_HASH_TABLE_H


#define INVALID_HASH_TABLE_ITEM  0
#define INVALID_HASH_TABLE       0
#define INVALID_VALUE            0

static unsigned int hashTableSize = 14000;

typedef struct _HashTableItem
{
    char* key;
    unsigned int value;

    struct _HashTableItem* next;
} HashTableItem;


typedef struct
{
    unsigned size;
    HashTableItem** table;
} HashTable;



/*************************************************************************
 FUNCTION:
    InitHashTable()

 DESCRIPTION:
    Initializes hash table

 PARAMETERS:
    None

 RETURN VALUE:
    Pointer on hash table on success, or INVALID_HASH_TABLE on error.
************************************************************************/
HashTable* InitHashTable();



/*************************************************************************
 FUNCTION:
    DeleteHashTable()

 DESCRIPTION:
    Releases all resources of the hash table

 PARAMETERS:
    hashTable: HashTable* - valid pointer to HashTable returned by InitHashTable()

 RETURN VALUE:
    None
************************************************************************/
void DeleteHashTable(HashTable* hashTable);



/*************************************************************************
 FUNCTION:
    HashTableInsert()

 DESCRIPTION:
    Inserts a new key-value pair into the hash table

 PARAMETERS:
    hashTable: HashTable* - valid pointer to HashTable returned by InitHashTable()
    key: const char* - key of a pair
    value: unsigned int - value of a pair

 RETURN VALUE:
    None
************************************************************************/
void HashTableInsert(HashTable* hashTable, const char* key, unsigned int value);



/*************************************************************************
 FUNCTION:
    HashTableSearch()

 DESCRIPTION:
    Performs key search for the hash table

 PARAMETERS:
    hashTable: HashTable* - valid pointer to HashTable returned by InitHashTable()
    key: const char* - key of a pair

 RETURN VALUE:
    Value for a key on success or INVALID_KEY
************************************************************************/
unsigned int HashTableSearch(HashTable* hashTable, const char* key);



/*************************************************************************
 FUNCTION:
    ToHash()

 DESCRIPTION:
    Counts hash for string(djb2)
    http://www.cse.yorku.ca/~oz/hash.html

 PARAMETERS:
    str: const char* - string

 RETURN VALUE:
    Hash for string
************************************************************************/
static unsigned int ToHash(const char* str);



/*************************************************************************
 FUNCTION:
    MakeItem()

 DESCRIPTION:
    Initialisation function for HashTableItem, allocates memory and copies values

 PARAMETERS:
    key: const char* - key
    value: unsigned int - value

 RETURN VALUE:
    Pointer to HashTableItem
************************************************************************/
static HashTableItem* MakeItem(const char* key, unsigned int value);



/*************************************************************************
 FUNCTION:
    SetSize()

 DESCRIPTION:
    Sets size of hash table

 PARAMETERS:
    size: unsigned int - size of hash table

 RETURN VALUE:
    None
************************************************************************/
void SetSize(unsigned int size);



#endif //SOURCE_HASH_TABLE_H
