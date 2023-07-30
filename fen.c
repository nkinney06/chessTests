// This will only have effect on Windows with MSVC
#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS 1
    #define restrict __restrict
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "data.h"
#include "main.h"
#include "sliding.h"
#include "stepping.h"

int64_t my_getline(char **restrict line, size_t *restrict len, FILE *restrict fp) {
    // Check if either line, len or fp are NULL pointers
    if(line == NULL || len == NULL || fp == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    // Use a chunk array of 128 bytes as parameter for fgets
    char chunk[128];

    // Allocate a block of memory for *line if it is NULL or smaller than the chunk array
    if(*line == NULL || *len < sizeof(chunk)) {
        *len = sizeof(chunk);
        if((*line = malloc(*len)) == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }
	// "Empty" the string
    (*line)[0] = '\0';

    while(fgets(chunk, sizeof(chunk), fp) != NULL) {
        // Resize the line buffer if necessary
        size_t len_used = strlen(*line);
        size_t chunk_used = strlen(chunk);

        if(*len - len_used < chunk_used) {
            // Check for overflow
            if(*len > SIZE_MAX / 2) {
                errno = EOVERFLOW;
                return -1;
            } else {
                *len *= 2;
            }
            
            if((*line = realloc(*line, *len)) == NULL) {
                errno = ENOMEM;
                return -1;
            }
        }

        // Copy the chunk to the end of the line buffer
        memcpy(*line + len_used, chunk, chunk_used);
        len_used += chunk_used;
        (*line)[len_used] = '\0';

        // Check if *line contains '\n', if yes, return the *line length
        if((*line)[len_used - 1] == '\n') {
            return len_used;
        }
    }

    return -1;
}

void setupFen(char *fen, char *fencolor, char *fencastling, char *fenenpassant, int fenhalfmoveclock, int fenfullmovenumber)
{
	int i, myfile, myrank, counter;
	 
	clear_board();
	myfile    = 1;
	myrank    = 8;
	i         = 0;
	counter   = 0;
	fifty     = fenhalfmoveclock;
	hply      = fenfullmovenumber;

	while ((counter < 64) && (fen[i] != '\0')) {	// '1' through '8':

		if (((int) fen[i] > 48) && ((int) fen[i] < 57)) {
			myfile+= (int) fen[i] - 48;
			counter+= (int) fen[i] - 48;
			} else {
				switch (fen[i]) {

				case '/':
				myrank--;
				myfile = 1;
				break;

				case 'P':
				WHITE_PAWNS |= bitset[counter];
				squares[counter] = W_PAWN;
				myfile += 1;
				counter += 1;
				break;

				case 'N':
				WHITE_KNIGHTS |= bitset[counter];
				squares[counter] = W_KNIGHT;
				myfile += 1;
				counter += 1;
				break;

				case 'B':
				WHITE_BISHOPS |= bitset[counter];
				squares[counter] = W_BISHOP;
				myfile += 1;
				counter += 1;
				break;

				case 'R':
				WHITE_ROOKS |= bitset[counter];
				squares[counter] = W_ROOK;
				myfile += 1;
				counter += 1;
				break;

				case 'Q':
				WHITE_QUEEN |= bitset[counter];
				squares[counter] = W_QUEEN;
				myfile += 1;
				counter += 1;
				break;

				case 'K':
				WHITE_KING |= bitset[counter];
				squares[counter] = W_KING;
				myfile += 1;
				counter += 1;
				break;

				case 'p':
				BLACK_PAWNS |= bitset[counter];
				squares[counter] = B_PAWN;
				myfile += 1;
				counter += 1;
				break;

				case 'n':
				BLACK_KNIGHTS |= bitset[counter];
				squares[counter] = B_KNIGHT;
				myfile += 1;
				counter += 1;
				break;

				case 'b':
				BLACK_BISHOPS |= bitset[counter];
				squares[counter] = B_BISHOP;
				myfile += 1;
				counter += 1;
				break;

				case 'r':
				BLACK_ROOKS |= bitset[counter];
				squares[counter] = B_ROOK;
				myfile += 1;
				counter += 1;
				break;

				case 'q':
				BLACK_QUEEN |= bitset[counter];
				squares[counter] = B_QUEEN;
				myfile += 1;
				counter += 1;
				break;

				case 'k':
				BLACK_KING |= bitset[counter];
				squares[counter] = B_KING;
				myfile += 1;
				counter += 1;
				break;

				default:
				break;
				
				}
			}
		i++;
	}
 
	WHITE_PIECES     =  WHITE_PAWNS  | WHITE_KNIGHTS | WHITE_BISHOPS | WHITE_ROOKS   | WHITE_QUEEN | WHITE_KING ;
	BLACK_PIECES     =  BLACK_QUEEN  | BLACK_PAWNS   | BLACK_KNIGHTS | BLACK_BISHOPS | BLACK_ROOKS | BLACK_KING ;
	OCCUPIED_SQUARES =  WHITE_PIECES | BLACK_PIECES;
	OPEN_SQUARES     =~ OCCUPIED_SQUARES;

	if (fencolor[0] == 'b') { side = DARK; } else { side = LIGHT; }
	xside = side ^ 1;

	if (strstr(fencastling, "K")) { castle  += 1; }
	if (strstr(fencastling, "Q")) { castle  += 2; }
	if (strstr(fencastling, "k")) { castle  += 4; }
	if (strstr(fencastling, "q")) { castle  += 8; }
	 
	 if ( strstr(fenenpassant, "-") ) {ep = -1; } else {
		 ep = fenenpassant[0] - 'a';
		 ep += 8 * (8 - (fenenpassant[1] - '0'));
		 }
	set_hash();
	
	return;
}

void readFen( char *line ) {
		int fieldNum = 0;
		char fen[100];
		char fencolor[2]; 
		char fencastling[5];
		char fenenpassant[3];
		int fenhalfmoveclock;
		int fenfullmovenumber;
		char * word;
		word = strtok(line," ");
		while (word != NULL) {
			fieldNum++;
			switch(fieldNum) {
				case 1:
					strcpy(fen,word);
					break;
				case 2:
					strcpy(fencolor,word);
					break;
				case 3:
					strcpy(fencastling,word);
					break;
				case 4:
					strcpy(fenenpassant,word);
					break;
				case 5:
					sscanf(word, "%d", &fenhalfmoveclock);
					break;
				case 6:
					sscanf(word, "%d", &fenfullmovenumber);
					break;
				default:
					break;
				}
			word = strtok (NULL," ");
			}
	setupFen(fen, fencolor, fencastling, fenenpassant, fenhalfmoveclock, fenfullmovenumber);
	return;
}

void readEcoPositions(const char * myfilename, int number) {
	int lineNum,fieldNum;
	char fen[100];
	char fencolor[2]; 
	char fencastling[5];
	char fenenpassant[3];
	FILE * fp;
	char * fields;
	char * line = NULL;
	size_t len = 0;
	char temp[100];
	ssize_t read;
	clear_board();

	fp = fopen("experiment5.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	lineNum = 0;
	while ((read = my_getline(&line, &len, fp)) != -1) {
		lineNum++;

		if (lineNum == number) {
			fieldNum = 0;
			fields = strtok(line,",");
			while (fields != NULL) {
				fieldNum++;
				switch(fieldNum) {
					case 4:
						strcpy(temp,fields);
						break;
					case 3:
						strcpy(eco,fields);
						break;
					case 2:
						strcpy(variation,fields);
						break;
					case 1:
						strcpy(opening,fields);
						break;
					default:
						break;
					}
				fields = strtok (NULL,",");
				}
			}	
		}
	fclose(fp);
	fieldNum = 0;
	fields = strtok(temp," ");
	while (fields != NULL) {
		fieldNum++;
		switch(fieldNum) {
			case 1:
				strcpy(fen,fields);
				break;
			case 2:
				strcpy(fencolor,fields);
				break;
			case 3:
				strcpy(fencastling,fields);
				break;
			case 4: 
				strcpy(fenenpassant,fields);
				break;
			default:
				break;
			}
		fields = strtok (NULL," ");
		}
		setupFen(fen, fencolor, fencastling, fenenpassant, 0, 0);
	return;
}

void readProblem(int number) {
	int lineNum,fieldNum,end;
	FILE * fp;
	char * fields;
	char * line = NULL;
	size_t len = 0;
	char temp[100];
	ssize_t read;
	clear_board();
	fp = fopen("experiment6.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	
	lineNum = 0;
	while ((read = my_getline(&line, &len, fp)) != -1) {
		lineNum++;
		if ( lineNum > number )
			break;
		if (lineNum == number) {
			fieldNum = 0;
			fields = strtok(line,",");
			while (fields != NULL) {
				fieldNum++;
				switch(fieldNum) {
					case 5: // the solution to the problem
						strcpy(solution,fields);
						end = strlen(solution); 
						if( solution[end-1] == '\n' ) solution[end-1] = 0;
						break;
					case 4: // the fen string
						strcpy(temp,fields);
						break;
					case 3: // the opening
						strcpy(opening,fields);
						break;
					case 2: // the eco code
						strcpy(eco,fields);
						break;
					case 1: // the rating
						strcpy(rating,fields);
						break;
					default:
						break;
					}
				fields = strtok (NULL,",");
				}
			}
		} // finished reading the fields in experiment6.txt file, now set up the fen
	fclose(fp);
	readFen(temp);
	return;
}

void readTestPositions(const char * myfilename, int number) {
	int lineNum,fieldNum;
	char fen[100];
	char fencolor[2]; 
	char fencastling[5];
	char fenenpassant[3];
	FILE * fp;
	char * fields;
	char * line = NULL;
	size_t len = 0;
	char temp[100];
	ssize_t read;
	clear_board();

	fp = fopen("testPositions.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	lineNum = 0;
	while ((read = my_getline(&line, &len, fp)) != -1) {
		lineNum++;

		if (lineNum == number) {

			fieldNum = 0;
			fields = strtok(line,",");
			while (fields != NULL) {
				fieldNum++;
				switch(fieldNum) {
					case 3:
						strcpy(temp,fields);
						break;
					case 2:
						strcpy(eco,fields);
						break;
					case 1:
						strcpy(opening,fields);
						break;
					default:
						break;
					}
				fields = strtok (NULL,",");
				}
			}	
		}
	fclose(fp);
	fieldNum = 0;
	fields = strtok(temp," ");
	while (fields != NULL) {
		fieldNum++;
		switch(fieldNum) {
			case 1:
				strcpy(fen,fields);
				break;
			case 2:
				strcpy(fencolor,fields);
				break;
			case 3:
				strcpy(fencastling,fields);
				break;
			case 4: 
				strcpy(fenenpassant,fields);
				break;
			default:
				break;
			}
		fields = strtok (NULL," ");
		}
		setupFen(fen, fencolor, fencastling, fenenpassant, 0, 0);
	return;
}

void perft() {
	char s[256];
	int i, j, totalPositions = 7;
	for ( i = 0 ; i < totalPositions ; i++ ) {
		switch(i) {
			case 0:
				strcpy(s,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
				printf("\nrunning preformance test for %s\n",s);
				readFen(s);
				for (j=1;j<6;j++) {
					retVal = 0; retCap = 0; retEpp = 0;
					enumerate(j);
					printf("nodes: %llu\tcaps: %llu\tepp: %llu\n",retVal,retCap,retEpp);
				}
				break;
			case 1:
				strcpy(s,"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");
				printf("\nrunning preformance test for %s\n",s);
				readFen(s);
				for (j=1;j<6;j++) {
					retVal = 0; retCap = 0; retEpp = 0;
					enumerate(j);
					printf("nodes: %llu\tcaps: %llu\tepp: %llu\n",retVal,retCap,retEpp);
				}
				break;
			case 2:
				strcpy(s,"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 0");
				printf("\nrunning preformance test for %s\n",s);
				readFen(s);
				for (j=1;j<6;j++) {
					retVal = 0; retCap = 0; retEpp = 0;
					enumerate(j);
					printf("nodes: %llu\tcaps: %llu\tepp: %llu\n",retVal,retCap,retEpp);
				}
				break;
			case 3:
				strcpy(s,"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
				printf("\nrunning preformance test for %s\n",s);
				readFen(s);
				for (j=1;j<6;j++) {
					retVal = 0; retCap = 0; retEpp = 0;
					enumerate(j);
					printf("nodes: %llu\tcaps: %llu\tepp: %llu\n",retVal,retCap,retEpp);
				}
				break;				
			case 4:
				strcpy(s,"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 49 8");
				printf("\nrunning preformance test for %s\n",s);
				readFen(s);
				for (j=1;j<6;j++) {
					retVal = 0; retCap = 0; retEpp = 0;
					enumerate(j);
					printf("nodes: %llu\tcaps: %llu\tepp: %llu\n",retVal,retCap,retEpp);
				}
				break;
			case 5:
				strcpy(s,"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
				printf("\nrunning preformance test for %s\n",s);
				readFen(s);
				for (j=1;j<6;j++) {
					retVal = 0; retCap = 0; retEpp = 0;
					enumerate(j);
					printf("nodes: %llu\tcaps: %llu\tepp: %llu\n",retVal,retCap,retEpp);
				}
				break;
			case 6:
				strcpy(s,"rnbqkb1r/pppp1ppp/5n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R b KQkq - 3 5");
				printf("\nrunning preformance test for %s\n",s);
				readFen(s);
				for (j=1;j<6;j++) {
					retVal = 0; retCap = 0; retEpp = 0;
					enumerate(j);
					printf("nodes: %llu\tcaps: %llu\tepp: %llu\n",retVal,retCap,retEpp);
				}
				break;
		}
	}
}

void print_fen () {
	char fen[100];
	int empty_counter,i;
	strcpy(fen,"");
	empty_counter = 0;

	for (i=0; i<64; i++) {
		
		if ( ( empty_counter > 0 ) & ( squares[i] > 0 ) ) { 
			printf("%d",empty_counter); 
			empty_counter = 0; 
			}
		
		if ( ( i%8 == 0 ) & ( i > 0 ) ) { 
			if ( empty_counter > 0 ) { 
				printf("%d",empty_counter); 
				empty_counter = 0;
				}
			printf("%s","/"); 
			} 
		
		switch (squares[i]) {
		
			case 0:
			empty_counter++;
			break;
			
			case 1:
			printf("%s","P");
			break;
			
			case 3:
			printf("%s","N");
			break;
			
			case 5:
			printf("%s","B");
			break;
			
			case 6:
			printf("%s","R");
			break;
			
			case 7:
			printf("%s","Q");
			break;
			
			case 2:
			printf("%s","K");
			break;
			
			case 9:
			printf("%s","p");
			break;
			
			case 11:
			printf("%s","n");
			break;
			
			case 13:
			printf("%s","b");
			break;
			
			case 14:
			printf("%s","r");
			break;
			
			case 15:
			printf("%s","q");	
			break;
			
			case 10:
			printf("%s","k");
			break;
			
			default:		
			break;
			
			}
		}
		
		printf(" %s ", side ? "b" : "w" );
		
		if ( castle > 0 ) {
			if ( castle & 1 ) { printf("K"); }
			if ( castle & 2 ) { printf("Q"); }
			if ( castle & 4 ) { printf("k"); }
			if ( castle & 8 ) { printf("q"); }
		} else { printf("-"); }
			
		if ( ep > 0 ) { 
			printf(" %c%d ",COL(ep) + 'a',8 - ROW(ep));
			} else { printf(" %s ","-"); }
		
		printf("%d ",fifty);
		printf("%d",hply);
		return;
}