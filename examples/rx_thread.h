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

#ifndef RX_THREAD_H
#define RX_THREAD_H

#include <liquid/liquid.h>
#include <uhd/usrp/multi_usrp.hpp>

typedef struct
{
  uhd::usrp::multi_usrp::sptr * rx;
} rx_thread_data;

void * rx_worker (void * _data);
#endif
