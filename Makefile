include config.mk

OBJS := i960mc.o

all: $(OBJS)
	${LD} -o sim960 $(OBJS)

.c.o :
	@echo CC $<
	@${CC} ${CFLAGS} -c $< -o $@

.cc.o :
	@echo CXX $<
	@${CXX} ${CXXFLAGS} -c $< -o $@

clean: 
	@echo Cleaning...
	@rm -r i960mc.o sim960
