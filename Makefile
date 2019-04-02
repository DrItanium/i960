include config.mk

I960JX_OBJS := opcodes.o Operand.o
I960JX_SIM_OBJS := sim960jx.o \
	core.o \
	dispatcher.o \
	ProcessControls.o \
	NormalRegister.o \
	QuadRegister.o \
	TripleRegister.o \
	DoubleRegister.o \
	ArithmeticControls.o \
	$(I960JX_OBJS)
I960JX_DEC_OBJS := decode960jx.o \
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



.PHONY: options

# generated via g++ -MM -std=c++17 *.cc *.h



ArithmeticControls.o: ArithmeticControls.cc types.h ArithmeticControls.h
DoubleRegister.o: DoubleRegister.cc types.h DoubleRegister.h \
 NormalRegister.h ProcessControls.h
NormalRegister.o: NormalRegister.cc NormalRegister.h types.h \
 ProcessControls.h
Operand.o: Operand.cc Operand.h types.h
ProcessControls.o: ProcessControls.cc ProcessControls.h types.h
QuadRegister.o: QuadRegister.cc types.h QuadRegister.h NormalRegister.h \
 ProcessControls.h
TripleRegister.o: TripleRegister.cc types.h TripleRegister.h \
 NormalRegister.h ProcessControls.h
core.o: core.cc types.h core.h NormalRegister.h ProcessControls.h \
 DoubleRegister.h TripleRegister.h QuadRegister.h ArithmeticControls.h \
 memiface.h Operand.h Instruction.h InternalDataRam.h ConditionCode.h \
 conditional_kinds.def opcodes.h opcodes.def \
 ProcessorControlBlock.h MemoryMap.h

decode960jx.o: decode960jx.cc types.h opcodes.h Instruction.h Operand.h \
 opcodes.def
dispatcher.o: dispatcher.cc types.h core.h NormalRegister.h \
 ProcessControls.h DoubleRegister.h TripleRegister.h QuadRegister.h \
 ArithmeticControls.h memiface.h Operand.h Instruction.h \
 InternalDataRam.h ConditionCode.h conditional_kinds.def opcodes.h \
 opcodes.def
opcodes.o: opcodes.cc types.h opcodes.h Instruction.h Operand.h \
 opcodes.def
sim960jx.o: sim960jx.cc types.h NormalRegister.h ProcessControls.h \
 ArithmeticControls.h Operand.h opcodes.h Instruction.h \
 opcodes.def
ArithmeticControls.o: ArithmeticControls.h types.h
ConditionCode.o: ConditionCode.h types.h
DoubleRegister.o: DoubleRegister.h types.h NormalRegister.h \
 ProcessControls.h
Instruction.o: Instruction.h types.h Operand.h
InternalDataRam.o: InternalDataRam.h types.h
MemoryMap.o: MemoryMap.h types.h
NormalRegister.o: NormalRegister.h types.h ProcessControls.h
Operand.o: Operand.h types.h
PMCONRegister.o: PMCONRegister.h types.h
ProcessControls.o: ProcessControls.h types.h
ProcessorControlBlock.o: ProcessorControlBlock.h types.h
QuadRegister.o: QuadRegister.h types.h NormalRegister.h ProcessControls.h
Records.o: Records.h types.h ProcessControls.h ArithmeticControls.h
TripleRegister.o: TripleRegister.h types.h NormalRegister.h \
 ProcessControls.h
bus.o: bus.h
core.o: core.h types.h NormalRegister.h ProcessControls.h \
 DoubleRegister.h TripleRegister.h QuadRegister.h ArithmeticControls.h \
 memiface.h Operand.h Instruction.h InternalDataRam.h ConditionCode.h \
 conditional_kinds.def
memiface.o: memiface.h types.h
opcodes.o: opcodes.h types.h Instruction.h Operand.h opcodes.def
operations.o: operations.h types.h
types.o: types.h
