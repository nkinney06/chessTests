#include "data.h"

////////////////////////////////////////////////////////////////////////////////////////
// the board representation
////////////////////////////////////////////////////////////////////////////////////////
int squares[64];
U64 WHITE_KING, WHITE_QUEEN, WHITE_ROOKS, WHITE_BISHOPS, WHITE_KNIGHTS, WHITE_PAWNS, WHITE_PIECES;
U64 BLACK_KING, BLACK_QUEEN, BLACK_ROOKS, BLACK_BISHOPS, BLACK_KNIGHTS, BLACK_PAWNS, BLACK_PIECES;
U64 OCCUPIED_SQUARES, OPEN_SQUARES;

////////////////////////////////////////////////////////////////////////////////////////
// game state variables
////////////////////////////////////////////////////////////////////////////////////////   
int side;              // the side to move
int xside;             // the side not to move
int castle;            // bitfield 1 = 0-0(w), 2 = 0-0-0(w), 4 = 0-0(b), 8 = 0-0-0(b)
int ep;                // the en passant square. if white moves e2e4, the en passant square is set to e3
int fifty;             // the number of moves since a capture or pawn move, usedto handle the fifty-move-draw rule
int hash;              // a (more or less) unique number that corresponds to theposition
int ply;               // the number of half-moves (ply) since the root of the search tree
int hply;              // h for history; the number of ply since the beginning of the game
char eco[8];           // keep track of the eco code in experiment 1 start positions
char opening[64];      // opening
char variation[64];    // keep track of the variation in experiment 1 start positions
char rating[8];        // for experiment 6
char solution[256];    // for experiment 6

////////////////////////////////////////////////////////////////////////////////////////
// move list variables
////////////////////////////////////////////////////////////////////////////////////////
gen_t gen_dat[GEN_STACK];    // gen_dat is some memory for move lists
int first_move[MAX_PLY];     // The move list for ply n is from first_move[n] to first_move[n + 1].
hist_t hist_dat[HIST_STACK]; // array of hist_t's so we can take back the moves we make

////////////////////////////////////////////////////////////////////////////////////////
// search variables
////////////////////////////////////////////////////////////////////////////////////////
int history[64][64]; // the history heuristic array 
BOOL follow_pv;      // used for move ordering

////////////////////////////////////////////////////////////////////////////////////////
// search variables
////////////////////////////////////////////////////////////////////////////////////////
int max_time;              // search for max_time milliseconds 
int max_depth;             // or until it finishes searching max_depth ply. 
int start_time;            // the time when the engine starts
int stop_time;             // and when it should stop 
int nodes;                 // the number of nodes we've searched
move pv[MAX_PLY][MAX_PLY]; // a "triangular" PV array
int pv_length[MAX_PLY];    // see "How Computers Play Chess" by Levy and Newborn.

////////////////////////////////////////////////////////////////////////////////////////
// special numbers for zobrist position hash
////////////////////////////////////////////////////////////////////////////////////////
int hash_piece[16][64];  /* indexed by piece [color][type][square] */
int hash_side;
int hash_ep[64];

////////////////////////////////////////////////////////////////////////////////////////
// castle variables
////////////////////////////////////////////////////////////////////////////////////////
int castle_mask[64] = {
	 7, 15, 15, 15,  3, 15, 15, 11,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	13, 15, 15, 15, 12, 15, 15, 14
    };

U64 WHITE_00_AMASK  = 0x7000000000000000;
U64 WHITE_000_AMASK = 0x1c00000000000000;
U64 BLACK_00_AMASK  = 0x0000000000000070;
U64 BLACK_000_AMASK = 0x000000000000001c;
U64 WHITE_00_PMASK  = 0x6000000000000000;
U64 WHITE_000_PMASK = 0x0e00000000000000;
U64 BLACK_00_PMASK  = 0x0000000000000060;
U64 BLACK_000_PMASK = 0x000000000000000e;

////////////////////////////////////////////////////////////////////////////////////////
// piece codes and piece values
///////////////////////////////////////////////////////////////////////////////////////
int piece_value[16] = { 0, 100, 0, 300, 0, 300, 500, 900, 0, 100, 0, 300, 0, 300, 500, 900 };
char displaycodes[17] = {".PKN.BRQ.pkn.brq"};
int piececodes[13] = { 0,1,2,3,5,6,7,9,10,11,13,14,15 };
int W_PAWN	    = 1;
int W_KNIGHT	= 3;
int W_BISHOP	= 5;
int W_ROOK	    = 6;
int W_QUEEN	    = 7;
int W_KING	    = 2;
int B_PAWN	    = 9;
int B_KNIGHT	= 11;
int B_BISHOP	= 13;
int B_ROOK	    = 14;
int B_QUEEN	    = 15;
int B_KING	    = 10;