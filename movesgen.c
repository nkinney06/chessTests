#include <stdio.h>
#include <stdlib.h>
#include "sliding.h"
#include "stepping.h"
#include "data.h"
#include "main.h"
#include "knowledge.h"

int isattacked ( U64 bitmap, int side_to_move ) {
U64 temp_target = bitmap;
U64 sliding_attackers;
int tosq;
if ( side_to_move ) {
	while ( temp_target ) {
		tosq = firstone( temp_target );
		if ( BLACK_PAWNS   & whitepawnattacks[ tosq ] ) { return 1; }
		if ( BLACK_KNIGHTS &    knightattacks[ tosq ] ) { return 1; }
		if ( BLACK_KING    &      kingattacks[ tosq ] ) { return 1; }
		sliding_attackers = ( BLACK_ROOKS   | BLACK_QUEEN );
		if ( bitcount( sliding_attackers ) ) {
            if ( Rmagic( tosq,OCCUPIED_SQUARES ) & sliding_attackers ) { return 1; };
			}
		sliding_attackers = ( BLACK_BISHOPS | BLACK_QUEEN );
		if ( bitcount( sliding_attackers ) ) {
            if ( Bmagic( tosq,OCCUPIED_SQUARES ) & sliding_attackers ) { return 1; };
			}
		temp_target ^= bitset[ tosq ];
	}
	} else { 
		while ( temp_target ) {
			tosq = firstone( temp_target );
			if ( WHITE_PAWNS   & blackpawnattacks[ tosq ] ) { return 1; }
			if ( WHITE_KNIGHTS &    knightattacks[ tosq ] ) { return 1; }
			if ( WHITE_KING    &      kingattacks[ tosq ] ) { return 1; }
			sliding_attackers = ( WHITE_ROOKS | WHITE_QUEEN );
			if ( bitcount( sliding_attackers ) != 0 ) {
				if ( Rmagic( tosq,OCCUPIED_SQUARES ) & sliding_attackers ) { return 1; };
			}
			sliding_attackers = ( WHITE_BISHOPS | WHITE_QUEEN );
			if ( bitcount( sliding_attackers ) != 0 ) {
				if ( Bmagic( tosq,OCCUPIED_SQUARES ) & sliding_attackers ) { return 1; };
			}
			temp_target ^= bitset[ tosq ];
		}
	}
return 0;
}

void gen_promote(int from, int to, int bits, int prom)
{
	gen_dat[first_move[ply + 1]].m.b.from    = from;
	gen_dat[first_move[ply + 1]].m.b.to      = to;
	gen_dat[first_move[ply + 1]].m.b.promote = prom;
	gen_dat[first_move[ply + 1]].m.b.bits    = (bits | 32);
	gen_dat[first_move[ply + 1]].score       = 1000000 + (piece_value[prom] * 10);
	first_move[ply + 1]++;
	return;
}

void gen_push(int from, int to, int bits)
{
	gen_dat[first_move[ply + 1]].m.b.from    = from;
	gen_dat[first_move[ply + 1]].m.b.to      = to;
	gen_dat[first_move[ply + 1]].m.b.promote = 0;
	gen_dat[first_move[ply + 1]].m.b.bits    = bits;
	
	if (squares[to] > 0)
		gen_dat[first_move[ply + 1]].score = 1000000 + (piece_value[squares[to]] * 10) - piece_value[squares[from]];
	else
		gen_dat[first_move[ply + 1]].score = history[from][to];
	
	first_move[ply + 1]++;
	return;
}

