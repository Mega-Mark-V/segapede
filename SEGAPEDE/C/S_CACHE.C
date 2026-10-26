/*
#include "s_cache.h"
*/
#include "bbdefs.h"
#include "bbproto.h"
#include "bbart.h"

#define SC_STATS

#define FIRST_CELL_IN_CACHE 0
#define LAST_CELL_IN_CACHE  1535
#define ALLOCATION_UNIT     (4 * 4)
#define SPRITES_IN_CACHE    ((LAST_CELL_IN_CACHE - FIRST_CELL_IN_CACHE + 1) / ALLOCATION_UNIT)


typedef struct {
    unsigned short id;
    short location;
    short size;
    short predecessor;
    short successor;
}CacheEntry, *CacheEntryPtr;

short CCBRecord;
CacheEntryPtr CCB;
short head;
short tail;

#ifdef SC_STATS
 long SCTries;
 long SCMisses;
#endif


short InitSCache(void);
short GetSCacheLocation(short id);
short GetSpriteSize(short location);
void  InvalidateSCacheEntry(short id);
void  InvalidateSCache(void);


short InitSCache(void){
short i;

    if(CCBRecord = alloc_record(SPRITES_IN_CACHE * sizeof(CacheEntry))){
        CCB = lock_record(CCBRecord);
        for(i = 0; i < SPRITES_IN_CACHE; i++){
            CCB[i].location = FIRST_CELL_IN_CACHE + i * ALLOCATION_UNIT;
            CCB[i].size = 0;
            CCB[i].id = -1;
            CCB[i].predecessor = (i - 1) * sizeof(CacheEntry);
            CCB[i].successor = (i + 1) * sizeof(CacheEntry);
        }
        CCB[--i].successor = -1;
    }
    head = 0;
    tail = (SPRITES_IN_CACHE - 1) * sizeof(CacheEntry);

    #ifdef SC_STATS
    SCTries = SCMisses = 0L;
    #endif

    return CCBRecord;
}

short GetSCacheLocation(short id){
short i;
unsigned long *sb;
register CacheEntryPtr c = CCB;
register CacheEntryPtr cp;

    #ifdef SC_STATS
    SCTries++;
    #endif

    i = head;
    cp = (CacheEntryPtr)((char *)c + i);
    while((i >= 0) && (cp->id != id)){
        i = cp->successor;
        cp = (CacheEntryPtr)((char *)c + i);
    }

    if(i >= 0){
        /* if it's already on the head, we're done */
        if(i != head){
            /* cut it out of the list */
            if(i == tail){
                tail = cp->predecessor;
            } else {
                ((CacheEntryPtr)((char *)c + cp->successor))->predecessor = cp->predecessor;
            }
            ((CacheEntryPtr)((char *)c + cp->predecessor))->successor = cp->successor;

            /* tack it on the head */
            cp->successor = head;
            ((CacheEntryPtr)((char *)c + head))->predecessor = i;
            cp->predecessor = -1;
            head = i;
        }
    } else {

        #ifdef SC_STATS
        SCMisses++;
        #endif

        /* remove the tail element */
        i = tail;
        cp = (CacheEntryPtr)((char *)c + i);
        tail = cp->predecessor;
        ((CacheEntryPtr)((char *)c + tail))->successor = -1;

        /* put it on the head */
        cp->successor = head;
        cp->predecessor = -1;
        ((CacheEntryPtr)((char *)c + head))->predecessor = i;
        head = i;

        /* fill it up */
        spriteBlocks[spriteDMAIndex] = &spriteCells[16 * id];
        cp->size = 15; /* ][][][][][][][][]*/
        cp->id = id;

        /* send the called-for sprite to cell #CCB[i].location in the VDP */
        spriteTargets[spriteDMAIndex] = cp->location << 5;
        spriteDMAIndex++;
    }
    return cp->location;
}

short GetSpriteSize(short id){
short i;
register CacheEntryPtr c = CCB;
register CacheEntryPtr cb;

    i = head;
    cb = (CacheEntryPtr)((char *)c + i);
    while((i >= 0) && (cb->id != id)){
        i = cb->successor;
        cb = (CacheEntryPtr)((char *)c + i);
    }
    if(i < 0) return 0;
    return cb->size;
}

void InvalidateSCacheEntry(short id){
short i;

    for(i = 0; i < SPRITES_IN_CACHE; i++){
        if(CCB[i].id == id){

            /* tag it invalid */
            CCB[i].id = -1;

            /* if it's already on the tail, we're done */
            if(i == tail) break;

            /* cut it out of the list */
            if(i == head){
                head = CCB[i].successor;
            } else {
                CCB[CCB[i].predecessor].successor = CCB[i].successor;
            }
            CCB[CCB[i].successor].predecessor = CCB[i].predecessor;

            /* tack it on the tail */
            CCB[i].predecessor = tail;
            CCB[tail].successor = i * sizeof(CacheEntry);
            CCB[i].successor = -1;
            tail = i * sizeof(CacheEntry);
            break;
        }
    }
}

void InvalidateSCache(void){
short i;

    for(i = 0; i < SPRITES_IN_CACHE; i++){
        CCB[i].id = -1;
    }
}
