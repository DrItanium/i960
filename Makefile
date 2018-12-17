include config.mk

I960MC_OBJS := misc.o sim.o numericops.o core.o dispatcher.o numericsisa.o
I960JX_OBJS := misc.o sim.o core.o dispatcher.o 
OBJS := $(I960MC_OBJS) $(I960JX_OBJS)

I960MC_PROG := sim960mc
I960JX_PROG := sim960jx
PROGS := $(I960JX_PROG) $(I960MC_PROG)


all: $(I960MC_PROG) $(I960JX_PROG)

options:
	@echo Build Options
	@echo ------------------
	@echo CFLAGS = ${CFLAGS}
	@echo CXXFLAGS = ${CXXFLAGS}
	@echo LDFLAGS = ${LDFLAGS}
	@echo ------------------

$(I960MC_PROG): $(I960MC_OBJS)
	@echo LD ${I960MC_PROG}
	@${LD} ${LDFLAGS} -o ${I960MC_PROG} ${I960MC_OBJS}

$(I960JX_PROG): $(I960JX_OBJS)
	@echo LD ${I960JX_PROG}
	@${LD} ${LDFLAGS} -o ${I960JX_PROG} ${I960JX_OBJS}

.cc.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -f ${OBJS} ${PROGS}

numericops.o: operations.h types.h archlevel.h conditional_kinds.def numericops.cc
sim.o: operations.h types.h opcodes.h archlevel.h conditional_kinds.def sim.cc
misc.o: archlevel.h types.h conditional_kinds.def misc.cc
core.o: archlevel.h types.h memiface.h core.h operations.h opcodes.h conditional_kinds.def core.cc
dispatcher.o: archlevel.h types.h core.h opcodes.h conditional_kinds.def dispatcher.cc numerics_dispatch.def
numericsisa.o: archlevel.h types.h core.h memiface.h opcodes.h conditional_kinds.def numericsisa.cc


.PHONY: options
