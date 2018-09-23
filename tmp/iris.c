// micro iris emulator
#include <stdio.h>
#define OP_ARGS Core_t* c, Instruction_t* target
typedef unsigned short word;
typedef signed short sword;
typedef unsigned char byte;
typedef unsigned int dword;
typedef dword raw_instruction;
#define MemorySize (0xFFFF + 1)
#define RegisterCount 256
typedef union Register {
	word u;
	sword i;
	byte b;
} Register_t;
typedef struct Core {
	word ip;
	Register_t registers[RegisterCount];
	word data[MemorySize];
	word stack[MemorySize];
	raw_instruction code[MemorySize];
	byte exec;
	byte incrementNext;
} Core_t;

typedef union Instruction {
	raw_instruction value;
	struct {
		raw_instruction op : 8;
		raw_instruction dest : 8;
		union {
			struct {
				raw_instruction src1 : 8;
				raw_instruction src2 : 8;
			} reg3;
			struct {
				raw_instruction src1 : 8;
				raw_instruction imm8 : 8;
			} reg2WithImm;
			struct {
				raw_instruction src : 8;
			} reg2;
			struct {
				raw_instruction addr : 16;
			} regWithImm16;
		};
	};
} Instruction_t;

void initCore(Core_t* c ) {
	c->exec = 1;
	c->ip = 0x0000;
	c->incrementNext = 1;
	for (int i = 0; i < RegisterCount; ++i) {
		c->registers[i].u = 0;
	}
	for(int i = 0; i < MemorySize; ++i) {
		c->data[i] = 0;
		c->stack[i] = 0;
		c->code[i] = 0;
	}
}

void shutdownCore(Core_t* c) {
	// clear memory to be on the safe side
	for(int i = 0; i < MemorySize; ++i) {
		c->data[i] = 0;
		c->stack[i] = 0;
		c->code[i] = 0;
	}
}
#define X(name, op) \
	word name ## raw ( const Register_t* src1, const Register_t* src2) { \
		return src1->u op src2->u ; \
	} \
	void name ( OP_ARGS ) { \
	byte destInd = target->dest; \
	byte src1Ind = target->reg3.src1;  \
	byte src2Ind = target->reg3.src2; \
	(c->registers[destInd]).u = name ## raw ( &(c->registers[src1Ind]), &(c->registers[src2Ind])); \
	} \
	void name ## i (OP_ARGS) { \
	Register_t tmp; \
	byte destInd = target->dest; \
	byte src1Ind = target->reg2WithImm.src1; \
	word src2Ind = target->reg2WithImm.imm8; \
	tmp.u = src2Ind; \
	c->registers[destInd].u = name ## raw ( &(c->registers[src1Ind]), &tmp); \
	}
#define Y(name, op) \
	word name ## raw ( const Register_t* src1, const Register_t* src2) { \
		return src1->i op src2->i ; \
	} \
	void name ( Core_t* c, Instruction_t* target) { \
	byte destInd = target->dest; \
	byte src1Ind = target->reg3.src1;  \
	byte src2Ind = target->reg3.src2; \
	(c->registers[destInd]).i = name ## raw ( &(c->registers[src1Ind]), &(c->registers[src2Ind])); \
	} \
	void name ## i (Core_t* c , Instruction_t* target) { \
	Register_t tmp; \
	byte destInd = target->dest; \
	byte src1Ind = target->reg2WithImm.src1; \
	word src2Ind = target->reg2WithImm.imm8; \
	tmp.u = src2Ind; \
	c->registers[destInd].i = name ## raw ( &(c->registers[src1Ind]), &tmp); \
	}
X(addo, +)
X(subo, -)
X(mulo, *)
X(divo, /)
X(remo, %)
X(shlo, <<)
X(shro, >>)
Y(addi, +)
Y(subi, -)
Y(muli, *)
Y(divi, /)
Y(remi, %)
Y(shli, <<)
Y(shri, >>)
X(and, &)
X(or, |)
X(xor, ^)
#undef Y
#undef X

void notraw(Register_t* dest, const Register_t* src) {
	dest->u = ~(src->u);
}
void not(OP_ARGS) {
	byte destInd = target->dest;
	byte srcInd = target->reg2.src;
	notraw(&c->registers[destInd], &c->registers[srcInd]);
}
void nand(OP_ARGS) {
	Register_t tmp0, tmp1;
	byte destInd = target->dest;
	byte src1Ind = target->reg3.src1;
	byte src2Ind = target->reg3.src2;
	notraw(&tmp0, &c->registers[src1Ind]);
	notraw(&tmp1, &c->registers[src2Ind]);
	c->registers[destInd].u = orraw(&tmp0, &tmp1);
}

void move(Core_t* c, Instruction_t* target) {
	byte destInd = target->dest;
	byte srcInd = target->reg2.src;
	if (destInd != srcInd) {
		c->registers[destInd].u = c->registers[srcInd].u;
	}
}
void set(Core_t* c, Instruction_t* target) {
	byte destInd = target->dest;
	word immediate = target->regWithImm16.addr;
	c->registers[destInd].u = immediate;
}

