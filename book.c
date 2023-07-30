#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data.h"
#include "main.h"

/* the opening book file, declared here so we don't have to include stdio.h in
   a header file */
   
FILE *book_file_white;
FILE *book_file_black;

void open_book()
{
	srand(time(NULL));
	book_file_white = fopen("bookWhite.txt", "r");
	book_file_black = fopen("bookBlack.txt", "r");
	if ( !book_file_white || !book_file_black )
		printf("Opening book missing.\n");
}

/* close_book() closes the book file. This is called when the program exits. */
void close_book()
{
	if (book_file_white)
		fclose(book_file_white);
	book_file_white = NULL;
	if (book_file_black)
		fclose(book_file_black);
	book_file_black = NULL;
}

/* book_move() returns a book move (in integer format) or -1 if there is no
   book move. */

int book_move()
{
	char line[256];
	char book_line[256];
	int i, j, m;
	int move[50];   /* the possible book moves */
	int count[50];  /* the number of occurrences of each move */
	int moves = 0;
	int total_count = 0;
	
	if (!book_file_black || !book_file_white || hply > 40)
		return -1;
	
	/* line is a string with the current line, e.g., "e2e4 e7e5 g1f3 " */
	line[0] = '\0';
	j = 0;
	for (i = 0; i < hply; ++i)
		j += sprintf(line + j, "%s ", move_str(hist_dat[i].m.b));
	
	/* compare line to each line in the opening book */
	fseek( side == LIGHT ? book_file_white : book_file_black, 0, SEEK_SET);
	while (fgets(book_line, 256, side == LIGHT ? book_file_white : book_file_black)) {
		if (book_match(line, book_line)) {
			
			/* parse the book move that continues the line */
			m = parse_move(&book_line[strlen(line)]);
			if (m == -1)
				continue;
			m = gen_dat[m].m.u;
			
			/* add the book move to the move list, or update the move's
			   count */
			for (j = 0; j < moves; ++j)
				if (move[j] == m) {
					++count[j];
					break;
				}
			if (j == moves) {
				move[moves] = m;
				count[moves] = 1;
				++moves;
			}
			++total_count;
		}
	}
				
	/* no book moves? */
	if (moves == 0)
		return -1;
				
	/* Think of total_count as the set of matching book lines.
	   Randomly pick one of those lines (j) and figure out which
	   move j "corresponds" to. */
	j = rand() % total_count;
	for (i = 0; i < moves; ++i) {
		j -= count[i];
		if (j < 0)
			return move[i];
	}
	return -1;  /* shouldn't get here */
}

/* follows line number from white/black book */
void follow_line( int number )
{
	int lineNum,m;
	FILE * fp;
	char * word;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen(side == LIGHT ? "bookWhite.txt" : "bookBlack.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	lineNum = 0;
	while ((read = my_getline(&line, &len, fp)) != -1) {
		if (lineNum == number) {
			word = strtok (line," ");
			while (word != NULL) {
				m = parse_move(word);
				if (m == -1 || !makemove(gen_dat[m].m.b))
					printf("Illegal move %s.\n\n",word);
				else {
					ply = 0;
					gen();
					print_result();
				}
				word = strtok (NULL," ");
				}
			}
			lineNum++;
		}
	fclose(fp);
}

/* book_match() returns TRUE if the first part of s2 matches s1. */

BOOL book_match(char *s1, char *s2)
{
	int i;

	for (i = 0; i < (signed int)strlen(s1); ++i)
		if (s2[i] == '\0' || s2[i] != s1[i])
			return FALSE;
	return TRUE;
}
				
				
				
				
				
				
				
				
				
				
				
				
			