include config.mk

OBJS := misc.o sim.o numericops.o core.o
PROG := sim960


all: $(PROG)

options:
	@echo Build Options
	@echo ------------------
	@echo CFLAGS = ${CFLAGS}
	@echo CXXFLAGS = ${CXXFLAGS}
	@echo LDFLAGS = ${LDFLAGS}
	@echo ------------------

$(PROG): $(OBJS)
	@echo LD ${PROG}
	@${LD} ${LDFLAGS} -o ${PROG} ${OBJS}

.c.o :
	@echo CC $<
	@${CC} ${CFLAGS} -c $< -o $@

.cc.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -f ${OBJS} ${PROG}

numericops.o: operations.h types.h archlevel.h conditional_kinds.def numericops.cc
sim.o: operations.h types.h opcodes.h archlevel.h conditional_kinds.def sim.cc
misc.o: archlevel.h types.h conditional_kinds.def misc.cc
core.o: archlevel.h types.h operations.h opcodes.h conditional_kinds.def core.cc

.PHONY: options
