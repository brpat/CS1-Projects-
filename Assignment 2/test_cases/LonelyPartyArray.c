// Brijesh Patel br168467

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LonelyPartyArray.h"


LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
	LPA *party;
	int i, max = num_fragments * fragment_length;

	if(num_fragments <= 0 || fragment_length <= 0)
		return NULL;

	party = malloc(sizeof(LonelyPartyArray));  // create lonely party array struct

	if(party == NULL)
	{
		return NULL;
	}
	party->num_fragments = num_fragments;
	party->fragment_length = fragment_length;
	party->num_active_fragments = 0;
	party->size = 0;

	party->fragments = malloc(sizeof(int *) * (num_fragments));  // create array of fragment pointers

	if(party->fragments == NULL)
	{
		free(party);  // free up until this point
		return NULL;
	}

	for(i =0; i < num_fragments; i++)
	{
		party->fragments[i] = NULL;   
	}

	party->fragment_sizes = malloc(sizeof(int) *(num_fragments));

	if(party->fragment_sizes == NULL)
	{
		free(party->fragments);
		free(party);
	}

	for(i = 0; i < num_fragments; i++)
	{
		party->fragment_sizes[i] = 0;
	}
	
	printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n",max, num_fragments);
	
	
	return party;
}

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
	int i;
	if(party == NULL)
	{
		return NULL;
	}

	if(party->fragment_sizes != NULL) 
		free(party->fragment_sizes);
		
	if(party->fragments != NULL)
		{
			for(i = 0; i < party->num_fragments; i++)
			{
				if(party->fragments[i] != NULL)
					free(party->fragments[i]);
			}	
			free(party->fragments);
		}	
	
	free(party);
	
	printf("-> The LonelyPartyArray has returned to the void.\n");

	return NULL;
}

int set(LonelyPartyArray *party, int index, int key)
{
	int row, col, j, min, max; 
	bool emptyCell = true;

	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in set().\n");
		return LPA_FAILURE;
	}

	
	row = index / party->fragment_length;
	row = row -(row % 1);  

	min = row * party->fragment_length;  // get lower index;
	max = (party->fragment_length-1) + min; // get upper index;

	col = index - min; 

	if(index < 0 || index > (party->num_fragments * party->fragment_length) -1 )
	{
		printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n",index,row,col);
		return LPA_FAILURE;
	}

	if(party->fragments[row] == NULL)  // allocated NULL fragment
	 {
	 	party->fragments[row] = malloc(sizeof(int) * (party->fragment_length));
	 	party->num_active_fragments++;  // update number of allocated fragments 	

	 	for(j = 0; j < party->fragment_length; j++)
	 	{
	 		party->fragments[row][j] = UNUSED;  // set all cells in fragment to UNUSED 
	 	}
	 	printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n",row, party->fragment_length, min, max);
	 }

	 if(party->fragments[row][col] == UNUSED)
	 	emptyCell = false;

	  party->fragments[row][col] = key; // set key into fragment cell
	  	if(emptyCell == false)
	  	{
	  		party->size++;
	  		party->fragment_sizes[row]++;
	  	}
	 
	 return LPA_SUCCESS;
	
}


int get(LonelyPartyArray *party, int index)
{
	int row, col, j, min, max; 

	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in get().\n");
		return LPA_FAILURE;
	}

	row = index / party->fragment_length; // location of index
	row = row -(row % 1); 

	min = row * party->fragment_length;  // get lower index;
	max = (party->fragment_length-1) + min; // get upper index;

	col = index - min; 



	if(row >= 0 && col >=0 && row < party->num_fragments)
	{	
		if(party->fragments[row] == NULL )  // if index is in bounds but is UNUSED
			return UNUSED;
	}

	
	if(index < 0 || index > (party->num_fragments * party->fragment_length) -1 )
	{	
		printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n",index,row,col);
		return LPA_FAILURE;
	}

	return party->fragments[row][col];
}