void gen_caps() { // a copy of the move generator below exclusively for captures

U64 temp_moves,temp_board;
int from,tosq;

/* so far, we have no moves for the current ply */
first_move[ply + 1] = first_move[ply];

if ( side ) { // if its black to move
	
	// knight moves for black
	temp_board = BLACK_KNIGHTS;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = knightattacks[from] & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// bishop moves for black
	temp_board = BLACK_BISHOPS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Bmagic(from,OCCUPIED_SQUARES) & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// rook moves for black
	temp_board = BLACK_ROOKS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Rmagic(from,OCCUPIED_SQUARES) & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// queen moves for black
	temp_board = BLACK_QUEEN;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = ( Rmagic(from,OCCUPIED_SQUARES) | Bmagic(from,OCCUPIED_SQUARES) ) & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// pawn moves for black
	temp_board = BLACK_PAWNS;
	while ( temp_board ) {
		from = firstone( temp_board );
		if ( ( bitset[ from ] << 8 ) & OPEN_SQUARES ) {
			if ( rank[ from ] == 6 ) {
				gen_promote(from, from+8, 16, B_KNIGHT );
				gen_promote(from, from+8, 16, B_BISHOP );
				gen_promote(from, from+8, 16, B_ROOK   );
				gen_promote(from, from+8, 16, B_QUEEN  );
				}
			}
		temp_moves = blackpawnattacks[from] & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			if ( rank[ tosq ] == 7 ) {
				gen_promote(from, tosq, 17, B_KNIGHT );
				gen_promote(from, tosq, 17, B_BISHOP );
				gen_promote(from, tosq, 17, B_ROOK   );
				gen_promote(from, tosq, 17, B_QUEEN  );
				} else {
				gen_push(from, tosq, 17 );
				}
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
	// pawn en passant moves
	if ( ep > 0 ) {
		temp_board = whitepawnattacks[ ep ] & BLACK_PAWNS;
		while ( temp_board ) {
			from = firstone( temp_board );
			gen_push( from, ep, 21 );
			temp_board ^= bitset[from];
		}
	}

	// king moves for black
	temp_board = BLACK_KING;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = kingattacks[from] & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
	} 

	else { // if its white to move
	
	// knight moves for white
	temp_board = WHITE_KNIGHTS;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = knightattacks[from] & BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );	
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
	// bishop moves for white
	temp_board = WHITE_BISHOPS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Bmagic(from,OCCUPIED_SQUARES) & BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// rook moves for white
	temp_board = WHITE_ROOKS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Rmagic(from,OCCUPIED_SQUARES) & BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// queen moves for white
	temp_board = WHITE_QUEEN;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = ( Rmagic(from,OCCUPIED_SQUARES) | Bmagic(from,OCCUPIED_SQUARES) ) & BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
		
	// pawn moves for white
	temp_board = WHITE_PAWNS;
	while ( temp_board ) {
		from = firstone( temp_board );
		if ( ( bitset[ from ] >> 8 ) & OPEN_SQUARES ) {
			if ( rank[ from ] == 1 ) {
				gen_promote(from, from-8, 16, W_KNIGHT );
				gen_promote(from, from-8, 16, W_BISHOP );
				gen_promote(from, from-8, 16, W_ROOK   );
				gen_promote(from, from-8, 16, W_QUEEN  );
				}
			}
		temp_moves = whitepawnattacks[from] & BLACK_PIECES; 
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			if ( rank[ tosq ] == 0 ) {
				gen_promote(from, tosq, 17, W_KNIGHT );
				gen_promote(from, tosq, 17, W_BISHOP );
				gen_promote(from, tosq, 17, W_ROOK   );
				gen_promote(from, tosq, 17, W_QUEEN  );
				} else {
				gen_push(from, tosq, 17);
				}
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}

	// white en passant moves
	if ( ep > 0 ) {
		temp_board = blackpawnattacks[ ep ] & WHITE_PAWNS;
		while ( temp_board ) {
			from = firstone( temp_board );
			gen_push( from, ep, 21);
			temp_board ^= bitset[from];
		}
	} 
	
	// king moves for white
	temp_board = WHITE_KING;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = kingattacks[from] & BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
    }

	return;
	
}

