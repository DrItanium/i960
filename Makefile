include config.mk

OBJS := coreops.o misc.o sim.o numericops.o core.o
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

coreops.o: operations.h types.h archlevel.h coreops.cc
numericops.o: operations.h types.h archlevel.h numericops.cc
sim.o: operations.h types.h opcodes.h archlevel.h sim.cc
misc.o: archlevel.h types.h misc.cc
core.o: archlevel.h types.h core.cc

.PHONY: options
