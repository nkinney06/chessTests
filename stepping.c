#include "stepping.h"

U64 MS1BTABLE[256] =
{
0x0000000000000000, 0x0000000000000000, 0x0000000000000001, 0x0000000000000001, 0x0000000000000002, 0x0000000000000002, 0x0000000000000002, 0x0000000000000002, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003,
0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004, 0x0000000000000004,
0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005,
0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005, 0x0000000000000005,
0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006,
0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006,
0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006,
0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000006,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007
};

U64 M1  = 0x5555555555555555;
U64 M2  = 0x3333333333333333;
U64 M4  = 0x0F0F0F0F0F0F0F0F;
U64 M8  = 0x00FF00FF00FF00FF;
U64 M16 = 0x0000FFFF0000FFFF;
U64 M32 = 0x00000000FFFFFFFF;

int DEBRUIJNINDEX[64] = 
{ 
	63, 0,  58,  1, 59, 47, 53,  2, 
	60, 39, 48, 27, 54, 33, 42,  3,
	61, 51, 37, 40, 49, 18, 28, 20,
	55, 30, 34, 11, 43, 14, 22,  4,
	62, 57, 46, 52, 38, 26, 32, 41,
	50, 36, 17, 19, 29, 10, 13, 21,
	56, 45, 25, 31, 35, 16,  9, 12,
	44, 24, 15,  8, 23,  7,  6,  5 
};

U64 bitset[64] =
{
	0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 
	0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 
	0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 
	0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 
	0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 
	0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 
	0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 
	0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 
	0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 
	0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 
	0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 
	0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 
	0x0001000000000000, 0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 
	0x0010000000000000, 0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 
	0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 
	0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000 
};

U64 whitepawnattacks[64] = 
{
	0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
	0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
	0x0000000000000002, 0x0000000000000005, 0x000000000000000A, 0x0000000000000014, 
	0x0000000000000028, 0x0000000000000050, 0x00000000000000A0, 0x0000000000000040, 
	0x0000000000000200, 0x0000000000000500, 0x0000000000000A00, 0x0000000000001400, 
	0x0000000000002800, 0x0000000000005000, 0x000000000000A000, 0x0000000000004000, 
	0x0000000000020000, 0x0000000000050000, 0x00000000000A0000, 0x0000000000140000, 
	0x0000000000280000, 0x0000000000500000, 0x0000000000A00000, 0x0000000000400000, 
	0x0000000002000000, 0x0000000005000000, 0x000000000A000000, 0x0000000014000000, 
	0x0000000028000000, 0x0000000050000000, 0x00000000A0000000, 0x0000000040000000, 
	0x0000000200000000, 0x0000000500000000, 0x0000000A00000000, 0x0000001400000000, 
	0x0000002800000000, 0x0000005000000000, 0x000000A000000000, 0x0000004000000000, 
	0x0000020000000000, 0x0000050000000000, 0x00000A0000000000, 0x0000140000000000, 
	0x0000280000000000, 0x0000500000000000, 0x0000A00000000000, 0x0000400000000000, 
	0x0002000000000000, 0x0005000000000000, 0x000A000000000000, 0x0014000000000000, 
	0x0028000000000000, 0x0050000000000000, 0x00A0000000000000, 0x0040000000000000 
};

U64 blackpawnattacks[64] = 
{
	0x0000000000000200, 0x0000000000000500, 0x0000000000000A00, 0x0000000000001400, 
	0x0000000000002800, 0x0000000000005000, 0x000000000000A000, 0x0000000000004000, 
	0x0000000000020000, 0x0000000000050000, 0x00000000000A0000, 0x0000000000140000, 
	0x0000000000280000, 0x0000000000500000, 0x0000000000A00000, 0x0000000000400000, 
	0x0000000002000000, 0x0000000005000000, 0x000000000A000000, 0x0000000014000000, 
	0x0000000028000000, 0x0000000050000000, 0x00000000A0000000, 0x0000000040000000, 
	0x0000000200000000, 0x0000000500000000, 0x0000000A00000000, 0x0000001400000000, 
	0x0000002800000000, 0x0000005000000000, 0x000000A000000000, 0x0000004000000000, 
	0x0000020000000000, 0x0000050000000000, 0x00000A0000000000, 0x0000140000000000, 
	0x0000280000000000, 0x0000500000000000, 0x0000A00000000000, 0x0000400000000000, 
	0x0002000000000000, 0x0005000000000000, 0x000A000000000000, 0x0014000000000000, 
	0x0028000000000000, 0x0050000000000000, 0x00A0000000000000, 0x0040000000000000, 
	0x0200000000000000, 0x0500000000000000, 0x0A00000000000000, 0x1400000000000000, 
	0x2800000000000000, 0x5000000000000000, 0xA000000000000000, 0x4000000000000000, 
	0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 
	0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 
};

