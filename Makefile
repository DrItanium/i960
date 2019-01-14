include config.mk

I960JX_OBJS := sim.o core.o dispatcher.o opcodes.o
OBJS := $(I960JX_OBJS)

I960JX_PROG := sim960jx
PROGS := $(I960JX_PROG) 


all: $(I960JX_PROG)

options:
	@echo Build Options
	@echo ------------------
	@echo CFLAGS = ${CFLAGS}
	@echo CXXFLAGS = ${CXXFLAGS}
	@echo LDFLAGS = ${LDFLAGS}
	@echo ------------------


$(I960JX_PROG): $(I960JX_OBJS)
	@echo LD ${I960JX_PROG}
	@${LD} ${LDFLAGS} -o ${I960JX_PROG} ${I960JX_OBJS}

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


.PHONY: options
