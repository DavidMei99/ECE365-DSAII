//
// ECE-365 Fall 2019 Assignment #2
// Di Mei
// hash.cpp
//

#include "hash.h"

/**
 precomputed sequence of selected prime numbers:
 becasue an average dictionary contains 50000 words, rehash table size will
 be a prime number at least as twice as 50000.
*/
int prime_array[] = {11, 100003, 200003, 400009, 800011, 1600033, 3200003};

// initialize a hashTable
hashTable::hashTable(int size)
{
    capacity = getPrime(size);
    filled = 0;
    data.resize(capacity);
    for (int i = 0; i < data.size(); i++)
    {
        data[i].isOccupied = false;
        data[i].isDeleted = false;
    }
}

// hash function sdbm
// reference: http://www.cse.yorku.ca/~oz/hash.html
int hashTable::hash(const std::string &key)
{
    unsigned int hash_val = 0;
    unsigned long key_len = key.length();
    for (int i = 0; i < key_len; i++) {
        hash_val = (key[i]) + (hash_val << 6) + (hash_val << 16) - hash_val;
    }
    return hash_val % capacity;
}

// insert a specified key into the hashtable
int hashTable::insert(const std::string &key, void *pv)
{
    int pos = hash(key);
    if (contains(key)) return 1;
    if (2 * filled >= capacity) {
        if (rehash() == false) return 2;
    }
    // collision resolution: linear probing
    while (data[pos].isOccupied) {
        pos++;
        if (pos == capacity) pos = 0;
    }
    data[pos].key = key;
    data[pos].isOccupied = true;
    data[pos].isDeleted = false;
    data[pos].pv = pv;
    filled++;
    return 0;
}

// the rehash function makes hashtable bigger
bool hashTable::rehash()
{
    std::vector<hashItem> temp = data;
    int new_size = getPrime(capacity);
    try {
        data.resize(new_size);
    }
    catch (std::bad_alloc) {
        return false;
    }
    capacity = new_size;
    filled = 0;
    for (int i = 0; i < data.size(); i++) {
        data[i].key = "";
        data[i].isOccupied = false;
        data[i].isDeleted = false;
    }
    for (int i = 0; i < temp.size(); i++) {
        if (temp[i].isOccupied && !temp[i].isDeleted) {
            insert(temp[i].key, temp[i].pv);
        }
    }
    return true;
}

// search for an item with the specified key
int hashTable::findPos(const std::string &key)
{
    int pos = hash(key);
    while (data[pos].isOccupied) {
        if (data[pos].key == key && data[pos].isDeleted == false) {
            return pos;
        }
        else {
            pos++;
            if (pos == capacity) pos = 0;
        }
    }
    return -1;
}

// generate a prime twice as large as the initial hashtable size
unsigned int hashTable::getPrime(int size)
{
    unsigned int hash_size = 0;
    for (int i = 0; i < sizeof(prime_array); i++) {
        if ((hash_size = prime_array[i]) > 2 * size) {
            break;
        }
    }
    return hash_size;
}

// check if the specified key is in the hashtable
bool hashTable::contains(const std::string &key)
{
    if (findPos(key) < 0) return false;
    return true;
}

// get the pointer associated with the specified key
void *hashTable::getPointer(const std::string &key, bool *b)
{
    int pos = findPos(key);
    if (contains(key)) {
        if (b != NULL) *b = true;
        return data[pos].pv;
    }
    else {
        if (b != NULL) *b = false;
        return NULL;
    }
}

// set the pointer associated with the specified key
int hashTable::setPointer(const std::string &key, void *pv)
{
    int pos = findPos(key);
    if (contains(key)) {
        data[pos].pv = pv;
        return 0;
    }
    return 1;
}

// lazily delete the item with the specified key
bool hashTable::remove(const std::string &key)
{
    int pos = findPos(key);
    if (contains(key)) {
        data[pos].isDeleted = true;
        return true;
    }
    return false;
}
