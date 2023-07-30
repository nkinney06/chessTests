#include <stdio.h>
#include <string.h>
#include "data.h"
#include "sliding.h"
#include "stepping.h"
#include "main.h"
#include "knowledge.h"

int evalOne()
{
	///////////////////////////////////////////////////////////////////
	// variables
	///////////////////////////////////////////////////////////////////
	//int squareCount[64];  
	int whitescore, blackscore, square;
	int whitepawns, whiteknights, whitebishops, whiterooks, whitequeens;
	int blackpawns, blackknights, blackbishops, blackrooks, blackqueens;
	int whitekingsquare, blackkingsquare;
	int whitetotalmat, blacktotalmat;
	int i,tempSide;
	BOOL endgame;
	U64 temp, whitepassedpawns, blackpassedpawns;

	///////////////////////////////////////////////////////////////////
	// save the kingsquares for later eval
	///////////////////////////////////////////////////////////////////
	whitekingsquare = firstone(WHITE_KING);
	blackkingsquare = firstone(BLACK_KING);

	///////////////////////////////////////////////////////////////////
	// material count
	///////////////////////////////////////////////////////////////////
	whitepawns    = bitcount(WHITE_PAWNS);
	whiteknights  = bitcount(WHITE_KNIGHTS);
	whitebishops  = bitcount(WHITE_BISHOPS);
	whiterooks    = bitcount(WHITE_ROOKS);
	whitequeens   = bitcount(WHITE_QUEEN);
	whitetotalmat = 300 * whiteknights + 300 * whitebishops + 500 * whiterooks + 900 * whitequeens;
	blackpawns    = bitcount(BLACK_PAWNS);
	blackknights  = bitcount(BLACK_KNIGHTS);
	blackbishops  = bitcount(BLACK_BISHOPS);
	blackrooks    = bitcount(BLACK_ROOKS);
	blackqueens   = bitcount(BLACK_QUEEN);
	blacktotalmat = 300 * blackknights + 300 * blackbishops + 500 * blackrooks + 900 * blackqueens;
	whitescore = whitetotalmat + 100 * whitepawns;
	blackscore = blacktotalmat + 100 * blackpawns;
	
	///////////////////////////////////////////////////////////////////
	// phase of the game
	///////////////////////////////////////////////////////////////////
	endgame = (whitetotalmat < 1500 || blacktotalmat < 1500);

	///////////////////////////////////////////////////////////////////
	// time calculation
	///////////////////////////////////////////////////////////////////
	// if ( TIME_BONUS_ONE > 0 ) {
		// memset(squareCount,0,sizeof squareCount);
		// for ( i = 0 ; i < hply ; i++ ) {
			// squareCount[(int)hist_dat[i].m.b.to] = ( squareCount[(int)hist_dat[i].m.b.from] + 1 );
			// squareCount[(int)hist_dat[i].m.b.from] = 0;
		// }
		// for ( i = 0 ; i < 64 ; i++ ) {
			// if ( squareCount[i] > 0 ) {
				// if ( squares[i] < 8 )
					// whitescore += TIME_BONUS_ONE * squareCount[i];
				// else
					// blackscore += TIME_BONUS_ONE * squareCount[i];
			// }
		// }
	// }
	
	///////////////////////////////////////////////////////////////////
	// space advantage calculation
	///////////////////////////////////////////////////////////////////
	int legalMovesWhite=countMoves(0); 
	int legalMovesBlack=countMoves(1);
	whitescore += BONUS_MOVE_OPTIONS_ONE * legalMovesWhite;
	blackscore += BONUS_MOVE_OPTIONS_ONE * legalMovesBlack;
	
	///////////////////////////////////////////////////////////////////
	// evaluate the white pawns
	///////////////////////////////////////////////////////////////////
	whitepassedpawns = 0; // save position of passers for rook evaluation
	temp = WHITE_PAWNS;
	while (temp) {
		  square = firstone(temp);
		  
		// beginner knowledge
		whitescore += PAWNPOS_W[square];    
		if (endgame)
			whitescore += PAWN_OWN_DISTANCE[DISTANCE[square][whitekingsquare]];

		// doubled pawns
		if ((WHITE_PAWNS ^ bitset[square]) & FILEMASK[square])
			 whitescore -= PENALTY_DOUBLED_PAWN_ONE;
		 
		// passed pawns 
		if (!(PASSED_WHITE[square] & BLACK_PAWNS)) {
			if ( !( ( PASSED_WHITE[square] & FILEMASK[square] ) & WHITE_PAWNS ) ) {
				whitescore += BONUS_PASSED_PAWN_ONE;
				whitepassedpawns ^= bitset[square];
			}
		}
					
		// isolated pawns
		if (!(ISOLATED_WHITE[square] & WHITE_PAWNS)) {
			whitescore -= PENALTY_ISOLATED_PAWN_ONE;
		} else { 
		
		// backward
		if ((whitepawnattacks[square - 8] & BLACK_PAWNS))
			if (!(BACKWARD_WHITE[square] & WHITE_PAWNS))
				whitescore -= PENALTY_BACKWARD_PAWN_ONE;	
		}
		temp ^= bitset[square];
	}
 
	///////////////////////////////////////////////////////////////////
	// evaluate the white knights
	///////////////////////////////////////////////////////////////////
	temp = WHITE_KNIGHTS;
	while (temp) {
		square = firstone(temp);
		whitescore += KNIGHTPOS_W[square];

		// good knight bonus
		if ( rank[square] < 5 )
			if ( !(OUTPOST_WHITE[square] & BLACK_PAWNS) )
				if ( blackpawnattacks[square] & WHITE_PAWNS )
					whitescore += GOOD_KNIGHT_BONUS_ONE;

		temp ^= bitset[square];
	}
	
	///////////////////////////////////////////////////////////////////
	// evaulate the white bishops
	///////////////////////////////////////////////////////////////////

	// bishop pair
	if (WHITE_BISHOPS)
		if ((WHITE_BISHOPS & WHITE_SQUARES) && (WHITE_BISHOPS & BLACK_SQUARES))
			whitescore += BONUS_BISHOP_PAIR_ONE;

 
	temp = WHITE_BISHOPS;
	while (temp) {
		square = firstone(temp);
		whitescore += BISHOPPOS_W[square];

		// bad dark squared bishop
		if ( bitset[square] & 0x40a040000000000 )
			if ( bitcount( WHITE_PAWNS & 0x100800000000 ) > 1 )
				if ( bitcount( BLACK_PAWNS & 0x1008000000 ) >= 1 )
					whitescore -= BAD_BISHOP_PENALTY_ONE;

		// bad light squared bishop
		if ( bitset[square] & WHITE_SQUARES )
			if ( bitcount( WHITE_PAWNS & 0x1008000000 ) > 1 )
				if ( bitcount( BLACK_PAWNS & 0x10080000 ) > 1 )
					whitescore -= BAD_BISHOP_PENALTY_ONE;

		temp ^= bitset[square];
	}
	
	///////////////////////////////////////////////////////////////////
	// evaulate the white rooks
	///////////////////////////////////////////////////////////////////
	temp = WHITE_ROOKS;
	while (temp) {
		square = firstone(temp);
		whitescore += ROOKPOS_W[square];

		// rook behind passed pawns
		if (FILEMASK[square] & whitepassedpawns)
			if ((unsigned int) square > firstone(FILEMASK[square] & whitepassedpawns))
				whitescore += BONUS_ROOK_BEHIND_PASSED_PAWN_ONE;

		if ( !(FILEMASK[square] & WHITE_PAWNS) ) {
		
		// rook on semi open file
			if ( !(FILEMASK[square] & BLACK_PAWNS) )
				whitescore += ROOK_SEMI_OPEN_FILE_BONUS_ONE;
		
		// rook on open file
			else
				whitescore += ROOK_OPEN_FILE_BONUS_ONE;
		}

		// rook on seventh bonus
		if (RANKS[square] == 2)
			whitescore += ROOK_ON_SEVENTH_BONUS_ONE;

		temp ^= bitset[square];
	}
	   
	///////////////////////////////////////////////////////////////////
	// evaulate the white queen
	///////////////////////////////////////////////////////////////////
	temp = WHITE_QUEEN;
	while (temp) {
		  square = firstone(temp);
		  whitescore += QUEENPOS_W[square];
		  temp ^= bitset[square];
	}
	
	///////////////////////////////////////////////////////////////////
	// evaulate the white queen
	///////////////////////////////////////////////////////////////////
	if (endgame) {
		whitescore += KINGPOS_ENDGAME_W[whitekingsquare]; 
	} else {
		// pawn shields are beginniner knowledge
		whitescore += KINGPOS_W[whitekingsquare];
		whitescore += BONUS_PAWN_SHIELD_STRONG * bitcount(KINGSHIELD_STRONG_W[whitekingsquare] & WHITE_PAWNS);
		whitescore += BONUS_PAWN_SHIELD_STRONG * bitcount(KINGSHIELD_STRONG_B[blackkingsquare] & WHITE_PAWNS);
		whitescore += BONUS_PAWN_SHIELD_WEAK * bitcount(KINGSHIELD_WEAK_W[whitekingsquare] & WHITE_PAWNS); 
		whitescore += BONUS_PAWN_SHIELD_WEAK * bitcount(KINGSHIELD_WEAK_B[blackkingsquare] & WHITE_PAWNS);
	}
	
	///////////////////////////////////////////////////////////////////
	// evaluate the black pawns
	///////////////////////////////////////////////////////////////////
	blackpassedpawns = 0; // save the position for the rook evaulation
	temp = BLACK_PAWNS;
	while (temp) {
		square = firstone(temp);
		
		// beginner knowledge
		blackscore += PAWNPOS_W[MIRROR[square]]; 
		if (endgame)
			blackscore += PAWN_OWN_DISTANCE[DISTANCE[square][blackkingsquare]];

		// passed pawns
		if (!(PASSED_BLACK[square] & WHITE_PAWNS)) {  
			if ( !( ( PASSED_BLACK[square] & FILEMASK[square] ) & BLACK_PAWNS ) ) {
				blackscore += BONUS_PASSED_PAWN_ONE;
				blackpassedpawns ^= bitset[square];
			}
		}

		// doubled pawns
		if ((BLACK_PAWNS ^ bitset[square]) & FILEMASK[square])
			blackscore -= PENALTY_DOUBLED_PAWN_ONE;

		// isolated pawns
		if (!(ISOLATED_BLACK[square] & BLACK_PAWNS)) {
			blackscore -= PENALTY_ISOLATED_PAWN_ONE;
		} else {
			
		// backward pawn
			if ((blackpawnattacks[square + 8] & WHITE_PAWNS))
				if (!(BACKWARD_BLACK[square] & BLACK_PAWNS))
					blackscore -= PENALTY_BACKWARD_PAWN_ONE;
		}
		temp ^= bitset[square];
	}
	   
	///////////////////////////////////////////////////////////////////
	// evaluate the black knights
	///////////////////////////////////////////////////////////////////
	temp = BLACK_KNIGHTS;
	while (temp) {
		square = firstone(temp);
		blackscore += KNIGHTPOS_W[MIRROR[square]];

		// good knight bonus
		if ( rank[square] > 2 )
			if ( !(OUTPOST_BLACK[square] & WHITE_PAWNS) ) 
				if ( whitepawnattacks[square] & BLACK_PAWNS ) 
					blackscore += GOOD_KNIGHT_BONUS_ONE;

		temp ^= bitset[square];
	}
	   
	///////////////////////////////////////////////////////////////////
	// evaluate the black bishops
	///////////////////////////////////////////////////////////////////
 
	// bishop pair
	if (BLACK_BISHOPS)
		if ((BLACK_BISHOPS & WHITE_SQUARES) && (BLACK_BISHOPS & BLACK_SQUARES))
			blackscore += BONUS_BISHOP_PAIR_ONE; 

	temp = BLACK_BISHOPS;
	while (temp) {
		square = firstone(temp);
		blackscore += BISHOPPOS_W[MIRROR[square]];

		// light squared bishop
		if ( bitset[square] & 0x40a04 ) 
			if ( bitcount( BLACK_PAWNS & 0x8100000 ) > 1 ) 
				if ( bitcount( WHITE_PAWNS & 0x810000000 ) >= 1 ) 
					blackscore -= BAD_BISHOP_PENALTY_ONE;

		// dark squared bishop
		if ( bitset[square] & BLACK_SQUARES )
			if ( bitcount( BLACK_PAWNS & 0x0000000810000000 ) > 1 ) 
				if ( bitcount( WHITE_PAWNS & 0x81000000000 ) > 1 ) 
					blackscore -= BAD_BISHOP_PENALTY_ONE;

		temp ^= bitset[square];
	}
	
	///////////////////////////////////////////////////////////////////
	// evaluate the black rooks
	///////////////////////////////////////////////////////////////////
	temp = BLACK_ROOKS;
	while (temp) {
		square = firstone(temp);
		blackscore += ROOKPOS_W[MIRROR[square]];

		// rook behind passed pawn
		if (FILEMASK[square] & blackpassedpawns)
			if ((unsigned int) square < lastone(FILEMASK[square] & blackpassedpawns))
				blackscore += BONUS_ROOK_BEHIND_PASSED_PAWN_ONE;


		if ( !(FILEMASK[square] & BLACK_PAWNS) ) {
			
		// rook on semi open file
			if ( !(FILEMASK[square] & WHITE_PAWNS) )
				blackscore += ROOK_SEMI_OPEN_FILE_BONUS_ONE;
				
		// rook on open file
			else
				blackscore += ROOK_OPEN_FILE_BONUS_ONE;
		}

		// rook on seventh
		if (RANKS[square] == 7)
			blackscore += ROOK_ON_SEVENTH_BONUS_ONE;
		
		temp ^= bitset[square];
	}
	   
	///////////////////////////////////////////////////////////////////
	// evaluate the black queen
	///////////////////////////////////////////////////////////////////
 	temp = BLACK_QUEEN;
	while (temp) {
		square = firstone(temp);
		blackscore += QUEENPOS_W[MIRROR[square]];
		temp ^= bitset[square];
	}
	   
	///////////////////////////////////////////////////////////////////
	// evaluate the black king
	///////////////////////////////////////////////////////////////////
	if (endgame) {
		blackscore += KINGPOS_ENDGAME_W[MIRROR[blackkingsquare]]; 
	} else {
		// pawn shields are beginniner knowledge
		blackscore += KINGPOS_W[MIRROR[blackkingsquare]];
		blackscore += BONUS_PAWN_SHIELD_STRONG * bitcount(KINGSHIELD_STRONG_B[blackkingsquare] & BLACK_PAWNS);
		blackscore += BONUS_PAWN_SHIELD_STRONG * bitcount(KINGSHIELD_STRONG_W[whitekingsquare] & BLACK_PAWNS);
		blackscore += BONUS_PAWN_SHIELD_WEAK * bitcount(KINGSHIELD_WEAK_B[blackkingsquare] & BLACK_PAWNS); 
		blackscore += BONUS_PAWN_SHIELD_WEAK * bitcount(KINGSHIELD_WEAK_W[whitekingsquare] & BLACK_PAWNS); 
	}
	   
	///////////////////////////////////////////////////////////////////
	// return the score relative to the side to move
	///////////////////////////////////////////////////////////////////
	if (side == LIGHT)
		return whitescore - blackscore;
	return blackscore - whitescore;

}












