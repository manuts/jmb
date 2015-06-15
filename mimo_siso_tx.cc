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
#include <string>
#include <fftw3.h>

#define FFT_LEN     64
#define CP_LEN      64

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();
  opt_data opts;
  opt_handler((void *)&opts, argc, argv);

  unsigned int M = FFT_LEN;
  unsigned int cp_len = CP_LEN;
  unsigned int num_channels;    // number of tx channels
  unsigned int num_samples_sent;// number of samples sent in send()
  std::vector<size_t> channels; // vector of channel indices
  std::vector<std::complex<float> *> transmit_buffer;
  std::vector<std::complex<float> *> file_read_buffer;
  std::vector<std::complex<float> *> x;
  std::vector<std::complex<float> *> X;
  std::vector<FILE *>                fp;
  std::vector<fftwf_plan>            ifft;
  std::complex<float> dsp_gain(opts.dsp_gain, 0.0);

  uhd::usrp::multi_usrp::sptr u;
  init_usrp(&u, "134.147.118.217", &opts, true);
  num_channels = u->get_tx_num_channels();
  for(size_t chan = 0; chan < num_channels; chan++) {
    channels.push_back(chan);
    transmit_buffer.push_back((std::complex<float> *) malloc 
      (sizeof(std::complex<float>)*(M + cp_len)));
    file_read_buffer.push_back((std::complex<float> *) malloc 
      (sizeof(std::complex<float>)*M));
    X.push_back((std::complex<float> *) fftwf_malloc 
      (sizeof(std::complex<float>)*M));
    x.push_back((std::complex<float> *) fftwf_malloc 
      (sizeof(std::complex<float>)*M));
    memset(transmit_buffer[chan], 0, 
           sizeof(std::complex<float>)*(M + cp_len));
    fp.push_back(fopen(boost::str(
        boost::format("dat-files/tx1-ofdm-freq-%d.dat") % (chan + 1)).c_str(),
        "rb"));
//    if (!fp[chan]) {
//      std::cout << "Error reading "
//                << boost::format("dat-files/tx1-ofdm-freq-%d.dat") % (chan + 1)
//                << ". Exiting.\n";
//      exit(1);
//    }
    ifft.push_back(fftwf_plan_dft_1d(M,
                                     reinterpret_cast<fftwf_complex *> (X[chan]),
                                     reinterpret_cast<fftwf_complex *> (x[chan]),
                                     FFTW_BACKWARD,
                                     FFTW_ESTIMATE));
  }

  uhd::stream_args_t tx_stream_args(cpu, wire);
  tx_stream_args.channels = channels;
  uhd::tx_streamer::sptr tx_stream = 
    u->get_tx_stream(tx_stream_args);

  uhd::tx_metadata_t txmd;
  u->set_time_now(uhd::time_spec_t(0.0));
  txmd.start_of_burst = true;
  txmd.end_of_burst = false;
  txmd.has_time_spec = true;

  uhd::time_spec_t event(1.0);
  txmd.time_spec = event;

  unsigned int pid = 0;
  // tranmsit null samples
  num_samples_sent = tx_stream->send(transmit_buffer,
                                     M + cp_len,
                                     txmd);
  txmd.start_of_burst = false;
  txmd.has_time_spec = false;
  while(1) {
    std::cout << "Transmitting Packet "
              << ++pid << std::endl;
    if(!(fread(file_read_buffer[0],
               sizeof(std::complex<float>),
               M,
               fp[0])))
      break;
    // apply dsp gain
    volk_32fc_s32fc_multiply_32fc(x[0],
                                  file_read_buffer[0],
                                  dsp_gain,
                                  M);
    // do ifft
    fftwf_execute(ifft[0]);
    // prefix cyclic prefix
    memcpy(transmit_buffer[0],
           x[0] + M - cp_len,
           cp_len*sizeof(std::complex<float>));
    // copy signal to the transmit buffer
    memcpy(transmit_buffer[0] + cp_len,
           x[0],
           M*sizeof(std::complex<float>));
    // transmit it with USRP
    num_samples_sent = tx_stream->send(transmit_buffer,
                                       M + cp_len,
                                       txmd);
  }
  // end of burst
  txmd.end_of_burst = true;
  num_samples_sent = tx_stream->send(transmit_buffer,
                                     0,
                                     txmd);

  for(size_t chan = 0; chan < num_channels; chan++) {
    free(transmit_buffer[chan]);
    fftwf_free(X[chan]);
    fftwf_free(x[chan]);
    fftwf_destroy_plan(ifft[chan]);
    if(fp[chan])
      fclose(fp[chan]);
  }
  return 0;
}
