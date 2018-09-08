include config.mk

COMMON_OBJS := coreops.o misc.o
OBJSKA := ${COMMON_OBJS} i960ka.o
PROGKA := sim960ka
OBJSKB := ${COMMON_OBJS} numericops.o i960kb.o
PROGKB := sim960kb

OBJS := $(OBJSKA) $(OBJSKB)
PROGS := $(PROGKA) $(PROGKB)

all: $(PROGKA) $(PROGKB)

options:
	@echo Build Options
	@echo ------------------
	@echo CFLAGS = ${CFLAGS}
	@echo CXXFLAGS = ${CXXFLAGS}
	@echo LDFLAGS = ${LDFLAGS}
	@echo ------------------

$(PROGKA): $(OBJSKA)
	@${LD} ${LDFLAGS} -o ${PROGKA} $(OBJSKA)
	@echo LD $<

$(PROGKB): $(OBJSKB)
	@${LD} ${LDFLAGS} -o ${PROGKB} $(OBJSKB)
	@echo LD $<

.c.o :
	@echo CC $<
	@${CC} ${CFLAGS} -c $< -o $@

.cc.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -f ${OBJS} ${PROGS}

coreops.o: operations.h types.h archlevel.h coreops.cc
numericops.o: operations.h types.h archlevel.h numericops.cc
i960ka.o: operations.h types.h opcodes.h archlevel.h i960ka.cc
i960kb.o: operations.h types.h opcodes.h archlevel.h i960kb.cc
misc.o: archlevel.h types.h misc.cc

.PHONY: options
