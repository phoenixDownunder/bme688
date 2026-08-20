/* shmCbme688Prepare.c */

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

#define BME688

#include <time.h>
struct timespec ts; /* Used in icm20948ConvenientBits.h */

#if __has_include("i2cIncludes.h")
#include "i2cIncludes.h"
#endif

#include "../bme688.d/bme688DataStructures.h"
#include "../bme688.d/bme688Handlers.h"

int main(int argc, char ** argv) {
   int status = 0;
/* Local globals!! */
   s_osrs_h  = 0x08; /* osr_h = 0x04. */
   s_osrs_p  = 0x08; /* osr_p = 0x04. */
   s_osrs_t  = 0x08; /* osr_t = 0x04. */
   s_filter  = 0x01; /* Section 5.3.3.4 BME688 Datasheet IIR Filter Coefficient - 001 = 1 */
   s_l_time  = 0x05; /* runLoop Time 101 = 1000mS */

   int active_nb_conv = 0;
   int active_gas_wait = 0;
   int active_gas_wait_shared = 0;

   int active_res_heat = 0;
   __u16 active_raw_gas = 0x00;
   __u8  active_raw_gas_range = 0x00;
   __u8  active_idac_heat = 0xAA;

   ptr_common_shmMap = connectToSharedMemory(shmCName); /* Global pointer. */
   if (ptr_common_shmMap == NULL) {
      printf("Unable to connect to shared memory, exiting.\n");
      exit(0);  /* Just in case. */
   }
   void * p_aMap = &common_aMap; /* Global pointer. */
   setterInUse = 1;

   if (setterInUse) {
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.writeLock, 0x01);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688InUse, 0x01); /* Permanent. */
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688_osrs_InUse, 0x01); /* Permanent. */
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688_osrs_h, s_osrs_h);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688_osrs_p, s_osrs_p);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688_osrs_t, s_osrs_t);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688_filter, s_filter);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688_l_time, s_l_time);

      status |= writeshmU32(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688GasMan_active_nb_conv,  (__u32)(active_nb_conv));
      status |= writeshmU32(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time, (__u32)(active_gas_wait));
      status |= writeshmU32(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time_shared, (__u32)(active_gas_wait_shared));
      status |= writeshmU32(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688GasMan_active_res_heat_temp, (__u32)(active_res_heat));
      status |= writeshmU16( ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688GasMan_active_raw_gas, active_raw_gas);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688GasMan_active_raw_gas_range, active_raw_gas_range);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.bme688GasMan_active_idac_heat_temp, active_idac_heat);
      status |= writeshmByte(ptr_common_shmMap, p_aMap, &common_aMap.shm.writeLock, 0x00);
      setterInUse = 0;
   }
   printf("Completed: %d\n", status);
   exit(0);
}
