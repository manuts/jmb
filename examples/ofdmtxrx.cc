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

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();

  opt_data opts;
  opt_handler((void *)&opts, argc, argv);

  uhd::device_addr_t tx_addr, rx_addr;
  uhd::usrp::multi_usrp::sptr tx;
  uhd::usrp::multi_usrp::sptr rx;
  tx_addr["addr0"] = "134.147.118.216";
  rx_addr["addr0"] = "134.147.118.217";
  tx = uhd::usrp::multi_usrp::make(tx_addr);
  rx = uhd::usrp::multi_usrp::make(rx_addr);
  uhd::usrp::subdev_spec_t tx_subdev_spec("A:0 B:0");
  uhd::usrp::subdev_spec_t rx_subdev_spec("A:0 B:0");
  tx->set_tx_subdev_spec(tx_subdev_spec, uhd::usrp::multi_usrp::ALL_MBOARDS);
  rx->set_rx_subdev_spec(rx_subdev_spec, uhd::usrp::multi_usrp::ALL_MBOARDS);

  return 0;
}
