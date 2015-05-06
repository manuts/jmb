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

typedef struct
{
  uhd::usrp::multi_usrp::sptr
} tx_thread_data;

void * tx_worker (void * _data)
{
  tx_thread_data * data = (tx_thread_data *)_data;
}

typedef struct
{
  uhd::usrp::multi_usrp::sptr
} rx_thread_data;

void * rx_worker (void * _data)
{
  rx_thread_data * data = (rx_thread_data *)_data;
}

int UHD_SAFE_MAIN(int argc, char **argv)
{
  uhd::set_thread_priority_safe();
}
