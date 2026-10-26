
#define FIRST_CELL_IN_CACHE 0
#define LAST_CELL_IN_CACHE  1024
#define ALLOCATION_UNIT     (4*4)
#define CACHE_SIZE          ((LAST_CELL_IN_CACHE - FIRST_CELL_IN_CACHE + 1) / ALLOCATION_UNIT)

/* This definition controls the gathering of statistics */
#define SC_STATS    1

/* This is the data structure that we use to maintain our cache */

typedef struct sc{
  short id;         /*
  short left;       /* points to lesser subtree */
  short right;      /* points to greater subtree */
  short next;       /* points to older entry */
  short prev;       /* points to newer entry */
}SpriteCacheEntry, *SpriteCacheEntryPtr;

static short root;      /* points to root of search chain */
static short newest;    /* points to young end of age chain */
static short oldest;    /* points to old end of age chain */
static SpriteCacheEntry cache[CACHE_SIZE];
#if SC_STATS
  unsigned long cache_tries;
  unsigned long cache_misses;
#endif

void InitSpriteCache(void){
int i;

  root = -1;
  newest = 0;
  oldest = CACHE_SIZE - 1;
  for(i = 0; i < CACHE_SIZE/2; i++){
    cache[i].id = -1;
    cache[i].left = 2 * i;
    cache[i].right = 2 * i + 1;
    cache[i].next = i+1;
    cache[i].prev = i-1;
  }
  if(CACHE_SIZE & 1) cache[i-1].next = -1;
  for(; i < CACHE_SIZE; i++){
    cache[i].id = -1;
    cache[i].left = -1;
    cache[i].right = -1;
    cache[i].next = i+1;
    cache[i].prev = i-1;
  }
  cache[i-1].next = -1;
#if SC_STATS
  cache_tries = cache_misses = 0L;
#endif
}

short FindSpriteCacheEntry(short id){
short p;
short t;

#if SC_STATS
  cache_tries++;
#endif
  if(root == -1){
    t = oldest;
    oldest = cache[oldest].prev;
    root = t;
  }
  p = root;
  do{
    if(cache[p].id == id){          /* found */

    }
    if(cache[p].id < id){           /* take left branch */
      p = cache[p].left;
    } else {                        /* take right branch */
      p = cache[p].right;
    }
  } while(p >= 0);
}
