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

void * tx_worker (void * _data)
{
  // number of subcarriers
  unsigned int M;
  // cyclic prefix length
  unsigned int cp_len;
  // number of OFDM symbols per packet
  unsigned int frame_len;
  // number of samples per frame at
  // the output of modulator
  unsigned int modulator_buffer_len;
  // vector of pointers to the modulator buffers.
  // TODO modifications for multichannel modulators
  std::complex<float> ** modulator_buffer;
  // usrp buffer
  std::vector<std::complex<float> *> usrp_buffer;
  // payload len
  unsigned int payload_len;
  // header len
  unsigned int header_len;
  // payload data
  unsigned char * payload;
  // header data
  unsigned char * header;
  // number of channels available with USRP
  unsigned int num_channels;
  // channel vector
  std::vector<size_t> channels;
  // number of samples sent in each call of send
  unsigned int num_samples_sent;
  
  tx_thread_data * data = (tx_thread_data *)_data;

  M = (data->mod)->get_M();
  cp_len = (data->mod)->get_cp_len();
  frame_len = (data->mod)->get_frame_len();
  modulator_buffer_len = (M + cp_len)*frame_len;
  header_len = (data->mod)->get_h_usr_len();
  payload_len = (data->mod)->get_payload_dec_len();
  num_channels = (*(data->tx))->get_tx_num_channels();

  payload = (unsigned char *) malloc 
    (payload_len*sizeof(unsigned char));
  header = (unsigned char *) malloc 
    (header_len*sizeof(unsigned char));
  modulator_buffer = (std::complex<float> **) 
    malloc(num_channels*sizeof(std::complex<float> *));
  for(size_t chan = 0; chan < num_channels; chan++) {
    channels.push_back(chan);
    modulator_buffer[chan] = (std::complex<float> *) malloc 
      (sizeof(std::complex<float>)*modulator_buffer_len);
    usrp_buffer.push_back(modulator_buffer[chan]);
  }

  uhd::stream_args_t tx_stream_args(cpu, wire);
  tx_stream_args.channels = channels;
  uhd::tx_streamer::sptr tx_stream = 
    (*(data->tx))->get_tx_stream(tx_stream_args);

  uhd::tx_metadata_t txmd;
  (*(data->tx))->set_time_now(uhd::time_spec_t(0.0));
  txmd.start_of_burst = true;
  txmd.end_of_burst = false;
  txmd.has_time_spec = true;

  uhd::time_spec_t event(0.1);
  txmd.time_spec = event;
  *(data->tx_begin) = time(NULL);

  unsigned int i;
  *(data->pid) = 0;
  while(time(NULL) - *(data->tx_begin) < data->runtime)
  {
//    printf("tx packet id: %6u\n", *(data->pid));
    header[0] = (*(data->pid) >> 8) & 0xff;
    header[1] = (*(data->pid)     ) & 0xff;
    for(i = 2; i < header_len; i++)
      header[i] = rand() & 0xff;

    for(i = 0; i < payload_len; i++)
      payload[i] = rand() & 0xff;

    (data->mod)->assemble_frame(header, payload);
    (data->mod)->assemble_output_samples(modulator_buffer[0]);
    num_samples_sent = tx_stream->send(usrp_buffer,
                                       modulator_buffer_len,
                                       txmd);
    txmd.start_of_burst = false;
    txmd.has_time_spec = false;
    (*(data->pid))++;
  }

  // transmission ends
  *(data->tx_end) = time(NULL);
  std::cout << "Exiting tx thread\n";
  // send the last packet
  txmd.end_of_burst = true;
  tx_stream->send(usrp_buffer,
                  0,
                  txmd);

  for(size_t chan = 0; chan < num_channels; chan++) {
    free(modulator_buffer[chan]);
  }
  free(modulator_buffer);

  pthread_exit(NULL);
}
