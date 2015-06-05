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

#define SIMULATION  0

unsigned int num_frames_detected;
unsigned int num_valid_headers_received;
unsigned int num_valid_bytes_received;
unsigned int num_valid_packets_received;

float runtime = 60.0;
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
    // compute true carrier offset
//  double samplerate = *((double*)_userdata);
//  float cfo = _stats.cfo * samplerate / (2*M_PI);
//  printf("***** rssi=%7.2fdB evm=%7.2fdB, cfo=%7.3f kHz, ", _stats.rssi, _stats.evm, cfo*1e-3f);

  if (_header_valid) {
      unsigned int packet_id = (_header[0] << 8 | _header[1]);
      printf("rx packet id: %6u", packet_id);
      if (_payload_valid) printf("\n");
      else                printf(" PAYLOAD INVALID\n");
  } else {
      printf("HEADER INVALID\n");
  }

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

  opt_data opts;
  opt_handler((void *)&opts, argc, argv);
  simulator_data * modem;
  tx_thread_data * tx_data;
  rx_thread_data * rx_data;
  uhd::usrp::multi_usrp::sptr tx;
  uhd::usrp::multi_usrp::sptr rx;
  pthread_t tx_thread, rx_thread;

  unsigned char * p = NULL;
  void * user_data = (void *)&(opts.samp_rate);

  liquid::ofdm::modulator mod(opts.M,
                              opts.cp_len,
                              opts.taper_len,
                              p);
  liquid::ofdm::demodulator dem(opts.M,
                                opts.cp_len,
                                opts.taper_len,
                                p,
                                callback,
                                user_data);

  num_frames_detected         = 0;
  num_valid_headers_received  = 0;
  num_valid_bytes_received    = 0;
  num_valid_packets_received  = 0;

  #if SIMULATION
    modem = (simulator_data *) malloc (sizeof(simulator_data));
    modem->mod = &mod;
    modem->dem = &dem;
    simulator((void *)modem);
    free(modem);
  #else
    init_usrp(&tx, &rx, &opts);
    tx_data = (tx_thread_data *) malloc (sizeof(tx_thread_data));
    rx_data = (rx_thread_data *) malloc (sizeof(rx_thread_data));
    tx_data->tx = &tx;
    rx_data->rx = &rx;
    tx_data->mod = &mod;
    rx_data->dem = &dem;
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
  #endif
    printf("    frames detected     : %6u\n", num_frames_detected);
    printf("    valid headers       : %6u\n", num_valid_headers_received);
    printf("    valid packets       : %6u\n", num_valid_packets_received);
    printf("    bytes received      : %6u\n", num_valid_bytes_received);
  return 0;
  std::cout << "TX runtime : "
            << tx_end - tx_begin
            << std::endl;
  std::cout << "RX runtime : "
            << rx_end - rx_begin
            << std::endl;
}
