# paths
srcdir			= .

# programs
CC				:= gcc
CXX				:= g++
MV				:= mv -f
RM				:= rm -f

# flags
INCLUDE_CFLAGS	= -I./ -I/target/include
CFLAGS			= 	$(INCLUDE_CFLAGS) -g -O2 -Wall -fPIC
LDFLAGS			= 	-L/target/lib
LDFLAGS			+= 	-luhd -lliquid -lm -lc -lboost_system-mt -lboost_thread-mt -lliquidgr
LDFLAGS			+= 	-lboost_program_options-mt -lpthread -lvolk
env				 	= 	LD_LIBRARY_PATH="/target/lib/"

examples_src		:=							\
	examples/ofdmtxrx.cc					\

examples_obj		= $(patsubst %.cc, %.o, $(examples_src))

examples_exe		= $(patsubst %.cc, %.exe, $(examples_src))

all							: $(examples_exe)

$(examples_exe)	: %.exe : %.o
	$(env) $(CXX) $^ -o $@ $(LDFLAGS)

$(examples_obj)	: %.o :	%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

clean			:
	$(RM) $(examples_obj)
