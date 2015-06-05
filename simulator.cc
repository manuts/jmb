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

void * simulator (void * _data)
{
  std::cout << "Running simulator\n";
  unsigned int num_frames = 10;
  simulator_data * data = (simulator_data *)_data;
  // assertions
  assert((data->mod)->get_M() 
      == (data->dem)->get_M());
  assert((data->mod)->get_frame_len() 
      == (data->dem)->get_frame_len());
  assert((data->mod)->get_payload_dec_len() 
      == (data->dem)->get_payload_dec_len());
  assert((data->mod)->get_payload_mod_len() 
      == (data->dem)->get_payload_mod_len());
  assert((data->mod)->get_payload_enc_len() 
      == (data->dem)->get_payload_enc_len());
  assert((data->mod)->get_cp_len() 
      == (data->dem)->get_cp_len());

  unsigned int frame_len;
  unsigned int M;
  unsigned int cp_len;
  unsigned int sig_buff_len;
  std::complex<float> * sig_buff;
  unsigned char payload[OFDMFRAME_P_LEN];
  unsigned char header[OFDMFRAME_H_USR];
  unsigned int pid;

  frame_len = (data->mod)->get_frame_len();
  M = (data->mod)->get_M();
  cp_len = (data->mod)->get_cp_len();
  sig_buff_len = (M + cp_len)*frame_len;
  sig_buff = (std::complex<float> *) malloc 
                 (sizeof(std::complex<float>)*sig_buff_len);

  unsigned int i;
  for(pid = 0; pid < num_frames; pid++)
  {
    printf("tx packet id: %6u\n", pid);
    header[0] = (pid >> 8) & 0xff;
    header[1] = (pid     ) & 0xff;
    for(i = 2; i < OFDMFRAME_H_USR; i++)
      header[i] = rand() & 0xff;

    for(i = 0; i < OFDMFRAME_P_LEN; i++)
      payload[i] = rand() & 0xff;

    (data->mod)->assemble_frame(header, payload);
    (data->mod)->assemble_output_samples(sig_buff);
    (data->dem)->demodulate_samples(sig_buff, sig_buff_len);
  }
  free(sig_buff);
  return NULL;
}
