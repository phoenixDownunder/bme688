/* shmCPrepare.c */

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
 * shmCPrepare.c
 *
 * Amended August 2025
 *
 * For testing Shared Memory.
 * See also shmCDisplay.c and shmCRelease.c.
 *
 * gcc -o shmCPrepare shmCPrepare.c -I$HOME/headers/ -li2c -lm -Wall -pedantic -std=c2x
 */

/* Defines needed for the i2cDevicesHeader.h */
#define GENERIC

#include <time.h>
struct timespec ts; /* Used in icm20948ConvenientBits.h */

#if __has_include("i2cIncludes.h")
#include "i2cIncludes.h"
#endif

int main(int c, char ** v) {
   void * ptr;
   printf("\nOpen or Create COMMON Shared Memory: %s...\n", shmCName);
/* Open(0) or Creat(1) */
   ptr = openSharedMemory(shmCName, SHAREDMEMORYSIZE, 0);
   if (ptr == NULL) {
      printf("Attempting to CREATE COMMON shared memory...\n");
      ptr = openSharedMemory(shmCName, SHAREDMEMORYSIZE, 1);
      if (ptr == NULL) {
         printf("Unable to access COMMON shared memory. Exiting.\n");
         exit(0);
      }
   }
   if (ptr_common_shmMap == NULL) {
      printf("Unable to access COMMON shared memory. Exiting.\n");
      exit(0);
   }

/* If successful, ptr_common_shmMap and ptr_common_aMap have been set. */
   printf("Connection successful...\n");
   printf("Writing 0xCA to...      LOCAL MEMORY common_aMap.shm.readLock...\n");
   printf("Writing 0xCC to COMMON SHARED MEMORY ptr_common_shmMap.shm.readLock...\n");
   printf("\n");
   writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.readLock, 0xCC);
   common_aMap.shm.readLock = 0xCA;
   displayLocalMemoryBank(0);
   displaySharedMemoryBank(0);
   writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.readLock, 0x00);
   common_aMap.shm.readLock = 0x00;
   printf("Clearing readLocks. Exiting.\n");
// shm_unlink(shmName); /* remove the shared memory object */
   return 0;
}
