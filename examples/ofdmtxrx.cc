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

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();

  opt_data opts;
  opt_handler((void *)&opts, argc, argv);

  uhd::usrp::multi_usrp::sptr tx;
  uhd::usrp::multi_usrp::sptr rx;
//  init_usrp(&tx, &rx, &opts);

  unsigned char * p = NULL;     // for default carrier allocaiton
  std::cout << opts.M << std::endl;
  ofdm_mod mod(opts.M,
               opts.cp_len,
               opts.taper_len,
               p);

  std::cout << mod.get_M() << std::endl;

  return 0;
}
