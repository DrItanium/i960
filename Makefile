include config.mk

I960JX_OBJS := Operand.o \
	Instruction.o
I960JX_SIM_OBJS := sim960jx.o \
	core.o \
	ProcessControls.o \
	NormalRegister.o \
	QuadRegister.o \
	TripleRegister.o \
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
.cpp.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -f ${OBJS} ${PROGS}



.PHONY: options

# generated via g++ -MM -std=c++17 *.cc *.h



ArithmeticControls.o: ArithmeticControls.h types.h
ConditionCode.o: ConditionCode.h types.h
Instruction.o: Instruction.h types.h Operand.h opcodes.h opcodes.def
InternalDataRam.o: InternalDataRam.h types.h
MemoryMap.o: MemoryMap.h types.h
NormalRegister.o: NormalRegister.h types.h ProcessControls.h
Operand.o: Operand.h types.h
PMCONRegister.o: PMCONRegister.h types.h
ProcessControls.o: ProcessControls.h types.h
ProcessorControlBlock.o: ProcessorControlBlock.h types.h
QuadRegister.o: QuadRegister.h types.h NormalRegister.h ProcessControls.h
Records.o: Records.h types.h ProcessControls.h ArithmeticControls.h
StartupRecord.o: StartupRecord.h types.h
TripleRegister.o: TripleRegister.h types.h NormalRegister.h \
 ProcessControls.h
core.o: core.h types.h NormalRegister.h ProcessControls.h \
 TripleRegister.h QuadRegister.h ArithmeticControls.h memiface.h \
 Operand.h Instruction.h opcodes.h opcodes.def InternalDataRam.h \
 ConditionCode.h ProcessorControlBlock.h MemoryMap.h StartupRecord.h
memiface.o: memiface.h types.h
opcodes.o: opcodes.h types.h opcodes.def
types.o: types.h
ArithmeticControls.o: ArithmeticControls.cpp types.h ArithmeticControls.h
Instruction.o: Instruction.cpp Instruction.h types.h Operand.h opcodes.h \
 opcodes.def
NormalRegister.o: NormalRegister.cpp NormalRegister.h types.h \
 ProcessControls.h
Operand.o: Operand.cpp Operand.h types.h
ProcessControls.o: ProcessControls.cpp ProcessControls.h types.h
QuadRegister.o: QuadRegister.cpp types.h QuadRegister.h NormalRegister.h \
 ProcessControls.h
TripleRegister.o: TripleRegister.cpp types.h TripleRegister.h \
 NormalRegister.h ProcessControls.h
core.o: core.cpp types.h core.h NormalRegister.h ProcessControls.h \
 TripleRegister.h QuadRegister.h ArithmeticControls.h memiface.h \
 Operand.h Instruction.h opcodes.h opcodes.def InternalDataRam.h \
 ConditionCode.h ProcessorControlBlock.h MemoryMap.h StartupRecord.h
decode960jx.o: decode960jx.cc types.h opcodes.h opcodes.def Instruction.h \
 Operand.h
sim960jx.o: sim960jx.cc types.h NormalRegister.h ProcessControls.h \
 ArithmeticControls.h Operand.h opcodes.h opcodes.def Instruction.h
