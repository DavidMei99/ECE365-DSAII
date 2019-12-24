//
// ECE-365 Fall 2019 Assignment #2
// Di Mei
// heap.cpp
//

#include "heap.h"

// constructor: initialize a heap
heap::heap(int capacity)
{
    data.resize(capacity + 1);
    mapping = hashTable(capacity * 2);
    this->capacity = capacity; // set the capacity of the heap
    currsize = 0;
}

// return node's position in the vector data
int heap::getPos(node *ptr_n)
{
    return int(ptr_n - &data[0]);
}

// insert a new node into the binary heap
// start from heap's last node and percolate up
int heap::insert(const std::string &id, int key, void *pv)
{

    if (currsize < capacity) {
        if (mapping.contains(id)) {
            return 2; // return 2 if a node with the given id already exists
        }
        else {
            currsize++;
            data[currsize].id = id;
            data[currsize].key = key;
            data[currsize].pData = pv;
            mapping.insert(id, &data[currsize]);
            percolateUp(currsize); // use percolate up to ensure heap order
            return 0;
        }
    }
    else {
        return 1; // return 1 if the heap is already filled to capacity
    }
}

// set the key of the specified node to the specified value
int heap::setKey(const std::string &id, int key)
{
    if (!(mapping.contains(id))) return 1; // return 1 if a node with the given id does not exist

    node *ptr_n = static_cast<node *>(mapping.getPointer(id)); // pointer pointing to new id

    int old_key, pos;
    old_key = ptr_n->key;
    ptr_n->key = key;

    pos = getPos(ptr_n); // get the current node position
    if (key > old_key) {
        percolateDown(pos);
    }
    else if (key < old_key) {
        percolateUp(pos);
    }
    return 0;
}

// return the data associated with the smallest key and delete that node from the binary heap
int heap::deleteMin(std::string *pId, int *pKey, void *ppData)
{
    if (currsize == 0) return 1; // return 1 if the heap is empty

    if (pId != NULL) *pId = data[1].id;
    // If pKey is supplied, write to that address the key of the node deleted
    if (pKey != NULL) *pKey = data[1].key;
    // If ppData is supplied, write to that address the associated void pointer
    if (ppData != NULL) *(static_cast<void **>(ppData)) = data[1].pData;

    // to delete the topmost node, remove it and percolate down to keep heap order
    mapping.remove(data[1].id);
    data[1] = data[currsize--];
    percolateDown(1);
    return 0;
}

// delete the node with the specified id from the binary heap
int heap::remove(const std::string &id, int *pKey, void *ppData)
{
    // Check if node with given id exists
    if (mapping.contains(id)) {
        node *ptr_n = static_cast<node *> (mapping.getPointer(id));     // Reference to specified node
        
        int old_key = ptr_n -> key;
        // if pKey is supplied, write to that address the key of the node being deleted
        if (pKey != NULL) *pKey = ptr_n -> key;
        // if ppData is supplied, write to that address the associated void pointer.
        if (ppData != NULL) *(static_cast<void **> (ppData)) = ptr_n->pData;
        
        // delete item with specified id from hashtable
        mapping.remove(id);
        *ptr_n = data[currsize--];
        int new_key = ptr_n->key;
        int pos = getPos(ptr_n);
        
        // maintain heap order
        if (new_key < old_key) {
            percolateUp(pos);
        }
        else if (new_key > old_key) {
            percolateDown(pos);
        }
        return 0;
    }
    else {
        return 1;
    }
}

// start from posCur in a heap,
// the new element is percolated up the heap until the correct location is found
void heap::percolateUp(int posCur)
{
    node temp = data[posCur];
    for (; posCur > 1 && temp.key < data[posCur/2].key; posCur /= 2) {
        data[posCur] = data[posCur/2];
        mapping.setPointer(data[posCur].id, &data[posCur]); // remap in hashtable
    }
    data[posCur] = temp;
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

// start from posCur in a heap,
// the new element is percolated down the heap until the correct location is found
// implemented based on Figure 6.12 in textbook
void heap::percolateDown(int posCur)
{
    node temp = data[posCur];
    int child, left, right;
    // be careful about a parent node with only one child
    for (; (posCur * 2) <= currsize; posCur = child) {
        left = posCur * 2;
        right = left + 1;
        if ((left < currsize) && (data[right].key < data[left].key)) {
            child = right;
        }
        else {
            child = left;
        }
        if (data[child].key < temp.key) {
            data[posCur] = data[child];
            mapping.setPointer(data[posCur].id, &data[posCur]);
        }
        else {
            break;
        }
    }
    data[posCur] = temp;
    mapping.setPointer(data[posCur].id, &data[posCur]);
}
