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

#define TX_IS_N200 1
#define TX_IS_X300 0

void init_usrp(uhd::usrp::multi_usrp::sptr * tx,
               uhd::usrp::multi_usrp::sptr * rx,
               opt_data * opts) 
{
  size_t num_tx_chans, num_rx_chans;
  uhd::device_addr_t tx_addr, rx_addr;
  tx_addr["addr0"] = "134.147.118.212";
  rx_addr["addr0"] = "134.147.118.217";
  *tx = uhd::usrp::multi_usrp::make(tx_addr);
  *rx = uhd::usrp::multi_usrp::make(rx_addr);
  uhd::usrp::subdev_spec_t tx_subdev_spec_N200("A:0");
  uhd::usrp::subdev_spec_t tx_subdev_spec_X300("A:0 B:0");
  uhd::usrp::subdev_spec_t rx_subdev_spec("A:0 B:0");
  if(TX_IS_N200)
    (*tx)->set_tx_subdev_spec(tx_subdev_spec_N200,
                              uhd::usrp::multi_usrp::ALL_MBOARDS);
  else if(TX_IS_X300)
    (*tx)->set_tx_subdev_spec(tx_subdev_spec_X300,
                              uhd::usrp::multi_usrp::ALL_MBOARDS);
  (*rx)->set_rx_subdev_spec(rx_subdev_spec,
                            uhd::usrp::multi_usrp::ALL_MBOARDS);

  num_tx_chans = (*tx)->get_tx_num_channels();
  for (size_t chan = 0; chan < num_tx_chans; chan++) {
    (*tx)->set_tx_rate(opts->samp_rate, chan);
    uhd::tune_request_t tx_tune_request(opts->cent_freq);
    uhd::tune_result_t tx_tune_result;
    tx_tune_result = (*tx)->set_tx_freq(tx_tune_request, chan);
    (*tx)->set_tx_gain(opts->txgain, chan);
    (*tx)->set_tx_antenna("TX/RX", chan);
  }

  num_rx_chans = (*rx)->get_rx_num_channels();
  for (size_t chan = 0; chan < num_rx_chans; chan++) {
    (*rx)->set_rx_rate(opts->samp_rate, chan);
    uhd::tune_request_t rx_tune_request(opts->cent_freq);
    uhd::tune_result_t rx_tune_result;
    rx_tune_result = (*rx)->set_rx_freq(rx_tune_request, chan);
    (*rx)->set_rx_gain(opts->rxgain, chan);
    (*rx)->set_rx_antenna("TX/RX", chan);
  }
}