int countMoves( int sideToCount ) { // copy of the move generator that returns the move count for indicated side

U64 temp_moves,temp_board;
int from,tosq,movesWhite,movesBlack;
int moveCount = 0;

if ( sideToCount ) { // if its black to move
	
	// knight moves for black
	temp_board = BLACK_KNIGHTS;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = knightattacks[from] & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// bishop moves for black
	temp_board = BLACK_BISHOPS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Bmagic(from,OCCUPIED_SQUARES) & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// rook moves for black
	temp_board = BLACK_ROOKS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Rmagic(from,OCCUPIED_SQUARES) & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// queen moves for black
	temp_board = BLACK_QUEEN;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = ( Rmagic(from,OCCUPIED_SQUARES) | Bmagic(from,OCCUPIED_SQUARES) ) & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// pawn moves for black
	temp_board = BLACK_PAWNS;
	while ( temp_board ) {
		from = firstone( temp_board );
		if ( ( bitset[ from ] << 8 ) & OPEN_SQUARES ) {
			if ( rank[from] == 1 ) { 
				if ( ( bitset[ from ] << 16 ) & OPEN_SQUARES ) {
					moveCount++;
					} 
				}
			if ( rank[ from ] == 6 ) {
				moveCount++;
				moveCount++;
				moveCount++;
				moveCount++;
				} else {
				moveCount++;
				}
			}
		temp_moves = blackpawnattacks[from] & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			if ( rank[ tosq ] == 7 ) {
				moveCount++;
				moveCount++;
				moveCount++;
				moveCount++;
				} else {
				moveCount++;
				}
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
	// pawn en passant moves
	if ( ep > 0 ) {
		temp_board = whitepawnattacks[ ep ] & BLACK_PAWNS;
		while ( temp_board ) {
			from = firstone( temp_board );
			moveCount++;
			temp_board ^= bitset[from];
		}
	}

	// king moves for black
	temp_board = BLACK_KING;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = kingattacks[from] & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	if (castle & 4) { moveCount++; }
	if (castle & 8) { moveCount++; }
	
	} 

	else { // if its white to move
	
	// knight moves for white
	temp_board = WHITE_KNIGHTS;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = knightattacks[from] & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );	
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
	// bishop moves for white
	temp_board = WHITE_BISHOPS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Bmagic(from,OCCUPIED_SQUARES) & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// rook moves for white
	temp_board = WHITE_ROOKS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Rmagic(from,OCCUPIED_SQUARES) & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// queen moves for white
	temp_board = WHITE_QUEEN;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = ( Rmagic(from,OCCUPIED_SQUARES) | Bmagic(from,OCCUPIED_SQUARES) ) & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
		
	// pawn moves for white
	temp_board = WHITE_PAWNS;
	while ( temp_board ) {
		from = firstone( temp_board );
		if ( ( bitset[ from ] >> 8 ) & OPEN_SQUARES ) {
			if ( rank[from] == 6 ) { 
				if ( ( bitset[ from ] >> 16 ) & OPEN_SQUARES ) {
					moveCount++;
					} 
				}
			if ( rank[ from ] == 1 ) {
				moveCount++;
				moveCount++;
				moveCount++;
				moveCount++;
				} else {
				moveCount++;
				}
			}
		temp_moves = whitepawnattacks[from] & BLACK_PIECES; 
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			if ( rank[ tosq ] == 0 ) {
				moveCount++;
				moveCount++;
				moveCount++;
				moveCount++;
				} else {
				moveCount++;
				}
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}

	// white en passant moves
	if ( ep > 0 ) {
		temp_board = blackpawnattacks[ ep ] & WHITE_PAWNS;
		while ( temp_board ) {
			from = firstone( temp_board );
			moveCount++;
			temp_board ^= bitset[from];
		}
	} 
	
	// king moves for white
	temp_board = WHITE_KING;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = kingattacks[from] & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			moveCount++;
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	if (castle & 1) { moveCount++; }
	if (castle & 2) { moveCount++; }
	
    }

	return moveCount;
	
}

