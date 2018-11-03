//Brijesh Patel br168467

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"
#include <ctype.h>
#include <stdbool.h>


TrieNode *createTrieNode(void)
{
	return calloc(1, sizeof(TrieNode));
}


// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)	
		return;
		
	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a firstWordtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int usefirstWordtrieFormatting)
{
	char buffer[1026];

	if (usefirstWordtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

void InsertString(TrieNode **root, char *str)
{
	TrieNode *tmp = NULL;
	int i = 0, len, index;

	if(*root == NULL)
		*root = createTrieNode();

	tmp = *root;

	len = strlen(str);

	if(len == 0)
	{	
		tmp->count++;
		return;
	}
	// creates path of string in trie(lowercase)
	while(i < len)
	{	
		index = tolower(str[i]) - 'a';
		
		if(tmp->children[index] == NULL)
				tmp->children[index] = createTrieNode();	
		
		tmp = tmp->children[index];
		i++;
	}
	
	tmp->count++;

}

void stripPunctuators(char *str, char *array, bool *flag)
{
	int len = strlen(str), i = 0, k = 0;

	while(i < len)
	{
		// only alphbetical letters allowed
		if(!isalpha(str[i]))
			{		
				if(str[i] == '.' || str[i] == '!' || str[i] == '?')	
					*flag = true;				
				i++;
				continue;
			}

		array[k++] = str[i];
		// Makes sure not to have extra characters
		// If  next word is shorter than previous word
		array[k] = '\0';
		i++;
	}	
}

TrieNode *buildTrie(char *filename)
{
	TrieNode *t = NULL, *tmp = NULL;
	FILE *fp;
	char buffer[MAX_CHARACTERS_PER_WORD], hold[MAX_CHARACTERS_PER_WORD];
	bool firstWord = true, end = false;

	fp = fopen(filename, "r");

	if(fp == NULL)
		return t;

	while(fscanf(fp, " %s", buffer) != EOF)
	{	
		stripPunctuators(buffer, hold, &end);
		// insert punctuation free string into the trie
		InsertString(&t, hold);

	   if(firstWord != true)
				InsertString(&tmp->subtrie, hold);
		// if there is only one word in the sentence
	   if(firstWord == true && end == true)
			{
				end = false;
				continue;
			}
	   else if(firstWord == true)
				firstWord = false;
		// we have reached the end of a sentence(no subtrie insertion !!)	
		else if(end == true)
			{
				firstWord = true;
	            end = false;
	            continue;
			}

		tmp = getNode(t, hold);
	}

	fclose(fp);
	return t;
}

int processInputFile(TrieNode *root, char *filename)
{
	FILE *fp = NULL;
	char buffer[MAX_CHARACTERS_PER_WORD], freqBuffer[MAX_CHARACTERS_PER_WORD];
	TrieNode *tmp = NULL;
	int n, i, len;

	fp = fopen(filename, "r");

	if(fp == NULL)
		return 0;
 
	while(fscanf(fp, " %s", buffer) != EOF)
	{
		if(strcmp(buffer, "!") == 0)
				printTrie(root, 0);

		else if(buffer[0] == '@')
		{
			// grab in string 
			fscanf(fp," %s", buffer);
			strcpy(freqBuffer, buffer);
			len = strlen(buffer);

			// convert to lowercase after printing original string
			for(i = 0; i < len; i++)
				buffer[i] = tolower(buffer[i]);	

			if(containsWord(root, buffer) == 0)
				{
				// if word is not in Trie then put it on a new line 
				printf("%s\n",freqBuffer);
				return 0;
				}

			// The first word should be printed as is.
			printf("%s", freqBuffer);
			
			i = 0;
			strcpy(freqBuffer, buffer);
						
			fscanf(fp, " %s", buffer);

			n = atoi(buffer);
		
			while(i < n)
			{	
				tmp = getNode(root, freqBuffer);
				// If a word does not have a subtrie, loop ends prematurely
				if(tmp != NULL)
					getMostFrequentWord(tmp->subtrie, buffer);
				else 
					break;
				
				// print spaces before instead of after to avoid checking last word
				if(strlen(buffer) != 0)
					printf(" %s", buffer);

				strcpy(freqBuffer, buffer);

				i++;
			}
			// New line after each command;
			puts("");
		}
		else 
		{	
			// need to check for empty file
			// need to check for capital letters
			printf("%s\n",buffer);
			len = strlen(buffer);
			
			for(i = 0; i < len; i++)
				buffer[i] = tolower(buffer[i]);

			tmp = getNode(root, buffer);

			if(tmp == NULL)
			{
				printf("(INVALID STRING)\n");
				continue;
			}

			if(tmp->subtrie == NULL)
			{
				printf("(EMPTY)\n");
				continue;
			}

			printTrie(tmp->subtrie, 1);
		}			
	}
	fclose(fp);
	return 0;
}


TrieNode *destroyTrie(TrieNode *root)
{
	int i;
	
	if(root == NULL)
		return NULL;

	// recursively free subtries before freeing the roots
    destroyTrie(root->subtrie);

	// recursively travel to the last node of each path and then start freeing
	for(i = 0; i < 26; i++)
		destroyTrie(root->children[i]);
	
	free(root);

	return NULL;

}

TrieNode *getNode(TrieNode *root, char *str)
{
	TrieNode *tmp = NULL;
	int len, i = 0, index;

	if(containsWord(root, str) == 0)
		return NULL;
		
	if(root == NULL)
		return NULL;

	tmp = root;

	len = strlen(str);

	if(len == 0)
		return tmp;

	while(i < len)
	{
		index = tolower(str[i]) - 'a';

		if(tmp->children[index] == NULL)
			return NULL;

		tmp = tmp->children[index];
		
		i++;
	}

	return tmp;

}

// This function gets the string with the highest frequency(helper function for GetMostFrequentWord)
void getString(TrieNode *root, char *buffer, int k, int max, char *buf, bool *flag)
{
	int i;
	
	if (root == NULL)
		return;
	
	// flag - if two there are two strings with the max count return the one 
	//that comes first in the alphabet
	if(*flag == 0)
		if (root->count ==  max)
			{
			strcpy(buf, buffer);
			*flag = 1;
			return;
			}	

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		getString(root->children[i], buffer, k + 1, max, buf, flag);
	}

	buffer[k] = '\0';
}

// returns the node with the largest count, max is passed by reference to avoid resetting 
TrieNode *frequentHelper(TrieNode *root, int *max)
{
	int len, i;
    
	static TrieNode *tmp = NULL;

	if(root == NULL)
		return NULL;

	for (i = 0; i < 26; i++)
	{   
		if(root->count > *max)
			{
			  *max = root->count;
			   tmp = root;
			}
		// recursively travel the entire trie, to and change value of max
	    // only if the current nodes count is larger than max
		frequentHelper(root->children[i], max);
	}

	return tmp;
}

void getMostFrequentWord(TrieNode *root, char *str)
{
	TrieNode *tmp = NULL;
	char ptr[MAX_CHARACTERS_PER_WORD];
	bool flag = 0;
	int max = 0;

	// just copy and return the empty string
	if(root == NULL)
		{	
			strcpy(str, "");
			return;
		}
	else 
		tmp = frequentHelper(root, &max);
		getString(root, str, 0, tmp->count, ptr, &flag);
		strcpy(str, ptr);

}

int containsWord(TrieNode *root, char *str)
{
	int len, i = 0, index;
	TrieNode *tmp = NULL;

	if(root == NULL)
		return 0;

	tmp = root;
	
	len = strlen(str);

	// if searching for the empty string
	if(len == 0 && tmp->count >=1)
		return 1;
		
	while(i < len)
	{
		index = tolower(str[i]) - 'a';

		if(tmp->children[index] == NULL)
			return 0;

		tmp = tmp->children[index];	
		i++;
	}

	if(tmp->count >= 1)
		return 1;
	else 
		return 0;
	
}
// this function is doing the actual counting
int prefixHelper(TrieNode *root)
{
	int i;
    int ct = 0;

	if(root == NULL)
		return 0;

	if (root->count >= 1)
		ct += root->count;

	for (i = 0; i < 26; i++)
		ct += prefixHelper(root->children[i]);

	return ct;

}
// will get location of terminal node of the string passed in
TrieNode *prefixHelper2(TrieNode *root, char *str)
{
	TrieNode *tmp = NULL;
	int len, i = 0, index;
	
	if(root == NULL)
		return NULL;

	tmp = root;

	len = strlen(str);

	if(len == 0)
		return tmp;

	while(i < len)
	{
		index = tolower(str[i]) - 'a';

		if(tmp->children[index] == NULL)
			return NULL;

		tmp = tmp->children[index];
		
		i++;
	}

	return tmp;

}

int prefixCount(TrieNode *root, char *str)
{
	int i, len, k = 0, index;
    int count = 0;
	TrieNode *tmp = root;
	
	if(root == NULL)
		return 0;

	len = strlen(str);
	// empty string node count
	if(len == 0)
		{	
			count = prefixHelper(root);
			return count;
		}
   // get prefix location
	else
	{
	tmp = prefixHelper2(root, str);

	count = prefixHelper(tmp);
	}

	return count;
}


double difficultyRating(void)
{
	return 4.0;
}

double hoursSpent(void)
{
	return 20.0;
}


// This acts as the main() function for this test case.
int main(int argc, char **argv)
{
	TrieNode *t = NULL;

	t = buildTrie(argv[1]);
	
	processInputFile(t, argv[2]);

	t = destroyTrie(t);

	return 0;
}


