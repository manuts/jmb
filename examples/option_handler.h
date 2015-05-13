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

#ifndef OPTION_HANDLER_H
#define OPTION_HANDLER_H

typedef struct
{
  double cent_freq;         // center frequency of transmission
  double samp_rate;         // usrp samping rate
  double txgain;            // tx frontend gain
  double rxgain;            // rx frontend gain
  unsigned int M;           // number of subcarriers
  unsigned int cp_len;      // length of cyclic prefix
  unsigned int taper_len;   // taper prefix length
  bool verbose;
} opt_data;

void opt_handler(void * opts, int argc, char** argv);
#endif
