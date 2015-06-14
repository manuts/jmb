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

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();
  opt_data opts;
  opt_handler((void *)&opts, argc, argv);

  // read samples from file
  FILE * fp;
  fp = fopen("dat-files/tx1-ofdm-freq-1.dat", "rb");
  if (!fp) {
    std::cout << "File read error. Exiting.\n";
    return 1;
  }

  uhd::usrp::multi_usrp::sptr u;
  init_usrp(&u, "134.147.118.212", &opts)

  return 0;
}