int delete(LonelyPartyArray *party, int index)
{
	int row, col, j, min, max; 

	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in delete().\n");
		return LPA_FAILURE;
	}

	row = index / party->fragment_length; // location of index
	row = row -(row % 1);  

	min = row * party->fragment_length;  // get lower index;
	max = (party->fragment_length-1) + min; // get upper index;

	col = index - min;

	if(index < 0 || index > (party->num_fragments * party->fragment_length) -1 )
	{
		printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n",index,row,col);
		return LPA_FAILURE;
	} 

	if(party->fragments[row] == NULL) 
		return LPA_FAILURE;


	if(party->fragments[row][col] == UNUSED) // if cell is already empty
		return LPA_FAILURE;

	party->fragments[row][col] = UNUSED;

	
	party->size--;
	party->fragment_sizes[row]--;
	
	if(party->fragment_sizes[row] == 0)   // no more used cells in fragment
	{
		free(party->fragments[row]);
		party->fragments[row] = NULL;
		party->num_active_fragments--;
		printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", row, party->fragment_length, min, max);
		return LPA_SUCCESS;
	}
	return LPA_SUCCESS;
}

int printIfValid(LonelyPartyArray *party, int index)
{
	int row, col, j, min, max; 

	if(party == NULL)
		return LPA_FAILURE;

	row = index / party->fragment_length; // location of index
	row = row -(row % 1); 

	min = row * party->fragment_length;  // get lower index;
	max = (party->fragment_length-1) + min; // get upper index;

	col = index - min;

	if(index < 0 || index > (party->num_fragments * party->fragment_length) -1 )
		return LPA_FAILURE;

	if(party->fragments[row] == NULL) 
		return LPA_FAILURE;

	
	if(party->fragments[row][col] == UNUSED)
		return LPA_FAILURE;

	printf("%d\n",party->fragments[row][col]);

	return LPA_SUCCESS;
}

LonelyPartyArray *resetLonelyPartyArray(LPA *party)
{
	int i, total = party->num_fragments * party->fragment_length;
	if(party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
		return party;
	}


	for(i = 0; i < party->num_fragments; i++)  // deallocate and reset values of fragments array 
	{
		if(party->fragments[i] != NULL)  // deallocate only active fragments
		{
			free(party->fragments[i]);
			party->fragments[i] = NULL;
		}
		
	}
	
	for(i = 0; i < party->num_fragments; i++) // reset fragment_sizes array
		party->fragment_sizes[i] = 0;
	

	party->num_active_fragments = 0;
	party->size = 0;

	printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", total, party->num_fragments);

	return party;

}

int getSize(LonelyPartyArray *party)
{
	if(party == NULL)
		return -1;

	return party->size;
}

int getCapacity(LonelyPartyArray *party)
{
	if(party == NULL)
		return -1;

	return party->num_fragments * party->fragment_length;
	
}

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
	if(party == NULL)
		return 0;

	return party->num_fragments * (long long unsigned int)(party->fragment_length * sizeof(int));;
}

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
	long long unsigned int totalSize = 0, i, j;

	if(party == NULL)
		return totalSize;

	totalSize+= (long long unsigned int)sizeof(party);  // size of struct pointer
	totalSize+= (long long unsigned int)sizeof(*party); // size of struct

		for(i = 0; i < party->num_fragments; i++)
		{
			totalSize+= (long long unsigned int)sizeof(party->fragments[i]);  // size of fragments pointers

			if(party->fragments[i] != NULL) // only get active fragments size
			{
				totalSize+= (long long unsigned int)party->fragment_length * sizeof(int);
			}
		}
		totalSize+= (long long unsigned int)party->num_fragments *sizeof(int); // get size of fragment_sizes array
		
	return totalSize;

}

int getAllocatedCellCount(LonelyPartyArray *party)
{
	return party->num_active_fragments * party->fragment_length;
}

double difficultyRating(void)
{
	return 4.0;
}

double hoursSpent(void)
{
	return 30.0;
}




