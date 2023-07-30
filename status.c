#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data.h"
#include "main.h"

/* print_result() checks to see if the game is over, and if so,
   prints the result. */

int space()
{
	int i, tempEp, tempSide, whitescore=0, blackscore=0;
	tempSide = side;
	tempEp = ep;

	side = LIGHT;
	ep   =  -1;
	gen();
	for (i = first_move[ply]; i < first_move[ply + 1]; ++i) {
		if (!makemove(gen_dat[i].m.b))
			continue;
		whitescore++;
		takeback();
	}

	side = DARK;
	gen();
	for (i = first_move[ply]; i < first_move[ply + 1]; ++i) {
		if (!makemove(gen_dat[i].m.b))
			continue;
		blackscore++;
		takeback();
	}
	side = tempSide;
	ep = tempEp;

	if (side == LIGHT)
	return whitescore - blackscore;
	return blackscore - whitescore;	   
}
   
void print_result()
{
	int i;
	/* is there a legal move? */
	for (i = 0; i < first_move[1]; ++i)
		if (makemove(gen_dat[i].m.b)) {
			takeback();
			break;
		}
	if (i == first_move[1]) {
		if (side) {
			if ( isattacked( BLACK_KING,0 ) )
				printf("0-1 {Black mates}\n");
			else
				printf("1/2-1/2 {Stalemate}\n");
		} else {
			if ( isattacked( WHITE_KING,1 ) )
				printf("1-0 {White mates}\n");
			else
				printf("1/2-1/2 {Stalemate}\n");
		}
	}
	else if (reps() == 2)
		printf("1/2-1/2 {Draw by repetition}\n");
	else if (fifty >= 100) 
		printf("1/2-1/2 {Draw by fifty move rule}\n");
}

int hash_rand()
{
	int i;
	int r = 0;
	for (i = 0; i < 32; ++i)
		r ^= rand() << i;
	return r;
}

void init_hash()
{
	int i, j;
	srand(0);
	for (i = 0; i < 16; ++i)
		for (j = 0; j < 64; ++j)
			hash_piece[i][j] = hash_rand();
	hash_side = hash_rand();
	for (i = 0; i < 64; ++i)
		hash_ep[i] = hash_rand();
}

void set_hash()
{
	int i;
	hash = 0;	
	for (i = 0; i < 64; ++i)
		if (squares[i] != 0)
			hash ^= hash_piece[squares[i]][i];
	if (side == DARK)
		hash ^= hash_side;
	if (ep != -1)
		hash ^= hash_ep[ep];
}

void display () {
	int i;
	printf("\n ");
	for (i=0; i<64; i++) {
		printf( "%c " , displaycodes[squares[i]] );
		if ( i % 8 == 7 ) { printf("\n "); }
		}
	printf("\n");
}

int parse_move(char *s)
{
	int from, to, i;

	/* make sure the string looks like a move */
	if (s[0] < 'a' || s[0] > 'h' ||
			s[1] < '0' || s[1] > '9' ||
			s[2] < 'a' || s[2] > 'h' ||
			s[3] < '0' || s[3] > '9')
		return -1;

	from = s[0] - 'a';
	from += 8 * (8 - (s[1] - '0'));
	to = s[2] - 'a';
	to += 8 * (8 - (s[3] - '0'));

	for (i = 0; i < first_move[1]; ++i)
		if (gen_dat[i].m.b.from == from && gen_dat[i].m.b.to == to) {

			/* if the move is a promotion, handle the promotion piece;
			   assume that the promotion moves occur consecutively in
			   gen_dat. */
			if (gen_dat[i].m.b.bits & 32)
				switch (s[4]) {
					case 'N':
					case 'n':
						return i;
					case 'B':
					case 'b':
						return i + 1;
					case 'R':
					case 'r':
						return i + 2;
					default:  /* assume it's a queen */
						return i + 3;
				}
			return i;
		}

	/* didn't find the move */
	return -1;
}