U64 knightattacks[64] = 
{
	0x0000000000020400, 0x0000000000050800, 0x00000000000A1100, 0x0000000000142200, 
	0x0000000000284400, 0x0000000000508800, 0x0000000000A01000, 0x0000000000402000, 
	0x0000000002040004, 0x0000000005080008, 0x000000000A110011, 0x0000000014220022, 
	0x0000000028440044, 0x0000000050880088, 0x00000000A0100010, 0x0000000040200020, 
	0x0000000204000402, 0x0000000508000805, 0x0000000A1100110A, 0x0000001422002214, 
	0x0000002844004428, 0x0000005088008850, 0x000000A0100010A0, 0x0000004020002040, 
	0x0000020400040200, 0x0000050800080500, 0x00000A1100110A00, 0x0000142200221400, 
	0x0000284400442800, 0x0000508800885000, 0x0000A0100010A000, 0x0000402000204000, 
	0x0002040004020000, 0x0005080008050000, 0x000A1100110A0000, 0x0014220022140000, 
	0x0028440044280000, 0x0050880088500000, 0x00A0100010A00000, 0x0040200020400000, 
	0x0204000402000000, 0x0508000805000000, 0x0A1100110A000000, 0x1422002214000000, 
	0x2844004428000000, 0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000, 
	0x0400040200000000, 0x0800080500000000, 0x1100110A00000000, 0x2200221400000000, 
	0x4400442800000000, 0x8800885000000000, 0x100010A000000000, 0x2000204000000000, 
	0x0004020000000000, 0x0008050000000000, 0x00110A0000000000, 0x0022140000000000, 
	0x0044280000000000, 0x0088500000000000, 0x0010A00000000000, 0x0020400000000000 
};

U64 kingattacks[64] = 
{
	0x0000000000000302, 0x0000000000000705, 0x0000000000000E0A, 0x0000000000001C14, 
	0x0000000000003828, 0x0000000000007050, 0x000000000000E0A0, 0x000000000000C040, 
	0x0000000000030203, 0x0000000000070507, 0x00000000000E0A0E, 0x00000000001C141C, 
	0x0000000000382838, 0x0000000000705070, 0x0000000000E0A0E0, 0x0000000000C040C0, 
	0x0000000003020300, 0x0000000007050700, 0x000000000E0A0E00, 0x000000001C141C00, 
	0x0000000038283800, 0x0000000070507000, 0x00000000E0A0E000, 0x00000000C040C000, 
	0x0000000302030000, 0x0000000705070000, 0x0000000E0A0E0000, 0x0000001C141C0000, 
	0x0000003828380000, 0x0000007050700000, 0x000000E0A0E00000, 0x000000C040C00000, 
	0x0000030203000000, 0x0000070507000000, 0x00000E0A0E000000, 0x00001C141C000000, 
	0x0000382838000000, 0x0000705070000000, 0x0000E0A0E0000000, 0x0000C040C0000000, 
	0x0003020300000000, 0x0007050700000000, 0x000E0A0E00000000, 0x001C141C00000000, 
	0x0038283800000000, 0x0070507000000000, 0x00E0A0E000000000, 0x00C040C000000000, 
	0x0302030000000000, 0x0705070000000000, 0x0E0A0E0000000000, 0x1C141C0000000000, 
	0x3828380000000000, 0x7050700000000000, 0xE0A0E00000000000, 0xC040C00000000000, 
	0x0203000000000000, 0x0507000000000000, 0x0A0E000000000000, 0x141C000000000000, 
	0x2838000000000000, 0x5070000000000000, 0xA0E0000000000000, 0x40C0000000000000 
};

int bitcount ( U64 board ) {
board = ( board & M1  ) + ( ( board >> 1  ) & M1  );
board = ( board & M2  ) + ( ( board >> 2  ) & M2  );
board = ( board & M4  ) + ( ( board >> 4  ) & M4  );
board = ( board & M8  ) + ( ( board >> 8  ) & M8  );
board = ( board & M16 ) + ( ( board >> 16 ) & M16 );
board = ( board & M32 ) + ( ( board >> 32 ) & M32 );
return board;
}

int firstone ( U64 board ) { // faster than lastone
	return DEBRUIJNINDEX[ ( ( board & -board ) * 0x07EDD5E59A4E28C2 ) >> 58 ]; 
}

unsigned int lastone(U64 bitmap) { // Eugene Nalimov's bitScanReverse
	int result = 0;
	if (bitmap > 0xFFFFFFFF) {
		bitmap >>= 32;
		result = 32;
	}
	if (bitmap > 0xFFFF) {
		bitmap >>= 16;
		result += 16;
	}
	if (bitmap > 0xFF) {
		bitmap >>= 8;
		result += 8;
	}
	return result + MS1BTABLE[bitmap];
}

U64 reverse ( U64 v ) {
	v = ((v >> 1)  & 0x5555555555555555) | ((v & 0x5555555555555555) << 1 );
	v = ((v >> 2)  & 0x3333333333333333) | ((v & 0x3333333333333333) << 2 );
	v = ((v >> 4)  & 0x0F0F0F0F0F0F0F0F) | ((v & 0x0F0F0F0F0F0F0F0F) << 4 );
	v = ((v >> 8)  & 0x00FF00FF00FF00FF) | ((v & 0x00FF00FF00FF00FF) << 8 );
	v = ((v >> 16) & 0x0000FFFF0000FFFF) | ((v & 0x0000FFFF0000FFFF) << 16);
	v = ( v >> 32             ) | ( v               << 32);
	return v;
}