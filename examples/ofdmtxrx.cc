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

#include <iostream>

#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <liquid/liquid.h>

#include "tx_thread.h"
#include "rx_thread.h"
#include "option_handler.h"
#include "ofdm_modem.h"

void init_usrp(uhd::usrp::multi_usrp::sptr * tx,
               uhd::usrp::multi_usrp::sptr * rx,
               opt_data * opts);

int callback(unsigned char *  _header,
             int              _header_valid,
             unsigned char *  _payload,
             unsigned int     _payload_len,
             int              _payload_valid,
             framesyncstats_s _stats,
             void *           _userdata);

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();

  opt_data opts;
  opt_handler((void *)&opts, argc, argv);

  uhd::usrp::multi_usrp::sptr tx;
  uhd::usrp::multi_usrp::sptr rx;
  init_usrp(&tx, &rx, &opts);

  std::cout << rx->get_pp_string();

  unsigned char * p = NULL;     // for default carrier allocaiton
  void * user_data = NULL;      // don't know what this if for :(
  unsigned char header[8];
  unsigned int payload_len = 1024;
  unsigned char * payload = (unsigned char *)malloc(payload_len);
  std::complex<float> * tx_buff;
  unsigned int pid = 0;

  header[0] = (pid >> 8) & 0xff;
  header[1] = (pid     ) & 0xff;
  for (int i=2; i<8; i++)
    header[i] = rand() & 0xff;

  for(unsigned int i = 0; i < payload_len; i++)
  {
    payload[i] = rand()%256;
  }

  ofdm_mod mod(opts.M,
               opts.cp_len,
               opts.taper_len,
               p);
  mod.set_payload_len(payload_len);

  ofdm_dem dem(opts.M,
               opts.cp_len,
               opts.taper_len,
               p,
               callback,
               user_data);

  mod.assemble_symbol(header,
                      payload,
                      tx_buff);
  std::cout << "Framelength in OFDM Symbols :"
            << mod.get_framelen()
            << std::endl;
  mod.print_framegen_config();

  free(payload);
  return 0;
}
