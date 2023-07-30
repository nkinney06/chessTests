#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/timeb.h>
#include "data.h"
#include "sliding.h"
#include "stepping.h"
#include "main.h"
#include "knowledge.h"

int check_result()
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
				return 1; 
			else
				return 0; 
		} else {
			if ( isattacked( WHITE_KING,1 ) )
				return -1; 
			else
				return 0; 
		}
	}
	else if (reps() == 2)
		return 0; 
	else if (fifty >= 100)
		return 0; 
	else
		return -2; // game is not over
}

////////////////////////////////////////////////////////////////////////
// test parameters using 1000 test positions
////////////////////////////////////////////////////////////////////////
void experiment1() {
	int i, j, m, n, finalScore;
	char param[256];
	printf("parameter\tvalue\tscore\teco\tvariation\n");
	for ( i = 0 ; i < 12 ; i++ ) { // set parameter values to 0	
		PENALTY_DOUBLED_PAWN_ONE = 0;
		PENALTY_ISOLATED_PAWN_ONE = 0;
		PENALTY_BACKWARD_PAWN_ONE = 0;
		BONUS_PASSED_PAWN_ONE = 0;
		ROOK_OPEN_FILE_BONUS_ONE = 0;
		ROOK_ON_SEVENTH_BONUS_ONE = 0;
		BONUS_ROOK_BEHIND_PASSED_PAWN_ONE = 0;
		BONUS_BISHOP_PAIR_ONE = 0;
		BAD_BISHOP_PENALTY_ONE = 0;
		GOOD_KNIGHT_BONUS_ONE = 0;
		BONUS_MOVE_OPTIONS_ONE = 0;
		ROOK_SEMI_OPEN_FILE_BONUS_ONE = 0;
							
		for ( j = 0 ; j <= 50 ; j++ ) {
			switch(i) { // incriment the parameter value
				case 0:
					BONUS_MOVE_OPTIONS_ONE = j;
					strcpy(param,"SPACE");
					break;
				case 1:
					PENALTY_DOUBLED_PAWN_ONE = j;
					strcpy(param,"DOUBLED_PAWN");
					break;
				case 2:
					PENALTY_ISOLATED_PAWN_ONE = j;
					strcpy(param,"ISOLATED_PAWN");
					break;
				case 3:
					PENALTY_BACKWARD_PAWN_ONE = j;
					strcpy(param,"BACKWARD_PAWN");
					break;
				case 4: 
					BONUS_PASSED_PAWN_ONE = j;
					strcpy(param,"PASSED_PAWN");
					break;
				case 5:
					ROOK_OPEN_FILE_BONUS_ONE = j;
					strcpy(param,"ROOK_OPEN_FILE");
					break;
				case 6:
					ROOK_ON_SEVENTH_BONUS_ONE = j;
					strcpy(param,"ROOK_ON_SEVENTH");
					break;
				case 7:
					BONUS_ROOK_BEHIND_PASSED_PAWN_ONE = j;
					strcpy(param,"ROOK_PASSEED_PAWN");
					break;
				case 8:
					BONUS_BISHOP_PAIR_ONE = j;
					strcpy(param,"BISHOP_PAIR");
					break;
				case 9:
					BAD_BISHOP_PENALTY_ONE = j;
					strcpy(param,"BAD_BISHOP");
					break;
				case 10:
					GOOD_KNIGHT_BONUS_ONE = j;
					strcpy(param,"GOOD_KNIGHT");
					break;
				case 11:
					ROOK_SEMI_OPEN_FILE_BONUS_ONE = j;
					strcpy(param,"ROOK_SEMI_OPEN");
					break;
				default:
					break;
			}
		
			for ( m = 0 ; m < 1000 ; m++ ) {
				finalScore = 0;
				for ( n = 0 ; n < 2 ; n++ ) {
					init_board();
					readTestPositions("testPositions.txt",m+1);
					gen();
					int gameResult = -2, numMoves = 0;
					while ( gameResult < -1 ) {  /* computer's turn */
						if ( ( n % 2 ) == 0 ) { // player one is white in even games 
							if (side == LIGHT)
								evalPtr = &evalOne;
							else
								evalPtr = &evalTwo;
						} else {
							if (side == DARK)
								evalPtr = &evalOne;
							else
								evalPtr = &evalTwo;
						}
						think(0);
						makemove(pv[0][0].b);
						numMoves++;
						ply = 0;
						gen(); // must be called before check_result()
						gameResult = check_result();
						if ( numMoves > 200 )
							gameResult = 0; // draw for extra long games
						}
						if ( ( n % 2 ) == 0 ) {   // player one is white
							if (gameResult == 1)  // player one wins
								finalScore += 1;
							if (gameResult == -1) // player two wins
								finalScore -= 1;	
						} else {
							if (gameResult == -1) // player one wins
								finalScore += 1;
							if (gameResult == 1)  // player two wins
								finalScore -= 1;
						}
				}
				printf("%s\t%d\t%d\t%s\t%s\n",param,j,finalScore,eco,opening);
			}
		}
	}	
	exit(0);
}

