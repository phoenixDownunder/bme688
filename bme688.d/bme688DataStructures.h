/* bme688DataStructures.h */

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
 * Additional Data Structures for the bme688
 * May 2025.
 * Amended August 2025. 
 * Amended October 2025. 
 * Amended March 2026. 
 * The Data Structures below have been somewhat shortened.
 * Most variables are now local.
 * The results are passed to the COMMON SHARED MEMORY.
 */

#ifndef BME688_DATA_STRUCTURES_H
#define BME688_DATA_STRUCTURES_H

#define BME688_CHIP_ID_REG    0xD0
#define BME688_VARIANT_ID_REG 0xF0
#define BME688_RESET_REG      0xE0

#define GAS_R_MSB_0    0x2C
#define GAS_R_LSB_0    0x2D /* Note mistake in doc. */
#define GAS_R_MSB_1    0x3D
#define GAS_R_LSB_1    0x3E
#define GAS_R_MSB_2    0x4E
#define GAS_R_LSB_2    0x4F

#define CTRL_GAS_0  0x70
#define CTRL_GAS_1  0x71

#define IDAC_HEAT_0 0x50
#define RES_HEAT_0  0x5A
#define GAS_WAIT_0  0x64
#define GAS_WAIT_SHARED 0x6E

#define CTRL_CONFIG 0x75
#define CTRL_MODE   0x74
#define CTRL_MEAS   0x74
#define CTRL_HUM    0x72

#define MEAS_STATUS_0 0x1D
#define MEAS_STATUS_1 0x2E
#define MEAS_STATUS_2 0x3F
#define SUB_MEAS_INDEX_0 0x1E
#define SUB_MEAS_INDEX_1 0x2F
#define SUB_MEAS_INDEX_2 0x40

#define MEAS_STATUS_NEW_DATA_MASK       0x80
#define MEAS_STATUS_GAS_MEASURING_MASK  0x40
#define MEAS_STATUS_MEASURING_MASK      0x20
#define MEAS_STATUS_TPHG_MEASURING_MASK 0x60
/* Lower 4 bits. */
#define MEAS_STATUS_GAS_MEAS_INDEX_MASK 0x0F
#define MEAS_STATUS_GAS_RANGE_MASK      0x0F

/* gasValidMask = 0x20 reg(0) 0x2D - Bits 4 and 5 - gas_valid_r and NOT heat_stab_r */
/* heatStabMask = 0x10 reg(0) 0x2D - Bits 4 and 5 - NOT gas_valid_r and heat_stab_r */
#define GAS_R_LSB_GAS_VALID_MASK 0x20
#define GAS_R_LSB_HEAT_STAB_MASK 0x10

#define PAR_T1_L 0xE9
#define PAR_T1_H 0xEA
#define PAR_T2_L 0x8A
#define PAR_T2_H 0x8B
#define PAR_T3   0x8C

#define TEMP_0_MSB  0x22
#define TEMP_0_LSB  0x23
#define TEMP_0_XSB  0x24
#define TEMP_1_MSB  0x33
#define TEMP_1_LSB  0x34
#define TEMP_1_XSB  0x35
#define TEMP_2_MSB  0x44
#define TEMP_2_LSB  0x45
#define TEMP_2_XSB  0x46
#define G_TEMPERATURE_MAX  85
#define G_TEMPERATURE_MIN -40

#define PAR_P1_L 0x8E
#define PAR_P1_H 0x8F
#define PAR_P2_L 0x90
#define PAR_P2_H 0x91
#define PAR_P3   0x92
#define PAR_P4_L 0x94
#define PAR_P4_H 0x95
#define PAR_P5_L 0x96
#define PAR_P5_H 0x97
#define PAR_P6   0x99
#define PAR_P7   0x98
#define PAR_P8_L 0x9C
#define PAR_P8_H 0x9D
#define PAR_P9_L 0x9E
#define PAR_P9_H 0x9F
#define PAR_PA   0xA0

#define PRES_0_MSB 0x1F
#define PRES_0_LSB 0x20
#define PRES_0_XSB 0x21
#define PRES_1_MSB 0x30
#define PRES_1_LSB 0x31
#define PRES_1_XSB 0x32
#define PRES_2_MSB 0x41
#define PRES_2_LSB 0x42
#define PRES_2_XSB 0x43

#define PAR_H1_L 0xE2
#define PAR_H1_H 0xE3
#define PAR_H2_L 0xE2
#define PAR_H2_H 0xE1
#define PAR_H3   0xE4
#define PAR_H4   0xE5
#define PAR_H5   0xE6
#define PAR_H6   0xE7
#define PAR_H7   0xE8

#define HUM_0_MSB 0x25
#define HUM_0_LSB 0x26
#define HUM_1_MSB 0x36
#define HUM_1_LSB 0x37
#define HUM_2_MSB 0x47
#define HUM_2_LSB 0x48

