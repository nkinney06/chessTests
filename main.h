#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// from movesgen.c
int isattacked ( U64 bitmap, int side_to_move );
void gen_promote(int from, int to, int bits, int prom);
void gen_push(int from, int to, int bits);
void gen_caps();
int countMoves( int sideToCount );
void gen();
void takeback();
BOOL makemove(move_bytes m);
void enumerate(int depth);

// from fen.c
int64_t my_getline(char **restrict line, size_t *restrict len, FILE *restrict fp);
void setupFen(char *fen, char *fencolor, char *fencastling, char *fenenpassant, int fenhalfmoveclock, int fenfullmovenumber);
void readFen( char *line );
void readEcoPositions(const char * myfilename, int number);
void readProblem(int number);
void readTestPositions(const char * myfilename, int number);
void perft();
void print_fen();

// from search.c
int get_ms();
void think(int output);
void checkup();
void sort_pv();
void sort(int from);
int quiesce(int alpha,int beta);
int search(int alpha, int beta, int depth);
int reps();

// from book.c
void open_book();
void close_book();
int book_move();
void follow_line( int number );
BOOL book_match(char *s1, char *s2);

// from parameters.c
void parameters(int argc, char *argv[]);

// from status.c
int space();
void print_result();
int hash_rand();
void init_hash();
void set_hash();
void display ();
int parse_move(char *s);
char *move_str(move_bytes m);
void init_board (void);
void clear_board ();

// evaluation functions
int (*evalPtr)();
int evalOne();
int evalTwo();

// from main.c
void xboard();

// from stepping.c
int bitcount ( U64 board );
int firstone ( U64 board );
unsigned int lastone(U64 bitmap);
U64 reverse ( U64 v );

// from experiments.c
int check_result();
void experiment1();
void experiment2();
int experiment3(int games);
void experiment4();