#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/* address line struct */
typedef struct Add_Line{
char* tag_bits;
char* index_bits;
char* offset_bits;
}add_line;

/* linked list struct */
typedef struct Node{
add_line *address;
struct Node *next;
}Node;

/*cache line struct*/
typedef struct Cache_Line{
int valid;
char* tag;
}cache_line;

/*cache set struct*/
typedef struct Cache_Set{
Node *head;
cache_line **lines;
int set_index;
}cache_set;

/* cache struct */
typedef struct Cache{
int mem_reads;
int mem_writes;
int hits;
int misses;
cache_set **sets;
int num_sets;
int num_lines;
int numtag_bits;
int bits;
}cache;

Node* createNode(add_line *address);

cache_line* createLine(int numtag_bits);

void createSet(cache_set *set,int assocNum);

void createCache(cache *cacheA, int assocNum, int num_sets, int numtag_bits);

int powerOfTwo(int n);

char* convertBin(char* apple);

void insertLinkedList(Node ** head, add_line* address);

void  replace_algo(cache* cache, cache_set *set, add_line* add_line);

add_line* load_to_cacheA(cache *cache,char* new_mem_address, int numtag_bits, int numindex_bits, int numoffset_bits);

add_line* load_to_cacheB(cache *cache,char* new_mem_address, int numtag_bits, int numindex_bits, int numoffset_bits);

void read_or_write(cache *cache, add_line* line, char instruction);