////////////////////////////////////////////////////////////////////////
// use the balsa500 opening lines to test all 12 parameters
////////////////////////////////////////////////////////////////////////
void experiment2() {
	int i, j, k, finalScore = 0;
	float data[10];
	char param[256];
	printf("param\tvalue\tmean\tlower\tupper\n");
	for ( k = 0 ; k < 12 ; k++ ) { // set parameter values to 0	
		PENALTY_DOUBLED_PAWN_ONE = 0;
		PENALTY_ISOLATED_PAWN_ONE = 0;
		PENALTY_BACKWARD_PAWN_ONE = 0;
		BONUS_PASSED_PAWN_ONE = 0;
		ROOK_OPEN_FILE_BONUS_ONE = 0;
		ROOK_ON_SEVENTH_BONUS_ONE = 0;
		BONUS_ROOK_BEHIND_PASSED_PAWN_ONE = 0;
		BONUS_BISHOP_PAIR_ONE = 0;
		BAD_BISHOP_PENALTY_ONE = 0;
		GOOD_KNIGHT_BONUS_ONE = 0;
		BONUS_MOVE_OPTIONS_ONE = 0;
		ROOK_SEMI_OPEN_FILE_BONUS_ONE = 0;
		
		for ( j = 0 ; j <= 50 ; j++ ) {
			switch(k) { // incriment the parameter value
				case 0:
					BONUS_MOVE_OPTIONS_ONE = j;
					strcpy(param,"SPACE");
					break;
				case 1:
					ROOK_SEMI_OPEN_FILE_BONUS_ONE = j;
					strcpy(param,"ROOK_SEMI_OPEN_FILE");
					break;
				case 2:
					PENALTY_DOUBLED_PAWN_ONE = j;
					strcpy(param,"DOUBLED_PAWN");
					break;
				case 3:
					PENALTY_ISOLATED_PAWN_ONE = j;
					strcpy(param,"ISOLATED_PAWN");
					break;
				case 4:
					PENALTY_BACKWARD_PAWN_ONE = j;
					strcpy(param,"BACKWARD_PAWN");
					break;
				case 5: 
					BONUS_PASSED_PAWN_ONE = j;
					strcpy(param,"PASSED_PAWN");
					break;
				case 6:
					ROOK_OPEN_FILE_BONUS_ONE = j;
					strcpy(param,"ROOK_OPEN_FILE");
					break;
				case 7:
					ROOK_ON_SEVENTH_BONUS_ONE = j;
					strcpy(param,"ROOK_ON_SEVENTH");
					break;
				case 8:
					BONUS_ROOK_BEHIND_PASSED_PAWN_ONE = j;
					strcpy(param,"ROOK_PASSEED_PAWN");
					break;
				case 9:
					BONUS_BISHOP_PAIR_ONE = j;
					strcpy(param,"BISHOP_PAIR");
					break;
				case 10:
					BAD_BISHOP_PENALTY_ONE = j;
					strcpy(param,"BAD_BISHOP");
					break;
				case 11:
					GOOD_KNIGHT_BONUS_ONE = j;
					strcpy(param,"GOOD_KNIGHT");
					break;
				default:
					break;
			}
			memset(data, 0, sizeof(data));
			for ( i = 0 ; i < 1000 ; i++ ) { // play 1000 games total
				if (i % 100 == 0)  // data point evey 100 games (50 lines)
					finalScore = 0;
				init_board();
				gen();
				follow_line(i/2);
				int gameResult = -2, numMoves = 0;
				while ( gameResult < -1 ) {
					if ( ( i % 2 ) == 0 ) { // player one is which in even games 
						if (side == LIGHT)
							evalPtr = &evalOne;
						else
							evalPtr = &evalTwo;
					} else {
						if (side == DARK)
							evalPtr = &evalOne;
						else
							evalPtr = &evalTwo;
					}
					think(0);
					makemove(pv[0][0].b);
					numMoves++;
					ply = 0;
					gen(); // must be called before check_result()
					gameResult = check_result();
					if ( numMoves > 200 )
						gameResult = 0; // draw for extra long games
					}
					if ( ( i % 2 ) == 0 ) {   // player one is white
						if (gameResult == 1)  // player one wins
							finalScore += 1;
						if (gameResult == -1) // player one wins
							finalScore -= 1;	
					} else {
						if (gameResult == -1) // player one wins
							finalScore += 1;
						if (gameResult == 1)  // player one wins
							finalScore -= 1;
					}
				if ((i+1) % 100 == 0)
					data[i/100] = finalScore;
			} // finished playing 1000 games
			  // calculate the mean and standard deviation
			float sum = 0.0, mean, SD = 0.0;
			for (i = 0; i < 10; ++i)
				sum += data[i];
			mean = sum / 10;
			for (i = 0; i < 10; ++i)
				SD += pow(data[i] - mean, 2);
			printf("%s\t%d\t%5.3f\t%5.3f\t%5.3f\n",param,j,mean,mean-sqrt(SD/10),mean+sqrt(SD/10));
		} // finished with one of the parameters
	}
	exit(0);
}