void loadData(OP_ARGS) {
	c->registers[target->dest].u = c->data[c->registers[target->reg2.src].u];
}
void storeData(OP_ARGS) {
	c->data[c->registers[target->dest].u] = c->registers[target->reg2.src].u;
}
void loadStack(OP_ARGS) {
	c->registers[target->dest].u = c->stack[c->registers[target->reg2.src].u];
}
void storeStack(OP_ARGS) {
	c->stack[c->registers[target->dest].u] = c->registers[target->reg2.src].u;
}
void push(OP_ARGS) {
	storeStack(c, target);
	// then increment the stack pointer
	Instruction_t uop;
	// don't set the operation as we're directly calling the function
	uop.dest = target->dest;
	uop.reg2WithImm.src1 = target->dest;
	uop.reg2WithImm.imm8 = 1;
	addoi(c, &uop);
}
void pop(OP_ARGS) {
	loadStack(c, target);
	// then decrement the stack pointer in src1
	Instruction_t uop;
	// suboi is being called directly, don't worry about setting the op
	uop.dest = target->reg2.src;
	uop.reg2WithImm.src1 = target->reg2.src;
	uop.reg2WithImm.imm8 = 1;
	suboi(c, &uop);
}
void bial(OP_ARGS) {
	c->incrementNext = 0;
	word returnAddress = c->ip + 1;
	c->registers[target->dest].u = returnAddress;
	c->ip = target->regWithImm16.addr;
}
void bral(OP_ARGS) {
	c->incrementNext = 0;
	word returnAddress = c->ip + 1;
	c->registers[target->dest].u = returnAddress;
	c->ip = c->registers[target->reg2.src].u;
}
void br(OP_ARGS) {
	c->incrementNext = 0;
	c->ip = c->registers[target->dest].u;
}
void bm(OP_ARGS) {
	c->incrementNext = 0;
	c->ip = target->regWithImm16.addr;
}
void ret(OP_ARGS) {
	// dest - sp 
	// src1 - temporary storage
	Instruction_t uop0, uop1;
	uop0.dest = target->reg2.src;
	uop0.reg2.src = target->dest;
	pop(c, &uop0);
	uop1.dest = target->reg2.src;
	br(c, &uop1);
}

void call(OP_ARGS) {
	// dest - link register
	// src1 - stack pointer
	// src2 - target address
	// bral followed by a push onto the stack, then restore dest to what it was before
	// equiv:
	// bral dest src2
	// push src1 dest # at destination
	// # restore dest happens as well
	word destOriginal = c->registers[target->dest].u;
	Instruction_t uop0, uop1;
	uop0.dest = target->dest;
	uop0.reg2.src = target->reg3.src2;
	bral(c, &uop0);
	uop1.dest = target->reg3.src1;
	uop1.reg2.src = target->dest;
	push(c, &uop1);
	c->registers[target->dest].u = destOriginal; // restore dest
}

void swap(OP_ARGS) {
	// dest - sp
	// src1 - reg
	// src2 - reg
	// push sp, src1
	// move src1, src2
	// pop src2, sp
	word sp = target->dest;
	word src1 = target->reg3.src1;
	word src2 = target->reg3.src2;
	Instruction_t uop0, uop1, uop2;
	uop0.dest = sp;
	uop0.reg2.src = src1;
	push(c, &uop0);
	uop1.dest = src1;
	uop1.reg2.src = src2;
	move(c, &uop1);
	uop2.dest = src2;
	uop2.reg2.src = sp;
	pop(c, &uop2);
}

void cycle(Core_t* c) {
	Instruction_t* target = 0;
	while (c->exec != 0) {
		target = (Instruction_t*)&(c->code[c->ip]);
	switch(target->op) {
#define Y(index, op) case index : op (c, target); break; 
#define X(index, op) \
		Y((2 * index), op) \
		Y(((2 * index) + 1), op ## i) 
		X(0, addo)
		X(1, subo)
		X(2, mulo)
		X(3, divo)
		X(4, remo)
		X(5, shlo)
		X(6, shro)
		X(7, addi)
		X(8, subi)
		X(9, muli)
		X(10, divi)
		X(11, remi)
		X(12, shli)
		X(13, shri)
		X(14, and)
		X(15, or)
		X(16, xor)
		Y(34, not)
		Y(35, nand)
		Y(36, move)
		Y(37, set)
		Y(38, loadData)
		Y(39, storeData)
		Y(40, push)
		Y(41, pop)
		Y(42, bial)
		Y(43, br)
		Y(44, bm)
		Y(45, ret)
		Y(46, bral)
		Y(47, call)
		Y(48, swap)
		default: break;
#undef Y
#undef X
	}
		if (c->incrementNext != 0) {
			c->ip++;
		}
		c->incrementNext = 1;
	}
}

int main() {
	Core_t c;
	initCore(&c);
	cycle(&c);
	shutdownCore(&c);

	return 0;
}