#define PAR_G1   0xED
#define PAR_G2_L 0xEB
#define PAR_G2_H 0xEC
#define PAR_G3   0xEE
#define RES_HEAT_RANGE_REG 0x02
#define RES_HEAT_VALUE_REG 0x00

/* Using the bme688 as reference (3130) - correctionFactors are unused.
 * They were used when testing and developing the software.
 * Left here for historical reference.
 * #define g_dh_correctionFactor 3130
 * #define g_ih_correctionFactor  400
 * #define g_h_correctionFactor     0
 * #define g_p_correctionFactor     0
 * #define g_t_correctionFactor     0
 */

#define BME688BUFFERLENGTH 256
#define OFF_MODE      0
#define FORCED_MODE   1
#define PARALLEL_MODE 2
#define GASMAN_BUFFER_SIZE 10
#define GASMAN_MAX_TEMPERATURE 360
#define GASMAN_MAX_WAIT_TIME 4032
#define GASMAN_MAX_WAIT_TIME_SHARED 4032

/* MAXSTRSIZE (128 + 64) is a sensible string length in the config files.
 * if it is exceeded, then a fault will occur. */
#define MAXSTRSIZE    192
#define MAXSTATECOUNT 64
#define MAXLOOPCOUNT  99
#define MAXLOOPDELAY  1000 * 3600
#define MINLOOPDELAY  250
#define MAX_GAS_CYCLE_TIME 60
#define MIN_GAS_CYCLE_TIME 10
#define MAXERRORSTRSIZE 64
#define STYLECOUNT 7

/* The ohm symbol for fun. */
#define ohm "\u2126"

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * GLOBALS
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* Global device pointer... don't like doing this but convenient. */
   static int fp;

/* g_mode: 0 sleep mode, 1 forced mode, 2 parallel mode. */
/* There are several options to set the mode of operation...
 *
 * Descriptor parallel: In the fsm config file(s)...
 * AFTER using the descriptor "profile"...
 * use the descriptor "parallel" to set PARALLEL_MODE.
 *
 * If FORCED_MODE(default) is to be used...
 * can still use the descriptor "profile" to allow the...
 * parallel mode registers (gas_wait_time[n] and res_heat_temp[n])
 * and nb_conv to be set.
 */
   int g_mode = 0x00; /* Separately, the mode register value 0, 1 or 2). */

/* The OSRS sampling rates are 0 (off), 1x, 2x, 4x, 8x (the default), 16x   */
   __u8 g_osrs_h = 0x08;
   __u8 g_osrs_p = 0x08;
   __u8 g_osrs_t = 0x08;
   __u8 g_filter = 1; /* 000 == filter off, 001=1,010=3,011=7,100=15,101=31,110=63,111=127. */
   __u8 g_l_time = 0x05; /* 1000mS */

/* The following are set by the setters, then pulled in at the appropriate time. */
   __u8 s_osrs_h = 0xFF;
   __u8 s_osrs_p = 0xFF;
   __u8 s_osrs_t = 0xFF;
   __u8 s_filter = 0xFF;
   __u8 s_l_time = 0xFF;

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Common FORCED and PARALLEL MODE Globals.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

   sig_atomic_t g_sig  = 0; /* Signal sig purely for experimentation. */
   sig_atomic_t g_quit = 0;

   int setterInUse     = 0;
   int reset           = 0;
   int runLoopInUse    = 0;
   int _display        = 1;
   int debug           = 0; /* Lots of noise to decipher... */
   int debug2          = 0; /* Even more information and will affcet timing. */
   int debug3          = 0; /* An extreme case and will upset timing!! */
   int g_prev          = 0;
   int  g_loop_counter = 1;
   int  g_temperature  = 0;
   long g_t_fine       = 0; /* long vs __s64 vs Int. */

   sig_atomic_t g_enable_gas    = 0; /* Boolean: Do the Gas Analysis.      */
   sig_atomic_t g_active_alarm  = 0; /* Used by signal for timeHandler.    */
   int g_style                  = 1; /* 1 sets the default output data format style. */
   int g_enable_g_style         = 0; /* Used in fsm mode. */
   int g_active_gas_valid       = 0; /* Boolean: Gas Analysis is active.   */
   int g_active_gas_heat_stable = 0;
   int g_active_step            = 0; /* 0 .. 9 */
   int g_active_field           = 0; /* 0 .. 2 */

   int g_active_nb_conv         = 0; /* 0 .. 9, 0 in FORCED MODE. */
   int g_active_idac            = 0;
   int g_active_raw_gas         = 0x00; /* Originally __u16 */
   int g_active_raw_gas_range   = 0x00; /* Was __u8. */
   int g_active_gas_wait_time_shared = 0;
   int g_active_gas_wait_time   = 0;
   int g_active_res_heat_temp   = 0;
   int g_active_gas_meas_index  = 0;
   int g_active_sub_meas_index  = 0; /* Whole byte. */