void gen() {

U64 temp_moves,temp_board;
int from,tosq;

/* so far, we have no moves for the current ply */
first_move[ply + 1] = first_move[ply];

if ( side ) { // if its black to move
	
	// knight moves for black
	temp_board = BLACK_KNIGHTS;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = knightattacks[from] & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// bishop moves for black
	temp_board = BLACK_BISHOPS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Bmagic(from,OCCUPIED_SQUARES) & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// rook moves for black
	temp_board = BLACK_ROOKS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Rmagic(from,OCCUPIED_SQUARES) & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// queen moves for black
	temp_board = BLACK_QUEEN;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = ( Rmagic(from,OCCUPIED_SQUARES) | Bmagic(from,OCCUPIED_SQUARES) ) & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// pawn moves for black
	temp_board = BLACK_PAWNS;
	while ( temp_board ) {
		from = firstone( temp_board );
		if ( ( bitset[ from ] << 8 ) & OPEN_SQUARES ) {
			if ( rank[from] == 1 ) { 
				if ( ( bitset[ from ] << 16 ) & OPEN_SQUARES ) {
					gen_push(from, from+16, 24 ); 
					} 
				}
			if ( rank[ from ] == 6 ) {
				gen_promote(from, from+8, 16, B_KNIGHT );
				gen_promote(from, from+8, 16, B_BISHOP );
				gen_promote(from, from+8, 16, B_ROOK   );
				gen_promote(from, from+8, 16, B_QUEEN  );
				} else {
				gen_push(from, from+8, 16 ); 
				}
			}
		temp_moves = blackpawnattacks[from] & WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			if ( rank[ tosq ] == 7 ) {
				gen_promote(from, tosq, 17, B_KNIGHT );
				gen_promote(from, tosq, 17, B_BISHOP );
				gen_promote(from, tosq, 17, B_ROOK   );
				gen_promote(from, tosq, 17, B_QUEEN  );
				} else {
				gen_push(from, tosq, 17 );
				}
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
	// pawn en passant moves
	if ( ep > 0 ) {
		temp_board = whitepawnattacks[ ep ] & BLACK_PAWNS;
		while ( temp_board ) {
			from = firstone( temp_board );
			gen_push( from, ep, 21 );
			temp_board ^= bitset[from];
		}
	}

	// king moves for black
	temp_board = BLACK_KING;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = kingattacks[from] & ~BLACK_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	if (castle & 4) { gen_push(E8, G8, 2); }
	if (castle & 8) { gen_push(E8, C8, 2); }
	
	} 

	else { // if its white to move
	
	// knight moves for white
	temp_board = WHITE_KNIGHTS;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = knightattacks[from] & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );	
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	
	// bishop moves for white
	temp_board = WHITE_BISHOPS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Bmagic(from,OCCUPIED_SQUARES) & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// rook moves for white
	temp_board = WHITE_ROOKS;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = Rmagic(from,OCCUPIED_SQUARES) & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
	
	// queen moves for white
	temp_board = WHITE_QUEEN;
	while ( temp_board ) {
		from = firstone( temp_board );
        temp_moves = ( Rmagic(from,OCCUPIED_SQUARES) | Bmagic(from,OCCUPIED_SQUARES) ) & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push(from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	} 
		
	// pawn moves for white
	temp_board = WHITE_PAWNS;
	while ( temp_board ) {
		from = firstone( temp_board );
		if ( ( bitset[ from ] >> 8 ) & OPEN_SQUARES ) {
			if ( rank[from] == 6 ) { 
				if ( ( bitset[ from ] >> 16 ) & OPEN_SQUARES ) {
					gen_push(from, from-16, 24); 
					} 
				}
			if ( rank[ from ] == 1 ) {
				gen_promote(from, from-8, 16, W_KNIGHT );
				gen_promote(from, from-8, 16, W_BISHOP );
				gen_promote(from, from-8, 16, W_ROOK   );
				gen_promote(from, from-8, 16, W_QUEEN  );
				} else {
				gen_push(from, from-8, 16); 
				}
			}
		temp_moves = whitepawnattacks[from] & BLACK_PIECES; 
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			if ( rank[ tosq ] == 0 ) {
				gen_promote(from, tosq, 17, W_KNIGHT );
				gen_promote(from, tosq, 17, W_BISHOP );
				gen_promote(from, tosq, 17, W_ROOK   );
				gen_promote(from, tosq, 17, W_QUEEN  );
				} else {
				gen_push(from, tosq, 17);
				}
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}

	// white en passant moves
	if ( ep > 0 ) {
		temp_board = blackpawnattacks[ ep ] & WHITE_PAWNS;
		while ( temp_board ) {
			from = firstone( temp_board );
			gen_push( from, ep, 21);
			temp_board ^= bitset[from];
		}
	} 
	
	// king moves for white
	temp_board = WHITE_KING;
	while ( temp_board ) {
		from = firstone( temp_board );
		temp_moves = kingattacks[from] & ~WHITE_PIECES;
		while ( temp_moves ) {
			tosq = firstone( temp_moves );
			gen_push( from, tosq, squares[tosq] > 0 );
			temp_moves ^= bitset[tosq];
			}
		temp_board ^= bitset[from];
	}
	if (castle & 1) { gen_push(E1, G1, 2); }
	if (castle & 2) { gen_push(E1, C1, 2); }
	
    }

	return;
	
}

void takeback()
{
	move_bytes m;
	side ^= 1;
	xside ^= 1;
	--ply;
	--hply;
	
	m = hist_dat[hply].m.b;
	castle = hist_dat[hply].castle;
	ep = hist_dat[hply].ep;
	fifty = hist_dat[hply].fifty;
	hash = hist_dat[hply].hash;
	
	squares[(int)m.from] = hist_dat[hply].piece;
	squares[(int)m.to]   = hist_dat[hply].capture;
	
	if (m.bits & 2) {
		switch(m.to) {
			case 62:
				squares[F1] = 0;
				squares[H1] = W_ROOK;
				WHITE_ROOKS  ^= 0xa000000000000000;
				WHITE_PIECES ^= 0xa000000000000000;
				break;
			case 58:
				squares[D1] = 0;
				squares[A1] = W_ROOK;
				WHITE_ROOKS  ^= 0x0900000000000000;
				WHITE_PIECES ^= 0x0900000000000000;
				break;
			case 6:
				squares[F8] = 0;
				squares[H8] = B_ROOK;
				BLACK_ROOKS  ^= 0x00000000000000a0;
				BLACK_PIECES ^= 0x00000000000000a0;
				break;
			case 2:
				squares[D8] = 0;
				squares[A8] = B_ROOK;
				BLACK_ROOKS  ^= 0x0000000000000009;
				BLACK_PIECES ^= 0x0000000000000009;
				break;
			default: 
				break;
		}
	}
	
	U64 from_bitmap = bitset[(int)m.from];	
	U64 tosq_bitmap = bitset[(int)m.to];	
	U64 move_bitmap = from_bitmap | tosq_bitmap ;
	
	// update the bitboards 
	switch ( hist_dat[hply].piece ) {
			case 1:
			WHITE_PAWNS   ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 2:
			WHITE_KING    ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 3:
			WHITE_KNIGHTS ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 5:
			WHITE_BISHOPS ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 6:
			WHITE_ROOKS   ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 7:
			WHITE_QUEEN   ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 9:
			BLACK_PAWNS   ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 10:
			BLACK_KING    ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 11:
			BLACK_KNIGHTS ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 13:
			BLACK_BISHOPS ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 14:
			BLACK_ROOKS   ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 15:
			BLACK_QUEEN   ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
		}
		
	// handle the captures
	if (m.bits & 1) { 
		switch ( hist_dat[hply].capture ) {
			case 1:
			WHITE_PAWNS   ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 2:
			WHITE_KING    ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 3:
			WHITE_KNIGHTS ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 5:
			WHITE_BISHOPS ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 6:
			WHITE_ROOKS   ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 7:
			WHITE_QUEEN   ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 9:
			BLACK_PAWNS   ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 10:
			BLACK_KING    ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 11:
			BLACK_KNIGHTS ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 13:
			BLACK_BISHOPS ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 14:
			BLACK_ROOKS   ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 15:
			BLACK_QUEEN   ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 0:
			break;
			}
		}
		
	// handle the promotions
	if (m.bits & 32) {
		switch ( m.promote ) {
			case 3:
			WHITE_KNIGHTS ^= tosq_bitmap;
			WHITE_PAWNS	  ^= tosq_bitmap;
			break;
			case 5:
			WHITE_BISHOPS ^= tosq_bitmap;
			WHITE_PAWNS   ^= tosq_bitmap;
			break;
			case 6:
			WHITE_ROOKS   ^= tosq_bitmap;
			WHITE_PAWNS   ^= tosq_bitmap;
			break;
			case 7:
			WHITE_QUEEN   ^= tosq_bitmap;
			WHITE_PAWNS   ^= tosq_bitmap;
			break;
			case 11:
			BLACK_KNIGHTS ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
			case 13:
			BLACK_BISHOPS ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
			case 14:
			BLACK_ROOKS   ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
			case 15:
			BLACK_QUEEN   ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
		}
	}

	if (m.bits & 4) {
		if (side == LIGHT) {
			squares[m.to + 8] = B_PAWN;
			BLACK_PAWNS  ^= bitset[ ep + 8 ];
			BLACK_PIECES ^= bitset[ ep + 8 ];
		}
		else {
			squares[m.to - 8] = W_PAWN;
			WHITE_PAWNS  ^= bitset[ ep - 8 ];
			WHITE_PIECES ^= bitset[ ep - 8 ];
		}
	}
	
	// update the bitboards
	OCCUPIED_SQUARES =  WHITE_PIECES | BLACK_PIECES;
	OPEN_SQUARES     =~ OCCUPIED_SQUARES;
	
}

BOOL makemove(move_bytes m)
{
		
	// test to see if a castle move is legal and move the rook
	// (the king is moved with the usual move code later)
	if (m.bits & 2) {
		int from, to;
		switch (m.to) {
			case 62: // white short castle
				if ( isattacked( WHITE_00_AMASK,1 )  | ( OCCUPIED_SQUARES & WHITE_00_PMASK ) )  { return FALSE; }
				from = H1;
				to   = F1;
				WHITE_ROOKS  ^= 0xa000000000000000;
				WHITE_PIECES ^= 0xa000000000000000;
				break;
			case 58: // white long castle
				if ( isattacked( WHITE_000_AMASK,1 ) | ( OCCUPIED_SQUARES & WHITE_000_PMASK ) ) { return FALSE; }
				from = A1;
				to   = D1;
				WHITE_ROOKS  ^= 0x0900000000000000;
				WHITE_PIECES ^= 0x0900000000000000;
				break;
			case 6: // black short castel
				if ( isattacked( BLACK_00_AMASK,0 )  | ( OCCUPIED_SQUARES & BLACK_00_PMASK ) )  { return FALSE; }
				from = H8;
				to   = F8;
				BLACK_ROOKS  ^= 0x00000000000000a0;
				BLACK_PIECES ^= 0x00000000000000a0;
				break;
			case 2: // black long castle
				if ( isattacked( BLACK_000_AMASK,0 ) | ( OCCUPIED_SQUARES & BLACK_000_PMASK ) ) { return FALSE; }
				from = A8;
				to   = D8;
				BLACK_ROOKS  ^= 0x0000000000000009;
				BLACK_PIECES ^= 0x0000000000000009;
				break;
			default:
				from = -1;
				to = -1;
				break;
		}
		squares[ to ] = squares[from];
		squares[from] = 0;
	}		

	/* back up information so we can take the move back later. */
	hist_dat[hply].m.b = m;
	hist_dat[hply].capture = squares[(int)m.to];
	hist_dat[hply].castle = castle;
	hist_dat[hply].ep = ep;
	hist_dat[hply].fifty = fifty;
	hist_dat[hply].hash = hash;
	hist_dat[hply].piece = squares[(int)m.from];
	++ply;
	++hply;
	
	U64 from_bitmap = bitset[(int)m.from];	
	U64 tosq_bitmap = bitset[(int)m.to];	
	U64 move_bitmap = from_bitmap | tosq_bitmap ;
	// update the bitboards
	switch ( squares[(int)m.from] ) {
			case 1:
			WHITE_PAWNS   ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 2:
			WHITE_KING    ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 3:
			WHITE_KNIGHTS ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 5:
			WHITE_BISHOPS ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 6:
			WHITE_ROOKS   ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 7:
			WHITE_QUEEN   ^= move_bitmap;
			WHITE_PIECES  ^= move_bitmap;
			break;
			case 9:
			BLACK_PAWNS   ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 10:
			BLACK_KING    ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 11:
			BLACK_KNIGHTS ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 13:
			BLACK_BISHOPS ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 14:
			BLACK_ROOKS   ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
			case 15:
			BLACK_QUEEN   ^= move_bitmap;
			BLACK_PIECES  ^= move_bitmap;
			break;
		}
		
	// handle the captures
	if (m.bits & 1) { 
		switch ( squares[(int)m.to] ) {
			case 1:
			WHITE_PAWNS   ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 2:
			WHITE_KING    ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 3:
			WHITE_KNIGHTS ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 5:
			WHITE_BISHOPS ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 6:
			WHITE_ROOKS   ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 7:
			WHITE_QUEEN   ^= tosq_bitmap;
			WHITE_PIECES  ^= tosq_bitmap;
			break;
			case 9:
			BLACK_PAWNS   ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 10:
			BLACK_KING    ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 11:
			BLACK_KNIGHTS ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 13:
			BLACK_BISHOPS ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 14:
			BLACK_ROOKS   ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 15:
			BLACK_QUEEN   ^= tosq_bitmap;
			BLACK_PIECES  ^= tosq_bitmap;
			break;
			case 0:
			break;
			}
		}
	
	// handle the promotions
	if (m.bits & 32) {
		switch ( m.promote ) {
			case 3:
			WHITE_KNIGHTS ^= tosq_bitmap;
			WHITE_PAWNS	  ^= tosq_bitmap;
			break;
			case 5:
			WHITE_BISHOPS ^= tosq_bitmap;
			WHITE_PAWNS   ^= tosq_bitmap;
			break;
			case 6:
			WHITE_ROOKS   ^= tosq_bitmap;
			WHITE_PAWNS   ^= tosq_bitmap;
			break;
			case 7:
			WHITE_QUEEN   ^= tosq_bitmap;
			WHITE_PAWNS   ^= tosq_bitmap;
			break;
			case 11:
			BLACK_KNIGHTS ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
			case 13:
			BLACK_BISHOPS ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
			case 14:
			BLACK_ROOKS   ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
			case 15:
			BLACK_QUEEN   ^= tosq_bitmap;
			BLACK_PAWNS   ^= tosq_bitmap;
			break;
		}
	}
	
	// handle the enpassant
	if (m.bits & 4) {
		if ( side ) {
				WHITE_PAWNS   ^= ( bitset[ ep - 8 ] );
				WHITE_PIECES  ^= ( bitset[ ep - 8 ] );
				squares[ ep - 8 ] = 0;
			} else {
				BLACK_PAWNS   ^= ( bitset[ ep + 8 ] );
				BLACK_PIECES  ^= ( bitset[ ep + 8 ]  );
				squares[ ep + 8 ] = 0;
			}
		}
	
	// move the piece
	if (m.bits & 32) {
		squares[(int)m.to]   = m.promote;
		squares[(int)m.from] = 0;
		} else {
		squares[(int)m.to]   = squares[(int)m.from];
		squares[(int)m.from] = 0;
		}
	
	// update the bitboards
	OCCUPIED_SQUARES =  WHITE_PIECES | BLACK_PIECES;
	OPEN_SQUARES     =~ OCCUPIED_SQUARES;
	
	// update the castle, en passant, and
	// fifty-move-draw variables
	castle &= castle_mask[(int)m.from] & castle_mask[(int)m.to];
	
	if (m.bits & 8) {
		if ( side ) { ep = m.to - 8; } else { ep = m.to + 8; }
		} else { ep = -1; }
		
	if (m.bits & 17) { fifty = 0; } else { fifty++; }
	
	// switch sides and test for legality (if we can capture
	// the other guy's king, it's an illegal position and
    // we need to take the move back)

	side  ^= 1;
	xside ^= 1;
		
	if ( side ) {
		if ( isattacked( WHITE_KING,1 ) ) {
			takeback();
			return FALSE;
			}
	} else {
		if ( isattacked( BLACK_KING,0 ) ) {
			takeback();
			return FALSE;
		}
	}
		
	set_hash();
	
	return TRUE;
}
	
void enumerate(int depth) {
	int i;
	int num_caps_gen;
	int num_caps_gen_caps;
	if ( depth == 0 ) { 
		retVal += 1; 
		if ( hist_dat[hply-1].m.b.bits & 1 )
			retCap += 1;
		if ( hist_dat[hply-1].m.b.bits & 4 )
			retEpp += 1;
		return; 
		}
	gen();
	num_caps_gen = 0;  // this loop is designed to test the capture generator
	for ( i = first_move[ply]; i < first_move[ply + 1] ; i++ ) {
		if ( gen_dat[i].m.b.bits & 1 )
			num_caps_gen++;
		}
	gen_caps();
	num_caps_gen_caps = 0;
	for ( i = first_move[ply]; i < first_move[ply + 1] ; i++ ) {
	if ( gen_dat[i].m.b.bits & 1 )
		num_caps_gen_caps++;
	}
	if (num_caps_gen != num_caps_gen_caps)
		printf("capture_generator error\n");
	gen();
	for ( i = first_move[ply]; i < first_move[ply + 1] ; i++ ) {
		if (!makemove(gen_dat[i].m.b)) { continue; }
		enumerate( depth-1 );
		takeback();
		}
}

	
	