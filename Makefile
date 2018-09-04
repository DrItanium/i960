include config.mk

OBJS := i960ka.o
PROG := sim960ka

all: $(OBJS)
	${LD} -o ${PROG} $(OBJS)

.c.o :
	@echo CC $<
	@${CC} ${CFLAGS} -c $< -o $@

.cc.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -f ${OBJS} ${PROG}

i960ka.o: opcodes.h i960ka.cc
