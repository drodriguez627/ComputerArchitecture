#ifndef _first_h
#define _first_h

/* node struct */
struct node {
        int key;
        char *word;
        int occur;
        int super;
        struct node *child[26];
};

/* Function runDict
Reads dict file and adds words to trie  */
void runDict(FILE *dict_file);



/* Function readData
  goes through data file comparing to dict */
void readData(FILE *data_file);



/* Function createNode
 Creates a node struct with default values */
struct node *createNode();



/*  Function addDictWord
Adds word to trie */
void addDictWord(char *word);



/*  Function matchStr
runs through trie finding occurences, increasing occur  */
void matchStr(char *str);




/* Function printResult
  prints all words from trie   */
void printResult();



/*  Function outputFile
 recursively writes all the words in trie to output file
*/
void outputFile(FILE *out_file, struct node *ptr);




/* Function deallocTrie
 deallocates each node of the trie, starting with head
*/
void deallocTrie(struct node *ptr);

#endif