/* The following are set by the setters. FORCED MODE only. See fsm for PARALLEL.  */
   int s_active_nb_conv              = 0xFF;
   int s_active_gas_wait_time        = 0xFF;
   int s_active_gas_wait_time_shared = 0xFF;
   int s_active_res_heat_temp        = 0xFF;

   char g_es[MAXERRORSTRSIZE]; /* Was going to make this local... */

   static __u8 g_rawDataBuffer[BME688BUFFERLENGTH] = { 0 };
   static __u8 g_dataIsValid[BME688BUFFERLENGTH]   = { 0 };

/* End of Globals. */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Finite State Machine...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* State Machine Handlers...
 *
 * The following structures are for the Finite State Machine configuration.
 * The aim is to provide a definitive series of states for the bme688 to step
 * through with dependencies for state changes. This allow for a wide variety
 * of configurations to be created to suit the enviroment used.
 * The state machine configurations are pre-prepared and recovered from a FILE.
 *
 * The default EDITABLE TEXT FILE name is bme688FiniteStateConfiguration.txt but
 * the desired config file can be selected with the -f option.
 * bme688 [fsm] -f <filename>.txt [-s 0..MAXSTYLECOUNT]
 * where fsm can be specified without the use of the -f option.
 * -s option is the output data style option.
 */

   struct state_t {
      int ID;         /* StateID an incremental value.         */
      int state;      /* State - numeric 0... user defined.    */
      int sensor;     /* TPH sensors == 0, TPHG sensors == 1.  */
      int loop;       /* loop count in this state.             */
      int loop_delay; /* loop cycle time in mS. 250mS minimum. */
      int terminate;  /* set to 1 in an fsm terminate state.   */
      int style;      /* Specifies the output format style.    */

/* The gas_cycle_time is the time delay between multiple gas cycles.
 * If a gas cycle is invoked, no further gas cycles are permitted
 * until this gas_cycle_time times out. It is handled by an asynchronous
 * sigaction signal(SIGALRM, timeOutHandler) and the alarm(gas_cycle_time)
 * is in SECONDS. The minimum is set to MIN_GAS_CYCLE_TIME above.
 * If the gas cycle is unused, the gas_cycle_time is ignored.
 * Only used in FORCED_MODE. In PARALLEL_MODE, the gas_cycle_time
 * is handled separately but a settle time is still used.
 * MAX_GAS_CYCLE_TIME is there for timeout convenience.
 */
      int gas_cycle_time;
      int gas_wait_time_shared;

/* use_profile specifies the PARALLEL mode gas cycle for this specific state...
 * Each state may differ. FORCED_MODE is the default mode for each state.
 */
      int use_parallel;
      int use_profile;
      int _windex; /* Wait index counter when loading config. */
      int _tindex; /* Temp index counter when loading config. */
      int _iindex; /* idac index counter when loading config. */
      int p_res_heat_temp[GASMAN_BUFFER_SIZE];
      int p_gas_wait_time[GASMAN_BUFFER_SIZE];
      int p_idac[GASMAN_BUFFER_SIZE];
 
      int nb_conv;

/* Change of function here... anomaly was to be a boolean operation but
 * has now been changed to a type_of_anomaly variable. No size change necessary.
 * default(0), TPHG... temperature(1), pressure(2), humidity(3), gas (4).
 * Also room for other options such as temperature AND pressure(5).
 * The options allow for other states to test and branch.
 */
      int anomaly;
/* Anomaly value. Changed from Int to float in case small values required. */
      double anomalyValue1;
/* Second optional anomaly value in the case of a range. */
      double anomalyValue2;
/* 0 off, 1 greater than, -1 less than, 2 beyond range [anomalyValue2 < anomalyValue1] */
      int anomalyCondition;

      int inext;
      int inextA;
      struct state_t * next;
      struct state_t * nextOnAnomaly;
      struct state_t * prev;
   };

/* Need to save the ptrs corresponding to the states
 * and the index ID because the
 * linked list s->next are not sequential.
 * Used to backfill the s->next ptrs.
 * Painfully, these are not local. Global.
 */
   struct state_ptrs {
      int valid;
      int state;
      struct state_t * ptr;
   };

/* fsm determines the use of the config file bme688FiniteStateConfiguration.txt file.
 * This default name can be overwritten at startup...
 * ...for example...
 * bme688 fsm -f fileName.txt [debug]
 * bme688     -f fileName.txt [debug] [debug] [debug]
 */
   int fsm = 0;
   char * fsmName = "bme688FiniteStateConfiguration.txt";

   int stateID = 0; /* Current ID, not user visible */
   struct state_t * head    = NULL; /* First node   */
   struct state_t * state_p = NULL; /* Working node */
   struct state_ptrs ptrs[MAXSTATECOUNT];

/* End of Finite State Machine definitions. */

#endif
