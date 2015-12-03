CC=g++
CCFLAGS=-g3 -O0 -std=c++11 -Ihashes -DDEBUG
LFLAGS=-g3 -std=c++11 -O0

OBJS=xclm.o logging.o patcher.o hashes/sha0.o
PROG=xclm

# create dependencies see: https://stackoverflow.com/questions/2394609/makefile-header-dependencies
DEPS := $(OBJS:.o=.d)
-include $(DEPS)

.PHONY : clean

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LFLAGS) -o $(PROG) $(OBJS)

%.o: %.cc
	@# create dependencies
	$(CC) $(CCFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	@# compile
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(PROG)
	rm -f $(OBJS:.o=.d)
	rm -f xclm.log
