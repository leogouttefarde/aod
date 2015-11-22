/*! \file applyPatch.c
 *  \brief	   This implements the applyPatch program.
 *  \author    Amrit Kumar 
 *  \author    Jean-Louis Roch
 *  \version   1.0
 *  \date      2015
 *  \warning   Usage: applyPatch patchFile originalFile 
 *  \copyright GNU Public License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static inline int taille_ligne(char *ligne)
{
	if (ligne == NULL)
		return 0;

	// char cur = 0;
	// int i = 0;

	// int count = 0;
	// while (cur != '\n' && (cur = ligne[i++]))
	// 	count++;

	// return count;
	return strlen(ligne);
}

static inline int cout_subst(char *src, char *dest)
{
	if (!strcmp(src, dest)) {
		return 0;
	}

	else {
		return 10 + taille_ligne(dest);
	}
}


/*!
 * Function that implements applyPatch
 * @param  patchFile File pointer to the patchFile.
 * @param  originalFile File pointer to the originalFile.
 * \returns { 0 if succeeds, exit code otherwise}
 */
int computeCost(FILE *patchFile, FILE *originalFile)
{
	char buffer[100]; // should suffice for (Instruction, 32-bit int)
	char instruction; // +,=,d,D
	long int k, m, i; // k is the line number, m is the second parameter of D 
	long int current=0; // the current value of k being read
	long int previous=0; // the value of the k for the last instruction
	int res;
	char *str_patch=NULL; // string to hold the second line of + and =
	char *str_file=NULL;  // string to hold the lines of the original file
	size_t len_patch=0;
	size_t len_file=0;
	unsigned long int cost = 0;

	while(fgets(buffer,100, patchFile)!=NULL) // Read the patch file till the end
	{
		res=sscanf(buffer, "%c %ld %ld", &instruction, &k, &m);
		// works but find a better method to read a variable number of 
		// arguments such as using strtok()
		
		if(res<2) // must be of the format + k or = k or d k or d k m
		{
			fprintf(stderr, "!!!!! instruction must be followed by a line number  !!!!!\n"); 
			exit(EXIT_FAILURE);
		}
		current = k;
		

		if(current<previous) // k must be increasing
		{
			fprintf(stderr, "!!!!! k must be increasing  !!!!!\n"); 
			exit(EXIT_FAILURE);
		}
		
		switch (instruction)
		{
			case '+':
				getline(&str_patch, &len_patch, patchFile);
				cost += 10 + taille_ligne(str_patch);
				// printf("+ : %d\n", 10 + taille_ligne(str_patch));


				if (k==0) ;//printf("%s", str_patch); // insert at the beginning
				else{
					// copy all the intermediate lines including the k-th line of the original file
					for(i=0;i<current-previous;i++) 
					{
						getline(&str_file, &len_file, originalFile);
						// printf("%s",str_file);	
					}
					// printf("%s",str_patch); // insert the line after k-th line of the original file 
				}
				previous=current;
				break;
			case '=':
				if(current<=previous) // k must be strictly increasing
				{
					fprintf(stderr, "!!!!! k must be strictly increasing for =  !!!!!\n"); 
					exit(EXIT_FAILURE);
				}

				getline(&str_patch, &len_patch, patchFile);
				// Copy all intermediate lines of the original file
				// except the k-th line 
				for(i=0;i<current-previous;i++)
				{
					getline(&str_file, &len_file, originalFile);

					// if(i<current-previous-1)
						// printf("%s",str_file);

					if (i == current-previous-1) {
						cost += cout_subst(str_file, str_patch);
				// printf("= : %d  %s > %s\n t=%d", cout_subst(str_file, str_patch), str_file, str_patch, taille_ligne(str_patch));
					}
				}
				// printf("%s",str_patch); // Replace the k-th line
				previous=current;
				break;
			case 'd':
				// printf("d : %d\n", 10);
				cost += 10;
				// Copy all intermediate lines of the original file
				// except the k-th line 
				for(i=0;i<current-previous;i++)
				{
					getline(&str_file, &len_file, originalFile);
					// if(i<current-previous-1)
					// 	printf("%s",str_file);	
				}
				previous=current;
				break;

			case 'D':
				// printf("D : %d\n", 15);
				cost += 15;
				// Format check D k m
				if(res<3)
				{
					fprintf(stderr, "!!!!! m of D k m  is missing  !!!!!\n"); 
					exit(EXIT_FAILURE);
				}
			
				// Copy intermediate lines including the k-1 th line of the
				// original file	
				for(i=0;i<current-previous-1;i++)
				{
					getline(&str_file, &len_file, originalFile);
					// printf("%s",str_file);	
				}
				// Delete m lines starting from the k-th line of the original
				// file
				for(i=0;i<m;i++)
					getline(&str_file, &len_file, originalFile);
				
				previous=current+m-1;
				break;
			
			default:
				fprintf(stderr, "!!!!! patchFile Format Error !!!!!\n"); exit(EXIT_FAILURE);
				break;
		
		}

		// printf("cost = %d\n", cost);
	}	

	printf("%d\n", cost);
	// printf("Patch cost = %d\n", cost);

	// Copy the remaining lines if any
	// while(getline(&str_file, &len_file, originalFile)!=-1) printf("%s",str_file);	
	free(str_patch);
    	free(str_file);
	return 0;

}

/**
 * Main function
 * \brief Main function
 * \param argc  A count of the number of command-line arguments
 * \param argv  An argument vector of the command-line arguments.
 * \warning Must be called with two filenames patchFile, originalFile as commandline parameters and in the given order.
 * \returns { 0 if succeeds, exit code otherwise}
 */
int main (int argc, char *argv[]) {
	FILE *patchFile;
	FILE *originalFile;
	
	if(argc<3){
		fprintf(stderr, "Usage: ./patchCost patchFile originalFile\n");
	    exit(EXIT_FAILURE); /* indicate failure.*/
	}

	patchFile = fopen(argv[1] , "r" );
	originalFile = fopen(argv[2] , "r" );
	
	if (patchFile==NULL) {fprintf(stderr, "Error opening patchFile\n"); exit(EXIT_FAILURE);}
	if (originalFile==NULL) {fprintf (stderr, "Error opening originalFile\n"); exit(EXIT_FAILURE);}

	computeCost(patchFile, originalFile);

	fclose(patchFile);
	fclose(originalFile);
	return 0;
}
