include config.mk

OBJS := i960mc.o
PROG := sim960

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

i960mc.o: opcodes.h i960mc.cc
