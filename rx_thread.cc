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

void * rx_worker (void * _data)
{
  // vector of pointers to the modulator buffers.
  // TODO modifications for multichannel modulators
  std::complex<float> ** demodulator_buffer;
  // usrp buffer
  std::vector<std::complex<float> *> usrp_buffer;
  // usrp buffer len
  size_t usrp_buffer_len;
  // number of channels available with USRP
  unsigned int num_channels;
  // channel vector
  std::vector<size_t> channels;
  // number of samples received in each call of recv
  unsigned int num_samples_rcvd;
  // timeout
  float timeout = 0.2;
  // file to save data;
  FILE * f_rx_sig;
  // total number of samples received
  unsigned int num_accumulated_samples;

  rx_thread_data * data = (rx_thread_data *)_data;

  num_channels = (*(data->rx))->get_tx_num_channels();

  if(!(data->online_decoding))
    f_rx_sig = fopen("/tmp/rx_sig", "wb");

  for(size_t chan = 0; chan < num_channels; chan++)
    channels.push_back(chan);
  uhd::stream_args_t rx_stream_args(cpu, wire);
  rx_stream_args.channels = channels;
  uhd::rx_streamer::sptr rx_stream = 
    (*(data->rx))->get_rx_stream(rx_stream_args);

  usrp_buffer_len = rx_stream->get_max_num_samps();
  demodulator_buffer = (std::complex<float> **) 
    malloc(num_channels*sizeof(std::complex<float> *));
  for(size_t chan = 0; chan < num_channels; chan++) {
    usrp_buffer.push_back((std::complex<float> *) malloc 
      (sizeof(std::complex<float>)*usrp_buffer_len));
    demodulator_buffer[chan] = usrp_buffer[chan];
  }

  uhd::rx_metadata_t rxmd;
  (*(data->rx))->set_time_now(uhd::time_spec_t(0.0), 0);

  uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
  stream_cmd.stream_now = false;
  stream_cmd.time_spec = uhd::time_spec_t(0.1);
  rx_stream->issue_stream_cmd(stream_cmd);
  *(data->streamer_error) = false;
  *(data->rx_begin) = time(NULL);
  num_accumulated_samples = 0;

  while(time(NULL) - *(data->rx_begin) < data->runtime)
  {
    num_samples_rcvd = rx_stream->recv(usrp_buffer,
                                       usrp_buffer_len,
                                       rxmd,
                                       timeout);
    if(rxmd.error_code) {
      *(data->streamer_error) = true;
      memmove(data->rxmd_to_main, &rxmd,
              sizeof(uhd::rx_metadata_t));
      break;
    }
    timeout = 0.1;
    num_accumulated_samples += num_samples_rcvd;
    if(data->online_decoding) {
      (data->dem)->demodulate_samples(demodulator_buffer[0],
                                      num_samples_rcvd);
    }
    else {
      assert(fwrite((void *)demodulator_buffer[0],
                    sizeof(std::complex<float>),
                    num_samples_rcvd,
                    f_rx_sig) ==
             num_samples_rcvd);
    }
  }

  *(data->rx_end) = time(NULL);
  std::cout << "Exiting rx thread\n";
  // stop rx streaming
  stream_cmd.stream_mode = 
    uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
  rx_stream->issue_stream_cmd(stream_cmd);

  if(!(data->online_decoding)) {
    fclose(f_rx_sig);
    unsigned int samples_remaining = num_accumulated_samples;
    unsigned int num_samples_to_process;
    f_rx_sig = fopen("/tmp/rx_sig", "rb");
    time_t decode_begin = time(NULL);
    while(samples_remaining)
    {
      num_samples_to_process = 
        (samples_remaining > usrp_buffer_len) ?
        usrp_buffer_len :
        samples_remaining;
      assert(
          fread((void *)demodulator_buffer[0],
                sizeof(std::complex<float>),
                num_samples_to_process,
                f_rx_sig) == 
                num_samples_to_process);
      (data->dem)->demodulate_samples(demodulator_buffer[0],
                                      num_samples_to_process);
      samples_remaining -= num_samples_to_process;
    }
    *(data->time_for_offline_decoding) = 
      time(NULL) - decode_begin;
  }

  for(size_t chan = 0; chan < num_channels; chan++) {
    free(usrp_buffer[chan]);
  }
  free(demodulator_buffer);
  pthread_exit(NULL);
}
