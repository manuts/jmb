# paths
srcdir			= .

# programs
CC				:= gcc
CXX				:= g++
MV				:= mv -f
RM				:= rm -f

# flags
INCLUDE_CFLAGS	= -I./ -I/target/include -I./include
CFLAGS			= 	$(INCLUDE_CFLAGS) -g -O2 -Wall -fPIC
LDFLAGS			= 	-L/target/lib
LDFLAGS			+= 	-luhd -lliquid -lm -lc -lboost_system-mt -lboost_thread-mt -lliquidgr
LDFLAGS			+= 	-lboost_program_options-mt -lpthread -lvolk -lfftw3f
env				 	= 	LD_LIBRARY_PATH="/target/lib"

sup_src			:=									\
	tx_thread.cc									\
	rx_thread.cc									\
	option_handler.cc							\
	init_usrp.cc									\
	simulator.cc									\

sup_obj					= $(patsubst %.cc, %.o, $(sup_src))

all							: ofdmtxrx.exe mimo_siso_tx.exe

mimo_siso_tx.exe: $(sup_obj) mimo_siso_tx.o
	$(env) $(CXX) $^ -o $@ $(LDFLAGS)

ofdmtxrx.exe		: $(sup_obj) ofdmtxrx.o
	$(env) $(CXX) $^ -o $@ $(LDFLAGS)

mimo_siso_tx.o	: mimo_siso_tx.cc
	$(CXX) $(CFLAGS) -c $< -o $@

ofdmtxrx.o			: ofdmtxrx.cc
	$(CXX) $(CFLAGS) -c $< -o $@

$(sup_obj)			: %.o :	%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

clean						:
	$(RM) ofdmtxrx.exe ofdmtxrx.o
	$(RM) mimo_siso_tx.exe mimo_siso_tx.o
	$(RM) $(sup_obj)
