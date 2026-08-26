/* shmCRelease.c */

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
 * shmCRelease.c
 *
 * Amended April 2025
 *
 * For testing Shared Memory.
 * See also shmCPrepare.c and shmCDisplay.c.
 *
 * C program for Consumer process illustrating POSIX shared-memory API.
 * make shmCRelease or gcc -o shmCRelease shmCRelease.c -I$HOME/headers/ -li2c -lm -Wall -pedantic -std=c2x
 */

/* Defines needed for the i2cDevicesHeader.h */
#define BME688

#include <time.h>
struct timespec ts; /* Used in icm20948ConvenientBits.h */

#if __has_include("i2cIncludes.h")
#include "i2cIncludes.h"
#endif

int main() {
   void * ptr = NULL;
   ptr = openSharedMemory(shmCName, SHAREDMEMORYSIZE, 0);
   if (ptr == NULL) exit(0);
   __initialiseSharedMemory(ptr, 0x00);
   shm_unlink(shmCName); /* remove the shared memory object */
   printf("COMMON Shared Memory unlinked. Exiting.\n");
   exit(0);
}
