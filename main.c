#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/timeb.h>
#include "data.h"
#include "sliding.h"
#include "stepping.h"
#include "main.h"
#include "knowledge.h"

int main(int argc, char **argv)
{
	
	// get parameters from the command line
	parameters(argc, argv);
	
	//initialization of stuff
	initmagicmoves();	
	init_hash();
	init_board();
	open_book();
	max_time = 1 << 25;
	evalPtr = &evalOne;	
	max_depth = 2;
	
	// uncomment one of the following lines and compile to run an experiment
	// experiment1(); return 0;
	// experiment2(); return 0;
	// printf("score for playerOne is %d\n",experiment3(100)); return 0;
	// experiment4(); return 0;
	
	max_depth = 6;
	// xboard(); // uncomment this line to communicate with xboard/winboard
	int computer_side;
	char s[256];
	int m;

	printf("\n");
	printf("Nick's Simple Chess Program (NSCP)\n");
	printf("version 1.0, 12/22/20\n");
	printf("Copyright 2020 Nick Kinney\n");
	printf("\n");
	printf("\"help\" displays a list of commands.\n");
	printf("\n");

	gen();
	computer_side = -1;
	
	for (;;) {
		if (side == computer_side) {  /* computer's turn */			
			/* think about the move and make it */
			think(1);
			if (!pv[0][0].u) {
				printf("(no legal moves)\n");
				computer_side = -1;
				continue;
			}
			printf("Computer's move: %s\n", move_str(pv[0][0].b));
			makemove(pv[0][0].b);
			ply = 0;
			gen();
			print_result();
			continue;
		}

		/* get user input */
		printf("tscp> ");
		if (scanf("%s", s) == EOF)
			return 0;
		if (!strcmp(s, "on")) {
			computer_side = side;
			continue;
		}
		if (!strcmp(s, "off")) {
			computer_side = -1;
			continue;
		}
		if (!strcmp(s, "st")) {
			if (scanf("%d", &max_time));
			max_time *= 1000;
			max_depth = 32;
			continue;
		}
		if (!strcmp(s, "sd")) {
			if (scanf("%d", &max_depth));
			printf("depth set to %d\n",max_depth);
			max_time = 1 << 25;
			continue;
		}
		if (!strcmp(s, "undo")) {
			if (!hply)
				continue;
			computer_side = -1;
			takeback();
			ply = 0;
			gen();
			continue;
		}
		if (!strcmp(s, "fen")) {
			printf("\nenter fen string for evaluation: ");
			if ( scanf(" %[^\n]s", s) == EOF )
				return 0;
			readFen(s);
			display();
			side == LIGHT ? printf("evalOne = %d\n",evalOne()) : printf("evalOne = %d\n\n",-1*evalOne());
			printf("space metric = %d\n\n",space());
			continue;
		}
		if (!strcmp(s, "new")) {
			computer_side = -1;
			init_board();
			gen();
			continue;
		}
		if (!strcmp(s, "ss")) {
			int squareCount[64];
			int whitescore = 0; int blackscore = 0; int i;
			TIME_BONUS_ONE = 1;
			memset(squareCount,0,sizeof squareCount);
			for ( i = 0 ; i < hply ; i++ ) {
				printf("parsing move %d to %d\n",(int)hist_dat[i].m.b.to,(int)hist_dat[i].m.b.from);
				squareCount[(int)hist_dat[i].m.b.to] = ( squareCount[(int)hist_dat[i].m.b.from] + 1 );
				squareCount[(int)hist_dat[i].m.b.from] = 0;
			}
			for ( i = 0 ; i < 64 ; i++ ) {
				if ( squareCount[i] > 0 ) {
					printf("%d\t%d\t%d\n",i,squares[i],squareCount[i]);
					if ( squares[i] < 8 )
						whitescore -= TIME_BONUS_ONE * squareCount[i];
					else
						blackscore -= TIME_BONUS_ONE * squareCount[i];
				}
			}
			printf("white time = %d, blackTime = %d, difference = %d\n",whitescore, blackscore, whitescore-blackscore);
		}
		if (!strcmp(s, "perft")) {
			perft();
			continue;
		}
		if (!strcmp(s, "d")) {
			display();
			continue;
		}
		if (!strcmp(s, "bye")) {
			printf("Share and enjoy!\n");
			break;
		}
		if (!strcmp(s, "help")) {
			printf("on - computer plays for the side to move\n");
			printf("off - computer stops playing\n");
			printf("sd n - search n ply per move\n");
			printf("undo - takes back a move\n");
			printf("new - starts a new game\n");
			printf("d - display the board\n");
			printf("bye - exit the program\n");
			printf("perft - run performance test of the move generator\n");
			printf("xboard - switch to XBoard mode\n");
			printf("Enter moves in coordinate notation, e.g., e2e4, e7e8Q\n");
			continue;
		}

		/* maybe the user entered a move? */
		m = parse_move(s);
		if (m == -1 || !makemove(gen_dat[m].m.b))
			printf("Illegal move.\n\n");
		else {
			ply = 0;
			gen();
			print_result();
		}
	}
	close_book();
    return 0;
}


