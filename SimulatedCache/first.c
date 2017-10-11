#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "first.h"

Node* createNode(add_line *address) {
    Node *newNode;
    newNode = (Node*)malloc(sizeof(Node));
    newNode->address = address;
    newNode->next = NULL;
    return newNode;
}
cache_line* createLine(int numtag_bits){
    cache_line* new_add_line;
    new_add_line = malloc(sizeof(cache_line));
    new_add_line->valid = -1;
    new_add_line->tag =  malloc(sizeof(char*) *numtag_bits);
    return new_add_line;
}
void createSet(cache_set *set,int assocNum){

    set->set_index = -1;
    set->head = NULL;
    set->lines = malloc(assocNum * sizeof(cache_line));

    return;
}
void createCache(cache *cache, int assocNum, int num_sets, int numtag_bits){
    cache->mem_reads = 0;
    cache->mem_writes = 0;
    cache->hits = 0;
    cache->misses= 0;
    cache->numtag_bits = numtag_bits;
    cache->num_sets= num_sets;
    cache->num_lines = assocNum;
    cache->sets = NULL;
    cache->bits = 0;

    int i,j;
    cache->sets =malloc(cache->num_sets * sizeof(cache_set*));
    for(j=0; j <cache->num_sets; j++){
        cache->sets[j]= malloc(sizeof(cache_set));

        createSet(cache->sets[j],assocNum);
        for(i=0; i < assocNum; i++){

            cache->sets[j]->lines[i] = createLine(numtag_bits);\
        }
    }
}
//checks to see if num is a power of 2
int powerOfTwo(int n) {
    while((n%2 == 0) && n > 1){
        n /= 2;
    }
    return n;
}

//converts hex num to binary representation
char* convertBin(char* apple){
    int i;
    char* binary_address = malloc(sizeof(char) * 49);

    for(i = 0; i <= strlen(apple); i++) {

        if(apple[i] == '0') {
            strcat(binary_address, "0000");
        }
	if(apple[i] == '1') {
            strcat(binary_address, "0001");
        }
	
        if(apple[i] == '2') {
            strcat(binary_address, "0010");
        }
        if(apple[i] == '3') {
            strcat(binary_address, "0011");
        }
        if(apple[i] == '4') {
            strcat(binary_address, "0100");
        }
        if(apple[i] == '5') {
            strcat(binary_address, "0101");
        }
        if(apple[i] == '6') {
            strcat(binary_address, "0110");
        }
        if(apple[i] == '7') {
            strcat(binary_address, "0111");
        }
        if(apple[i] == '8') {
            strcat(binary_address, "1000");
        }
        if(apple[i] == '9') {
            strcat(binary_address, "1001");
        }
        if(apple[i] == 'a') {
            strcat(binary_address, "1010");
        }
        if(apple[i] == 'b') {
            strcat(binary_address, "1011");
        }
        if(apple[i] == 'c') {
            strcat(binary_address, "1100");
        }
        if(apple[i] == 'd') {
            strcat(binary_address, "1101");
        }
        if(apple[i] == 'e') {
            strcat(binary_address, "1110");
        }
        if(apple[i] == 'f') {
            strcat(binary_address, "1111");
        }

        
    }
    strcat(binary_address,"\0");
    return binary_address;
}

//inserts into linked list
void insertLinkedList(Node ** head, add_line* address) {

    if( *head == NULL) {
        *head = createNode(address);
        return;
    }
    Node* current = *head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = createNode(address);


}

//FIFO algo for cache
void  replace_algo(cache* cache, cache_set *set, add_line* add_line){

	    

    //gets old head from linked list
    Node* prev_head = (Node*)malloc(sizeof(Node));
    prev_head = set->head;

    //deletes head
    set->head = set->head->next;
 
    //locate old head in cache and update value
    int i;
    for(i=0; i< cache->num_lines; i++){
    	if(strcmp(set->lines[i]->tag,prev_head->address->tag_bits) == 0){
    		set->lines[i]->tag = add_line->tag_bits;
    		break;
    	}
    }

    free(prev_head);
    return;
}

