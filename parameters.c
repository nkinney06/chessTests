#include <stdio.h>  
#include <string.h>       
#include <stdlib.h>  
#include "data.h"
#include "main.h"
#include "knowledge.h"
#include <getopt.h>

void parameters(int argc, char *argv[]) {
	int c;
	int  option_index    = 0;
	struct option long_options[] = {
		{"PENALTY_DOUBLED_PAWN_ONE",  required_argument, 0,  1 },
		{"PENALTY_ISOLATED_PAWN_ONE",required_argument, 0,  2 },
		{"PENALTY_BACKWARD_PAWN_ONE",required_argument, 0,  3 },
		{"BONUS_PASSED_PAWN_ONE",  required_argument, 0,  4 },			   
		{"ROOK_OPEN_FILE_BONUS_ONE",  required_argument, 0,  5 },
		{"ROOK_ON_SEVENTH_BONUS_ONE",required_argument, 0,  6 },
		{"BONUS_ROOK_BEHIND_PASSED_PAWN_ONE",  required_argument, 0,  7 },
		{"BONUS_BISHOP_PAIR_ONE", required_argument, 0,  8 },				   
		{"BAD_BISHOP_PENALTY_ONE",  required_argument, 0,  9 },
		{"GOOD_KNIGHT_BONUS_ONE",required_argument, 0,  10 },
		{"ROOK_SEMI_OPEN_FILE_BONUS_ONE",required_argument, 0,  11 },
		{"BONUS_MOVE_OPTIONS_ONE",required_argument, 0,  12 },
		{"TIME_BONUS_ONE",required_argument, 0,  25 },
		{"PENALTY_DOUBLED_PAWN_TWO",  required_argument, 0,  13 },
		{"PENALTY_ISOLATED_PAWN_TWO",required_argument, 0,  14 },
		{"PENALTY_BACKWARD_PAWN_TWO",required_argument, 0,  15 },
		{"BONUS_PASSED_PAWN_TWO",  required_argument, 0,  16 },			   
		{"ROOK_OPEN_FILE_BONUS_TWO",  required_argument, 0,  17 },
		{"ROOK_ON_SEVENTH_BONUS_TWO",required_argument, 0,  18 },
		{"BONUS_ROOK_BEHIND_PASSED_PAWN_TWO",  required_argument, 0,  19 },
		{"BONUS_BISHOP_PAIR_TWO", required_argument, 0,  20 },				   
		{"BAD_BISHOP_PENALTY_TWO",  required_argument, 0,  21 },
		{"GOOD_KNIGHT_BONUS_TWO",required_argument, 0,  22 },
		{"ROOK_SEMI_OPEN_FILE_BONUS_TWO",required_argument, 0,  23 },
		{"BONUS_MOVE_OPTIONS_TWO",required_argument, 0,  24 },
		{"TIME_BONUS_TWO",required_argument, 0,  26 },
		{0,         0,                 0, 0 }
	};

	while (1) {

	   c = getopt_long(argc, argv,"",long_options, &option_index); //looks for short options first

	   if (c == -1) //there are no more option characters, break out of loop
		   break;

	   switch (c) {
		   
		   case 0:
			   printf("option %s", long_options[option_index].name);
			   if (optarg)
				   printf(" with arg %s", optarg);
			   printf("\n");
			   break;
		   case 1:
			   PENALTY_DOUBLED_PAWN_ONE   = atoi(optarg);
			   break;
		   case 2:
			   PENALTY_ISOLATED_PAWN_ONE = atoi(optarg);
			   break;
		   case 3:
			   PENALTY_BACKWARD_PAWN_ONE = atoi(optarg);
			   break;
		   case 4:
			   BONUS_PASSED_PAWN_ONE = atoi(optarg);
			   break;	
		   case 5:
			   ROOK_OPEN_FILE_BONUS_ONE  = atoi(optarg);
			   break;
		   case 6:
			   ROOK_ON_SEVENTH_BONUS_ONE = atoi(optarg);
			   break;
			case 7:
			   BONUS_ROOK_BEHIND_PASSED_PAWN_ONE = atoi(optarg);
			   break;	
		   case 8:
			   BONUS_BISHOP_PAIR_ONE   = atoi(optarg);
			   break;
		   case 9:
			   BAD_BISHOP_PENALTY_ONE  = atoi(optarg);
			   break;
		   case 10:
			   GOOD_KNIGHT_BONUS_ONE = atoi(optarg);
			   break;
		   case 11:
			   ROOK_SEMI_OPEN_FILE_BONUS_ONE = atoi(optarg);
			   break;
		   case 12:
			   BONUS_MOVE_OPTIONS_ONE = atoi(optarg);
			   break;
		   case 25:
			   TIME_BONUS_ONE = atoi(optarg);
			   break;
		   case 13:
			   PENALTY_DOUBLED_PAWN_TWO   = atoi(optarg);
			   break;
		   case 14:
			   PENALTY_ISOLATED_PAWN_TWO = atoi(optarg);
			   break;
		   case 15:
			   PENALTY_BACKWARD_PAWN_TWO = atoi(optarg);
			   break;
		   case 16:
			   BONUS_PASSED_PAWN_TWO = atoi(optarg);
			   break;	
		   case 17:
			   ROOK_OPEN_FILE_BONUS_TWO  = atoi(optarg);
			   break;
		   case 18:
			   ROOK_ON_SEVENTH_BONUS_TWO = atoi(optarg);
			   break;
			case 19:
			   BONUS_ROOK_BEHIND_PASSED_PAWN_TWO = atoi(optarg);
			   break;	
		   case 20:
			   BONUS_BISHOP_PAIR_TWO   = atoi(optarg);
			   break;
		   case 21:
			   BAD_BISHOP_PENALTY_TWO  = atoi(optarg);
			   break;
		   case 22:
			   GOOD_KNIGHT_BONUS_TWO = atoi(optarg);
			   break;
		   case 23:
			   ROOK_SEMI_OPEN_FILE_BONUS_TWO = atoi(optarg);
			   break;
		   case 24:
			   BONUS_MOVE_OPTIONS_TWO = atoi(optarg);
			   break;
		   case 26:
			   TIME_BONUS_TWO = atoi(optarg);
			   break;
		   case '?':
			   break;
		   default:
			   printf("?? getopt returned character code 0%o ??\n", c);
			   break;
			   
		   }
	   }

	   if (optind < argc) {
		   printf("non-option ARGV-elements: ");
		   while (optind < argc)
			   printf("%s ", argv[optind++]);
		   printf("\n");
	   }		
}
