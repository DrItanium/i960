include config.mk

I960JX_OBJS := opcodes.o types.o Operand.o
I960JX_SIM_OBJS := sim.o \
	core.o \
	dispatcher.o \
	ProcessControls.o \
	NormalRegister.o \
	$(I960JX_OBJS)
I960JX_DEC_OBJS := decoder.o \
	$(I960JX_OBJS)

I960JX_SIM_PROG := sim960jx
I960JX_DEC_PROG := decode960jx
OBJS := $(I960JX_DEC_OBJS) $(I960JX_SIM_OBJS)
PROGS := $(I960JX_SIM_PROG) $(I960JX_DEC_PROG)


all: $(PROGS)

options:
	@echo Build Options
	@echo ------------------
	@echo CFLAGS = ${CFLAGS}
	@echo CXXFLAGS = ${CXXFLAGS}
	@echo LDFLAGS = ${LDFLAGS}
	@echo ------------------


$(I960JX_SIM_PROG): $(I960JX_SIM_OBJS)
	@echo LD ${I960JX_SIM_PROG}
	@${LD} ${LDFLAGS} -o ${I960JX_SIM_PROG} ${I960JX_SIM_OBJS}

$(I960JX_DEC_PROG): $(I960JX_DEC_OBJS)
	@echo LD ${I960JX_DEC_PROG}
	@${LD} ${LDFLAGS} -o ${I960JX_DEC_PROG} ${I960JX_DEC_OBJS}

.cc.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -f ${OBJS} ${PROGS}

# generated via g++ -MM -std=c++17 *.cc *.h


NormalRegister.o: NormalRegister.cc NormalRegister.h types.h \
 ProcessControls.h
Operand.o: Operand.cc Operand.h types.h
ProcessControls.o: ProcessControls.cc ProcessControls.h types.h
core.o: core.cc types.h core.h NormalRegister.h ProcessControls.h \
 DoubleRegister.h TripleRegister.h QuadRegister.h ArithmeticControls.h \
 memiface.h Operand.h Instruction.h conditional_kinds.def operations.h \
 opcodes.h opcodes.def
decoder.o: decoder.cc types.h opcodes.h Instruction.h Operand.h \
 opcodes.def
dispatcher.o: dispatcher.cc types.h core.h NormalRegister.h \
 ProcessControls.h DoubleRegister.h TripleRegister.h QuadRegister.h \
 ArithmeticControls.h memiface.h Operand.h Instruction.h \
 conditional_kinds.def opcodes.h opcodes.def
opcodes.o: opcodes.cc types.h opcodes.h Instruction.h Operand.h \
 opcodes.def
sim.o: sim.cc types.h NormalRegister.h ProcessControls.h \
 ArithmeticControls.h Operand.h operations.h opcodes.h Instruction.h \
 opcodes.def
types.o: types.cc types.h ArithmeticControls.h DoubleRegister.h \
 NormalRegister.h ProcessControls.h TripleRegister.h QuadRegister.h
ArithmeticControls.o: ArithmeticControls.h types.h
DoubleRegister.o: DoubleRegister.h types.h NormalRegister.h \
 ProcessControls.h
Instruction.o: Instruction.h types.h Operand.h
NormalRegister.o: NormalRegister.h types.h ProcessControls.h
Operand.o: Operand.h types.h
ProcessControls.o: ProcessControls.h types.h
QuadRegister.o: QuadRegister.h types.h NormalRegister.h ProcessControls.h
Records.o: Records.h types.h ProcessControls.h ArithmeticControls.h
TripleRegister.o: TripleRegister.h types.h NormalRegister.h \
 ProcessControls.h
bus.o: bus.h
core.o: core.h types.h NormalRegister.h ProcessControls.h \
 DoubleRegister.h TripleRegister.h QuadRegister.h ArithmeticControls.h \
 memiface.h Operand.h Instruction.h conditional_kinds.def
memiface.o: memiface.h types.h
opcodes.o: opcodes.h types.h Instruction.h Operand.h opcodes.def
operations.o: operations.h types.h
types.o: types.h
.PHONY: options
