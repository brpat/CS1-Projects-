// Brijesh Patel br168467

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "Ohce.h"

double difficultyRating(void)
{
	return 3.0;
}

double hoursSpent(void)
{
	return 10.0;  
}

int main(int argc, char **argv)
{
	void searchForEcho(char str[], bool *ptr, int num, const char echo[]); 

	int i, j, num;
	char *ptr = NULL, echo[5];
	bool echoValue = false;
	
	strcpy(echo, "echo");
	
	for(i = 1; i < argc; i++)
		{
			ptr = argv[i];  // point to current string argument
			num = strlen(argv[i]); 

			searchForEcho(argv[i], &echoValue, num, echo); 

			for(j = num - 1; j >= 0; j--)
				printf("%c",ptr[j]);
				
			if(i < argc-1) // no space after last string
			printf(" ");	
		}
		
	if(argc > 1) // Only put a new line if there is more then 1 string 
	puts("");   // specifically for UnitTest09.c
	
	if(echoValue == true)
		printf("Echo, echo, echo!\n");
	
	return 0;
}

void searchForEcho(char str[], bool *ptr, int num, const char echo[])
{
	int i = 0, j = 0, counter = 0;

	for(i = 0; i < num; i++)
	{
	while(j < 5) //  echo array constraint
		{
		if(counter <= 4) 
			{
			if(j < 5 && tolower(str[i]) == echo[j]) 
				{
				if(j < 5 &&tolower(str[i+1]) == echo[j+1]) 
					{
					counter++;

					if(counter == 3)
						{
						*ptr = true; // echo found
						return;
						}	
					j++;
					break;
					
					}
				else if(j < 5 && tolower(str[i+1]) != echo[j+1])
					{
					counter = 0; 
					j = 0; 
					break; // echo not found; Atleast not in this string
					}
				}
			}
		break;	
		}	
	}
}