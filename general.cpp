#include <stdbool.h>

typedef long hash_t;
const long N_COMANDS = 21;

#define MEM_MASK 0b10000000
#define REG_MASK 0b01000000
#define KON_MASK 0b00100000
#define CMD_MASK 0b00011111

enum operation_t {
	HLT = 0,
	ADD = 1,
	SUB = 2,
	MUL = 3,
	DIV = 4,
	SQRT = 5,
	IN = 6,
	RET = 7,
	DRW = 8,
	OU = 9,
	PUSH = 10,
	POP = 11,
	JMP = 12,
	JA = 13,
	JAE = 14,
	JB = 15,
	JBE = 16,
	JE = 17,
	JNE = 18,
	CALL = 19,
	MAXP = 20,
	ERR = -1
};

hash_t CMD_HASHES[N_COMANDS] = {
	5524552, // HLT
	4473921, // ADD
	4347219, // SUB
	5002573, // MUL
	5654852, // DIV
	1414680915, // SQRT
	20041, // IN
	5522770, // RET
	5722692, // DRW
	21839, // OU
	1213420880, // PUSH
	5263184, // POP
	5262666, // JMP
	16714, // JA
	4538698, // JAE
	16970, // JB
	4538954, // JBE
	17738, // JE
	4542026, // JNE
	1280065859, // CALL
	1347961165, // MAXP
};