/* xboard() is a substitute for main() that is XBoard
   and WinBoard compatible. See the following page for details:
   http://www.research.digital.com/SRC/personal/mann/xboard/engine-intf.html */

void xboard()
{
	int computer_side;
	char line[256], command[256];
	int m;
	int post = 0;

	signal(SIGINT, SIG_IGN);
	printf("\n");
	init_board();
	gen();
	computer_side = -1;
	for (;;) {
		fflush(stdout);
		if (side == computer_side) {
			think(post);
			if (!pv[0][0].u) {
				computer_side = -1;
				continue;
			}
			printf("move %s\n", move_str(pv[0][0].b));
			makemove(pv[0][0].b);
			ply = 0;
			gen();
			print_result();
			continue;
		}
		if (!fgets(line, 256, stdin))
			return;
		if (line[0] == '\n')
			continue;
		sscanf(line, "%s", command);
		if (!strcmp(command, "xboard"))
			continue;
		if (!strcmp(command, "new")) {
			init_board();
			gen();
			computer_side = DARK;
			continue;
		}
		if (!strcmp(command, "quit"))
			return;
		if (!strcmp(command, "force")) {
			computer_side = -1;
			continue;
		}
		if (!strcmp(command, "white")) {
			side = LIGHT;
			xside = DARK;
			gen();
			computer_side = DARK;
			continue;
		}
		if (!strcmp(command, "black")) {
			side = DARK;
			xside = LIGHT;
			gen();
			computer_side = LIGHT;
			continue;
		}
		if (!strcmp(command, "st")) {
			sscanf(line, "st %d", &max_time);
			max_time *= 1000;
			max_depth = 32;
			continue;
		}
		if (!strcmp(command, "sd")) {
			sscanf(line, "sd %d", &max_depth);
			max_time = 1 << 25;
			continue;
		}
		if (!strcmp(command, "time")) {
			sscanf(line, "time %d", &max_time);
			max_time *= 10;
			max_time /= 30;
			max_depth = 32;
			continue;
		}
		if (!strcmp(command, "otim")) {
			continue;
		}
		if (!strcmp(command, "go")) {
			computer_side = side;
			continue;
		}
		if (!strcmp(command, "hint")) {
			think(0);
			if (!pv[0][0].u)
				continue;
			printf("Hint: %s\n", move_str(pv[0][0].b));
			continue;
		}
		if (!strcmp(command, "undo")) {
			if (!hply)
				continue;
			takeback();
			ply = 0;
			gen();
			continue;
		}
		if (!strcmp(command, "remove")) {
			if (hply < 2)
				continue;
			takeback();
			takeback();
			ply = 0;
			gen();
			continue;
		}
		if (!strcmp(command, "post")) {
			post = 2;
			continue;
		}
		if (!strcmp(command, "nopost")) {
			post = 0;
			continue;
		}
		m = parse_move(line);
		if (m == -1 || !makemove(gen_dat[m].m.b))
			printf("Error (unknown command): %s\n", command);
		else {
			ply = 0;
			gen();
			print_result();
		}
	}
}