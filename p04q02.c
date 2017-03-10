#include <stdio.h>										
#include <stdlib.h>
#define ROWS 7
#define COLUMNS 16
#define PIXMATX_SIZE 44
#define TOTMATX_SIZE 98
#define myBmp "myBmp.bmp"
#define myBmp_copy "myBmp_copy.bmp"
#define myBmpFlip1 "myBMPFlipped1.bmp"
#define myBmpFlip2 "myBMPFlipped2.bmp"

int bmpData[ROWS][COLUMNS];
int *bmpPtr = &bmpData[0][0];

														// prototype declaration
void loadFile(const char* filename);
void printMenu();
void printArray();
void flipHorizontal();
void flipVertical();
void saveToFile( const char* filename);
void doAll();

														
void loadFile(const char* filename)						// loadFile() uploads bitmap file to array
{
	int i = 0, j = 0;									// index variables for bmpData array
	FILE *fPtr;											// fPtr = myBmp.bmp file pointer

	if ((fPtr = fopen(filename, "r")) == NULL)			// verification check to open file
		puts("File unable to be opened.");
	else
	{
		while (!feof(fPtr))								// scans contents of file and loads them into pointer bmpPtr
		{
			fscanf(fPtr, "%2x", bmpPtr);
			bmpPtr++; 
		}
		fclose(fPtr);									// file close
	}
}

void main()												// main() function
{
	loadFile(myBmp);									// loads origional bmp file
	printMenu();										// calls printmenu for user
}

void printMenu()										// printMenu() creates a recurring menu until user enters 'q'
{
	int in = 0;											// user input
	puts("Enter value to perform function: "); 
	
	while (in != 6)																	// while loop that generates menu untl 'q' is entered
	{
		printf("%-s\n%-s\n%-s\n%-s\n%-s\n%-s\n",									// prints strings to display menu
			"1. Print Array: displays bitmap memory map.",
			"2. Flip Horizontal: flips image horizontally and saves to file.",
			"3. Flip Vertical: flips image vertically and saves to file.",
			"4. Save to File: saves image to a file.",
			"5. Do All: loads, displays, flips horizontally, flips vertically.",
			"6. Quit: quits program.");
		printf("%s", "Command: ");													// asks user to enter a command

		scanf("%d", &in);															// gets user input
		if (!(in == 1 || in == 2 || in == 3 || in == 4 || in == 5 || in == 6))		// data security for proper input
			puts("Value entered incorrect, must be integer value from 1 - 6.\n");   
		else
		{
			printf("%c", '\n');														
			switch (in)																// depending on user input, will call specific functions
			{
			case 1:
				printArray(); break;												// prints bitmap memory map to console
			case 2:
				flipHorizontal(); break;											// flips bitmap image horizontally
			case 3:
				flipVertical(); break;												// flips bitmap image vertically
			case 4:
				saveToFile(myBmp_copy); break;										// saves copy of the image in a file for memory
			case 5:
				doAll(); break;														// performs several tasks
			case 6:
				puts("Termianting..."); exit(0); break;								// quits the program
			}
		}
	}
}

void printArray()										// printArray() displays bitmap memory map to console
{
	int i, j;
	printf("%s", "myBmp.bmp Memory Map: \n");			// information for the user
	for (i = 0; i < ROWS; i++)							// iterates through each row/column and puts character in hexadecimal value
	{
		for (j = 0; j < COLUMNS; j++)
		{
			printf("%-.2X%c", bmpData[i][j], ' ');
		} printf("%c",'\n');
	}
	puts("\n");											// for user readability
}

void flipHorizontal()									// flipHorizontal() transposes the image horizontally
{
	int m = 3, n = 6,					// m = starting row in memory map, n = starting column in memory map
		k = 0, i = 0, tmp;				// k, i = iteration variables, tmp = temporary variable
	int *l, *j;							// *l = first address, *j = last address
	bmpPtr = &bmpData[3][6];			// bmpPtr = points at beginning of memory map location in array
	j = bmpPtr + PIXMATX_SIZE;			// j = last address of memory map
	l = bmpPtr;							// stores address location

	while (bmpPtr < j)					// iterate until last location of memory map
	{
		tmp = *bmpPtr;					// replaces first value with last value
		*bmpPtr = *(bmpPtr + 6);
		*(bmpPtr + 6) = tmp;

		k++; bmpPtr++;					// increments
		if (k == 3)						// if at third increment (full pixel), then starts at next row
		{
			bmpPtr = bmpPtr + 8;
			k = 0;
		}
		i++;	
	}
	bmpPtr = l;							// resets pointer to first address
	
	while (m < ROWS)					// iterates through bmpData array and stores new horizontally flipped values in the pointer
	{
		while (n < COLUMNS)
		{
			bmpData[m][n] = *bmpPtr;
			bmpPtr++; n++;
		}
		m++; n = 0;
	}

	saveToFile(myBmpFlip1);				// save file to myBMPFlipped1.bmp
	puts("\n");
}

void flipVertical()
{		
	int m = 3, n = 6,					// m = starting row in memory map, n = starting column in memory map
		h, g, i = 33, tmp;				// h, g = iteration variables, tmp = temporary variable
	int *l;								// **l = first address
	bmpPtr = &bmpData[3][6];			// bmpPtr = points at beginning of memory map location in array
	l = bmpPtr;							// stores address location

	for (h = 0; h < 2; h++)				// iterates through 2 rows
	{
		for (g = 0; g < 11; g++)		// iterates through all columns
		{
			tmp = *bmpPtr;				// replaces first value with last value
			*bmpPtr = *(bmpPtr + i);
			*(bmpPtr + i) = tmp;
			bmpPtr++;
		} 
		i = 11;							// distance between elements changes after first row, sets for new value
	}

	bmpPtr = l;							// resets pointer to first address
	while (m < ROWS)					// sets array with new vertically flipped values stores in pointer
	{
		while (n < COLUMNS)
		{
			bmpData[m][n] = *bmpPtr;
			bmpPtr++;
			n++;
		}
		m++; n = 0;
	}
	saveToFile(myBmpFlip2);				// saves to file myBmpFlipped2.bmp
	puts("\n");
}

void saveToFile( const char* filename )				// takes file name as parameter to save memory map	
{
	int i = 0, j, k;								// i, j, k iteration variables
	FILE *sfPtr;									// file pointer
	if ((sfPtr = fopen(filename, "w")) == NULL)		// points file pointer to starting location of file if file exists
		puts("File could not be opened.");			// if no file, displays error message
	else
	{
		for (j = 0; j < ROWS; j++)					// iterates through rows
		{
			for (k = 0; k < COLUMNS; k++)			// iterates through columns
				{
				fprintf(sfPtr, "%.2X%c",			// prints to file contents of array in hexidecimal values
					bmpData[j][k], ' ');
				i++;
			}
			fprintf(sfPtr, "%c", '\n');				// creates a new carriage after every column has been printed
		}
	}
	fclose(sfPtr);									// closes the file
}

void doAll()					// doAll() does a sequence of tasks specified below
{
	loadFile(myBmp);			// loads the original memory map file
	printArray();				// displays the original memory map
	flipHorizontal();			// flips the memory map horizontally
	loadFile(myBmpFlip1);		// loads the flipped horizontal memory map
	printArray();				// displays the contents of the flipped horizontal memory mapa
	flipVertical();				// flips the horizontal memory map vertically, (so the original is now horizontally and vertically flipped)
	loadFile(myBmpFlip2);		// loads the vertically and horizontally memory map
	printArray();				// displays the vertically and horizontally memory map
}