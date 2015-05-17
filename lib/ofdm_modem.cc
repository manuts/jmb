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

#include "ofdm_modem.h"

ofdm_mod::ofdm_mod(unsigned int       _M,
                   unsigned int       _cp_len,
                   unsigned int       _taper_len,
                   unsigned char *    _p)
{
    // validate input
    if (_M < 8) {
        fprintf(stderr,"error: ofdm_mod::ofdm_mod(), number of subcarriers must be at least 8\n");
        throw 0;
    } else if (_cp_len < 1) {
        fprintf(stderr,"error: ofdm_mod::ofdm_mod(), cyclic prefix length must be at least 1\n");
        throw 0;
    } else if (_taper_len > _cp_len) {
        fprintf(stderr,"error: ofdm_mod::ofdm_mod(), taper length cannot exceed cyclic prefix length\n");
        throw 0;
    }

    // set internal properties
    M             = _M;
    cp_len        = _cp_len;
    taper_len     = _taper_len;
    p             = _p;
    ofdmflexframegenprops_init_default(&fgprops);
    fgprops.check           = LIQUID_CRC_32;
    fgprops.fec0            = LIQUID_FEC_NONE;
    fgprops.fec1            = LIQUID_FEC_HAMMING128;
    fgprops.mod_scheme      = LIQUID_MODEM_QPSK;
    fg = ofdmflexframegen_create(M, cp_len, taper_len, p, &fgprops);

    // allocate memory for frame generator output (single OFDM symbol)
    fgbuffer_len = M + cp_len;
    fgbuffer = (std::complex<float>*) malloc(fgbuffer_len * sizeof(std::complex<float>));
}

ofdm_mod::~ofdm_mod()
{
  free(fgbuffer);
}

unsigned int ofdm_mod::get_M(){
  return M;
}

unsigned int ofdm_mod::get_cp_len(){
  return cp_len;
}

unsigned int ofdm_mod::get_taper_len(){
  return taper_len;
}
