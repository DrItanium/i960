include config.mk

OBJSKA := i960ka.o coreops.o
PROGKA := sim960ka
OBJSKB := i960kb.o coreops.o numericops.o
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

coreops.o: operations.h types.h coreops.cc
numericops.o: operations.h types.h numericops.cc
i960ka.o: operations.h types.h opcodes.h i960ka.cc
i960kb.o: operations.h types.h opcodes.h i960kb.cc

.PHONY: options