//set index, tag, and offset bits of an address line for cache A
add_line* load_to_cacheA(cache *cache,char* new_mem_address, int numtag_bits, int numindex_bits, int numoffset_bits){

	
    char* new_add;
    new_add = convertBin(new_mem_address);

    char* bin_index = malloc(sizeof(char)*numindex_bits+1); 
    char* bin_tag =malloc(sizeof(char)*numtag_bits+1);
    char* bin_offset = malloc(sizeof(char)*numoffset_bits+1);

	
    strncpy(bin_tag,new_add,numtag_bits);
    
    strncpy(bin_index,new_add+(int)numtag_bits,numindex_bits);
    strncpy(bin_offset,new_add+(int)numindex_bits+(int)numtag_bits,numoffset_bits);


    //CREATE NEW LINE STRUCT 
    add_line* new_add_line;
    new_add_line = malloc(sizeof(add_line));
    new_add_line->tag_bits = bin_tag;
    new_add_line->index_bits = bin_index;
    new_add_line->offset_bits = bin_offset;
    free(new_add);
   
    return new_add_line;


}
//set index, tag, and offset bits of an address line for cache B
add_line* load_to_cacheB(cache *cache,char* new_mem_address, int numtag_bits, int numindex_bits, int numoffset_bits){
    char* new_add;

    new_add = convertBin(new_mem_address);
    char* bin_index = malloc(sizeof(char)*(numindex_bits+1)); 
    char* bin_tag =malloc(sizeof(char)*(numtag_bits+1));
    char* bin_offset = malloc(sizeof(char)*(numoffset_bits+1));
    strncpy(bin_index,new_add,numindex_bits);

    strncpy(bin_tag,new_add+numindex_bits,numtag_bits);
 
    strncpy(bin_offset,new_add+numindex_bits+numtag_bits,numoffset_bits);

    
    //CREATE NEW LINE STRUCT 
    add_line* new_add_line;
    new_add_line = malloc(sizeof(add_line));
    new_add_line->tag_bits = bin_tag;
    new_add_line->index_bits = bin_index;
    new_add_line->offset_bits = bin_offset;
    free(new_add);
    return new_add_line;

}

//update the cache with address lines
void read_or_write(cache *cache, add_line* line, char instruction){

    int i;
    int num =0;
    //int j;
    if(instruction != 'W') {
        num++;
    }
    if(instruction == 'W'){
        cache->mem_writes++;
    }

	//find correct set
	for(i=0; i < cache->num_sets; i++){
        // set index already exists 
        if((int)cache->sets[i]->set_index == (int) atoi(line->index_bits)){
            int j;
            for(j=0; j < cache->num_lines; j++){
            	//right line, right tag
            	if(strcmp(cache->sets[i]->lines[j]->tag,line->tag_bits) == 0){
 
                    cache->hits++;
                    return;
                }
            	//empty line  
            	if(cache->sets[i]->lines[j]->valid == -1){
            		cache->sets[i]->lines[j]->valid = 1;
            		cache->sets[i]->lines[j]->tag = line->tag_bits;


            		insertLinkedList(&(cache->sets[i]->head),line);
           
            		cache->mem_reads++;
            		cache->misses++;

            		return;
            	} 
            	// no empty line, no empty set, update cache
            	if(j == cache->num_lines -1){

            		insertLinkedList(&(cache->sets[i]->head),line);
 
            		replace_algo(cache,cache->sets[i],line);
 
            		cache->mem_reads++;
            		cache->misses++;
            		return;
            	}

        	}
        }       
    }

    //empty set
    for(i=0; i < cache->num_sets; i++){
    	//update set index and line 
   		if(cache->sets[i]->set_index == -1){
    		cache->sets[i]->set_index = (int) atoi(line->index_bits);
    	    cache->sets[i]->lines[0]->valid = 1;
        	cache->sets[i]->lines[0]->tag = line->tag_bits;
			insertLinkedList(&(cache->sets[i]->head),line);

			cache->mem_reads++;
           	cache->misses++;
            return;
   		}

    }

}


