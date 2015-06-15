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

std::string cpu = "fc32";    // cpu format for the streamer
std::string wire = "sc16";   // wire formate for the streamer

void init_usrp(uhd::usrp::multi_usrp::sptr * tx,
               uhd::usrp::multi_usrp::sptr * rx,
               std::string _tx_addr,
               std::string _rx_addr,
               opt_data * opts) 
{
  size_t num_tx_chans, num_rx_chans;
  uhd::device_addr_t tx_addr, rx_addr;
  tx_addr["addr0"] = _tx_addr;
  rx_addr["addr0"] = _rx_addr;
  *tx = uhd::usrp::multi_usrp::make(tx_addr);
  *rx = uhd::usrp::multi_usrp::make(rx_addr);
  uhd::usrp::subdev_spec_t subdev_spec_N200("A:0");
  uhd::usrp::subdev_spec_t subdev_spec_X300("A:0 B:0");

  if((*tx)->get_mboard_name() == "X300") {
    (*tx)->set_tx_subdev_spec(subdev_spec_X300,
                              uhd::usrp::multi_usrp::ALL_MBOARDS);
  }
  else if((*tx)->get_mboard_name() == "N200") {
    (*tx)->set_tx_subdev_spec(subdev_spec_N200,
                              uhd::usrp::multi_usrp::ALL_MBOARDS);
  }
  else {
    std::cout << "TX Motherboard not compatible\n"
              << "Subdevice specification for "
              << (*tx)->get_mboard_name()
              << " not known. Exiting\n";
    exit(1);
  }
  if((*rx)->get_mboard_name() == "X300")
    (*rx)->set_rx_subdev_spec(subdev_spec_X300,
                              uhd::usrp::multi_usrp::ALL_MBOARDS);
  else if((*rx)->get_mboard_name() == "N200")
    (*rx)->set_rx_subdev_spec(subdev_spec_N200,
                              uhd::usrp::multi_usrp::ALL_MBOARDS);
  else {
    std::cout << "RX Motherboard not compatible\n"
              << "Subdevice specification for "
              << (*tx)->get_mboard_name()
              << " not known. Exiting\n";
    exit(1);
  }

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

void init_usrp(uhd::usrp::multi_usrp::sptr * u,
               std::string addr,
               opt_data * opts,
               bool is_tx) 
{
  size_t num_chans;
  uhd::device_addr_t u_addr;
  u_addr["addr0"] = addr;
  *u = uhd::usrp::multi_usrp::make(u_addr);
  uhd::usrp::subdev_spec_t subdev_spec_N200("A:0");
  uhd::usrp::subdev_spec_t subdev_spec_X300("A:0 B:0");

  if(is_tx) {
    if((*u)->get_mboard_name() == "X300") {
      (*u)->set_tx_subdev_spec(subdev_spec_X300,
                                uhd::usrp::multi_usrp::ALL_MBOARDS);
    }
    else if((*u)->get_mboard_name() == "N200") {
      (*u)->set_tx_subdev_spec(subdev_spec_N200,
                                uhd::usrp::multi_usrp::ALL_MBOARDS);
    }
    else {
      std::cout << "TX Motherboard not compatible\n"
                << "Subdevice specification for "
                << (*u)->get_mboard_name()
                << " not known. Exiting\n";
      exit(1);
    }
    num_chans = (*u)->get_tx_num_channels();
    for (size_t chan = 0; chan < num_chans; chan++) {
      (*u)->set_tx_rate(opts->samp_rate, chan);
      uhd::tune_request_t tx_tune_request(opts->cent_freq);
      uhd::tune_result_t tx_tune_result;
      tx_tune_result = (*u)->set_tx_freq(tx_tune_request, chan);
      (*u)->set_tx_gain(opts->txgain, chan);
      (*u)->set_tx_antenna("TX/RX", chan);
    }
  }
  else {
    if((*u)->get_mboard_name() == "X300") {
      (*u)->set_rx_subdev_spec(subdev_spec_X300,
                                uhd::usrp::multi_usrp::ALL_MBOARDS);
    }
    else if((*u)->get_mboard_name() == "N200") {
      (*u)->set_rx_subdev_spec(subdev_spec_N200,
                                uhd::usrp::multi_usrp::ALL_MBOARDS);
    }
    else {
      std::cout << "RX Motherboard not compatible\n"
                << "Subdevice specification for "
                << (*u)->get_mboard_name()
                << " not known. Exiting\n";
      exit(1);
    }
    num_chans = (*u)->get_rx_num_channels();
    for (size_t chan = 0; chan < num_chans; chan++) {
      (*u)->set_rx_rate(opts->samp_rate, chan);
      uhd::tune_request_t rx_tune_request(opts->cent_freq);
      uhd::tune_result_t rx_tune_result;
      rx_tune_result = (*u)->set_rx_freq(rx_tune_request, chan);
      (*u)->set_rx_gain(opts->rxgain, chan);
      (*u)->set_rx_antenna("TX/RX", chan);
    }
  }
}
