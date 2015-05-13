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
#include <fstream>
#include <complex>
#include <liquid/liquid.h>
#include <ctime>

#include <uhd/utils/thread_priority.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/utils/safe_main.hpp>
#include <volk/volk.h>

#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

typedef struct
{
  uhd::usrp::multi_usrp::sptr * tx;
} tx_thread_data;

void * tx_worker (void * _data)
{
  tx_thread_data * data = (tx_thread_data *)_data;
  return NULL;
}

typedef struct
{
  uhd::usrp::multi_usrp::sptr * rx;
} rx_thread_data;

void * rx_worker (void * _data)
{
  rx_thread_data * data = (rx_thread_data *)_data;
  return NULL;
}

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();

  //operating parameters
  double cent_freq;         // center frequency of transmission
  double samp_rate;         // usrp samping rate
  double txgain;            // tx frontend gain
  double rxgain;            // rx frontend gain
  unsigned int M;           // number of subcarriers
  unsigned int cp_len;      // length of cyclic prefix
  unsigned int taper_len;   // taper prefix length
  bool verbose;

  //default parameter values
  double d_cent_freq      = 1850e6;
  double d_samp_rate      = 1000e3;
  double d_txgain         = 15.0;
  double d_rxgain         = 15.0;
  unsigned int d_M          = 48;
  unsigned int d_cp_len     = 6;
  unsigned int d_taper_len  = 4;

  //set the operating parameters
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h",
     "help message")
    ("freq,f",
     po::value<double>(&cent_freq)->default_value(d_cent_freq),
     "RF center frequency in Hz")
    ("rate,r",
     po::value<double>(&samp_rate)->default_value(d_samp_rate),
     "USRP Sampling rate")
    ("tx_gain",
     po::value<double>(&txgain)->default_value(d_txgain),
     "TX Front end gain")
    ("rx_gain",
     po::value<double>(&rxgain)->default_value(d_rxgain),
     "RX Front end gain")
    ("verbose,v",
     "Verbose")
    ("quite,q",
     "Quite")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  //print the help message
  if (vm.count("help")) {
    std::cout << boost::format("ofdmtxrx %s") % desc << std::endl;
    std::cout
      << std::endl
      << "Basic OFDM P2P Link\n"
      << std::endl;
      return 0;
  }

  //check sanity of options
  if (vm.count("verbose") && vm.count("quite")) {
    std::cout << "Conflicting Options" << std::endl;
    return 0;
  }
}
