#define MAX_CMD_BUFF     256   // Console command input buffer
#define MAX_MOV_BUFF    4096   // Max number of moves that we can store (all plies)
#define MAX_GAME_LINE   1024   // Max number of moves in the (game + search) line that we can store
typedef unsigned long long U64;
typedef unsigned long long BitMap;
typedef short SHORTINT;
typedef unsigned short USHORTINT;
typedef int BOOLTYPE;

////////////////////////////////////////////////////////////////////////////////////////
// rank and file arrays
////////////////////////////////////////////////////////////////////////////////////////
extern int rank[64];
extern const int FILES[64];
extern const int RANKS[64];
U64 RANKMASK[64];
U64 FILEMASK[64];
 
////////////////////////////////////////////////////////////////////////////////////////
// piece position arrays used for the evalulation function
////////////////////////////////////////////////////////////////////////////////////////
int PAWNPOS_W[64];
int KNIGHTPOS_W[64];
int BISHOPPOS_W[64];
int ROOKPOS_W[64];
int QUEENPOS_W[64];
int KINGPOS_W[64];
int KINGPOS_ENDGAME_W[64]; 
int PAWNPOS_B[64];
int KNIGHTPOS_B[64];
int BISHOPPOS_B[64];
int ROOKPOS_B[64];
int QUEENPOS_B[64];
int KINGPOS_B[64];
int KINGPOS_ENDGAME_B[64];
int MIRROR[64];

////////////////////////////////////////////////////////////////////////////////////////
// manhattan distance array and board index
////////////////////////////////////////////////////////////////////////////////////////
int DISTANCE[64][64];
int BOARDINDEX[9][9]; // index 0 is not used, only 1..8.

////////////////////////////////////////////////////////////////////////////////////////
// bitmaps used for the evaluation function
////////////////////////////////////////////////////////////////////////////////////////
BitMap OUTPOST_WHITE[64];
BitMap OUTPOST_BLACK[64];
BitMap PASSED_WHITE[64];
BitMap PASSED_BLACK[64];
BitMap ISOLATED_WHITE[64];
BitMap ISOLATED_BLACK[64];
BitMap BACKWARD_WHITE[64];
BitMap BACKWARD_BLACK[64];
BitMap KINGSHIELD_STRONG_W[64];
BitMap KINGSHIELD_STRONG_B[64];
BitMap KINGSHIELD_WEAK_W[64];
BitMap KINGSHIELD_WEAK_B[64];
BitMap WHITE_SQUARES;
BitMap BLACK_SQUARES;

////////////////////////////////////////////////////////////////////////////////////////
// parameters for the evalulation function
////////////////////////////////////////////////////////////////////////////////////////
int BONUS_PAWN_SHIELD_STRONG;
int BONUS_PAWN_SHIELD_WEAK;
int PAWN_OWN_DISTANCE[8];

////////////////////////////////////////////////////////////////////////////////////////
// parameters that are optimized by the genetic algorithm
////////////////////////////////////////////////////////////////////////////////////////
int PENALTY_DOUBLED_PAWN_ONE;		
int PENALTY_ISOLATED_PAWN_ONE;		
int PENALTY_BACKWARD_PAWN_ONE;		
int BONUS_PASSED_PAWN_ONE;
int ROOK_OPEN_FILE_BONUS_ONE;		
int ROOK_ON_SEVENTH_BONUS_ONE;		
int BONUS_ROOK_BEHIND_PASSED_PAWN_ONE;
int BONUS_BISHOP_PAIR_ONE;
int BAD_BISHOP_PENALTY_ONE;	
int GOOD_KNIGHT_BONUS_ONE;		
int ROOK_SEMI_OPEN_FILE_BONUS_ONE;
int BONUS_MOVE_OPTIONS_ONE;
int TIME_BONUS_ONE;
int PENALTY_DOUBLED_PAWN_TWO;		
int PENALTY_ISOLATED_PAWN_TWO;		
int PENALTY_BACKWARD_PAWN_TWO;		
int BONUS_PASSED_PAWN_TWO;
int ROOK_OPEN_FILE_BONUS_TWO;		
int ROOK_ON_SEVENTH_BONUS_TWO;	
int BONUS_ROOK_BEHIND_PASSED_PAWN_TWO;
int BONUS_BISHOP_PAIR_TWO;
int BAD_BISHOP_PENALTY_TWO;	
int GOOD_KNIGHT_BONUS_TWO;	
int ROOK_SEMI_OPEN_FILE_BONUS_TWO;
int BONUS_MOVE_OPTIONS_TWO;
int TIME_BONUS_TWO;

