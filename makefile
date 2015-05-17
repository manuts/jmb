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
LDFLAGS			+= 	-lboost_program_options-mt -lpthread -lvolk
env				 	= 	LD_LIBRARY_PATH="/target/lib/"

app_src		:=										\
	examples/ofdmtxrx.cc					\

sup_src			:=									\
	examples/tx_thread.cc					\
	examples/rx_thread.cc					\
	examples/option_handler.cc		\
	examples/init_usrp.cc					\

inc_hdr			:=									\
	include/ofdm_modem.h					\

lib_src			:=									\
	lib/ofdm_modem.cc							\

app_obj			= $(patsubst %.cc, %.o, $(app_src))

app_exe			= $(patsubst %.cc, %.exe, $(app_src))

sup_obj			= $(patsubst %.cc, %.o, $(sup_src))

lib_obj			= $(patsubst %.cc, %.o, $(lib_src))

all					: $(app_exe)

$(app_exe)	: %.exe : %.o $(sup_obj) $(lib_obj)
	$(env) $(CXX) $^ -o $@ $(LDFLAGS)

$(app_obj)	: %.o :	%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

$(sup_obj)	: %.o :	%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

$(lib_obj)	: %.o :	%.cc
	$(CXX) $(CFLAGS) -c $< -o $@

clean			:
	$(RM) $(app_obj) $(sup_obj) $(lib_obj)
