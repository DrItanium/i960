include config.mk

OBJSKA := i960ka.o
PROGKA := sim960ka
OBJSKB := i960kb.o
PROGKB := sim960kb

OBJS := $(OBJSKA) $(OBJSKB)
PROGS := $(PROGKA) $(PROGKB)

all: $(PROGKA) $(PROGKB)

$(PROGKA): $(OBJSKA)
	${LD} -o ${PROGKA} $(OBJSKA)

$(PROGKB): $(OBJSKB)
	${LD} -o ${PROGKB} $(OBJSKB)

.c.o :
	@echo CC $<
	@${CC} ${CFLAGS} -c $< -o $@

.cc.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -f ${OBJS} ${PROGS}

i960ka.o: ops.h types.h opcodes.h i960ka.cc
i960kb.o: ops.h types.h opcodes.h i960kb.cc
