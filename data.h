typedef unsigned long long  U64;
#define BOOL			int
#define TRUE			1
#define FALSE			0
#define GEN_STACK		1120
#define MAX_PLY			32
#define HIST_STACK		400
#define LIGHT			0
#define DARK			1

/* useful squares */
#define A1				56
#define B1				57
#define C1				58
#define D1				59
#define E1				60
#define F1				61
#define G1				62
#define H1				63
#define A8				0
#define B8				1
#define C8				2
#define D8				3
#define E8				4
#define F8				5
#define G8				6
#define H8				7
#define ROW(x)			(x >> 3)
#define COL(x)			(x & 7)

// This is the basic description of a move. promote is what
// piece to promote the pawn to, if the move is a pawn
// promotion. bits is a bitfield that describes the move,
// with the following bits:
// 1	capture
// 2	castle
// 4	en passant capture
// 8	pushing a pawn 2 squares
// 16	pawn move
// 32	promote
// It's union'ed with an integer so two moves can easily
// be compared with each other.

typedef struct {
	char from;
	char to;
	char promote;
	char bits;
} move_bytes;

typedef union {
	move_bytes b;
	int u;
} move;

// an element of the move stack. it's just a move with a score, so it can be sorted by the search functions.
typedef struct {
	move m;
	int score;
} gen_t;

// an element of the history stack, with the information necessary to take a move back.
typedef struct {
	move m;
	int capture;
	int castle;
	int ep;
	int fifty;
	int piece;
	int hash;
} hist_t;

////////////////////////////////////////////////////////////////////////////////////////
// the board representation
////////////////////////////////////////////////////////////////////////////////////////
extern int squares[64];
extern U64 WHITE_KING;
extern U64 WHITE_QUEEN;
extern U64 WHITE_ROOKS;
extern U64 WHITE_BISHOPS;
extern U64 WHITE_KNIGHTS;
extern U64 WHITE_PAWNS;
extern U64 WHITE_PIECES;
extern U64 BLACK_KING;
extern U64 BLACK_QUEEN;
extern U64 BLACK_ROOKS;
extern U64 BLACK_BISHOPS;
extern U64 BLACK_KNIGHTS;
extern U64 BLACK_PAWNS;
extern U64 BLACK_PIECES;
extern U64 OCCUPIED_SQUARES;
extern U64 OPEN_SQUARES;

////////////////////////////////////////////////////////////////////////////////////////
// game state variables
////////////////////////////////////////////////////////////////////////////////////////
extern int side;
extern int xside;
extern int castle;
extern int ep;
extern int fifty;
extern int hash;
extern int ply;
extern int hply;
extern char eco[8];
extern char opening[64];
extern char variation[64];
extern char rating[8];
extern char solution[256];

////////////////////////////////////////////////////////////////////////////////////////
// move list variables
////////////////////////////////////////////////////////////////////////////////////////
extern gen_t gen_dat[GEN_STACK];
extern int first_move[MAX_PLY];
extern hist_t hist_dat[HIST_STACK];

////////////////////////////////////////////////////////////////////////////////////////
// search variables
////////////////////////////////////////////////////////////////////////////////////////
extern int history[64][64];
extern BOOL follow_pv;

////////////////////////////////////////////////////////////////////////////////////////
// search variables
////////////////////////////////////////////////////////////////////////////////////////
extern int max_time;
extern int max_depth;
extern int start_time;
extern int stop_time;
extern int nodes;
extern move pv[MAX_PLY][MAX_PLY];
extern int pv_length[MAX_PLY];

////////////////////////////////////////////////////////////////////////////////////////
// zobrist position hash
////////////////////////////////////////////////////////////////////////////////////////
extern int hash_piece[16][64];
extern int hash_side;
extern int hash_ep[64];

////////////////////////////////////////////////////////////////////////////////////////
// castle variables
////////////////////////////////////////////////////////////////////////////////////////
extern int castle_mask[64];
extern U64 WHITE_00_AMASK;
extern U64 WHITE_000_AMASK;
extern U64 BLACK_00_AMASK;
extern U64 BLACK_000_AMASK;
extern U64 WHITE_00_PMASK;
extern U64 WHITE_000_PMASK;
extern U64 BLACK_00_PMASK;
extern U64 BLACK_000_PMASK;

////////////////////////////////////////////////////////////////////////////////////////
// perft variables
////////////////////////////////////////////////////////////////////////////////////////
U64	retVal;
U64	retCap;
U64	retEpp;

////////////////////////////////////////////////////////////////////////////////////////
// piece codes and piece values
///////////////////////////////////////////////////////////////////////////////////////
extern int piececodes[13];
extern char displaycodes[17];
extern int piece_value[16];
extern int W_PAWN;
extern int W_KNIGHT;
extern int W_BISHOP;
extern int W_ROOK;
extern int W_QUEEN;
extern int W_KING;
extern int B_PAWN;
extern int B_KNIGHT;
extern int B_BISHOP;
extern int B_ROOK;
extern int B_QUEEN;
extern int B_KING;



