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

#include "ofdmtxrx.h"

bool simulation = false;

unsigned int num_frames_detected;
unsigned int num_valid_headers_received;
unsigned int num_valid_bytes_received;
unsigned int num_valid_packets_received;

std::string cpu = "fc32";    // cpu format for the streamer
std::string wire = "sc16";   // wire formate for the streamer

int callback(unsigned char *  _header,
             int              _header_valid,
             unsigned char *  _payload,
             unsigned int     _payload_len,
             int              _payload_valid,
             framesyncstats_s _stats,
             void *           _userdata)
{
  // update global counters
  num_frames_detected++;

  if (_header_valid)
      num_valid_headers_received++;

  if (_payload_valid) {
      num_valid_packets_received++;
      num_valid_bytes_received += _payload_len;
  }
  return 0;
}

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();

  float runtime = 60.0;

  opt_data opts;
  opt_handler((void *)&opts, argc, argv);
  simulator_data * modem;
  tx_thread_data * tx_data;
  rx_thread_data * rx_data;
  uhd::usrp::multi_usrp::sptr tx;
  uhd::usrp::multi_usrp::sptr rx;
  pthread_t tx_thread, rx_thread;
  std::complex<float> tx_gain(0.25, 0.0);
  bool rx_streamer_error;
  uhd::rx_metadata_t rxmd_from_worker;
  float time_for_offline_decoding;

  unsigned char * p = NULL;
  void * user_data = (void *)&(opts.samp_rate);

  time_t tx_begin, tx_end, rx_begin, rx_end;
  unsigned int pid;
  bool online_decoding = false;

  OFDMFRAME_STRUCT * frame_struct;
  frame_struct = (OFDMFRAME_STRUCT *) malloc 
    (sizeof(OFDMFRAME_STRUCT));
  frame_struct->VERSN =  104;
  frame_struct->H_USR =  2;
  frame_struct->H_LEN =  frame_struct->H_USR + 6;
  frame_struct->H_EC1 =  LIQUID_FEC_GOLAY2412;
  frame_struct->H_EC2 =  LIQUID_FEC_NONE;
  frame_struct->H_CRC =  LIQUID_CRC_32;
  frame_struct->H_MOD =  LIQUID_MODEM_QPSK;
  frame_struct->P_LEN =  2048;
  frame_struct->P_EC1 =  LIQUID_FEC_NONE;
  frame_struct->P_EC2 =  LIQUID_FEC_NONE;
  frame_struct->P_CRC =  LIQUID_CRC_32;
  frame_struct->P_MOD =  LIQUID_MODEM_QPSK;

  liquid::ofdm::modulator mod(opts.M,
                              opts.cp_len,
                              opts.taper_len,
                              p,
                              frame_struct);
  mod.set_tx_gain(tx_gain);
  liquid::ofdm::demodulator dem(opts.M,
                                opts.cp_len,
                                opts.taper_len,
                                p,
                                callback,
                                user_data,
                                frame_struct);

  num_frames_detected         = 0;
  num_valid_headers_received  = 0;
  num_valid_bytes_received    = 0;
  num_valid_packets_received  = 0;

  if(simulation) {
    modem = (simulator_data *) malloc (sizeof(simulator_data));
    modem->mod = &mod;
    modem->dem = &dem;
    simulator((void *)modem);
    free(modem);
  } else {
    init_usrp(&tx, &rx, &opts);
    tx_data = (tx_thread_data *) malloc (sizeof(tx_thread_data));
    rx_data = (rx_thread_data *) malloc (sizeof(rx_thread_data));
    tx_data->tx = &tx;
    rx_data->rx = &rx;
    tx_data->mod = &mod;
    rx_data->dem = &dem;
    tx_data->tx_begin = &tx_begin;
    rx_data->rx_begin = &rx_begin;
    tx_data->tx_end = &tx_end;
    rx_data->rx_end = &rx_end;
    tx_data->pid = &pid;
    tx_data->runtime = runtime;
    rx_data->runtime = runtime;
    rx_data->streamer_error = &rx_streamer_error;
    rx_data->rxmd_to_main = &rxmd_from_worker;
    rx_data->online_decoding = online_decoding;
    rx_data->time_for_offline_decoding = &time_for_offline_decoding;
    if(pthread_create(&tx_thread, NULL, tx_worker, (void *)tx_data)){
      std::cout << "Error invoking tx thread\n";
      return 1;
    }
    if(pthread_create(&rx_thread, NULL, rx_worker, (void *)rx_data)){
      std::cout << "Error invoking rx thread\n";
      return 1;
    }
    pthread_join(tx_thread, NULL);
    pthread_join(rx_thread, NULL);
    free(tx_data);
    free(rx_data);
    free(frame_struct);
  }
  printf("    frames transmitted      : %6u\n", pid);
  printf("    frames detected         : %6u\n", num_frames_detected);
  printf("    valid headers           : %6u\n", num_valid_headers_received);
  printf("    valid packets           : %6u\n", num_valid_packets_received);
  printf("    bytes received          : %6u\n", num_valid_bytes_received);
  std::cout << "    tx runtime              : "
            << tx_end - tx_begin
            << std::endl;
  std::cout << "    rx runtime              : "
            << rx_end - rx_begin
            << std::endl;
  printf("    bit rate                : %e bps\n",
         float(num_valid_bytes_received*8)/(rx_end - rx_begin));
  if(!(online_decoding))
    printf("    decoding time           : %3.3f sec\n",
           time_for_offline_decoding);
  if(rx_streamer_error)
  {
    std::cout << "    rx thread exit with     : "
              << rxmd_from_worker.strerror() << "\n";
  }
  return 0;
}