////////////////////////////////////////////////////////////////////////
// play a match beteen two parameter sets and return the score
////////////////////////////////////////////////////////////////////////
int experiment3(int games) { // play games at random, return the score from 
	int i, finalScore = 0;
	for ( i = 0 ; i < games ; i++ ) {
		init_board();
		gen();
		int gameResult = -2, numMoves = 0;
		while ( gameResult < -1 ) {  /* computer's turn */
			if ( ( i % 2 ) == 0 ) { // player one is white in even games 
				if (side == LIGHT)
					evalPtr = &evalOne;
				else
					evalPtr = &evalTwo;
			} else {
				if (side == DARK)
					evalPtr = &evalOne;
				else
					evalPtr = &evalTwo;
			}
			think(0);
			makemove(pv[0][0].b);
			numMoves++;
			ply = 0;
			gen(); // must be called before check_result()
			gameResult = check_result();
			if ( numMoves > 200 )
				gameResult = 0; // draw for extra long games
			}
			if ( ( i % 2 ) == 0 ) {   // player one is white
				if (gameResult == 1)  // player one wins
					finalScore += 1;
				if (gameResult == -1) // player one wins
					finalScore -= 1;	
			} else {
				if (gameResult == -1) // player one wins
					finalScore += 1;
				if (gameResult == 1)  // player one wins
					finalScore -= 1;
			}
	}
	return finalScore;
}

////////////////////////////////////////////////////////////////////////
// find the best opening useing space as a heuristic
////////////////////////////////////////////////////////////////////////
void experiment4() {
	init_board();
	gen();
	close_book();
	evalPtr = &space;
	max_depth = 24;
	think(1);
	exit(0);
}
