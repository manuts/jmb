#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Channel Measurement
# Generated: Mon Apr 27 11:01:20 2015
##################################################

# Call XInitThreads as the _very_ first thing.
# After some Qt import, it's too late

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import time

prompt_to_quit = False

class channel_measurement(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Channel Measurement")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 1e6
        self.cent_freq = cent_freq = 2600e6

        ##################################################
        # Blocks
        ##################################################
        self.usrp_tx = uhd.usrp_sink(
        	",".join(("addr=134.147.118.216", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(2),
        	),
        )
        self.usrp_tx.set_subdev_spec("A:0 B:0", 0)
        self.usrp_tx.set_samp_rate(samp_rate)
        self.usrp_tx.set_center_freq(cent_freq, 0)
        self.usrp_tx.set_gain(15, 0)
        self.usrp_tx.set_antenna("TX/RX", 0)
        self.usrp_tx.set_center_freq(cent_freq, 1)
        self.usrp_tx.set_gain(15, 1)
        self.usrp_tx.set_antenna("TX/RX", 1)
        self.usrp_rx = uhd.usrp_source(
        	",".join(("addr=134.147.118.217", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(2),
        	),
        )
        self.usrp_rx.set_subdev_spec("A:0 B:0", 0)
        self.usrp_rx.set_samp_rate(samp_rate)
        self.usrp_rx.set_center_freq(cent_freq, 0)
        self.usrp_rx.set_gain(15, 0)
        self.usrp_rx.set_antenna("TX/RX", 0)
        self.usrp_rx.set_center_freq(cent_freq, 1)
        self.usrp_rx.set_gain(15, 1)
        self.usrp_rx.set_antenna("TX/RX", 1)
        self.blocks_null_source_1 = blocks.null_source(gr.sizeof_gr_complex*1)
        self.blocks_null_source_0 = blocks.null_source(gr.sizeof_gr_complex*1)
        self.blocks_null_sink_1 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_gr_complex*1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_null_source_0, 0), (self.usrp_tx, 0))    
        self.connect((self.blocks_null_source_1, 0), (self.usrp_tx, 1))    
        self.connect((self.usrp_rx, 0), (self.blocks_null_sink_0, 0))    
        self.connect((self.usrp_rx, 1), (self.blocks_null_sink_1, 0))    


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.usrp_tx.set_samp_rate(self.samp_rate)
        self.usrp_rx.set_samp_rate(self.samp_rate)

    def get_cent_freq(self):
        return self.cent_freq

    def set_cent_freq(self, cent_freq):
        self.cent_freq = cent_freq
        self.usrp_tx.set_center_freq(self.cent_freq, 0)
        self.usrp_tx.set_center_freq(self.cent_freq, 1)
        self.usrp_rx.set_center_freq(self.cent_freq, 0)
        self.usrp_rx.set_center_freq(self.cent_freq, 1)

    def get_tx_usrp_info(self):
        print "TX Channel 0"
        Keys = self.usrp_tx.get_usrp_info(0).keys()
        Vals = self.usrp_tx.get_usrp_info(0).vals()
        NumKeys = len(Keys)
        for i in range(NumKeys):
            print Keys[i], Vals[i]
        print "TX Channel 1"
        Keys = self.usrp_tx.get_usrp_info(1).keys()
        Vals = self.usrp_tx.get_usrp_info(1).vals()
        NumKeys = len(Keys)
        for i in range(NumKeys):
            print Keys[i], Vals[i]

    def get_rx_usrp_info(self):
        print "RX Channel 0"
        Keys = self.usrp_rx.get_usrp_info(0).keys()
        Vals = self.usrp_rx.get_usrp_info(0).vals()
        NumKeys = len(Keys)
        for i in range(NumKeys):
            print Keys[i], Vals[i]
        print ((self.usrp_rx).get_device()).get_subdev_name(0)
        print "RX Channel 1"
        Keys = self.usrp_rx.get_usrp_info(1).keys()
        Vals = self.usrp_rx.get_usrp_info(1).vals()
        NumKeys = len(Keys)
        for i in range(NumKeys):
            print Keys[i], Vals[i]

if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = channel_measurement()
    tb.get_tx_usrp_info()
    tb.get_rx_usrp_info()
    tb.start()
    if(prompt_to_quit == True):
        try:
            raw_input('Press Enter to quit: ')
        except EOFError:
            pass
        tb.stop()
    tb.wait()
