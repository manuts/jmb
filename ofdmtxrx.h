/*
 * Copyright (c) 2014, 2015 Manu T S
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OFDMTXRX_H
#define OFDMTXRX_H

#include <liquid/liquid.h>
#include <uhd/usrp/multi_usrp.hpp>
#include <liquid/ofdm.h>
#include <iostream>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <ctime>

extern std::string cpu;    // cpu format for the streamer
extern std::string wire;   // wire formate for the streamer

typedef struct
{
  double cent_freq;         // center frequency of transmission
  double samp_rate;         // usrp samping rate
  float dsp_gain;           // dsp gain
  double txgain;            // tx frontend gain
  double rxgain;            // rx frontend gain
  unsigned int M;           // number of subcarriers
  unsigned int cp_len;      // length of cyclic prefix
  unsigned int taper_len;   // taper prefix length
  bool verbose;
  unsigned int header_len;
  unsigned int payload_len;
} opt_data;

typedef struct
{
  liquid::ofdm::modulator * mod;
  liquid::ofdm::demodulator * dem;
  void * usr_data;
} simulator_data;

typedef struct
{
  uhd::usrp::multi_usrp::sptr * tx;
  liquid::ofdm::modulator * mod;
  time_t * tx_begin;
  time_t * tx_end;
  unsigned int * pid;
  float runtime;
} tx_thread_data;

typedef struct
{
  uhd::usrp::multi_usrp::sptr * rx;
  liquid::ofdm::demodulator * dem;
  time_t * rx_begin;
  time_t * rx_end;
  float runtime;
  bool * streamer_error;
  uhd::rx_metadata_t * rxmd_to_main;
  bool online_decoding;
  float * time_for_offline_decoding;
} rx_thread_data;

void opt_handler(void * _opts, int argc, char** argv);
void * tx_worker (void * _data);
void * rx_worker (void * _data);
void * simulator (void * _data);
void init_usrp(uhd::usrp::multi_usrp::sptr * tx,
               uhd::usrp::multi_usrp::sptr * rx,
               std::string _tx_addr,
               std::string _rx_addr,
               opt_data * opts);
void init_usrp(uhd::usrp::multi_usrp::sptr * u,
               std::string addr,
               opt_data * opts,
               bool is_tx);
#endif
