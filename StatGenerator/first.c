
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "first.h"

/*  Global variables of trie root and file line counter */
struct node *root;
int count = 1;

struct node *createNode() {
	struct node *currNode = (struct node*) malloc(sizeof(struct node));
	int i =0;

	currNode->key = -1;
	currNode->occur = 0;
	currNode->super = 0;
	currNode->word = NULL;

	/* Set all child to 0, to avoid mallocing every new node if it is not needed */
	while (i < 26) {
		currNode->child[i] = 0;
		i++;
	}
	return currNode;
}

void runDict(FILE *dictfile) {
	char c;
	int wordLength = 0;
	char buffer[256];

	/* go through each character in dict file */
	for(;;) {
		
		if(isalpha(c)) {
			/* character is an keybetical. add it to buffer */
			buffer[wordLength++] = tolower(c);
		}
		else {
			if (wordLength > 0) {
				buffer[wordLength] = '\0';
				addDictWord(buffer);
				wordLength = 0;
			}
		}
			
		if ((c = fgetc(dictfile)) == EOF) {
			/* We have a non-key char, check if we have a word stored in the buffer, if so call function to add it to trie */
		
			if (wordLength > 0) {
				buffer[wordLength] = '\0';
				addDictWord(buffer);
				wordLength = 0;
			}
			
			if (c == EOF) break;
		}

	}
}

void runData(FILE *datafile) {
	char c;
	int wordLength = 0;
	char buffer[256];

	/* Same thing as runDict, except this time we call matchStr instead of addDictWord */
	for(;;) {
		if(isalpha(c)) {

			buffer[wordLength++] = tolower(c);
		}
		else {
			if (wordLength > 0) {
				buffer[wordLength] = '\0';
				matchStr(buffer);
				wordLength = 0;
			}
		}
			
		if ((c = fgetc(datafile)) == EOF) {
			
			if (wordLength > 0) {
				buffer[wordLength] = '\0';
				matchStr(buffer);
				wordLength = 0;
			}
			if (c == EOF) break;
		}

	}
}
void matchStr(char *str) {
	int i=0;
	struct node *ptr;

	if (!root) return;

	ptr = root;

	while (i < strlen(str)) {
		int index = tolower(str[i]) - 'a';
		if (!(ptr = ptr->child[index])) return;
		if (i < strlen(str)-1) ptr->super += 1;
		i++;
	}
	ptr->occur += 1;
}


void addDictWord(char *word) {
	int i;
	struct node *ptr;

	if (root == NULL) {
 		root = createNode();
	}
	/* Traverse tree, adding nodes if needed, until we finish the word */
	ptr = root;
	for (i = 0; i < strlen(word); i++) {
		int index = tolower(word[i]) - 97;

		if (!ptr->child[index]) {
			ptr->child[index] = createNode();
			ptr->child[index]->key = index;
		}
		ptr = ptr->child[index];
	}
	/* Make all full words have char* word */
	ptr->word = (char*) malloc(sizeof(char) * (strlen(word)+1));
	for (i = 0; i < strlen(word); i++) {
		ptr->word[i] = word[i];
	}
	ptr->word[strlen(word)] = '\0';
}



void printResult() {
	FILE *outfile;
	char outfilename[64];
	
	sprintf(outfilename, "out%d.txt", count);
	outfile = fopen(outfilename,"w");
	outputFile(outfile,root);
	fclose(outfile);
}

void outputFile(FILE *out, struct node *ptr) {
	int i;

	if (ptr== NULL) {
		 return;
	}

 	if(!(ptr->word == NULL)) {
		fprintf(out,"%s %d %d\n",ptr->word,ptr->occur,ptr->super);
	}

	for (i = 0; i < 26; i++) {
		outputFile(out,ptr->child[i]);
	}
}

void deallocTrie(struct node *ptr) {
	int i=0;

	if (!ptr) return;

	while (i < 26) {
		deallocTrie(ptr->child[i]);
		i++;
	}
	if (ptr->word){
		free(ptr->word);
	}

	free(ptr);
}

int main(int argc, char **argv) {
	FILE *fp;
	char buffer[256];

	if (argc < 2 || !(fp = fopen(argv[1],"r"))) {
		printf("invalid input\n");
		return 0;
	}

	while (fgets(buffer,sizeof(buffer),fp)) {
		FILE *dictfile;
		FILE *datafile;
		root = NULL;

		/* Pull out file names from line */
		char *dictfilename = strtok(buffer," ");
		char *datafilename = strtok(NULL," \n");

		
		/* open dict file and read words */
		dictfile = fopen(dictfilename,"r");
		runDict(dictfile);
				
		/* open data file and increment count where needed */
		datafile = fopen(datafilename,"r");
		runData(datafile);

		/* Print trie to file and free it so we can continue to next file pair */	
		printResult();
		fclose(dictfile);
		fclose(datafile);
		deallocTrie(root);
		/*continue to next line of map file */
		count += 1;
	}
	fclose(fp);
	return 0;
}