int main(int argc, char const *argv[])
{
    int cacheSize;
    int blockSize;
    int assocNum;
    int num_sets;
    const char* assocString;
    FILE *fp;
    cache *cacheA = (cache*)malloc(sizeof(cache));
    cache *cacheB = (cache*)malloc(sizeof(cache));

    
    //ERROR CHECKS
    if(argc != 5){
        printf("Incorrect number of inputs\n");
        return 0;
    }
    
    cacheSize = atoi(argv[1]);
    assocString = argv[2];
    blockSize = atoi(argv[3]);
    fp = fopen(argv[4],"r");
    
        

    if(powerOfTwo(cacheSize) != 1){
        printf("Cache Size is incorrect\n");
        return 0;;
    }
    

    if(strlen(assocString) == 6){
        if(strcmp(assocString,"direct") != 0){
            printf("Incorrect associative\n");
            return 0;;
        }
        assocNum = 1;
        
    }
    

    if(strlen(assocString) == 5){
        if(strcmp(assocString,"assoc") != 0){
            printf("Incorrect associative\n");
            return 0;;
        }
        assocNum = cacheSize /blockSize;
    }
    
    
  
    if(strlen(assocString) >6){
         if(strncmp(assocString,"assoc",5) != 0){
 
            printf("Incorrect associative\n");
            return 0;;
        }
        char* nWay = malloc(sizeof(char)*10);
        strncpy(nWay,assocString+6,strlen(assocString)-6);
        int n = atoi(nWay);


        if(powerOfTwo(n) != 1){
            printf("Incorrect associative\n");
            return 0;;
        }
        assocNum = n;
    }

    

    if(powerOfTwo(blockSize) != 1){
        printf("Block size incorrect\n");
        return 0;;
    }

    if(fp == NULL){
        printf("File does not exist \n");
        return 0;;
    }

    //get parameters
    if(strlen(assocString) == 5 || blockSize == cacheSize){
        num_sets = 1;
    }
    else{
        num_sets = (cacheSize / (blockSize*assocNum));
    }

    double  numindex_bits, numtag_bits, numoffset_bits;
    numindex_bits = log(num_sets) / log(2) ;
    if(assocNum == cacheSize/blockSize){
        numindex_bits = 0.0;
    }
    numoffset_bits = log(blockSize) / log(2);
    numtag_bits  = 48.0 - (numindex_bits + numoffset_bits);
        
    //Create Caches
    createCache(cacheA,assocNum,num_sets,numtag_bits);
    createCache(cacheB,assocNum,num_sets,numtag_bits);

    char* ignoreD= (char*) malloc(sizeof(char)*30);
    char* address = (char*) malloc(sizeof(char)*30);
    char instruction;


//Run through each line of file
    while (fscanf(fp, "%s %c %s", ignoreD, &instruction,address) != EOF) {
        //zero out bits
        
        int difference;
        difference = 48 - (4*(strlen(address) - 2 ));
        difference = difference /4 ;
        
        char* new_mem_address = (char*)malloc(sizeof(char)*13);   
        int i;

	//add bits
        for(i = 0; i < difference; i++) {
            new_mem_address[i] = '0';   
        }

        strcat(new_mem_address,address+2);

	//Place line in cache A
        add_line *new_add_lineA = load_to_cacheA(cacheA,new_mem_address,numtag_bits,numindex_bits,numoffset_bits);
	//Same for cache B

    	add_line *new_add_lineB = load_to_cacheB(cacheB,new_mem_address,(int)numtag_bits,(int)numindex_bits,(int)numoffset_bits);

	//Update both caches
    	read_or_write(cacheA,new_add_lineA,instruction);
    	read_or_write(cacheB,new_add_lineB,instruction);
        
        free(new_mem_address);
    }

    
    printf("cache A\n");    
    printf("Memory reads: %d\n", cacheA->mem_reads);
    printf("Memory writes: %d\n", cacheA->mem_writes);
    printf("Cache hits: %d\n", cacheA->hits-1);
    printf("Cache misses: %d\n",cacheA->misses);

    printf("cache B\n");
    printf("Memory reads: %d\n", cacheB->mem_reads);
    printf("Memory writes: %d\n", cacheB->mem_writes);
    printf("Cache hits: %d\n", cacheB->hits-1);
    printf("Cache misses: %d\n",cacheB->misses);

    fclose(fp);
    free(ignoreD);
    free(address);
    return 0;
}

