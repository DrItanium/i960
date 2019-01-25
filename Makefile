include config.mk

I960JX_OBJS := opcodes.o
I960JX_SIM_OBJS := sim.o core.o dispatcher.o $(I960JX_OBJS)
I960JX_DEC_OBJS := decoder.o $(I960JX_OBJS)

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

sim.o: operations.h types.h opcodes.def opcodes.h conditional_kinds.def sim.cc
core.o: types.h memiface.h core.h operations.h opcodes.def opcodes.h conditional_kinds.def core.cc
dispatcher.o: types.h core.h opcodes.def opcodes.h conditional_kinds.def dispatcher.cc 
opcodes.o: types.h opcodes.def opcodes.h opcodes.cc
decoder.o: types.h operations.h opcodes.def opcodes.h conditional_kinds.def decoder.cc


.PHONY: options
