/* my LOCAL i2cDetect.c, NOT as replacement for i2cdetect */

/*
BSD-3-Clause

Copyright (c) 2026 Bruce Stephens
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.

IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * i2cDetect [adapter]
 * int __i2cScanAdapter(int adapter, int extendedRange)...
 * is a local variation of the standard utility i2cdetect and
 * is the simple equivalent of i2cdetect -y adapter without options.
 * Also displays a read register (0x00 only) result if a slave is found.
 * Reads the device registers 0x08-0x7F compared to the standard i2cdetect range 0x08-0x77.
 * There is an implied adapter address option, but 1 (/dev/i2c-1) is the default.
 * The extend option (1) allows scanning to start from slaveAddress 0x00
 * rather than the normal 0x08.
 * Checks that the fp is open, returns on fail.
 * Closes everything at the end.
 * None of the awareness protections in i2cdetect exist in i2cDetect.
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h> // exit and strtol
#include <unistd.h> // usleep and close
#include <fcntl.h>  // open
#include <sys/ioctl.h> // ioctl
#include <string.h>
#include <errno.h> /* errno */
#include <ctype.h> /* for toLower */
#include <linux/i2c-dev.h> // __u8
#include <i2c/smbus.h>

#if __has_include(<time.h>)
#include <time.h> /* CLOCKS_PER_SEC and timespec */
#include <sys/time.h> /* gettimeofday */
#endif

#define GENERIC
#include "i2cDevicesHeader.h"
#include "i2cMaster.h"
#include "i2cConvenientBits.h"
#include "i2cAdapterHandlers.h"

   struct timespec ts; /* Used in i2cConvenientBits.h */

int main(int argc, char ** argv) {
   int number = 1;
   int extend = 0;
   printf("%s [adapter: default 1] [extend:0 or 1]\n", argv[0]);
   if (argc > 1) number = strtol(argv[1], NULL, 10);
   if (argc > 2) extend = strtol(argv[2], NULL, 10);
   if ((number < 0) || (number > 127)) exit(0); /* Junk filter. */
   if ((extend != 0) && (extend != 1)) exit(0);
   return __i2cScanAdapter(number, extend);
}