/* move_str returns a string with move m in coordinate notation */
char *move_str(move_bytes m)
{
	static char str[6];
	int c;
	if (m.bits & 32) {
		switch (m.promote) {
			case 3:
				c = 'N';
				break;
			case 5:
				c = 'B';
				break;
			case 6:
				c = 'R';
				break;
			case 7:
				c = 'Q';
				break;
			case 11:
				c = 'n';
				break;
			case 13:
				c = 'b';
				break;
			case 14:
				c = 'r';
				break;
			case 15:
				c = 'q';
				break;
			default:
				break;
		}
		sprintf(str, "%c%d%c%d%c",
				COL(m.from) + 'a',
				8 - ROW(m.from),
				COL(m.to) + 'a',
				8 - ROW(m.to),
				c);
	}
	else
		sprintf(str, "%c%d%c%d",
				COL(m.from) + 'a',
				8 - ROW(m.from),
				COL(m.to) + 'a',
				8 - ROW(m.to));
	return str;
}

void init_board(void) {
	int i;	
	BLACK_PAWNS      =  0x000000000000FF00;
	WHITE_PAWNS      =  0x00FF000000000000;
	BLACK_KNIGHTS    =  0x0000000000000042;
	WHITE_KNIGHTS    =  0x4200000000000000;
	BLACK_BISHOPS    =  0x0000000000000024;
	WHITE_BISHOPS    =  0x2400000000000000;
	BLACK_KING       =  0x0000000000000010;
	WHITE_QUEEN      =  0x0800000000000000;
	BLACK_ROOKS      =  0x0000000000000081;
	WHITE_ROOKS      =  0x8100000000000000;
	BLACK_QUEEN      =  0x0000000000000008;
	WHITE_KING       =  0x1000000000000000;
	WHITE_PIECES     =  WHITE_PAWNS  | WHITE_KNIGHTS | WHITE_BISHOPS | WHITE_ROOKS   | WHITE_QUEEN | WHITE_KING ;
	BLACK_PIECES     =  BLACK_QUEEN  | BLACK_PAWNS   | BLACK_KNIGHTS | BLACK_BISHOPS | BLACK_ROOKS | BLACK_KING ;
	OCCUPIED_SQUARES =  WHITE_PIECES | BLACK_PIECES;
	OPEN_SQUARES     =~ OCCUPIED_SQUARES;
	side             =  0;
	xside            =  1;
	ep               =  -1;
	fifty            =  0;
	castle	         =  15;
	ply = 0;
	hply = 0;
	first_move[0] = 0;
	int int_squares[64] = {
						 14,11,13,15,10,13,11,14,
						  9, 9, 9, 9, 9, 9, 9, 9,
						  0, 0, 0, 0, 0, 0, 0, 0,
						  0, 0, 0, 0, 0, 0, 0, 0,
						  0, 0, 0, 0, 0, 0, 0, 0,
						  0, 0, 0, 0, 0, 0, 0, 0,
						  1, 1, 1, 1, 1, 1, 1, 1,
						  6, 3, 5, 7, 2, 5, 3, 6
						  }; 

	for (i=0;i<64;i++) { squares[i] = int_squares[i]; }
	set_hash();
}

void clear_board () {
	int i;
	BLACK_PAWNS      =  0x0000000000000000;
	WHITE_PAWNS      =  0x0000000000000000;
	BLACK_KNIGHTS    =  0x0000000000000000;
	WHITE_KNIGHTS    =  0x0000000000000000;
	BLACK_BISHOPS    =  0x0000000000000000;
	WHITE_BISHOPS    =  0x0000000000000000;
	BLACK_KING       =  0x0000000000000000;
	WHITE_QUEEN      =  0x0000000000000000;
	BLACK_ROOKS      =  0x0000000000000000;
	WHITE_ROOKS      =  0x0000000000000000;
	BLACK_QUEEN      =  0x0000000000000000;
	WHITE_KING       =  0x0000000000000000;
	WHITE_PIECES     =  0x0000000000000000;
	BLACK_PIECES     =  0x0000000000000000;
	OCCUPIED_SQUARES =  0x0000000000000000;
	OPEN_SQUARES     =  0x0000000000000000;
	side             =  0;
	xside            =  1;
	ep               =  -1;
	fifty            =  0;
	castle	         =  0;
	hash			 =  0;
	for( i = 0; i < 64; i++ ) { squares[i] = 0; }
}
