#include <stddef.h>
#include "mmgr.h"

#define SET_LOCK_BIT 0x01000000
#define CLEAR_LOCK_BIT (~SET_LOCK_BIT)
#define ADDRESS_MASK 0x00ffffff


typedef struct free_block {
        unsigned short size;
        struct free_block *next;
        struct free_block *prev;
        char body[2];
}FreeBlock, *FreeBlockPtr;

typedef struct record {
        unsigned short size;
        short owner;
        char body[2];
}Record, *RecordPtr;

static FreeBlockPtr FreeListHead;
static FreeBlockPtr FreeListTail;
static unsigned long *IndexTable;
static short FirstIndex;

static void insert_free_block(FreeBlockPtr address){
FreeBlockPtr test = FreeListHead;

    while(test && (test < address))
        test = test->next;
    if(test){
        address->next = test;
        address->prev = test->prev;
        if(test->prev)
            test->prev->next = address;
        else
           FreeListHead = address;
        test->prev = address;
    } else {
        address->prev = FreeListTail;
        address->next = 0L;
        if(FreeListTail)
            FreeListTail->next = address;
        else
            FreeListHead = address;
        FreeListTail = address;
    }
    if(((unsigned long)address) + ((unsigned long)address->size) == ((unsigned long)(address->next))){
        /* merge with following block */
        if(FreeListTail == address->next)
            FreeListTail = address;
        address->size += address->next->size;
        if(address->next->next)
            address->next->next->prev = address;
        address->next = address->next->next;
    }
    if(address->prev && ((unsigned long)(address->prev) + (unsigned long)(address->prev->size) == (unsigned long)address)){
        /* merge with preceding block */
        address->prev->size += address->size;
        address->prev->next = address->next;
        if(address->next)
            address->next->prev = address->prev;
        else
            FreeListTail = address->prev;
    }
}

void mmgr_init(unsigned char *location, unsigned short size, unsigned short indices){
    unsigned long i, j, count;

    count = -indices;
    j = 2;
    FreeListHead = FreeListTail = (FreeBlockPtr)location;
    IndexTable = (unsigned long *)((char *)(FreeListHead) + size);
    FreeListHead->next = FreeListHead->prev = 0L;
    FreeListHead->size = size - 4 * indices;
    for(i = -1; i > count; i--){
        IndexTable[i] = j++;
    }
    IndexTable[i] = 0L;
    FirstIndex = 1;
}

void mount_block(unsigned char *location, unsigned short size){
    ((FreeBlockPtr)location)->next = ((FreeBlockPtr)location)->prev = 0L;
    ((FreeBlockPtr)location)->size = size;
    insert_free_block((FreeBlockPtr)location);
}

unsigned short alloc_record(unsigned short size){
FreeBlockPtr temp = FreeListHead;
RecordPtr new;
unsigned short index = FirstIndex;

    if(!index)
        return(0);
    size = (size+sizeof(Record)-2+1)&~1;
    while(temp && (temp->size < size))
        temp = temp->next;
    if(!temp){
        garbage_collect();
        temp = FreeListHead;
        while(temp && (temp->size < size))
            temp = temp->next;
        if(!temp)
            return 0;       /* failed */
    }
    if(temp->size - (sizeof(FreeBlock) - 2) < size){
        /* too small to split */
        size = temp->size;
        new = (RecordPtr)temp;
        if(temp->prev)
            temp->prev->next = temp->next;
        else {
            FreeListHead = temp->next;
            FreeListHead->prev = 0L;
        }
        if(temp->next)
            temp->next->prev = temp->prev;
        else {
            FreeListTail = temp->prev;
            FreeListTail->next = 0L;
        }
    } else {
        /* split it up */
        temp->size -= size;
        new = (RecordPtr)(((char *)temp) + temp->size);
    }
    new->size = size;
    new->owner = index;
    FirstIndex = IndexTable[-(unsigned long)index];
    IndexTable[-(unsigned long)index] = 0x80000000L | (unsigned long)new;
    return(index);
}

void free_record(short index){
    FreeBlockPtr block;

    block = (FreeBlockPtr)(IndexTable[-(unsigned long)index] & ADDRESS_MASK);
    block->next = block->prev = 0L;
    insert_free_block((FreeBlockPtr)block);
    IndexTable[-(unsigned long)index] = FirstIndex;
    FirstIndex = index;
}

void *lock_record(short index){
    IndexTable[-(unsigned long)index] |= SET_LOCK_BIT;
    return (void *)((IndexTable[-(unsigned long)index] & ADDRESS_MASK) + offsetof(Record, body[0]));
}

void unlock_record(short index){
    IndexTable[-(unsigned long)index] &= CLEAR_LOCK_BIT;
}

void garbage_collect(){
}
