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

#include "option_handler.h"
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <iostream>

namespace po = boost::program_options;

void opt_handler(void * _opts, int argc, char** argv)
{
  opt_data * opts = (opt_data *)_opts;

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
     po::value<double>(&(opts->cent_freq))->default_value(d_cent_freq),
     "RF center frequency in Hz")
    ("rate,r",
     po::value<double>(&(opts->samp_rate))->default_value(d_samp_rate),
     "USRP Sampling rate")
    ("tx_gain",
     po::value<double>(&(opts->txgain))->default_value(d_txgain),
     "TX Front end gain")
    ("rx_gain",
     po::value<double>(&(opts->rxgain))->default_value(d_rxgain),
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
    exit(0);
  }

  //check sanity of options
  if (vm.count("verbose") && vm.count("quite")) {
    std::cout << "Conflicting Options Verbose and Quite."
      << " Please use only one of those."
      << std::endl;
    exit(0);
  }
}
