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

#ifndef OFDM_MODEM_H
#define OFDM_MODEM_H

#include <liquid/liquid.h>
#include <complex>
#include <stdlib.h>
#include <stdio.h>

// Read data to be transmitted. Modulate it and write to the 
// tx buffer
class ofdm_mod
{
  public:
    // default constructor
    // _M              :   OFDM: number of subcarriers
    // _cp_len         :   OFDM: cyclic prefix length
    // _taper_len      :   OFDM: taper prefix length
    ofdm_mod(unsigned int       _M,
             unsigned int       _cp_len,
             unsigned int       _taper_len,
             unsigned char *    _p);

    // destructor
    ~ofdm_mod();

    // transmitter methods
    void set_soft_gain(float gain_soft);
    void reset();
    unsigned int get_tx_buff_len();
    void set_mod(int _mod);
    unsigned int get_M();
    unsigned int get_cp_len();
    unsigned int get_taper_len();
    unsigned int get_payload_len();
    void set_payload_len(unsigned int _payload_len);
    void print_framegen_config();
    unsigned int get_framelen();
    

    // assemble OFDM symbol
    void assemble_symbol(unsigned char * _header,
                         unsigned char * _payload,
                         std::complex<float> * tx_buff);
                         // frame generator properties...

  private:
    unsigned int M;
    unsigned int cp_len;
    unsigned int taper_len;
    unsigned char * p;
    unsigned int payload_len;
    unsigned int tx_buff_len;
    int mod;
    int fec0;
    int fec1;
    int fgbuffer_len;
    std::complex<float> * fgbuffer;
    ofdmflexframegenprops_s fgprops;// frame generator properties
    ofdmflexframegen fg;            // frame generator object
};

// Read data from rx buffer. Demodulate it and store data.
class ofdm_dem
{
  public:
    // default constructor
    // _M               :   OFDM: number of subcarriers
    // _cp_len          :   OFDM: cyclic prefix length
    // _taper_len       :   OFDM: taper prefix length
    // _p               :   OFDM: subcarrier allocation
    ofdm_dem(unsigned int       _M,
             unsigned int       _cp_len,
             unsigned int       _taper_len,
             unsigned char *    _p,
             framesync_callback _callback,
             void *             _userdata);

    // destructor
    ~ofdm_dem();

    // receiver methods
    void reset();
    unsigned int get_M();

  private:
    ofdmflexframesync fs;           // frame synchronizer object
    unsigned int M;
    unsigned int cp_len;
    unsigned int taper_len;
    unsigned char * p;
    framesync_callback callback;
    void * userdata;
};

#endif
