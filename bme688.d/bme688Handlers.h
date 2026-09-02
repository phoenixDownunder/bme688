/* bme688Handlers.h */

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
 * bme688Handlers.h
 * Handlers for bme688 temperature, humidity, pressure and gas recovery.
 * The bme688 is a combined Temperature, Humidity Pressure and Gas sensor.
 * Current reference is Document Number BST-BME688-DS000-03
 * Revision 1.3 February 2024.
 * The interface is i2c.
 *
 * Amended September 2025. 
 * Amended January 2026.
 * Amended May 2026.
 *
 * Most variables are now local.
 * The results are passed to the COMMON SHARED MEMORY.
 *
 * May 2026 - Altered signal handling to use sigaction.
 * Online documentation for sigaction is SOOO olde. Hopeless.
 *
 */

#ifndef BME688_HANDLERS_H
#define BME688_HANDLERS_H

/* Have included sigaction signal handling in place of the
 * the older traditional signal handling...
 * sa.sa_sigaction handler option doesn't work. (June 2026)
 * sa.sa_sigaction handler now operational (July 2026!)
 * Not really needed but still.
 * See #define USE_SIGACTION in bme688.c.
 */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Main procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void intHandler(int sig); /* Also used by sigaction. */

#ifdef USE_SIGACTION
void saHandler(int sig, siginfo_t * info, void * context);
#endif

void _errorTerminate(int fp, int status);
static __s16 convertToS16(char upper, char lower);
static __u16 convertToU16(char upper, char lower);
static __s32 patchIt(__u8 msb, __u8 b2, __u8 b1, __u8 lsb);
void _clearGlobalBuffers();
void __printfcomma(int n);

void bme688PrintHelp();
void bme688PrintHelpHint();
void displayBME688Registers(int fp);
void _bme688Displayosrs_h(int fp);
void _bme688Displayosrs_p(int fp);
void _bme688Displayosrs_t(int fp);
void _bme688DisplayFilterCoefficient(int fp);
void _bme688DisplayLoopTime(int fp);
void bme688DisplayOSRS(int fp);

int _extractTheStaticRegisters(int fp, __u8 slaveAddress);
int _extractTheAltRegisters(int fp, __u8 slaveAddress);

int bme688ReadChipID(int fp, int _debug);

double _compensateTemperature(__s32 _adcT, int _debug);
double _compensatePressure(__s32 _adcP, int _debug);
double _compensateHumidity(__s32 _adcH, int _debug);
double _compensateHumidityDouble(__s32 _adcH, int _debug);
int    _convertGasOhms(int _rawGas, __u8 _gas_range, int _debug);

__u8 _prepareTemperatureSensorOSR(int r);
__u8 _preparePressureSensorOSR(int r);
__u8 _prepareHumiditySensorOSR(int r);
__u8 _prepareFilter(int r);
int setSensorOSRSRegisters(int fp, int _debug);
void bme688DisplaySensorMode(int fp);
int setSensorMode(int fp, int _mode, int _debug);

__s32 _recoverTemperatureRegisters(int fp, int _field, int _debug);
__s32 _recoverPressureRegisters(int fp, int _field, int _debug);
__s32 _recoverHumidityRegisters(int fp, int _field, int _debug);
int   _recoverGasRegisters(int fp, int _field, int _debug);

double bme688RecoverTemperature(int fp, int _field, int _debug);
double bme688RecoverPressure(int fp, int _field, int _debug);
double bme688RecoverHumidity(int fp, int _field, int _debug);
int    bme688GasManRecoverGas(int fp, int _field, int _debug);

void checkArgc(int __argc);
int checkargvOption(int fp, char * v);
int _doSetter(int __argc, char ** __argv);
int _doPrepareRunLoop(int __argc, char ** __argv);
int bme688CommandLineArgumentsPart1(int __argc, char ** __argv);
int bme688CommandLineArgumentsPart2(int __argc, char ** __argv);
void __doTest(int t);  /* a simple test routine... can be ignored. */
int bme688PreInit(int  __argc, char ** __argv);
int bme688PostInit(int __argc, char ** __argv);
int bme688Init(int __argc, char ** __argv); /* Uses the local copy of argv. */

int bme688Prepare_TPH_Operation(int fp, int _debug);
int bme688Checkshm(int fp, int _debug);
void bme688LoopDelay(__u8 l_time);

int  __bme688GasMan_DisplayGasConfig(int fp);
int  __bme688GasMan_Zero_Gas_Registers(int fp, int _debug);
int  __bme688GasMan_Set_idac(int fp, int _step, int _level, int _debug);
__u8 __bme688GasMan_Prepare_Timer_Reg(int _timerStep);
void __TEST_bme688GasMan_Prepare_Timer_Reg();
int  __bme688GasMan_Set_gas_wait_time_shared(int fp, int _mS, int _debug);
int  __bme688GasMan_Set_gas_wait_time(int fp, int _step, int _mS, int _debug);
int  __bme688GasMan_Set_heater_temperature(int fp, int _step, int targetTemp, int _debug);

int bme688GasMan_EnableRunGas(int fp, int enable);
int bme688GasMan_CheckRunGas(int fp); 
int bme688GasMan_Set_nb_conv(int fp, int _nb_conv, int _debug);
int bme688GasMan_Prepare_Forced_Operation(int fp, int _res_heat_temp, int _gas_wait_time, int _debug);

void bme688ClearActiveGasInfo(int fp);
int waitForNewDataReady(int fp, int _field, int _enable_gas, int _debug);
int readTPHSensorRegisters(int fp, int _field, int _enable_gas, int _debug);
double calculateDewPoint(int fp, double _temperature, double _relative_humidity);

int _lockSHM(int fp);
int _clearSHMLock(int fp);

int bme688ProcessData(int fp, int _field, int _enable_gas, int _debug);
int bme688DisplayProcessData(int fp, struct state_t * s, int _style, int _include_TPH, int _include_gas, int _debug);

/* Parallel Mode Handlers. */
int pmDetectMeasuringON(int fp, int _field, int _debug);
int pmDetectGasMeasuringON(int fp, int _field, int _debug);
int pmDetectGasMeasuringOFF(int fp, int _field, int _debug);
int pmDetectNewDataAvailable(int fp, int _field, int _debug);

int pmPrepare(int fp, struct state_t * s, int _debug);
int pmBeginParallelProcess(int fp, struct state_t * s, int _enable_gas, int _debug);
int pmProcessParallelData(int fp, struct state_t * s, int _field, int _enable_gas, int _saveIteration, int _debug);
int pmProcessParallelLoop(int fp, struct state_t * s, int _enable_gas, int _debug);

/* Finite State Handlers. */
void __fsmInitialiseStructStatePtrs();
struct state_t * __fsmCreateStateNode(struct state_t * _prev_state_p);
void __fsmDivideAndConquerFiniteStates(char * key, char * value);
int    fsmReadStateMachineConfig(char * fname, int _debug);
struct state_t * __fsmGetStatePtr(int _state);
int  __fsmEstablishListLinks(int _stateID, int _debug);
void   fsmPrintState(const struct state_t * s);
void   fsmPrintAllStates(const struct state_t * h);
void   fsmPrintAndFreeList(struct state_t * currentNode, int _debug);
void  _fsmLoopSleep(int mS);
int   _fsmEnableRunGas(int fp, int enableGas, int _debug);
int  __fsmAnomalyDetector(int fp, struct state_t * s, int _debug);
int   _fsmTraverseState(int fp, struct state_t * s, int _field, int _debug);
int   _fsmTraverseStateMachine(int fp, int _debug);
int    fsmStartFiniteStateMachineRunLoop(int fp, int _debug);

int bme688StartDirectRunLoop(int fp, int _debug);
int bme688ClearDown(int fp);

int   __bme688PerformExternalAnomalyOperation(int fp, struct state_t * s, int _debug);
int   __bme688PerformExternalOperation(int fp, struct state_t * s, int _debug);

int prepareSignal(int fp);
int bme688Main(int __argc, char ** argv);

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Convenience Handlers.
 * Yes, these are machine dependent but we are using Raspberry Pis.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* SIGNALS... was using standard signals, as such...
 * signalname(sig),  strsignal(sig)
 * sigdescr_np(sig), sigabbrev_np(sig) and sys_siglist[sig])
 * deprecated in Raspbian...
 * Created my own for convenience only.
 * Only x86/ARM signals as defined are used. Not PA-RISC.
 * Defined in headers/i2cCovenient.h bits...
 * char * signalname(int sig);
 * char * strsignal(int sig);
 * On error, return the empty string "" rather than NULL.
 * <Much much later> In the end, most have been commented out!
 * See the saHandler below.
 */

void intHandler(int sig) { /* Should be fast. */
   g_sig = sig; /* Save the sig value. */
/* printf("\n*** intHandler: %s(%d): %s...\n", signalname(sig), sig, strsignal(sig)); */
   switch (sig) {
      case SIGINT  : g_quit = 1; break;
      case SIGTERM : g_quit = 1; break;
      case SIGTSTP : g_quit = 1; break;
      case SIGUSR1 : g_quit = 1; break;
      case SIGALRM : g_active_alarm = 0; /* Reset the global Alarm variable. */
                     g_enable_gas   = 1; /* sig_atomic_t allow the gas to be rerun if applicable. */
                     break;
      default: break;
   }
   return;
}

/* Have left the printf notification below for interest really. Can/should be removed. */
#ifdef USE_SIGACTION
void saHandler(int sig, siginfo_t * info, void * context) {
   printf("\n*** saHandler: %s(%d): %s...\n", signalname(sig), sig, strsignal(sig));
   intHandler(sig); /* Above. */
   return;
}
#endif

/* This is the total failure handler. Tries to tidy then terminates process. */
void _errorTerminate(int fp, int status) {
   if (fp <= 0) {
      printf("\nUnable to find device\n");
      exit(0);
   }
   if (status < 0) {
      printf("\nInvalid status %d, exiting.\n", status);
      if (fsm) if (head != NULL) fsmPrintAndFreeList(head, 0);
      close(fp);
      exit(0);
   }
}

static __s16 convertToS16(char upper, char lower) {
   union byte2 b;
   b.signed_integer = 0; /* clear */
   b.byte[0] = lower;
   b.byte[1] = upper;
   return b.signed_integer;
}

static __u16 convertToU16(char upper, char lower) {
   union byte2 b;
   b.unsigned_integer = 0;
   b.byte[0] = lower;
   b.byte[1] = upper;
   return b.unsigned_integer;
}

static __s32 patchIt(__u8 msb, __u8 b2, __u8 b1, __u8 lsb) {
   union byte4 { /* Yes, machine dependent but we are using Raspberry Pis. */
      char byte[4];
      __u32 unsigned_integer;
      __s32 signed_integer; /* Typedef int but used for clarity. */
   } b;
   b.signed_integer = 0; /* clear */
   b.byte[0] = lsb;
   b.byte[1] = b1;
   b.byte[2] = b2;
   b.byte[3] = msb;
   return b.signed_integer;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * .c
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void _clearGlobalBuffers() {
   int i = 0;
   for (i = 0; i < BME688BUFFERLENGTH; i++) {
      g_rawDataBuffer[i] = 0x00;
      g_dataIsValid[i]   = 0x00;
   }
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * __printfComma:
 * A funny yet useful routine from somewhere deep in the Internet...
 * that I can no longer find...
 * to display a number with commas in the correct places.
 * THIS ROUTINE IS NOT MINE...
 * and belongs to and may be copyrighted by the owner, not me.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void __printfcomma (int n) {
   if (n < 0) {
      printf ("-");
      __printfcomma (-n);
      return;
   }
   if (n < 1000) {
      printf ("%d", n);
      return;
   }
   __printfcomma (n/1000);
   printf (",%03d", n%1000);
   return;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Help details for the bme688. Other parameters are ignored.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void bme688PrintHelp() {
   char * _processName = strrchr(getenv("_"), '/');
   _processName++;
   printf("\n");
   printf("%s HELP...\n", _processName);
   printf("There are 2 major modes of operation... FiniteStateMachine (fsm) mode and Individual command mode...\n");
   printf("fsm mode takes priority over Individual [with or without gas] Mode.\n");
   printf("fsm mode: Command line format to select Finite State Machine mode and optionally the config fileName is...\n");
   printf("%s fsm [-f fsmName] [-s 0..%d] [debug] [debug] [debug] (enter)\n", _processName, STYLECOUNT - 1);
   printf("... to select Finite State Machine mode and optionally the config fileName...\n");
   printf("fsm mode: ALL parameters are set in the [default] EDITABLE TEXT file %s.\n", fsmName);
   printf("fsm mode: There are a series of TEST fsmName config files in the ./tests/ directory.\n");
   printf("fsm mode: The -s [0..%d] selects the display output style. 0 is minimal, 1 is the default.\n", STYLECOUNT - 1);
   printf("\n");
   printf("Individual Command format is...\n");
   printf("%s [-n nn] [gas] [debug] [debug] [debug] [nodisplay] [-a adapterAddress][-b slaveAddress] ", _processName);
   printf("[-d][-D][-h][-p][-s][-t] [0,1,2,4,8(default),16] [-v]\n");
   printf("There are several pre-processing requirements before a gas operation can start, see SETTER below.\n");
   printf("The Temperature Pressure Humidity (TPH) without gas is the default operation.\n"); 
   printf("A typical   RUN command example would be    bme688(enter)\n");
   printf("...Output TPH (no gas) data display style set to 1 (default), loop_count is 1 (default), loop_time is 1000mS(default).\n");
   printf("A variation RUN command example would be    bme688 [-n 2] [-s 0] [debug] (enter)\n");
   printf("gas...\n");
   printf("gas: the keyword gas used to activate a FORCED_MODE gas operation.\n");
   printf("gas: There is a default 10 Second  TIMEOUT for each gas operation.\n");
   printf("gas: The gas TIMEOUT operates asynchronously and does not affect the TPH loop timeout.\n");
   printf("gas: If -n loop_counter * -l loop_delay is greater than the gas TIMEOUT, then the FORCED_MODE gas operation repeats.\n");
   printf("gas: SETTER GasMan: active_res_heat and active_wait_time MUST be set prior to any gas operation.\n");
   printf("gas: SETTER command example would be    bme688 -gw 200 -gt 100 (enter)\n");
   printf("gas: RUN    command example would be    bme688 -n 8 gas [-s 1] [debug] (enter)\n");
   printf("\n");
   printf("-h  : set over sampling rate 1x 2x 4x 8x 16x for humidity sensor in shared memory, then terminate.\n");
   printf("-p  : set over sampling rate 1x 2x 4x 8x 16x for pressure sensor in shared memory, then terminate.\n");
   printf("-t  : set over sampling rate 1x 2x 4x 8x 16x for temperature sensor in shared memory, then terminate.\n");
   printf("-i  : set the IIR Filter Coefficient 0, 1(default) 3, 7, 15, 31, 63, 127 in shared memory, then terminate.\n");
   printf("-d  : set default osrs for all sensors in shared memory, then terminate.\n");
   printf("-D  : set minimum osrs for all sensors in shared memory, then terminate.\n");
   printf("-gn : set GasMan nb_conv (0..9), then terminate.\n");
   printf("-gt : set GasMan current active_res_heat in degrees Celsius (0..~400), then terminate.\n");
   printf("-gw : set GasMan current active_gas_wait in mS (0..4032), then terminate.\n");
   printf("-l  : set Loop time mS or Seconds for runLoop in shared memory, then terminate.\n");
   printf("    : Use 0.5(mS), 62.5, 125, 250, 500, 1000(mS)(default) or 1(s), 10, 20 (as used in bme280).\n");
   printf("-n  : nn set loop mode [0 for infinite or nn seconds] and proceeds to the runLoop\n");
   printf("Use %s -n 0 to run continuously (Forced mode with cycle time 1Hz)...\n", _processName);
   printf("-s  : style: set the output display style [0..%d], default(1). style is NOT saved and must be set for each run.\n", STYLECOUNT - 1);
   printf("-v  : Display all current device registers, then terminate.\n");
   printf("registers : Display all current device registers, then terminate.\n");
   printf("-?  : Display this help summary\n");
   printf("?   : Display this help summary\n");
   printf("Arguments are processed in argument order. All other arguments are ignored\n");
   printf("There are 3 debug levels...\n");
   printf("Use %s [...] debug (enter) to run in DEBUG mode.\n", _processName);
   printf("Use %s [...] debug debug (enter) to run in additional information DEBUG mode.\n", _processName);
   printf("Use %s [...] debug debug debug (enter) to run in extreme detail DEBUG mode.\n", _processName);
   printf("Use %s nodisplay : to stop showing the results as they are received. Final results shown.\n", _processName);
   printf("Simple example...\n");
   printf("Use %s -gw 200 -gt 100 to set a basic gas setting, then...\n", _processName);
   printf("Use %s -n -6 -s 1 for example to get a result.\n", _processName);
   printf("Use stop2 or ^C for a controlled interrupt...\n\n");
   return;
}

void bme688PrintHelpHint() {
   char * _processName = strrchr(getenv("_"), '/'); _processName++;
   printf("\n");
   printf("Use %s -? for command options help.\n", _processName);
   printf("%s fsm [-f fsmName] [-s 0..5] [debug] to select Finite State Machine mode...\n", _processName);
   printf("Use %s -n 0 to run continuously (cycle time 1Hz)...\n", _processName);
   printf("Use %s -gw 200 -gt 100 to set a basic gas setting, then...\n", _processName);
   printf("Use %s -n -6 -s 1 for example to get a result.\n", _processName);
   printf("Use stop2 or ^C for a clean interrupt...\n");
   return;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Display the bme688 Registers.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void displayBME688Registers(int fp) {
   int _Offset      = 0x00; /* 0x10 */
   int AddressStart = 0x00; /* 0x1D */
   int AddressEnd   = 0xFF;
   int nvm_addr     = 0;
   int j = 0;
   if (fp < 0) {
      printf("bme688 device is not open\n");
      return;
   }
   printf("bme688 Registers...\n");
   printf("       ");
   for (j = 0; j < 0x10; j++) printf("  %x", j);
   for (nvm_addr = _Offset; nvm_addr < (AddressEnd + 1); nvm_addr++) {
      if (nvm_addr < AddressStart) {
         if (nvm_addr % 16 == 0) { printf("\n%02X:\t",nvm_addr); }
         printf("   ");
      } else {
         if (nvm_addr % 16 == 0) { printf("\n%02X:\t",nvm_addr); }
         if (g_dataIsValid[nvm_addr] == 1) {
            printf("\033[32m%02X\033[0m ", g_rawDataBuffer[nvm_addr]);
         } else { 
         if (g_dataIsValid[nvm_addr] == 2) {
/* red bold    printf("\033[31m\033[1m%02X\033[0m ", g_rawDataBuffer[nvm_addr]); */
               printf("\033[31m%02X\033[0m ", g_rawDataBuffer[nvm_addr]);
            } else {
/* red bold         printf("\033[31m\033[1m%02X\033[0m ", g_rawDataBuffer[nvm_addr]); */
/*                  printf("-- "); */
                    printf("\033[31m\033[1m-- \033[0m");
            }
         }
      }
   }
   printf("\n\n");
   return;
}

void _bme688Displayosrs_h(int fp) {
   printf("osr_h oversampling %s (0x%02X)\n",
      (g_osrs_h == 0) ? "off" : (g_osrs_h == 1) ? "x1" : (g_osrs_h == 2)  ? "x2" :
      (g_osrs_h == 4) ? "x4"  : (g_osrs_h == 8) ? "x8" : (g_osrs_h == 16) ? "x16" : "error", g_osrs_h);
}

void _bme688Displayosrs_p(int fp) {
   printf("osr_p oversampling %s (0x%02X)\n",
      (g_osrs_p == 0) ? "off" : (g_osrs_p == 1) ? "x1" : (g_osrs_p == 2)  ? "x2" :
      (g_osrs_p == 4) ? "x4"  : (g_osrs_p == 8) ? "x8" : (g_osrs_p == 16) ? "x16" : "error", g_osrs_p);
}

void _bme688Displayosrs_t(int fp) {
   printf("osr_t oversampling %s (0x%02X)\n",
      (g_osrs_t == 0) ? "off" : (g_osrs_t == 1) ? "x1" : (g_osrs_t == 2)  ? "x2" :
      (g_osrs_t == 4) ? "x4"  : (g_osrs_t == 8) ? "x8" : (g_osrs_t == 16) ? "x16" : "error", g_osrs_t);
}

void _bme688DisplayFilterCoefficient(int fp) {
   printf("Filter Coefficient  %s (0x%02X)\n",
      (g_filter == 0) ? "off" : (g_filter == 1) ? "1"  : (g_filter == 2) ? "3"  :
      (g_filter == 3) ? "7"   : (g_filter == 4) ? "15" : (g_filter == 5) ? "31" :
      (g_filter == 6) ? "63" : (g_filter == 7) ? "127" : "error", g_filter);
}

void _bme688DisplayLoopTime(int fp) {
   printf("Standby Time %s   (0x%02X)\n",
      (g_l_time == 0) ? "0.5mS" : (g_l_time == 1) ? "62.5mS" : (g_l_time == 2) ? "125mS" :
      (g_l_time == 3) ? "250mS" : (g_l_time == 4) ? "500mS"  : (g_l_time == 5) ? "1000mS" :
      (g_l_time == 6) ? "10S"   : (g_l_time == 7) ? "20S"    : "error", g_l_time);
}

void bme688DisplayOSRS(int fp) {
   _bme688Displayosrs_h(fp);
   _bme688Displayosrs_p(fp);
   _bme688Displayosrs_t(fp);
   _bme688DisplayFilterCoefficient(fp);
   _bme688DisplayLoopTime(fp);
   return;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Static and Alternate Register Data Recovery.
 * There is potential for read errors to occur here, abort at the end.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int _extractTheStaticRegisters(int fp, __u8 slaveAddress) {
   __u8 _slaveAddress = slaveAddress;
   __u8 _startAddress = 0x00;
   __u8 _length  = 0;
   int i2cResult = 0;
   int _error    = 0;
   char * e = "Static Register Block Read Error";

/* Chip ID... again for completeness... */
   _startAddress = BME688_CHIP_ID_REG; /* 0xD0 */
   _length = 0x01;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Variant ID... again for completeness... */
   _startAddress = BME688_VARIANT_ID_REG; /* 0xF0 */
   _length = 0x01;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Reset state */
   _startAddress = BME688_RESET_REG; /* 0xE0 */
   _length = 0x01;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Status Field 0... Ignore */
/* 0x1F..0x2D... Field 0 Sensor registers... error in documentation gas_r_lsb(2) 0x2D should be gas_r_lsb(0). */
/* Status Field 1... Register 0x2D part status Field 0... Ignore */
/* 0x30..0x3E... Field 1 Sensor registers... */
/* Status Field 2... Register 0x3E part status Field 1... Ignore */
/* 0x41..0x4F... Field 2 Sensor registers... */
/* Status Field 2... Register 0x4F part status Field 2... Ignore */

/* Gas parameters... */
   _startAddress = 0x50; /* idac_heat_0..9 */
   _length = 0x0A;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

   _startAddress = 0x5A; /* res_heat_0..9 */
   _length = 0x0A;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

   _startAddress = 0x64; /* gas_wait_0..9 and gas_wait_shared */
   _length = 0x0B; 
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* CTRL GAS */
   _startAddress = CTRL_GAS_1; /* 0x71, CTRL_HUM (0x72), status and SPI Page (0x73)... Ignore */
   _length = 0x03;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* osrs... */
   _startAddress = CTRL_MEAS; /* 0x74 and 0x75 Config */
   _length = 0x02;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Temperature Comps. */
   _startAddress = PAR_T2_L; /* 0x8A..C */
   _length = 0x03;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Temperature Comps. */
   _startAddress = PAR_T1_L; /* 0xE9..A */
   _length = 0x02;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Pressure Comps. */
   _startAddress = PAR_P1_L; /* 0x8E..F */
   _length = 0x02;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Pressure Comps. */
   _startAddress = PAR_P2_L; /* 0x90..3 */
   _length = 0x03;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Pressure Comps. */
   _startAddress = PAR_P4_L; /* 0x94... 0x99 */
   _length = 0x06;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Pressure Comps. */
   _startAddress = PAR_P8_L; /* 0x9C... 0x9F, 0xA0 */
   _length = 0x05;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Humidity Comps. */
   _startAddress = PAR_H2_H; /* 0xE1... 0xE8 */
   _length = 0x08;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Gas Comps. */
   _startAddress = PAR_G2_L; /* 0xEB... 0xEE */
   _length = 0x04;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Gas - heaterResistanceCorrectionFactor */
   _startAddress = 0x00;
   _length = 0x01;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

/* Gas - Res Heat Range. */
   _startAddress = 0x02;
   _length = 0x01;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, g_rawDataBuffer, g_dataIsValid);
   if (i2cResult < 0) { _error = -1; printf("%s: 0x%02X\n", e, _startAddress); }

   if (_error < 0) _errorTerminate(fp, _error);
   return 0;
}

/* Various registers appear to be unused but if not listed above, load them up and mark them red. */
int _extractTheAltRegisters(int fp, __u8 slaveAddress) {
   __u8 _slaveAddress = slaveAddress;
   int i = 0;
   int i2cResult = 0;
   int _error    = 0;
   for (i = 0x00; i < 0x0100; i++) {
      if (g_dataIsValid[i] == 1) {
      } else {
         i2cResult = i2cBlockRead(fp, _slaveAddress, (__u8)(i), 0x01, g_rawDataBuffer, g_dataIsValid);
         if (i2cResult < 0) { _error = -1; printf("Alternate Static Register Block Read Error: 0x%02X\n", i); }
         g_dataIsValid[i] = 2; /* Used by the display routine to display in red. */
      }   
   }
   if (_error < 0) _errorTerminate(fp, _error);
   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Read the bme688 ChipID and the Variant ID.
 * Show the value in this register is the expected value.
 * Device register    chip_id is 0xD0. (EXPECTEDSLAVEIDREG)
 * Device register variant_id is 0xF0. (EXPECTEDVARIANTIDREG)
 *
 * Returns 0 on success, -1 on failure.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688ReadChipID(int fp, int _debug) {
   __s32 i2cResult = 0;
   __u8 reg    = EXPECTEDSLAVEIDREG;
   __u8 mask   = 0xFF;
   __u8 chipID = 0x00;
   
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) {
      printf("The expected ChipID 0x%02X NOT found.\n", EXPECTEDSLAVEID);
      return -1;
   }
   chipID = (__u8)(i2cResult) & mask; /* Mask used when testing. */
   if (chipID != EXPECTEDSLAVEID) {
      printf("ChipID at register 0x%02X is 0x%02X. ", EXPECTEDSLAVEIDREG, chipID);
      printf("The expected ChipID 0x%02X NOT found.\n", EXPECTEDSLAVEID);
      return -1;
   }
   g_rawDataBuffer[reg] = chipID;
   g_dataIsValid[reg]   = 0x01;

   if (_debug) {
      printf("Chip    ID at register 0x%02X is 0x%02X. ", EXPECTEDSLAVEIDREG, chipID);
      printf("Expected ID is 0x%02X\n", EXPECTEDSLAVEID);
   }

   reg = EXPECTEDVARIANTIDREG; /* Variant ID register */
   chipID = ( (__u8)i2c_smbus_read_byte_data(fp, reg) ) & mask;
   g_rawDataBuffer[reg] = chipID;
   g_dataIsValid[reg]   = 0x01;

   if (_debug) {
      printf("Variant ID at register 0x%02X is 0x%02X. ", EXPECTEDVARIANTIDREG, chipID);
      printf("Expected ID is 0x%02X\n", EXPECTEDVARIANTID);
   }
   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Data Compensation Procedures.
 * Assumes the Static Registers have been recovered.
 *
 * It is assumed that the raw temp value is relatively linear and
 * scaled according to the fixed compensation values.
 * Thus the humidity and pressure calculations are dependent
 * on the g_t_fine value and can be scaled.
 * Was __S64, now LONG instead.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

double _compensateTemperature(__s32 _adcT, int _debug) {
   long t    = 0; /* LONG INT... 8 bytes. Safe in wild shift situations. Arguably wasteful. */
   long var1 = 0; /* Raspberry Pi more than capable here. */
   long var2 = 0;
   long var3 = 0;
/* Recover the compensation factors. */
   long T1 = (long)(convertToU16(g_rawDataBuffer[PAR_T1_H], g_rawDataBuffer[PAR_T1_L])); /* Unsigned */
   long T2 = (long)(convertToS16(g_rawDataBuffer[PAR_T2_H], g_rawDataBuffer[PAR_T2_L]));
   long T3 = (long)(convertToS16(0x00, g_rawDataBuffer[PAR_T3]));

/* Obscurely, the raw temperature value recovered involved moving
 * 4 bit units back and forth to give a 20 bit value.
 * This value is then reduced by 3 bits. (see below)
 * Thus only bit 7 of the XSB is used.
 */
   var1 = (_adcT >> 3) - (T1 << 1); /* Shift right == divide by 8. */
   var2 = (var1 * T2) >> 11;
   var3 = ((((var1 >> 1) * (var1 >> 1)) >> 12) * (T3 << 4)) >> 14;
   g_t_fine = var2 + var3; /* Global g_t_fine saved for pressure and humidity calculations. */
   t = ((g_t_fine * 5) + 128) >> 8;

   if (debug3) {
      printf("_compensateTemperature: RAW TEMPERATURE _adcT: 0x%04X, %d\n", _adcT, (int)(_adcT));
      printf("_compensateTemperature: g_t_fine: 0x%04lX, %d\n", g_t_fine, (int)(g_t_fine)); 
      printf("_compensateTemperature:        t: 0x%04lX, %d\n", t, (int)(t));
      printf("_compensateTemperature: Temperature is %5.2f%c%cC\n", t / 100.0, 0xC2, 0xB0);
   }
   return (double)(t); /* Returns temp * 100.0. */
}

double _compensatePressure(__s32 _adcP, int _debug) {
/* Using long integers to give the algorithm some space. */
   long p = 1048576 - (__s64)(_adcP); /* the Pressure */
   long var1 = 0;
   long var2 = 0;
   long var3 = 0;

   long P1 = (long)(convertToU16(g_rawDataBuffer[PAR_P1_H], g_rawDataBuffer[PAR_P1_L])); /* u16 */
   long P2 = (long)(convertToS16(g_rawDataBuffer[PAR_P2_H], g_rawDataBuffer[PAR_P2_L])); /* s16 */
   long P3 = (long)(convertToS16(0x00,                      g_rawDataBuffer[PAR_P3]));   /* s16 */
   long P4 = (long)(convertToS16(g_rawDataBuffer[PAR_P4_H], g_rawDataBuffer[PAR_P4_L])); /* ... */
   long P5 = (long)(convertToS16(g_rawDataBuffer[PAR_P5_H], g_rawDataBuffer[PAR_P5_L]));
   long P6 = (long)(convertToS16(0x00,                      g_rawDataBuffer[PAR_P6]));
   long P7 = (long)(convertToS16(0x00,                      g_rawDataBuffer[PAR_P7]));
   long P8 = (long)(convertToS16(g_rawDataBuffer[PAR_P8_H], g_rawDataBuffer[PAR_P8_L]));
   long P9 = (long)(convertToS16(g_rawDataBuffer[PAR_P9_H], g_rawDataBuffer[PAR_P9_L]));
   long PA = (long)(convertToU16(0x00,                      g_rawDataBuffer[PAR_PA]));   /* u16 */

   var1 = (g_t_fine >> 1) - 64000;
   var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * P6) >> 2;
   var2 = var2 + ((var1 * P5) << 1);
   var2 = (var2 >> 2) + (P4 << 16);
   var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * (P3 << 5)) >> 3) + ((P2 * var1) >> 1);
   var1 = var1 >> 18;
   var1 = ((32768 + var1) * P1) >> 15;
   p = (p - (var2 >> 12)) * ((__u32)(3125));

   if (p >= (1 << 30)) p = (p / var1) << 1;
      else             p = (p << 1) / var1;

   var1 = (P9 * (((p >> 3) * (p >> 3)) >> 13)) >> 12;
   var2 = ((p >> 2) * P8) >> 13;
   var3 = ((p >> 8) * (p >> 8) * (p >> 8) * PA) >> 17;
   p = p + ((var1 + var2 + var3 + (P7 << 7)) >> 4);

   if (debug3) {
      printf("_compensatePressure: RAW PRESSURE: _adcP: 0x%X, %d\n", _adcP, (int)(_adcP));
      printf("_compensatePressure: 1048576 - _adcP: 0x%08X, %d\n", 1048576 - _adcP, (int)(1048576 - _adcP));
      printf("_compensatePressure: p: 0x%lX, %d\n", p, (int)(p)); 
      printf("_compensatePressure: QNH is %.4fhPa\n", (double)(p / 100.0)); /* QNH or QNE or QFF. */
   }
   return (double)(p); /* Returns pressure * 100.0. */
}

double _compensateHumidity(__s32 _adcH, int _debug) {
/* Again using long rather than __s64 */   
   long var1 = 0;
   long var2 = 0;
   long var3 = 0;
   long var4 = 0;
   long var5 = 0;
   long var6 = 0;
   long h_comp = 0;
   long t_comp = 0;

   __u8  a = 0x00; 
   __u8  b = 0x00; 
   __u16 c = 0x00; 

   int H1; /* Done this way to stop -ansi from complaining. */
   int H2;
   int H3;
   int H4;
   int H5;
   int H6;
   int H7;
/* H1... */
   a =  g_rawDataBuffer[PAR_H1_L] & 0x0F;       /* Lower 4 bits <3:0> */
   b = (g_rawDataBuffer[PAR_H1_H] & 0x0F) << 4; /* Upper 4 bits <7:4> */
   c = (g_rawDataBuffer[PAR_H1_H] >> 4) << 8;   /* Yes can join. */

   H1 = (int)(c | b | a);
   H2 = (int)(convertToU16(g_rawDataBuffer[PAR_H2_H], (g_rawDataBuffer[PAR_H2_L] & 0xF0)) >> 4);
   H3 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H3])); /* Unneccesary I know. */
   H4 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H4]));
   H5 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H5]));
   H6 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H6]));
   H7 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H7]));

   t_comp = ((g_t_fine * 5) + 128) >> 8; /* Global g_t_fine from the temperature recovery. */

   var1 = _adcH - (H1 << 4) - (((t_comp * H3) / 100) >> 1);
   var2 = (H2 * (((t_comp * H4) / 100) + (((t_comp * ((t_comp * H5) / 100)) >> 6) / 100) + (1 << 14))) >> 10;
   var3 = var1 * var2;
   var4 = H6 << 7;
   var4 = (var4  + ((t_comp * H7) / 100 )) >> 4;
   var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
   var6 = (var4 * var5) >> 1;
/* h_comp = (var3 + var6) >> 12; */ /* Error in the documentation. */
   h_comp = (((var3 + var6) >> 10) * 1000) >> 12;

   if (debug3) {
      printf("_compensateHumidity: RAW HUMIDITY: _adcH: 0x%X\n", _adcH);
      printf("_compensateHumidity: comp_humidity is %lu\n", h_comp);
      printf("_compensateHumidity: Humidity is %f %% \n", (double)(h_comp / 1000.0));
   }
   return h_comp; /* Returns humidity * 1000.0. */
}

double _compensateHumidityDouble(__s32 _adcH, int _debug) {
   int H1 = 0;
   int H2 = 0;
   int H3 = 0;
   int H4 = 0;
   int H5 = 0;
   int H6 = 0;
   int H7 = 0;

   double fh;
   double var1;
   double var2;
   double var3;
   double var4;
   double ftemp_comp;

   __u8  a = 0x00; 
   __u8  b = 0x00; 
   __u16 c = 0x00; 

/* H1... */
   a =  g_rawDataBuffer[PAR_H1_L] & 0x0F;       /* Lower 4 bits <3:0> */
   b = (g_rawDataBuffer[PAR_H1_H] & 0x0F) << 4; /* Upper 4 bits <7:4> */
   c = (g_rawDataBuffer[PAR_H1_H] >> 4) << 8;   /* Yes can join. */
   H1 = (int)(c | b | a);
   H2 = (int)(convertToU16(g_rawDataBuffer[PAR_H2_H], (g_rawDataBuffer[PAR_H2_L] & 0xF0)) >> 4);
   H3 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H3]));
   H4 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H4]));
   H5 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H5]));
   H6 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H6]));
   H7 = (int)(convertToS16(0x00, g_rawDataBuffer[PAR_H7]));

   ftemp_comp = ((g_t_fine) / 5120.0f);

   var1 = (double)((float)_adcH) -
         (((double)H1 * 16.0f) + (((float)H3 / 2.0f) * ftemp_comp));
   var2 = var1 * ((double)(((double)H2 / 262144.0f) *
                    (1.0f + (((double)H4 / 16384.0f) * ftemp_comp) +
                    (((double)H5 / 1048576.0f) * ftemp_comp * ftemp_comp))));
    var3 = (double)H6 / 16384.0f;
    var4 = (double)H7 / 2097152.0f;
    fh = var2 + ((var3 + (var4 * ftemp_comp)) * var2 * var2);
    if (fh > 100.0f)
    {
        fh = 100.0f;
    }
    else if (fh < 0.0f)
    {
        fh = 0.0f;
    }

   if (debug3) {
      printf("_compensateHumidityDouble: RAW HUMIDITY: _adcH: 0x%X\n", _adcH);
      printf("_compensateHumidityDouble: DOUBLE humidity: %f\n", fh);
   }
   return fh; /* Returns DOUBLE humidity compensation. */
}

/* Potentially incorrect as I have no way of testing the result.
 * Also the algorithm described appears to have errors. See below.
 */

int _convertGasOhms(int _rawGas, __u8 _gas_range, int _debug) {
   __u64 var1 = 0;
   __s64 var2 = 0;
   double gas_res_d = 0.0;
   var1 = ((__u64)(262144)) >> (_gas_range & 0x0F);
   var2 = ((__s64)(_rawGas)) - 512;  /* Integer... potentially negative */
   var2 *= 3;
   var2 = var2 + 4096;
   gas_res_d = ((double)(var1)) / ((double)(var2));
   gas_res_d = 1000000.0 * gas_res_d;
   if (debug3) printf("DEBUG3: _convertGasOhms: DOUBLE gas: %f ohms\n", gas_res_d);
   return (int)(gas_res_d); /* Returns resistance ohms. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Device Preparation and Configuration Procedures.
 * Sets the (global) osrs
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

__u8 _prepareTemperatureSensorOSR(int r) {
   __u8 osr_t = 0x04; /* p36. Default oversampling x 8 */
   switch (r) {
      case  0: osr_t = 0x00; break; /* off */
      case  1: osr_t = 0x01; break;
      case  2: osr_t = 0x02; break;
      case  4: osr_t = 0x03; break;
      case  8: osr_t = 0x04; break;
      case 16: osr_t = 0x05; break;
      default: break;
   }
   osr_t = (osr_t << 5) & 0xE0; /* 0b 1110 0000 */
   return osr_t; /* Return the same. */
}

__u8 _preparePressureSensorOSR(int r) {
   __u8 osr_p = 0x00;
   switch (r) {
      case  0: osr_p = 0x00; break; /* off */
      case  1: osr_p = 0x01; break;
      case  2: osr_p = 0x02; break;
      case  4: osr_p = 0x03; break;
      case  8: osr_p = 0x04; break;
      case 16: osr_p = 0x05; break;
      default: break;
   }
   osr_p = (osr_p << 2) & 0x1C; /* 0b 0001 1100 */
   return osr_p;
}

__u8 _prepareHumiditySensorOSR(int r) {
   __u8 osr_h = 0x04; /* p36. Default oversampling x 8 */
   switch (r) {
      case  0: osr_h = 0x00; break; /* off */
      case  1: osr_h = 0x01; break;
      case  2: osr_h = 0x02; break;
      case  4: osr_h = 0x03; break;
      case  8: osr_h = 0x04; break;
      case 16: osr_h = 0x05; break;
      default: break;
   }
   osr_h = osr_h & 0x07; /* 0b 0000 0111 */
   return osr_h;
}

__u8 _prepareFilter(int r) {
   __u8 f = 0x01; /* Section 5.3.3.4 bme688 Datasheet */
   switch (r) {
      case   0: f = 0x00; break; /* off */
      case   1: f = 0x01; break;
      case   3: f = 0x02; break;
      case   7: f = 0x03; break;
      case  15: f = 0x04; break;
      case  31: f = 0x05; break;
      case  63: f = 0x06; break;
      case 127: f = 0x07; break;
      default: break;
   }
   f = (f << 2) & 0x1C; /* 0b 0001 1100 Also disables spi... */ 
   return f;
}

/* This routine reads the global osrs values and sets the osrs registers. */
int setSensorOSRSRegisters(int fp, int _debug) {
   int i2cResult = 0;
   __u8 reg   = 0x00;
   __u8 value = 0x00;

   if (debug3) printf("setSensorOSRSRegisters:...\n");

   reg = CTRL_CONFIG;
   value = _prepareFilter(g_filter);
   if (debug3) printf("Set Filter Register[0x%02X: 0x%02X], Filter: %d\n", reg, value, g_filter);
   i2cResult = i2c_smbus_write_byte_data(fp, reg, value);
   if (i2cResult < 0) printf("setSensorOSRSRegisters: Filter Write Error.\n");

   reg = CTRL_MEAS;
 /* 0b 1111 1100 - mask out mode bits. */
   value = (_prepareTemperatureSensorOSR(g_osrs_t) | _preparePressureSensorOSR(g_osrs_p)) & 0xFC;
   if (debug3) printf("Set osrs_t and osrs_p Register[0x%02X: 0x%02X]\n", reg, value);
   i2cResult = i2c_smbus_write_byte_data(fp, reg, value);
   if (i2cResult < 0) printf("setSensorOSRSRegisters: MEAS OSRS-T&P Write Error.\n");

   reg = CTRL_HUM;
   value = _prepareHumiditySensorOSR(g_osrs_h) & 0x07; /* 0b 0000 0111 - mask out the rest and spi. */
   if (debug3) printf("Set osrs_h Register[0x%02X: 0x%02X], g_osrs_h: 0x%02X\n", reg, value, g_osrs_h);
   i2cResult = i2c_smbus_write_byte_data(fp, reg, value);
   if (i2cResult < 0) printf("setSensorOSRSRegisters: Hum OSRS Write Error.\n");

   if (debug3) printf("Set Sensor OSRS Registers completed.\n");

   return (i2cResult == 0) ? 0 : -1;
}

void bme688DisplaySensorMode(int fp) {
   char * s = "bme688DisplaySensorMode: mode:";
   int i2cResult = 0;
   __u8 reg      = CTRL_MODE; /* 0x74 0b xxxx xxmm */

   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) {
      printf("%s Read Error.\n", s);
      close(fp);
      exit(0);
   }
/* Mask out everything except the 2 lower bits. */
   switch ((i2cResult & 0x03)) {
      case 0: printf("%s OFF_MODE.\n", s);      break;
      case 1: printf("%s FORCED_MODE.\n", s);   break;
      case 2: printf("%s PARALLEL_MODE.\n", s); break;
      default: break;
   }
   return;
}

int setSensorMode(int fp, int _mode, int _debug) {
   int i2cResult = 0;
   __u8 reg      = CTRL_MODE; /* 0x74 0b xxxx xxmm */
   __u8 mode     = 0x00; /* Sleep mode is the default. */

   switch (_mode) {
      case 0 :
      default: mode = 0x00; g_mode = OFF_MODE;      break; /* else... sleep */
      case 1 : mode = 0x01; g_mode = FORCED_MODE;   break;
      case 2 : mode = 0x02; g_mode = PARALLEL_MODE; break;
   }
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) {
      printf("setSensorMODE: Read Error.\n");
      close(fp);
      exit(0);
   }
/* Mask out lower 2 bits. Add in the mode. */
   mode = ((__u8)(i2cResult) & 0xFC) | mode;
   i2cResult = i2c_smbus_write_byte_data(fp, reg, mode);
   if (i2cResult < 0) {
      printf("setSensorMODE: Write Error.\n");
      close(fp);
      exit(0);
   }
   uDelay(50); /* Let the bme688 settle. Delay 50uS */
   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Raw Data Recovery.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

__s32 _recoverTemperatureRegisters(int fp, int _field, int _debug) {
   __s32 _adcT = 0;
   __u8  _usb  = 0x00;

   switch(_field) {
      case 0:
      default:
         _usb  = (g_rawDataBuffer[TEMP_0_MSB] & 0x80) ? 0xFF : 0x00; /* Yep, a fudge. */
         _adcT = patchIt(_usb, g_rawDataBuffer[TEMP_0_MSB],
                               g_rawDataBuffer[TEMP_0_LSB],
                               g_rawDataBuffer[TEMP_0_XSB]); 
         break;
      case 1:
         _usb  = (g_rawDataBuffer[TEMP_1_MSB] & 0x80) ? 0xFF : 0x00;
         _adcT = patchIt(_usb, g_rawDataBuffer[TEMP_1_MSB],
                               g_rawDataBuffer[TEMP_1_LSB],
                               g_rawDataBuffer[TEMP_1_XSB]); 
         break;
      case 2:
         _usb  = (g_rawDataBuffer[TEMP_2_MSB] & 0x80) ? 0xFF : 0x00;
         _adcT = patchIt(_usb, g_rawDataBuffer[TEMP_2_MSB],
                               g_rawDataBuffer[TEMP_2_LSB],
                               g_rawDataBuffer[TEMP_2_XSB]); 
         break;
   }
   _adcT = _adcT >> 4; /* signed right shift. */

   if (debug3) printf("_recoverTemperatureRegisters: TEMPERATURE: uncomp_temperature is: 0x%08X %d\n", _adcT, _adcT);

   return _adcT;
}

__s32 _recoverPressureRegisters(int fp, int _field, int _debug) {
   __s32 _adcP = 0;
   __u8  _usb  = 0x00;

   switch(_field) {
      case 0:
      default:
         _usb = (g_rawDataBuffer[PRES_0_MSB] & 0x80) ? 0xFF : 0x00; /* Yep, a fudge. */
         _adcP = patchIt(_usb, g_rawDataBuffer[PRES_0_MSB],
                               g_rawDataBuffer[PRES_0_LSB],
                               g_rawDataBuffer[PRES_0_XSB]);
         break;
      case 1:
         _usb = (g_rawDataBuffer[PRES_1_MSB] & 0x80) ? 0xFF : 0x00;
         _adcP = patchIt(_usb, g_rawDataBuffer[PRES_1_MSB],
                               g_rawDataBuffer[PRES_1_LSB],
                               g_rawDataBuffer[PRES_1_XSB]);
         break;
      case 2:
         _usb = (g_rawDataBuffer[PRES_2_MSB] & 0x80) ? 0xFF : 0x00;
         _adcP = patchIt(_usb, g_rawDataBuffer[PRES_2_MSB],
                               g_rawDataBuffer[PRES_2_LSB],
                               g_rawDataBuffer[PRES_2_XSB]);
         break;
   } 
   _adcP = _adcP >> 4; /* signed right shift. */

   if (debug3) printf("recoverPressureRegisters: PRESSURE: uncomp_pressure is: 0x%08X %d\n", _adcP, _adcP);

   return _adcP;
}   

__s32 _recoverHumidityRegisters(int fp, int _field, int _debug) {
   __s32 _adcH = 0;

   switch(_field) {
      case 0:
      default:
         _adcH = patchIt(0x00, 0x00, g_rawDataBuffer[HUM_0_MSB], g_rawDataBuffer[HUM_0_LSB]);
         break;
      case 1:
         _adcH = patchIt(0x00, 0x00, g_rawDataBuffer[HUM_1_MSB], g_rawDataBuffer[HUM_1_LSB]);
         break;
      case 2:
         _adcH = patchIt(0x00, 0x00, g_rawDataBuffer[HUM_2_MSB], g_rawDataBuffer[HUM_2_LSB]);
         break;
   }

   if (debug3) printf("_recoverHumidityRegisters: HUMIDITY: uncomp_humidity is: 0x%08X %d\n", _adcH, _adcH);

  return _adcH;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * GasMan Actual Gas Data Recovery.
 *
 * The raw data recovered from the bme688 consists of just a couple
 * registers and reference range data.
 * This data is "massaged" to say the least to produce a reading
 * ostensibly in ohms but it could be any type.
 * These figures are then interpreted by an external...
 * environment dependent algorithm(s) to produce an IAQ or
 * Index of Air Quality in the range of 0..500.
 * This is totally dependent on external parameters and algorithm definition.
 *
 * The actual raw data therefore becomes lost and is not available
 * for futher interpretation.
 *
 * For each transaction, the associated reference parameters include...
 * the heater temperature (set above)...
 * gas heating time (set above)...
 * the gas_range_r value (read only register)
 * the raw data (fields 0, 1 or 2.)
 * the ambient or most recent temperature...
 * the ambient or most recent humidity...
 * Additionally...
 * if the parallel mode is used, then a step index 0..9 is referenced.
 *
 * No direct access to the various algorithms is available and other
 * users would create their own algorithms for interpretation.
 *
 * Therefore, the raw data and the raw_gas_range are saved...
 * and associated reference parameters in the COMMON SHARED MEMORY
 * The resistance in ohms is calculated at display time if required.
 *
 * Recovers g_active_raw_gas and g_active_raw_gas_range. Returns g_active_raw_gas.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int _recoverGasRegisters(int fp, int _field, int _debug) {
   char * s = "_recoverGasRegisters";
   int   i2cResult = 0;
   __u8  regh   = GAS_R_MSB_0; /* reg 0x2C */
   __u8  regl   = GAS_R_LSB_0; /* reg 0x2D */
   __s32 _adcGu = 0x00;

   switch(_field) {
      case 0 : 
      default: break;
      case 1 : regh = GAS_R_MSB_1; regl = GAS_R_LSB_1; break;
      case 2 : regh = GAS_R_MSB_2; regl = GAS_R_LSB_2; break;
   }

/* g_rawDataBuffer[regl] has already been recovered as part of the gas status register.
 * but reread to recover the 2xlsb of the gas data and the range value. */
   g_rawDataBuffer[regh]  = 0;
   g_rawDataBuffer[regl]  = 0;
   g_active_raw_gas       = 0;
   g_active_raw_gas_range = 0;

   i2cResult = i2c_smbus_read_byte_data(fp, regh);
   if (i2cResult < 0) {
      printf("%s: read error: register: 0x%02X: 0x%02X\n", s, regh, i2cResult);
      _errorTerminate(fp, i2cResult);
   }
   g_rawDataBuffer[regh] = (__u8)(i2cResult & 0xFF);

   i2cResult = i2c_smbus_read_byte_data(fp, regl);
   if (i2cResult < 0) {
      printf("%s: read error: register: 0x%02X: 0x%02X\n", s, regl, i2cResult);
      _errorTerminate(fp, i2cResult);
   }
   g_rawDataBuffer[regl] = (__u8)(i2cResult & 0xFF);
   _adcGu = patchIt(0x00, 0x00, 0x00, g_rawDataBuffer[regh]); /* Don't shift at this stage. */
   _adcGu = _adcGu << 2;
   _adcGu = _adcGu | ((g_rawDataBuffer[regl] & 0xC0) >> 6); /* OR in low bits. */

   g_active_raw_gas = (int)(_adcGu);
   g_active_raw_gas_range = (int)(g_rawDataBuffer[regl] & MEAS_STATUS_GAS_RANGE_MASK); /* Read only register[3:0] 0..15 */
   if (_adcGu == 0) g_active_raw_gas_range = 0;

   if (debug3) {
      printf("DEBUG3: %s...\n", s);
      printf("*** regh: 0x%02X: 0x%02X (%d), ", regh, g_rawDataBuffer[regh], g_rawDataBuffer[regh]);
      printf("MSB << 2: 0x%02X (%d)\n", g_rawDataBuffer[regh] * 4, g_rawDataBuffer[regh] * 4);
      printf("*** regl: 0x%02X: 0x%02X (%d), ", regl, g_rawDataBuffer[regl], g_rawDataBuffer[regl]);
      printf("regl & 0xC0 : 0x%02X (%d), ", g_rawDataBuffer[regl] & 0xC0, g_rawDataBuffer[regl] & 0xC0);
      printf("LSB >> 6: 0x%02X (%d)\n", (g_rawDataBuffer[regl] & 0xC0) >> 6, (g_rawDataBuffer[regl] & 0xC0) >> 6);
      printf("*** g_active_raw_gas: 0x%02X (%d), ", g_active_raw_gas, g_active_raw_gas);
      printf("g_active_raw_gas_range: 0x%02X (%d)\n", g_active_raw_gas_range, g_active_raw_gas_range);
   }

   return (int)(_adcGu); /* Returns raw gas data */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Data Recovery Procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

double bme688RecoverTemperature(int fp, int _field, int _debug) {
   __s32 uncomp_temperature;
   if (fp <= 0) return -1.0;
   uncomp_temperature = _recoverTemperatureRegisters(fp, _field, _debug);
   return _compensateTemperature(uncomp_temperature, _debug);
}

double bme688RecoverPressure(int fp, int _field, int _debug) {
   __s32 uncomp_pressure;
   if (fp <= 0) return -1.0;
   uncomp_pressure = _recoverPressureRegisters(fp, _field, _debug);
   return _compensatePressure(uncomp_pressure, _debug);
}

double bme688RecoverHumidity(int fp, int _field, int _debug) {
   __s32 uh;
   char * s = "bme688RecoverHumidity";
   double h = 0.0;
   if (fp <= 0) return -1.0;
   uh = _recoverHumidityRegisters(fp, _field, _debug);
   if (_debug) printf("%s: raw uncomp_humidity is %u\n", s, uh);

   h = _compensateHumidity(uh, _debug);
   if (_debug) printf("%s: INTEGER Returning Humidity is %.4f%%\n", s, h / 1000.0);

   h = _compensateHumidityDouble(uh, _debug) * 1000.0;
   if (_debug) printf("%s: DOUBLE  Returning Humidity is %.4f%%\n", s, h / 1000.0);

   return h;
}

int bme688GasManRecoverGas(int fp, int _field, int _debug) {
   if (fp <= 0) return -1;
   return _recoverGasRegisters(fp, _field, _debug);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Initialise.
 *
 * The initialisation process is split into several parts.
 * The first part is to scan the MAIN arguments for each of the above parts. 
 * The second part used to intialise the device SETTINGS and then terminate.
 * The third part is to recover the run time parameters.
 * The forth part is to initialise the device.
 * The fifth part leads eventually to the device runLoop.
 * The sixth part is the Alarm interrupt and terminate process. Separate.
 *
 * Check argc and therefore pointers to argv are sensible
 * Simple procedure to eliminate basic rubbish.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void checkArgc(int __argc) {
   if ((__argc <= 0) || (__argc > EXPECTEDARGCMAX)) {
      printf("Invalid or missing command line options, exiting.\n");
      exit(0);
   }
}

/* Some of the argv options have option values 0,1,2,4,8 or 16. Not all. */
int checkargvOption(int fp, char * v) {
if ((*v != '0') &&
    (*v != '1') &&
    (*v != '2') &&
    (*v != '4') &&
    (*v != '8') &&
    ((v[0] != '1') && (v[1] != '6'))) {
       if (fp > 0) close(fp);
       printf("Error: %s: invalid option\n", v);
       return -1;
   }
   return 0;
}

int _doSetter(int __argc, char ** __argv) {
/* Replacements for argc and argv
 *
 * Use s = *++argv for convenience... if ( (*++argv)[0] == '-' )...
 */
   int _argc = __argc;
   int value;
   char * s;
   char ** pv;
   char * v;
   char g;
   pv = __argv;
   s = *pv;
   value = 0;

   while (--_argc > 0) {
      s = *++pv;
/*    if ( (*++argv or av)[0] == '-' ) ... */
      if ( (s[0] == '-') & (strlen(s) > 1) ) {
/* If this is true then look at the next character in the pv (argv) string */
         s = pv[0] + 1;

         switch (*s) {
/* The following could all be done by character conversion after checking.
 * Easier to test and set the value explicitly.
 */
         case 'g' :
            if (strlen(s) == 2) g = s[1]; else { printf("gasMan option error.\n"); exit(0); }

            switch(g) {

               case 'n' :
                          --_argc;
                          checkArgc(_argc);
                          v = (*++pv); /* v is a pointer */
                          value = atoi(v);
                          if ((value < 0) || (value > 10)) { printf("-gn s_active_nb_conv error: %s\n", v); exit(0); }
                          s_active_nb_conv = value;
                          printf("-gn s_active_nb_conv set: %d\n", s_active_nb_conv);
                          setterInUse = 1;
                          break;
               case 't' :
                          --_argc;
                          checkArgc(_argc);
                          v = (*++pv); /* v is a pointer */
                          value = atoi(v);
                          if ((value < 0) || (value > GASMAN_MAX_TEMPERATURE)) {
                             printf("-gt s_active_res_heat_temp error: %s\n", v); exit(0); }
                          s_active_res_heat_temp = value;
                          printf("-gt s_active_res_heat_temp: %d\n", s_active_res_heat_temp);
                          setterInUse = 1;
                          break;
               case 'w' :
                          --_argc;
                          checkArgc(_argc);
                          v = (*++pv); /* v is a pointer */
                          value = atoi(v);
                          if ((value < 0) || (value > GASMAN_MAX_WAIT_TIME)) {
                             printf("-gw s_active_gas_wait_time error: %s\n", v); exit(0); }
                          s_active_gas_wait_time = value;
                          printf("-gw set s_active_gas_wait_time: %d\n", s_active_gas_wait_time);
                          setterInUse = 1;
                          break;
               default : 
                          setterInUse = 1;
                          printf("Invalid gasMan option: %s, exiting..\n", s);
                          exit(0);
                          break;
               }
            break;

         case 'h' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv); /* v is a pointer */
                  printf("Humidity Sensor Over Sampling Rate... [0, 1, 2, 4, 8, 16]\n");
                  if (checkargvOption(fp, v) != 0) exit(0);
                  if (strlen(v) == 1) {
                     if (*v == '0') s_osrs_h = 0x00;
                     else
                     if (*v == '1') s_osrs_h = 0x01;
                     else
                     if (*v == '2') s_osrs_h = 0x02;
                     else
                     if (*v == '4') s_osrs_h = 0x04;
                     else
                     if (*v == '8') s_osrs_h = 0x08;
                  }
                  if (strlen(v) == 2) if ( (v[0] == '1') & (v[1] == '6') ) s_osrs_h = 0x10;
                  if (s_osrs_h == 0xFF) printf("Humidity Sensor Sample Rate unchanged\n");
                  else {
                     printf("Humidity Sensor Sample Rate %s, s_osrs_h: 0x%02X\n", v, s_osrs_h);
                     setterInUse = 1;      
                  }
                  break;
         case 'p' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv); /* v is a pointer */
                  printf("Pressure Sensor Over Sampling Rate... [0, 1, 2, 4, 8, 16]\n");
                  if (checkargvOption(fp, v) != 0) exit(0);
                  if (strlen(v) == 1) {
                     if (*v == '0') s_osrs_p = 0x00;
                     else
                     if (*v == '1') s_osrs_p = 0x01;
                     else
                     if (*v == '2') s_osrs_p = 0x02;
                     else
                     if (*v == '4') s_osrs_p = 0x04;
                     else
                     if (*v == '8') s_osrs_p = 0x08;
                  }
                  if (strlen(v) == 2) if ( (v[0] == '1') & (v[1] == '6') ) s_osrs_p = 0x10;
                  if (s_osrs_p == 0xFF) printf("Pressure Sensor Sample Rate unchanged\n");
                  else {
                     printf("Pressure Sensor Sample Rate %s, s_osrs_p: 0x%02X\n", v, s_osrs_p);
                     setterInUse = 1;
                  }
                  break;
         case 't' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv); /* v is a pointer */
                  printf("Temperature Sensor Over Sampling Rate... [0, 1, 2, 4, 8, 16]\n");
                  if (checkargvOption(fp, v) != 0) exit(0);
                  if (strlen(v) == 1) {
                     if (*v == '0') s_osrs_t = 0x00;
                     else
                     if (*v == '1') s_osrs_t = 0x01;
                     else
                     if (*v == '2') s_osrs_t = 0x02;
                     else
                     if (*v == '4') s_osrs_t = 0x04;
                     else
                     if (*v == '8') s_osrs_t = 0x08;
                  }
                  if (strlen(v) == 2) if ( (v[0] == '1') & (v[1] == '6') ) s_osrs_t = 0x10;
                  if (s_osrs_t == 0xFF) printf("Temperature Sensor Sample Rate unchanged\n");
                  else {
                     printf("Temperature Sensor Sample Rate %s, s_osrs_t: 0x%02X\n", v, s_osrs_t);
                     setterInUse = 1;
                  }
                  break;
         case 'i' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv); /* v is a pointer */
                  printf("IIR Filter Setting... [0, 1, 2, 3, 7, 15, 31, 63, 127]\n");

                  if (strlen(v) == 1) {
                     if (*v == '0') s_filter = 0;
                     else
                     if (*v == '1') s_filter = 1;
                     else
                     if (*v == '3') s_filter = 3;
                     else
                     if (*v == '7') s_filter = 7;
                  }
                  if (strlen(v) == 2) if ( (v[0] == '1') & (v[1] == '5') ) s_filter = 15;
                  if (strlen(v) == 2) if ( (v[0] == '3') & (v[1] == '1') ) s_filter = 31;
                  if (strlen(v) == 2) if ( (v[0] == '6') & (v[1] == '3') ) s_filter = 63;
                  if (strlen(v) == 3) if ( (v[0] == '1') & (v[1] == '2') & (v[2] == '7') ) s_filter = 127;
                  if (s_filter == 0xFF) printf("IIR Filter Coefficient unchanged\n");
                  else {
                     printf("IIR Filter Coefficient %s, s_filter: %3d\n", v, s_filter);
                     setterInUse = 1;
                  }
                  break;
         case 'd' :
                  s_osrs_h = 0x08;
                  s_osrs_p = 0x08;
                  s_osrs_t = 0x08;
                  s_filter = 0x01;
                  s_l_time = 0x05; /* 1000mS */
                  s_active_nb_conv  = 0;
                  s_active_gas_wait_time = 0;
                  s_active_res_heat_temp = 0;
                  printf("s_osrs_h: 0x%02X, s_osrs_p: 0x%02X, s_osrs_t: 0x%02X, ",
                          s_osrs_h, s_osrs_p, s_osrs_t);
                  printf("s_filter: 0x%02X, s_l_time: 0x%02X.\n", s_filter, s_l_time);
                  printf("s_active_nb_conv: %d, s_active_gas_wait_time: %d, s_active_res_heat_temp: %d\n",
                          s_active_nb_conv, s_active_gas_wait_time, s_active_res_heat_temp);
                  setterInUse = 1;      
                  break;
         case 'D' :
                  s_osrs_h = 0x01;
                  s_osrs_p = 0x01;
                  s_osrs_t = 0x01;
                  s_filter = 0x01;
                  s_l_time = 0x00; /* 0.5mS */
                  s_active_nb_conv  = 0;
                  s_active_gas_wait_time = 100;
                  s_active_res_heat_temp = 100; /* Degrees C. */
                  printf("s_osrs_h: 0x%02X, s_osrs_p: 0x%02X, s_osrs_t: 0x%02X, ",
                          s_osrs_h, s_osrs_p, s_osrs_t);
                  printf("s_filter: 0x%02X, s_l_time: 0x%02X.\n", s_filter, s_l_time);
                  printf("s_active_nb_conv: %d, s_active_gas_wait_time: %d, s_active_res_heat_temp: %d\n",
                          s_active_nb_conv, s_active_gas_wait_time, s_active_res_heat_temp);
                  setterInUse = 1;      
                  break;
         case 'l' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv); /* v is a pointer */
                  s_l_time = 0xFF;
                  printf("runLoop Loop Time: %s...\n", v);
                  if (strcmp(v, "0.5")  == 0) s_l_time = 0x00; /* 0.5mS  */
                  if (strcmp(v, "62.5") == 0) s_l_time = 0x01; /* 62.5mS */
                  if (strcmp(v, "125")  == 0) s_l_time = 0x02; /* 125mS  */
                  if (strcmp(v, "250")  == 0) s_l_time = 0x03; /* 250mS  */
                  if (strcmp(v, "500")  == 0) s_l_time = 0x04; /* 500mS  */
                  if (strcmp(v, "1")    == 0) s_l_time = 0x05; /* 1s     */
                  if (strcmp(v, "1000") == 0) s_l_time = 0x05; /* 1s     */
                  if (strcmp(v, "10")   == 0) s_l_time = 0x06; /* 10s    */
                  if (strcmp(v, "20")   == 0) s_l_time = 0x07; /* 20s    */
                  if (s_l_time == 0xFF) printf("runLoop Loop Time unchanged.\n");
                  else {
                     g_l_time = s_l_time;
                     setterInUse = 1;      
                  }
                  _bme688DisplayLoopTime(fp);
                  break;

         default: break;
         }
      }
   }
   return setterInUse;      
}

int _doPrepareRunLoop(int __argc, char ** __argv) {
/* Replacements for argc and argv
 *
 * Use s = *++argv for convenience... if ( (*++argv)[0] == '-' )...
 */
   int _argc = __argc;
   int value;
   char * s;
   char ** pv;
   char * v;
   pv = __argv;
   s = *pv;
   value = 0;

   while (--_argc > 0) {
      s = *++pv;
/*    if ( (*++argv or av)[0] == '-' ) ... */
      if ( (s[0] == '-') & (strlen(s) > 1) ) {
/* If this is true then look at the next character in the pv (argv) string */
         s = pv[0] + 1;

         switch (*s) {

         case 'a' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv);
                  if (strlen(v) > 2) { printf("Adapter argument %s too long\n", v); exit(0); }
                  value = strtol(v, NULL, 10);
                  if ((value == 20) || (value == 21)) break;
                  if ((value > 0) & (value < 100)) g_Adapter = value;
                  break;
         case 'b' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv);
                  if ( (v[0] == '0') && ((v[1] == 'x') || (v[1] == 'X')) )
                     if ( (strtol(v, NULL, 0) >= 0) && (strtol(v, NULL, 0) < 256) )
                        g_SlaveAddress = (__u8)( (int)(strtol(v, NULL, 0)) ); 
                  break;
         case 'n' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv);
                  g_loop_counter = 1;
                  if (strlen(v) > 3) { printf("Count argument %s too long\n", v); g_loop_counter = 1; break; }
                  value = strtol(v, NULL, 10);
                  if (value == 0) g_loop_counter = 0;
                  if ((value > 1) & (value < 100)) g_loop_counter = value;
                  break;

         case 's' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv);
                  g_style = 1;
                  if (strlen(v) > 1) {
                     printf("Style argument %s too long... [0..%d]\n", v, STYLECOUNT - 1);
                     break;
                  }
                  value = strtol(v, NULL, 10);
                  g_style = ((value < 0) || (value > STYLECOUNT - 1)) ? 1 : value;
                  g_enable_g_style = 1;
                  break;

/* Place the fsmName recovery here in case other options have been set.
 * Can't place it into fsmStartFiniteStateMachineRunLoop as
 * argc/argv are not available.
 */
          case 'f' :
                  --_argc;
                  checkArgc(_argc);
                  v = (*++pv);
                  fsmName = concat("", v);
                  break;

          default: break;
         }
      }
   }
   return 0;
}

int bme688CommandLineArgumentsPart1(int __argc, char ** __argv) {
   int i = 0;
   for (i = 0; i < __argc; i++)  {
      if ((strcmp(__argv[i],"-d") == 0) ||
          (strcmp(__argv[i],"-D") == 0) ||
          (strcmp(__argv[i],"-l") == 0) ||
          (strcmp(__argv[i],"-h") == 0) ||
          (strcmp(__argv[i],"-gn") == 0) ||
          (strcmp(__argv[i],"-gt") == 0) ||
          (strcmp(__argv[i],"-gw") == 0) ||
          (strcmp(__argv[i],"-p") == 0) ||
          (strcmp(__argv[i],"-t") == 0) ||
          (strcmp(__argv[i],"-i") == 0)) setterInUse = 1;
      if (__argv[i][0] == '?') { bme688PrintHelp(); exit(0); }
      if (strcmp(__argv[i],"-?") == 0) { bme688PrintHelp(); exit(0); }
   }
   if (setterInUse) _doSetter(__argc, __argv);
   return setterInUse;
}

int bme688CommandLineArgumentsPart2(int __argc, char ** __argv) {
   int i = 0;
   runLoopInUse = 0;
   for (i = 0; i < __argc; i++)  {
      if ((strcmp(__argv[i],"-a") == 0) ||
          (strcmp(__argv[i],"-b") == 0) ||
          (strcmp(__argv[i],"-f") == 0) ||
          (strcmp(__argv[i],"-s") == 0) ||
          (strcmp(__argv[i],"-n") == 0)) runLoopInUse = 1;
      if  ((strcmp(__argv[i],"registers") == 0) ||
           (strcmp(__argv[i],"-v") == 0)) {
          fp = i2cPrepareDevice(g_Adapter, g_SlaveAddress);
          if (fp > 0) {
             _extractTheStaticRegisters(fp, g_SlaveAddress);
             _extractTheAltRegisters(fp, g_SlaveAddress);
             displayBME688Registers(fp);
             close(fp);
          } else printf("Slave not available.\n"); 
          exit(0);
      }
   }
   if (runLoopInUse) _doPrepareRunLoop(__argc, __argv);
   runLoopInUse = 1; /* Can occur when no argcs. */
   return 1; /* Returns runLoopInUse anyway. */
}

void __doTest(int t) {
   if (t == 2) __TEST_bme688GasMan_Prepare_Timer_Reg();
   if (t == 1) __i2cScanAdapters(0,3,1,0);
   return;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 * Preliminary Initialisation called by bme688Init
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688PreInit(int __argc, char ** __argv) {
   __u8 tmp = 0;
   int  i   = 0;

#ifndef USE_SIGACTION
/* Caught to allow reasonable cleanup attempt. */
   if (debug) printf("Using original signals...\n");
   signal(SIGINT,  intHandler); /* ^C */
   signal(SIGTSTP, intHandler); /* ^Z */
   signal(SIGTERM, intHandler); /* Terminate */
#endif

   setlocale(LC_ALL, "");

   _clearGlobalBuffers(); /* The current process whatever that may be. */

/* Connect to or CREATe Shared memory...
 * Need connection to Shared Memory to recover the initial osrs
 * Create defaults in Shared Memory if new.
 * The shared memory if already created may contain the current non zero values.
 * HOWEVER if a setter has been used, one or more g_osrs... entries will have been altered.
 * If the setter has been used, there will be new s_osrs parameters in LOCAL memory.
 * Transfer to Shared Memory.
 * At this stage, the bme688 configuration is unknown.
 * Look at the shared Memory bme688_osrs_InUse to determine if the osrs have been set.
 * If the device is NOT in use, set the GLOBAL oversampling defaults just in case.
 * Humidity    Oversampling RATE osrs_h set to x 8 or 0x04 (as per the bme688 document)
 * Temperature Oversampling RATE osrs_t set to x 8 or 0x04
 * Pressure    Oversampling RATE osrs_p set to x 8 or 0x04
 * Can't write to the device as it isn't ready at this stage.
 */
   ptr_common_shmMap = connectToSharedMemory(shmCName); /* Global pointer. */
   if (ptr_common_shmMap == NULL) {
      printf("Unable to connect to shared memory, exiting.\n");
      exit(0);  /* Just in case. */
   }
   ptr_common_aMap = &common_aMap; /* Global pointer. */

/* Recover the debug and global options only. Parameters later. */
/* Don't use toLower functions on argvs here as it destroys case sensitive information. */
   debug = 0; debug2 = 0; debug3 = 0; _display = 1; fsm = 0; reset = 0; g_enable_gas = 0;
   for (i = 0; i < __argc; i++) {
        if (strcmp(__argv[i],"debug") == 0) {
           if (debug2 == 1) debug3 = 1;
           if (debug  == 1) debug2 = 1;
           if (debug  == 0) debug  = 1;
        }
        if (strcmp(__argv[i],"scan") == 0) { __doTest(1); exit(0); } /* Convenient for testing. */
        if (strcmp(__argv[i],"test") == 0) { __doTest(2); exit(0); } /* Convenient for testing. */
        if (strcmp(__argv[i],"nodisplay") == 0) _display = 0;
        if (strcmp(__argv[i],"fsm") == 0) fsm = 1;
        if (strcmp(__argv[i],"-f")  == 0) fsm = 1; /* For convenience. */
        if (strcmp(__argv[i],"gas") == 0) g_enable_gas = 1;
        if (strcmp(__argv[i],"reset") == 0) reset = 1;
   }
   if (debug3) printf("DEBUG3 mode On\n");
      else if (debug2) printf("DEBUG2 mode On\n");
           else if (debug)  printf("DEBUG  mode On\n");

/* See later for other argvs... */

   tmp = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_InUse);
   if (tmp == 0x01) {
      if (debug) printf("bme688_osrs_InUse IS set\n");
      g_osrs_h = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_h);
      g_osrs_p = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_p);
      g_osrs_t = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_t);
      g_filter = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_filter);
      g_l_time = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_l_time);

      g_active_nb_conv  = (int)(readshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_nb_conv));
      g_active_gas_wait_time =
         (int)(readshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time));
      g_active_gas_wait_time_shared =
         (int)(readshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time_shared));
      g_active_res_heat_temp =
         (int)(readshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_res_heat_temp));
      g_active_field = 0;
      g_active_step  = 0;
      g_active_raw_gas        = 0; /* Has to be read. */
      g_active_raw_gas_range  = 0; /* Has to be read. */
   } else {
      if (debug) printf("bme688_osrs_InUse NOT set\n");
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_InUse, 0x01);
      g_osrs_h  = 0x08; /* osr_h = 0x04. */
      g_osrs_p  = 0x08; /* osr_p = 0x04. */
      g_osrs_t  = 0x08; /* osr_t = 0x04. */
      g_filter  = 0x01; /* Section 5.3.3.4 bme688 Datasheet IIR Filter Coefficient - 001 = 1 */
      g_l_time  = 0x05; /* runLoop Time 0b101 = 1000mS */
      g_active_nb_conv  = 0;
      g_active_field = 0;
      g_active_step  = 0;
      g_active_res_heat_temp  = 0;
      g_active_gas_wait_time  = 0;
/* --- */
/* Included for startup and testing convenience. May 30 2026. */
      g_active_res_heat_temp  = 100;
      g_active_gas_wait_time  = 190;
/* --- */
      g_active_gas_wait_time_shared  = 0;
      g_active_idac = 0;
      g_active_raw_gas        = 0; /* Has to be read. */
      g_active_raw_gas_range  = 0; /* Has to be read. */
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_h, g_osrs_h);
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_p, g_osrs_p);
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_t, g_osrs_t);
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_filter, g_filter);
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_l_time, g_l_time);
      writeshmU32( ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_nb_conv, 0x00);
      writeshmU32( ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time, g_active_gas_wait_time);
      writeshmU32( ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time_shared, 0x00);
      writeshmU32( ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_res_heat_temp, g_active_res_heat_temp);
      writeshmU16( ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_raw_gas, 0x00);
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_raw_gas_range, 0x00);
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_idac_heat_temp, 0x00);
   }
/* Just in case, check for errors. */
   switch(g_osrs_h) {
      case 0: case 1: case 2: case 4: case 8: case 16: break;
      default: g_osrs_h = 0x08; break;
   }
   switch(g_osrs_p) {
      case 0: case 1: case 2: case 4: case 8: case 16: break;
      default: g_osrs_p = 0x08; break;
   }
   switch(g_osrs_t) {
      case 0: case 1: case 2: case 4: case 8: case 16: break;
      default: g_osrs_t = 0x08; break;
   }
   switch(g_filter) {
      case 0: case 1: case 3: case 7: case 15: case 31: case 63: case 127: break;
      default: g_filter = 0x01; break;
   }

   setterInUse = 0; /* Global. */
   setterInUse = bme688CommandLineArgumentsPart1(__argc, __argv);

   if (setterInUse) {
      writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_InUse, 0x01); /* Permanent. */
      if (s_osrs_h != 0xFF) writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_h, s_osrs_h);
      if (s_osrs_p != 0xFF) writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_p, s_osrs_p);
      if (s_osrs_t != 0xFF) writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_t, s_osrs_t);
      if (s_filter != 0xFF) writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_filter, s_filter);
      if (s_l_time != 0xFF) writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_l_time, s_l_time);

      if (s_active_nb_conv != 0xFF)
         writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_nb_conv,  s_active_nb_conv);
      if (s_active_gas_wait_time != 0xFF)
         writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time, s_active_gas_wait_time);
      if (s_active_res_heat_temp != 0xFF)
         writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_res_heat_temp, s_active_res_heat_temp);
      if (debug) { /* Doesn't matter about the local process g_... values. */
         g_osrs_h = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_h);
         g_osrs_p = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_p);
         g_osrs_t = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_t);
         g_filter = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_filter);
         g_l_time = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_l_time);
         g_active_nb_conv = (int)(readshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_nb_conv));
         g_active_gas_wait_time =
            (int)(readshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time));
         g_active_res_heat_temp =
            (int)(readshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_res_heat_temp));

         displaySharedMemoryBank(0);

         printf("setterInUse: g_osrs_h: 0x%02X, g_osrs_p: 0x%02X, g_osrs_t: 0x%02X, g_filter: %d, g_loop_time: 0x%02X\n",
                 g_osrs_h, g_osrs_p, g_osrs_t, g_filter, g_l_time);
         printf("setterInUse: g_active_field: %d, g_active_nb_conv: %d, g_active_gas_wait_time: %d, g_active_res_heat_temp: %d\n",
                 g_active_field, g_active_nb_conv, g_active_gas_wait_time, g_active_res_heat_temp);
         printf("setterInUse completed.\n");
         }
      exit(0);
   }
   runLoopInUse = bme688CommandLineArgumentsPart2(__argc, __argv);

   if (debug3) {
      printf("DEBUG3: Initialisation...\n");
      printf("DEBUG3: osrs_h: 0x%02X, osrs_p: 0x%02X, osrs_t: 0x%02X, filter: %d, loop_time: 0x%02X\n",
              g_osrs_h, g_osrs_p, g_osrs_t, g_filter, g_l_time);
      printf("DEBUG3: active_field: %d, active_nb_conv: %d, active_gas_wait_time: %d, active_res_heat_temp: %d\n",
              g_active_field, g_active_nb_conv,  g_active_gas_wait_time, g_active_res_heat_temp);
      printf("DEBUG3: runLoopInUse: %s\n", (runLoopInUse) ? "ON" : "OFF");
      displaySharedMemoryBank(0);
   }

   return runLoopInUse;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Second stage of Initialisation, prepare the device
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688PostInit(int __argc, char ** __argv) {
   int _i2cResult = 0;

/* Shared Memory parameters already sorted... now look for a live device. */

   fp = i2cPrepareDevice(g_Adapter, g_SlaveAddress);
   if (fp < 0) {
      printf("Unable to find device 0x%02X on /dev/i2c-%d\n", g_SlaveAddress, g_Adapter);
      exit(0);
   }

   _i2cResult = bme688ReadChipID(fp, debug); /* Disable (0) debug. if fail, terminate. */
   _errorTerminate(fp, _i2cResult);

   setSensorMode(fp, OFF_MODE, 0); /* Wait 50uS built in */

   _i2cResult = setSensorOSRSRegisters(fp, 0); /* debug off */
   _errorTerminate(fp, _i2cResult);
   _i2cResult = _extractTheStaticRegisters(fp, g_SlaveAddress);
   _errorTerminate(fp, _i2cResult);
   _i2cResult = _extractTheAltRegisters(fp, g_SlaveAddress);
   _errorTerminate(fp, _i2cResult);

   common_aMap.shm.readLock         = 0;
   common_aMap.shm.writeLock        = 0;
   common_aMap.shm.terminateProcess = 0;
   writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.terminateProcess, 0x00);

   return fp;
}

int bme688Init(int __argc, char ** __argv) {
   setterInUse      = 0; /* Just in case. */
   runLoopInUse     = 0;
   g_active_field   = 0;
   g_active_step    = 0;
   g_active_nb_conv = 0;
   g_enable_gas     = 0;
   g_quit           = 0;

   setterInUse = bme688PreInit(__argc, __argv);
   fp = bme688PostInit(__argc, __argv);
   if (fp <= 0) exit(0);

   if (runLoopInUse != 1) { close(fp); exit(0); }
   uDelay(100); /* Settle time 100uS */

   return fp;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Configuration checking and setting Procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688Prepare_TPH_Operation(int fp, int _debug) {
   int __attribute__((unused)) _status = 0;
   __s32 i2cResult = 0;
   __u8 reg  = CTRL_HUM;
   __u8 v    = 0x00;
   __u8 s    = 0x00;

   if (debug3) printf("bme688Prepare_TPH_Operation begin...\n");

/* Humidity Register. */
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("bme688Prepare_TPH_Operation: read error: register: 0x%02X\n", reg);
   _errorTerminate(fp, i2cResult);
   v = (__u8)(i2cResult & 0x07);
   s = _prepareHumiditySensorOSR(g_osrs_h) & 0x07;
   if (v != s) printf("Humidity OSRS incorrect: 0x%02X, should be: 0x%02X.\n", s, v);

   reg = CTRL_MEAS;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("bme688Prepare_TPH_Operation: read error: register: 0x%02X\n", reg);
   _errorTerminate(fp, i2cResult);
/* osrs_t and osrs_p Register. Mask out mode. */
   v = (__u8)(i2cResult & 0xFC);
   s = (_prepareTemperatureSensorOSR(g_osrs_t) | _preparePressureSensorOSR(g_osrs_p)) & 0xFC;
   if (v != s) printf("Pressure or Temperature OSRS incorrect: 0x%02X, should be: 0x%02X.\n", s, v);

   reg = CTRL_CONFIG;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("bme688Prepare_TPH_Operation: read error: register: 0x%02X\n", reg);
   _errorTerminate(fp, i2cResult);

/* Filter Register. */
   v = (__u8)(i2cResult & 0x1C);
   s = _prepareFilter(g_filter) & 0x1C;
   if (v != s) printf("Filter OSRS incorrect: 0x%02X, should be: 0x%02X.\n", s, v);

   if (debug3) printf("bme688Prepare_TPH_Operation completed.\n");

   return 0; /* Ignore the return. */
}

int bme688Checkshm(int fp, int _debug) {
    int update = 0;
    __u8 p = 0x00;

    p = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_h);
    if (p != g_osrs_h)  { g_osrs_h = p; update = 1; }
    p  = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_p);
    if (p != g_osrs_p)  { g_osrs_p = p; update = 1; }
    p  = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_osrs_t);
    if (p != g_osrs_t)  { g_osrs_t = p; update = 1; }
    p = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_filter);
    if (p != g_filter)  { g_filter = p; update = 1; }
    p = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688_l_time);
    if (p != g_l_time) { g_l_time = p;  update = 1; }

    if (update) {
       setSensorMode(fp, OFF_MODE, 0); /* debug off. */
       setSensorOSRSRegisters(fp, 0) ;
       printf("bme688Checkshm: UPDATED.\n");
    }
    if (update && debug3) bme688DisplayOSRS(fp);

    return 0;
}

void bme688LoopDelay(__u8 l_time) {
   switch((int)(l_time)) {
      case 0 : uDelay(500);  break;
      case 1 : mDelay(60);   break;
      case 2 : mDelay(125);  break;
      case 3 : mDelay(250);  break;
      case 4 : mDelay(500);  break;
      case 5 : mDelay(1000); break; /* Wait 1000mS in FORCED_MODE, could use the g_sb_time. */
      case 6 : sleep(10);    break;
      case 7 : sleep(20);    break;
      default: mDelay(1000); break;
   }
   return;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Gas Management Procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __bme688GasMan_DisplayGasConfig(int fp) {
   __s32 i2cResult = 0;
   int   i    = 0;
   __u8  reg  = 0;
   __u8  res  = 0x00;
   __u8  _slaveAddress = g_SlaveAddress;
   __u8  _startAddress = 0x00;
   __u8  _length       = 0x00;
   __u8  d[BME688BUFFERLENGTH] = {0x00};
   __u8  v[BME688BUFFERLENGTH] = {0x00};

   setSensorMode(fp, OFF_MODE, 0);
   uDelay(10); /* Let it settle. */
   printf("Gas Configuration...\n");

/* Ignore the errors here, just give a notification rather than exiting. */

   _startAddress = 0x50;
   _length = 0x0A;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, d, v);
   if (i2cResult < 0) printf("Static Register Block Read Error: 0x%02X\n", _startAddress);
/* _errorTerminate(fp, i2cResult); */

   _startAddress = 0x5A;
   _length = 0x0A;
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, d, v);
   if (i2cResult < 0) printf("Static Register Block Read Error: 0x%02X\n", _startAddress);
/* _errorTerminate(fp, i2cResult); */

   _startAddress = 0x64;
   _length = 0x0B; /* Includes 0x6E */
   i2cResult = i2cBlockRead(fp, _slaveAddress, _startAddress, _length, d, v);
   if (i2cResult < 0) printf("Static Register Block Read Error: 0x%02X\n", _startAddress);
/* _errorTerminate(fp, i2cResult); */

   reg = 0x50;
   printf("idac_heat: ");
   for (i = 0; i < GASMAN_BUFFER_SIZE; i++, reg++) printf("id%d [0x%02X:0x%02X]%s", i, reg, d[reg],
       (i == 4) ? "\n           " : " ");
   printf("\n");

   reg = 0x5A;
   printf("res_heat : ");
   for (i = 0; i < GASMAN_BUFFER_SIZE; i++, reg++) printf("rh%d [0x%02X:0x%02X]%s", i, reg, d[reg],
       (i == 4) ? "\n           " : " ");
   printf("\n");

   reg = 0x64;
   printf("gas_wait : ");
   for (i = 0; i < GASMAN_BUFFER_SIZE; i++, reg++) printf("gw%d [0x%02X:0x%02X]%s", i, reg, d[reg],
       (i == 4) ? "\n           " : " ");
   printf("\n");

   reg = 0x6E;
/* Already recovered.
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);
   res = (__u8)(i2cResult);
*/
   printf("gas_wait_time_shared[0x%02X]  : 0x%02X\n", reg, d[reg]);

/**/ printf("\n");
   reg = CTRL_GAS_1;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);
/* _errorTerminate(fp, i2cResult); */


   res = (__u8)(i2cResult);
   printf("ctrl_gas_1[0x%02X]            : 0x%02X\n", reg, res);
   printf("run_gas[0x%02X:b5]            : 0x%02X\n", reg, res & 0x20); /* Bit 5    */
   printf("nb_conv[0x%02X:b3:0]          : 0x%02X\n", reg, res & 0x0F); /* Bits 3:0 */

   reg = CTRL_GAS_0;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("heat_off[0x%02X:b3]           : 0x%02X\n", reg, res & 0x08); /* Bit 3 */

/**/ printf("\n");
   reg = GAS_R_MSB_0;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_r_msb_0[0x%02X:b3]        : 0x%02X\n", reg, res & 0xFF);

   reg = GAS_R_LSB_0;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_r_lsb_0[0x%02X]           : 0x%02X\n", reg, res & 0xFF);
   printf("gas_valid_r_0[0x%02X:b5]      : 0x%02X\n", reg, res & 0x20); /* Bit 5 */
   printf("heat_stab_r_0[0x%02X:b6]      : 0x%02X\n", reg, res & 0x40); /* Bit 6 */
   printf("gas_range_r_0[0x%02X:b3:0]    : 0x%02X\n", reg, res & 0x0F); /* Bits 3:0 */

   reg = MEAS_STATUS_0;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_measuring_0[0x%02X]       : 0x%02X\n", reg, res & 0x40);
   printf("gas_meas_index_0[0x%02X:b3:0] : 0x%02X\n", reg, res & 0x0F); /* Bits 3:0 */

/**/ printf("\n");
   reg = GAS_R_MSB_1;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_r_msb_1[0x%02X:b3]        : 0x%02X\n", reg, res & 0xFF);
 
   reg = GAS_R_LSB_1;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_r_lsb_1[0x%02X]           : 0x%02X\n", reg, res & 0xFF);
   printf("gas_valid_r_1[0x%02X:b5]      : 0x%02X\n", reg, res & 0x20); /* Bit 5 */
   printf("heat_stab_r_1[0x%02X:b6]      : 0x%02X\n", reg, res & 0x40); /* Bit 6 */
   printf("gas_range_r_1[0x%02X:b3:0]    : 0x%02X\n", reg, res & 0x0F); /* Bits 3:0 */
 
   reg = MEAS_STATUS_1; 
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_measuring_1[0x%02X]       : 0x%02X\n", reg, res & 0x40);
   printf("gas_meas_index_1[0x%02X:b3:0] : 0x%02X\n", reg, res & 0x0F); /* Bits 3:0 */

/**/ printf("\n");
   reg = GAS_R_MSB_2;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_r_msb_2[0x%02X:b3]        : 0x%02X\n", reg, res & 0xFF);
 
   reg = GAS_R_LSB_2;
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_r_lsb_2[0x%02X]           : 0x%02X\n", reg, res & 0xFF);
   printf("gas_valid_r_2[0x%02X:b5]      : 0x%02X\n", reg, res & 0x20); /* Bit 5 */
   printf("heat_stab_r_2[0x%02X:b6]      : 0x%02X\n", reg, res & 0x40); /* Bit 6 */
   printf("gas_range_r_2[0x%02X:b3:0]    : 0x%02X\n", reg, res & 0x0F); /* Bits 3:0 */
 
   reg = MEAS_STATUS_2; 
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) printf("Static Register Read Error: Register: 0x%02X\n", reg);

   res = (__u8)(i2cResult);
   printf("gas_measuring_2[0x%02X]       : 0x%02X\n", reg, res & 0x40);
   printf("gas_meas_index_2[0x%02X:b3:0] : 0x%02X\n", reg, res & 0x0F); /* Bits 3:0 */

   return 0; /* Ignore the return for __bme688GasMan_DisplayGasConfig */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Gas Configuration Procedures.
 *
 * We are only interested in the singular Forced Mode at this stage.
 * It is a subset of the Parallel mode which is configured using
 * multiple chemically defined algorithms.
 * Refer to the later preparation stage.
 * Setup configuration defined on p.19 of the bme688 Feb24 Document.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * GasMan Zero all the Gas associated registers.
 * for convenience, only show failures to suppress debug noise.
 * Write directly rather than using the setter routines below.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __bme688GasMan_Zero_Gas_Registers(int fp, int _debug) {
   int i2cResult = 0;
   int _status   = 0;
   int i = 0;
   __u8 reg = CTRL_GAS_1;
   __u8 cfg = 0x00;
   if (debug3) printf("__bme688GasMan_Zero_Gas_Registers... resetting all gas registers to zero.\n");

   i2cResult = bme688GasMan_EnableRunGas(fp, 0);   /* Disable rungas and set heat off. Ignore the gas status return. */
   g_active_nb_conv = bme688GasMan_Set_nb_conv(fp, 0, 0); /* nb_conv to 0 and run_gas = 0. */

   cfg = 0x00;
   reg = IDAC_HEAT_0; /* 0x50 */
   for (i = 0; i < GASMAN_BUFFER_SIZE; i++) {
        i2cResult = i2c_smbus_write_byte_data(fp, reg, cfg);
        if (i2cResult < 0) {
           printf("Register Write Error: Register: 0x%02X\n", reg);
           _errorTerminate(fp, i2cResult);
        }
        if (debug3) printf("DEBUG3: IDAC_HEAT  registers[0x%02X] set to 0x00\n", reg);
        reg++;
   }

   reg = RES_HEAT_0; /* 0x5A */
   for (i = 0; i < GASMAN_BUFFER_SIZE; i++) {
        i2cResult = i2c_smbus_write_byte_data(fp, reg, cfg);
        if (i2cResult < 0) {
           printf("Register Write Error: Register: 0x%02X\n", reg);
           _errorTerminate(fp, i2cResult);
        }
        if (debug3) printf("DEBUG3: RES_HEAT   registers[0x%02X] set to 0x00\n", reg);
        reg++;
   } 

   reg = GAS_WAIT_0; /* 0x64 */
   for (i = 0; i < GASMAN_BUFFER_SIZE; i++) {
        i2cResult = i2c_smbus_write_byte_data(fp, reg, cfg);
        if (i2cResult < 0) {
           printf("Register Write Error: Register: 0x%02X\n", reg);
           _errorTerminate(fp, i2cResult);
        }
        if (debug3) printf("DEBUG3: GAS_WAIT   registers[0x%02X] set to 0x00\n", reg);
        reg++;
   }

   reg = GAS_WAIT_SHARED; /* 0x6E */
   i2cResult = i2c_smbus_write_byte_data(fp, reg, cfg);
   if (i2cResult < 0) {
      printf("Register Write Error: Register: 0x%02X\n", reg);
      _errorTerminate(fp, i2cResult);
   }
   if (debug3) printf("DEBUG3: GAS_WAIT_SHARED register [0x%02X] set to 0x00\n", GAS_WAIT_SHARED);

/* Extract and update the data buffers, do it this way to provide a short settlement timing delay. */
   _status  = _extractTheStaticRegisters(fp, g_SlaveAddress);
   _status |= _extractTheAltRegisters(fp, g_SlaveAddress);
   if (_debug) displayBME688Registers(fp);
   _errorTerminate(fp, _status);

   return 0; /* If there are no write errors, return 0. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Gas Heater Initial Heater Configuration.
 * Setting the idac or initial heater current is optional.
 * See p38 of 60. Range is 0..0x7F or 127.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __bme688GasMan_Set_idac(int fp, int _step, int _level, int _debug) {
   int i2cResult = 0;
   __u8 reg = IDAC_HEAT_0; /* 0x50 */
   __u8 cfg = 0x00;

   cfg  = ((_level > 0) && (_level < 128)) ? ((__u8)(_level) & 0x7F) : 0x00; /* 0b 0xxx xxxx */
   reg += ((_step > 0) && (_step < GASMAN_BUFFER_SIZE)) ? (__u8)(_step) : 0x00;

   i2cResult = i2c_smbus_write_byte_data(fp, reg, cfg);

   if (debug3) printf("DEBUG3: __bme688GasMan_Set_idac[0x%02X: 0x%02X] (initial_heater_current: %d mA), %s.\n",
                       reg, cfg, ((cfg + 1) >> 3), (i2cResult == 0) ? "success" : "fail");
   if (i2cResult < 0) {
      printf("Register Write Error: Register: 0x%02X\n", reg);
      _errorTerminate(fp, i2cResult);
   }

   return cfg; /* Returns the current idac setting. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * GasMan Prepare timer register.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
 *
 * Uses strange register format 0(b) mm:00 0000...
 * where mm is 00, 01, 10, 11 or 0, 4, 16, 64...
 * At this stage, the timer value could be 1024, 1025, 1026, 1027...
 * which cannot be represented in the 8 bit register.
 * We could use round(), ceil(), trunc() to rationalise the value to the nearest 64 step...
 * 64 * 1 = 64 or 64 * 2 = 128... but these can be represented using lower multipliers.
 * Thus if _timerStep is greater than 1023, say 1024, the _t / 64 = 16 (2**10 / 2**6 = 2**4).
 * Therefore _t will have values from 16 (0(b) (11):01 0000) (1024) (0xD0)...
 * to 63 (0(b) (11):11 1111) (4032 or GASMAN_MAX_WAIT_TIME)
 *
 * By using Integer Division, effectively TRUNCs.
 * But with the larger steps, use round to effectively split the the wide (4, 16 and 64) steps.
 * Change of policy... was going to use CEIL.
 * The smaller intervals (mentioned) up to 63mS for example are catered for directly.
 *
 * 0x32 == 50mS, 0x59 == 100mS (100..103), 0x5F == 125mS (124..127),
 * 0x72 == 200mS (200..203), 0x7E == 250mS (248..251)
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

__u8 __bme688GasMan_Prepare_Timer_Reg(int _timerStep) {
   __u8 _value = 0x00;
   if (_timerStep <= 0)   return 0x00; /* Possibly force step = 1 if the value is set to zero or -ve. */
   if (_timerStep > GASMAN_MAX_WAIT_TIME) return 0xFF; /* Greater than 4032 (0(b) (11):11 1111) steps where 11 = 64x63. */

/* 1mS ... 63mS...*/
   if  (_timerStep < 64) _value = ((__u8)(_timerStep)) & 0x3F; /* 0..63 = 64 steps. */

/* 64mS ... 252mS... 4 * 63 steps = 252mS  */
   if ((_timerStep > 63) && (_timerStep < 256)) _value = 0x40 | ((__u8)(_timerStep / 4));

/* 253mS ... 1008mS... 16 * 63 steps = 1008mS */
   if ((_timerStep > 255) && (_timerStep < 1024)) _value = 0x80 | ((__u8)(_timerStep / 16));

/* Between 1009ms and 4032mS max... 64 * 63 steps = 4032 */
   if  (_timerStep > 1023) _value = 0xC0 | ((__u8)(_timerStep / 64));

   return _value; /* Returns the actual timer register value. */
}

void __TEST_bme688GasMan_Prepare_Timer_Reg() {
/* Was used for testing... */
int step = 0;
   for (step = 0; step < 64; step++)
      printf("step: %4d, value: 0x%02X\n", step, __bme688GasMan_Prepare_Timer_Reg(step));
   for (step = 64; step < 256; step += 4)
      printf("step: %4d, value: 0x%02X\n", step, __bme688GasMan_Prepare_Timer_Reg(step));
   for (step = 256; step < 1024; step += 16)
      printf("step: %4d, value: 0x%02X\n", step, __bme688GasMan_Prepare_Timer_Reg(step));
   for (step = 1024; step < 4033; step += 64)
      printf("step: %4d, value: 0x%02X\n", step, __bme688GasMan_Prepare_Timer_Reg(step));
   return;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * GasMan set gas wait time SHARED.
 * Sets the SHARED wait time before each TPHG measurement.
 * The value is 0..4032 or 64 * 0..63 (4032)
 * gas_wait_time_shared in 0.477mS steps cf 1mS for gas_wait_time steps above.
 * So maximum delay of 4032 steps of 0.477mS/step or roughly 1923.27mS.
 *
 * Use time in mSecs.
 * This routine is used in both FORCED and PARALLEL modes, but
 * g_active_gas_wait_time_shared is set as a result and also returned.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __bme688GasMan_Set_gas_wait_time_shared(int fp, int mS, int _debug) {
   int  _i2cResult = 0;
   int _timerStep = 0;
   int  _mS = mS;
   __u8 reg = GAS_WAIT_SHARED; /* 0x6E */
   __u8 cfg = 0x00; /* 0b mm:nn nnnn */

   if (_mS < 0)    _mS = 0;
   if (_mS > 1920) _mS = 1920; /* Near enough. */

   _timerStep = ceil(((double)(_mS)) / 0.477); /* 0..4032 ~= 1920mS */
   if (_timerStep < 0) _timerStep = 0;
/* Check if greater than 4032 steps or ~= 1920mS. */
   if (_timerStep > GASMAN_MAX_WAIT_TIME) _timerStep = 4032; /* Register set to 0b 11 - 11 1111 or 64 * 63 */

   cfg = __bme688GasMan_Prepare_Timer_Reg(_timerStep);
   _i2cResult |= i2c_smbus_write_byte_data(fp, reg, cfg);
   _i2cResult |= writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time_shared, _mS);
   g_rawDataBuffer[reg] = cfg; g_dataIsValid[reg] = 0x01; /* Patch the internal data structure for convenience. */
   g_active_gas_wait_time_shared = _mS;
   if (_i2cResult < 0) printf("__bme688GasMan_Set_gas_wait_time_shared: Register write error\n");
   _errorTerminate(fp, _i2cResult);

   if (debug2) printf("DEBUG2: __bme688GasMan_Set_gas_wait_time_shared: time: %dmS, timerSteps: %d, cfg: 0x%02X, %s.\n",
               _mS, _timerStep, cfg,  (_i2cResult == 0) ? "success" : "fail");

   return g_active_gas_wait_time_shared; /* Duplicated but convenient. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * GasMan set gas wait time.
 * Sets the wait time in 1mS units before each TPHG measurement.
 * _step is the index in the PARALLEL Mode.
 * _step is 0 in FORCED Mode.
 *
 * This routine is used in both FORCED and PARALLEL modes.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __bme688GasMan_Set_gas_wait_time(int fp, int _step, int _mS, int _debug) {
   int i2cResult = 0;
   __u8 reg = GAS_WAIT_0; /* 0x64 */
   __u8 cfg = 0x00; /* 0b mm:nn nnnn */
   reg += ( ((_step > 0) && (_step < GASMAN_BUFFER_SIZE)) ? _step : 0 );

   g_active_gas_wait_time = (_mS > GASMAN_MAX_WAIT_TIME) ? GASMAN_MAX_WAIT_TIME : (_mS < 0) ? 0 : _mS; /* Constrain to 0..4032 mS */

   cfg = __bme688GasMan_Prepare_Timer_Reg(g_active_gas_wait_time);
   i2cResult  = i2c_smbus_write_byte_data(fp, reg, cfg);
   i2cResult |= writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time, g_active_gas_wait_time);
   g_rawDataBuffer[reg] = cfg; g_dataIsValid[reg] = 0x01; /* Update the common data structure for convenience. */

   if (debug3) printf("DEGUG3: __bme688GasMan_Set_gas_wait_time: step: %d, res: 0x%02X, Time: %d mS, %s.\n",
                      _step, cfg, g_active_gas_wait_time, (i2cResult == 0) ? "success" : "fail");
   _errorTerminate(fp, i2cResult);

   return g_active_gas_wait_time; /* Return the already set g_active_gas_wait_time, not the Timer_Register value. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Gas Heater Configuration Procedures.
 * This procedure does not need to be fast as the heating process
 * takes mS anyway. So reread the device for the current parameters.
 * Setting the idac initial heater current is optional.
 *
 * The return value is the temperature set.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __bme688GasMan_Set_heater_temperature(int fp, int _step, int _targetTemp, int _debug) {
   char * s = "__bme688GasMan_Set_heater_temperature";
   int i2cResult = 0;
   int heaterResistanceCorrectionFactor = 0; /* res_heat_value g_rawDataBuffer[0x00] */
   long par_g1 = 0;
   long par_g2 = 0;
   long par_g3 = 0;
   long var1 = 0;
   long var2 = 0;
   long var3 = 0;
   long var4 = 0;
   long var5 = 0;
   long r100 = 0;
   int _amb_temp = (g_temperature != 0) ? g_temperature : 15.0; /* ISO 15.0C */
/* int _amb_temp = 15.0; */ /* ISO 15.0C */
   int __targetTemp = 0;
   int __step = 0; 

   __u8 reg = RES_HEAT_0; /* reg 0x5A */
   __u8 res = 0x00;
   __u8 res_heater_range = 0x00;

   __step = ((_step > 0) && (_step < GASMAN_BUFFER_SIZE)) ? _step : 0;
   reg += __step;
   __targetTemp = ((_targetTemp > 0) && (_targetTemp < GASMAN_MAX_TEMPERATURE)) ? _targetTemp : 0;

   if (__targetTemp == 0) {
      res = 0x00;
      i2cResult  = i2c_smbus_write_byte_data(fp, reg, res); /* Reset to zero, remove previous value. */
      g_rawDataBuffer[reg] = 0x00; g_dataIsValid[reg] = 0x01; /* Update the common data structure for convenience. */
      g_active_res_heat_temp = 0;
      i2cResult |= writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_res_heat_temp, 0x00);

      if (debug3) printf("DEBUG3: %s: step: %d, reg: 0x%02X, res: %d, Temperature: %d, %s.\n",
                          s, __step, reg, res, 0, (i2cResult == 0) ? "success" : "fail");
      _errorTerminate(fp, i2cResult);
      return 0; /* Returns target temperature = 0 */
   }

/* All the STATIC Gas Calibration Parameters have been extracted. */
   par_g1 = (long)(convertToS16(0x00, g_rawDataBuffer[PAR_G1]));
   par_g2 = (long)(convertToS16(g_rawDataBuffer[PAR_G2_H], g_rawDataBuffer[PAR_G2_L]));
   par_g3 = (long)(convertToS16(0x00, g_rawDataBuffer[PAR_G3]));
   heaterResistanceCorrectionFactor = (int)(g_rawDataBuffer[RES_HEAT_VALUE_REG]); /* reg 0x00, Signed -128 +127 */
   res_heater_range = (__u8)(g_rawDataBuffer[RES_HEAT_RANGE_REG]); /* reg 0x02. */
   res_heater_range = (res_heater_range & 0x30) >> 4; /* Mask out bits 5:4, shift down 4 bits... */

/* Document error pg 27, 10 -> 1000. The scaling value cannot be 1000 as the result is 0. */
   var1 = (long)((_amb_temp * par_g3) / 10) << 8;
   var2 = (par_g1 + 784) * (((((par_g2 + 154009) * __targetTemp * 5) / 100) + 3276800) / 10);
   var3 = var1 + (var2 >> 1);
   var4 = (var3 / (res_heater_range + 4));
   var5 = (131 * heaterResistanceCorrectionFactor) + 65535;
   r100 = (((var4 / var5) - 250) * 34);
   res  = ((r100 + 50) / 100);

   if (debug3) {
      printf("DEBUG3: %s: res_heater_range: 0x%02X, ", s, res_heater_range);
      printf("heaterResistanceCorrectionFactor: 0x%02X\n", heaterResistanceCorrectionFactor);
      printf("var1: %016ld\n", var1);
      printf("var2: %016ld\n", var2);
      printf("var3: %016ld\n", var3);
      printf("var4: %016ld\n", var4);
      printf("var5: %016ld\n", var5);
      printf("r100: %ld\n",r100);
      printf("res:  0x%02X, delta: 0x%02X\n",res, res - g_prev);
   }

   reg = RES_HEAT_0 + __step; /* Reset reg, used above. reg 0x5A + __step */
   i2cResult = i2c_smbus_write_byte_data(fp, reg, res);
   g_rawDataBuffer[reg] = res; g_dataIsValid[reg] = 0x01; /* Patch the data buffer. */
   i2cResult |= writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_res_heat_temp, __targetTemp);
   if (i2cResult < 0) printf("%s: Write res_heat_temp error: 0x%02X: 0x%02X\n", s, reg, res);

   if (debug3) printf("DEBUG3: %s: step: %d, reg: 0x%02X, res: 0x%02X, targetTemp: %3d, delta: 0x%02X, %s.\n",
                       s, __step, reg, res, __targetTemp, res - g_prev, (i2cResult == 0) ? "success" : "fail");
   _errorTerminate(fp, i2cResult);
   g_prev = res;
   g_active_res_heat_temp = __targetTemp; /* Only if all is successful. */

   return __targetTemp; /* Returns the temperature set if required, rather than an error/success status. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * GasMan run the Gas task in Forced mode.
 * Before running gas mode, turn off the heat_off in CTRL_GAS_0.
 * No delay following just in case the idac heat has been used.
 *
 * There are unknown bits in this register and in theory they
 * shouldn't be altered... Undocumented.
 *
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688GasMan_EnableRunGas(int fp, int enable) {
   char * s = "bme688GasMan_EnableRunGas";
   int i2cResult = 0;
   __u8 _reg   = CTRL_GAS_0; /* CTRL_GAS_0 heat off */
   __u8 hmask  = 0xF7; /* Ignore all bits except the heat_off bit[3] 0b xxxx 0xxx */
   __u8 rmask  = 0xDF; /* Save the nb_conv values Bits[3:0]: 0b xx0x nnnn */
   __u8 runGas = 0x20; /* Enable run_gas Bit[5] = 1 */
   __u8 _res   = 0x00;

   _reg = CTRL_GAS_0; /* CTRL_GAS_0 heat off just in case */
   i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   if (i2cResult < 0) printf("%s: register[0x%02X]: CTRL_GAS_0: heat off: read error\n", s, _reg);
   _errorTerminate(fp, i2cResult);
   _res = (__u8)(i2cResult) & hmask; /* Ignore all bits except bit 3. Set bit 3 to 0. */
   i2cResult = i2c_smbus_write_byte_data(fp, _reg, _res); /* heat_off */
   if (i2cResult < 0) printf("%s: register[0x%02X:0x%02X]: heat off: write error\n", s, _reg, _res);
   _errorTerminate(fp, i2cResult);

   _reg = CTRL_GAS_1; /* CTRL_GAS_1 runGAS and nb_conv register */
   i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   if (i2cResult < 0) printf("%s: register[0x%02X]: CTRL_GAS_1: runGas: read error\n", s, _reg);
   _errorTerminate(fp, i2cResult);
   _res = ((__u8)(i2cResult)) & rmask; /* 0xDF == 0b xx0x xxxx nb_conv[3:0] */
   if (enable) _res = _res | runGas;
   i2cResult = i2c_smbus_write_byte_data(fp, _reg, _res);
   if (i2cResult < 0) printf("%s: register[0x%02X:0x%02X]: CTRL_GAS_1: runGas: write error\n", s, _reg, _res);
   _errorTerminate(fp, i2cResult);

   return (enable == 1) ? 1 : 0; /* Return enabled or disabled. */
}

int bme688GasMan_CheckRunGas(int fp) { 
   int i2cResult = 0;
   __u8 _reg = CTRL_GAS_1; /* CTRL run GAS and nb_conv register */
   __u8 mask = 0x20; /* Test run_gas Bit[5] = 1, return 1 or 0 rather than 0b 00N0 0000 */
   i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   if (i2cResult < 0) printf("bme688GasMan_CheckRunGas: register[0x%02X]: CTRL_GAS_1: read error\n", _reg);
   _errorTerminate(fp, i2cResult);
   return (((__u8)(i2cResult) & mask) != 0) ? 1 : 0;
}

int bme688GasMan_Set_nb_conv(int fp, int _nb_conv, int _debug) {
   char * s = "bme688GasMan_Set_nb_conv";
   int i2cResult = 0;
   __u8 _reg  = CTRL_GAS_1; /* CTRL run GAS and nb_conv register */
   __u8 _res  = 0;
   __u8 nmask = 0x0F; /* nb_conv Bits[3:0]... */
   __u8 rmask = 0xD0; /* Contains "Do Not Change" Bits. Mask out the nb_conv: 0b xx0x 0000 disables runGas. */
   __u8 _cfg  = ((_nb_conv < 0) || (_nb_conv > 15)) ? 0x00 : (__u8)(_nb_conv) & nmask;

   i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   if (i2cResult < 0) printf("%s: register[0x%02X]: CTRL_GAS_1: nb_conv: read error\n", s, _reg);
   _errorTerminate(fp, i2cResult);
   _res = ((__u8)(i2cResult)) & rmask; /* 0xD0 == 0b xx0x 0000, effectively zeros nb_conv[3:0] */

   _cfg = (_res | _cfg); /* Add in any nb_conv value. */

   i2cResult = i2c_smbus_write_byte_data(fp, _reg, _cfg);
   if (i2cResult < 0) printf("%s: register[0x%02x: 0x%02X]: CTRL_GAS_1: write error\n", s, _reg, _cfg);
   _errorTerminate(fp, i2cResult);

   if (debug3) printf("DEBUG3: %s: register[0x%02x: 0x%02X]: nb_conv: 0x%02X\n", s, _reg, _cfg, _cfg & 0x0F);

   return (int)(_cfg & 0x0F); /* Return _nb_conv so it can be used later. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * FORCED mode...
 *
 * There are 2 mode of operation for the gas measurement operations,
 * FORCED mode and PARALLEL mode using bme688 internal data structures
 * for control. These operations are fundamentally different and cater
 * for different types of repetitive assessments of pollutants.
 * As stated, the PARALLEL assessment procedures are user defined and
 * can vary infinitely.
 * From our point of view, the FORCED mode provides the definitive
 * functionality that we can use repeatedly.
 * Arguably, the Raspberry Pi mode of control potentially provides
 * better redirections of control in cases when gas assessments must
 * be altered "in flight". Of course, this is user dependent.
 *
 * No matter what, for the FORCED mode, we zero everything beforehand.
 * res_heat, idac_heat, gas_wait and nb_conv are all set to 0.
 *
 * Keep checks simple...
 * Rather than try to fix range errors, these shouldn't exist so,
 * check and fail where necessary on range errors.
 *
 * Only g_active_field = 0 is used in the forced mode. See p41.
 * g_active_step is defined by _nb_conv and can be updated later.
 *
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688GasMan_Prepare_Forced_Operation(int fp, int _res_heat_temp, int _gas_wait_time, int _debug) {
   char * s = "bme688GasMan_Prepare_Forced_Operation";
   int status = 0;

/* Fail on basic errors. Easier to do it this way. */
   if (fp <= 0) return -1; /* Should never occur. */

   if ((_res_heat_temp < 0) || (_res_heat_temp > GASMAN_MAX_TEMPERATURE)) {
      printf("%s: _res_heat_temp parameter error %d\n", s, _res_heat_temp);
      return -1;
   }
   if ((_gas_wait_time < 0) || (_gas_wait_time > GASMAN_MAX_WAIT_TIME)) {
      printf("%s: _gas_wait_time parameter error %d\n", s, _gas_wait_time);
      return -1;
   }

/* Zero the g_active_field and g_active_step.
 * __bme688GasMan_Zero_Gas_Registers may have already been used so duplicates possible.
 */
   g_active_step    = 0; /* Default uses index 0 rather than nb_conv at this stage. */
   g_active_field   = 0; /* See p41. */
   g_active_nb_conv = bme688GasMan_Set_nb_conv(fp, 0, 0); /* _debug off. */
   g_active_gas_wait_time_shared = 0;
   g_active_gas_wait_time_shared = __bme688GasMan_Set_gas_wait_time_shared(fp, g_active_gas_wait_time_shared, 0); /* _debug off. */
   status |= writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_nb_conv, g_active_nb_conv);
   status |= writeshmU32(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_gas_wait_time_shared, g_active_gas_wait_time_shared);
   _errorTerminate(fp, status);

/* Set the active registers in register locations nb_conv = 0, these may get changed later if nb_conv has been set.
 * _gas_wait_time and _res_heat_temp have been defined in the fsm file rather than the profile stage.
 */
   g_active_gas_wait_time = __bme688GasMan_Set_gas_wait_time(fp, g_active_field, _gas_wait_time, 0); /* _debug off. */
   g_active_res_heat_temp = __bme688GasMan_Set_heater_temperature(fp, g_active_field, _res_heat_temp, 0); /* _debug off. */

   return 0; /* Returns 0 for success. */
}

void bme688ClearActiveGasInfo(int fp) {
   g_active_sub_meas_index  = 0;
   g_active_gas_meas_index  = 0;
   g_active_gas_valid       = 0;
   g_active_gas_heat_stable = 0;
   g_active_raw_gas         = 0;
   g_active_raw_gas_range   = 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * The first of 2 WAIT routines, FORCED_MODE and PARALLEL_MODE
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int waitForNewDataReady(int fp, int _field, int enable_gas, int debug) {
   __s32 i2cResult = 0;
   __u8  status = 0x00;
   __u8  reg    = MEAS_STATUS_0;    /* register 0x1D */
   __u8  reg2   = GAS_R_LSB_0;      /* register 0x2D */
   __u8  reg3   = SUB_MEAS_INDEX_0; /* register 0x1E */
   int counter  = GASMAN_MAX_WAIT_TIME + 16; /* 50uS steps should be enough. See the first stage below. */

   int _debug      = (debug == 1) ? 1 : 0;
   int _enable_gas = (enable_gas == 1) ? 1 : 0;
   int gas_found   = 0;
   int data_found  = 0;

   if (g_mode == 0) return -1;
   if (fp <= 0) return -1; /* Basic error check. */
   if (_debug) printf("waitForNewData: ");

   switch(_field) { /* Any _field errors removed here. */
/* reg 0x1D, reg2 0x2D, reg3 0x1E */
      case 0 :
      default: break;
/* reg 0x2E, reg2 0x3E, reg3 0x2F */
      case 1 : reg = MEAS_STATUS_1; reg2 = GAS_R_LSB_1; reg3 = SUB_MEAS_INDEX_1; break;
/* reg 0x3F, reg2 0x4F, reg3 0x40 */
      case 2 : reg = MEAS_STATUS_2; reg2 = GAS_R_LSB_2; reg3 = SUB_MEAS_INDEX_2; break;
   }

/* First Stage: At this stage, the MODE has been set and...
 * there is a (50uS) time delay between that and this stage
 * Look for the TPH MEASURING status 0x20 first.
 * Could use a simple time delay here (say uDelay(100)) but due to the speed of the RPi,
 * better lock to the state of the device.
 *
 * i2c_smbus_read/write error checking here short circuits the operation.
 *
 * Using different negative return values for diagnostic reasons.
 */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Find MEASURING...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
   i2cResult = i2c_smbus_read_byte_data(fp, reg);
   if (i2cResult < 0) return -1; /* Return device not available, rather than terminate. */
   status = (__u8)(i2cResult);

/* Wait for an active MEASURING state... 0x20, should be active already. */
   counter = 256;
   while ( ((status & MEAS_STATUS_MEASURING_MASK) == 0) && (counter > 0) ) {
      counter--;
      uDelay(2); /* 2uS This is a faster sync. */
      i2cResult = i2c_smbus_read_byte_data(fp, reg);
      if (i2cResult < 0) return -1; /* Return device not available. */
      status = (__u8)(i2cResult);
   }
   if (counter < 1) return -3; /* Returns Sync error. */

/*
 * Should now have an active MEASURING state... 0x20
 * Now check against the ACTIVE measuring mask 0x20...
 * If gas is enabled, look for measuring GAS mask 0x40...
 * The NEW_DATA (0x80) or the MEASURING_GAS (0x40) status may become active here.
 * 4032 is the GASMAN_MAX_WAIT_TIME. Set the timeout counter to 4048 (4032 + 16).
 */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * MEASURING... wait for gas or data
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
   counter = GASMAN_MAX_WAIT_TIME + 16;
   while ( ((status & MEAS_STATUS_MEASURING_MASK) != 0) && (counter > 0) ) {
      counter--;
      if ((status & MEAS_STATUS_NEW_DATA_MASK) == MEAS_STATUS_NEW_DATA_MASK) break;
      if ((status & MEAS_STATUS_GAS_MEASURING_MASK) == MEAS_STATUS_GAS_MEASURING_MASK) break;
      if (_debug & (counter % 4 == 0)) { printf("."); fflush(stdout); } 
      mDelay(1); /* 1mS steps */
      i2cResult = i2c_smbus_read_byte_data(fp, reg);
      if (i2cResult < 0) return -1; /* Return device not available. */
      status = (__u8)(i2cResult);
   }
   if (counter < 1) return -4; /* Measuring Mask Sync error */
/* At this stage, there is an active gas measuring or active new data, look for active new data... */
   gas_found = ((status & MEAS_STATUS_GAS_MEASURING_MASK) == MEAS_STATUS_GAS_MEASURING_MASK) ? 1 : 0;

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * If gas enabled, wait for data
 * Clear down all associated (mainly gas) data buffers.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
   g_rawDataBuffer[reg] = 0; g_dataIsValid[reg] = 0;

   if (gas_found) {
      g_rawDataBuffer[reg2] = 0; g_dataIsValid[reg2] = 0;
      g_rawDataBuffer[reg3] = 0; g_dataIsValid[reg3] = 0;
      bme688ClearActiveGasInfo(fp);
   }
   while ( (_enable_gas) & ((status & MEAS_STATUS_GAS_MEASURING_MASK) != 0) & (counter > 0) ) {
      counter--;
      if ((status & MEAS_STATUS_NEW_DATA_MASK) == MEAS_STATUS_NEW_DATA_MASK) break;
      if (_debug) { printf("."); fflush(stdout); } 
      mDelay(1); /* 1mS steps */
      i2cResult = i2c_smbus_read_byte_data(fp, reg);
      if (i2cResult < 0) return -1; /* Return device not available. */
      status = (__u8)(i2cResult);
   }

   g_rawDataBuffer[reg] = status; g_dataIsValid[reg] = 1;
   if (_debug) printf(" %dmS\n", (GASMAN_MAX_WAIT_TIME + 16 - counter));
   data_found = ((status & MEAS_STATUS_NEW_DATA_MASK) == MEAS_STATUS_NEW_DATA_MASK) ? 1 : 0;
   if (data_found == 0) return -4;

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * MUST have valid data, not necessarily valid gas
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
/* At this stage we MUST have valid new data, save the status register so it can be broken up.
 * Save meas_status_[0/1/2]... new_data(7), gas_measuring(6), measuring(5) and gas_meas_index_x(3:0).
 */
   if (counter < 1) {
      if (_debug) printf("Measuring status, Data Not Ready, Time out, status: 0x%02X, should be 0x80...\n", status);
      return -4; /* Return Measuring Sync timeout error. */
   }

   if (_enable_gas && gas_found) {
      g_active_gas_meas_index = g_rawDataBuffer[reg] & MEAS_STATUS_GAS_MEAS_INDEX_MASK; /* 0x0F Lower 4 bits */

      i2cResult = i2c_smbus_read_byte_data(fp, reg2);
      if (i2cResult < 0) return -1; /* Return device not available, serious failure here. */
      g_rawDataBuffer[reg2] = (__u8)(i2cResult); g_dataIsValid[reg2] = 1;
      g_active_gas_valid       = ((g_rawDataBuffer[reg2] & GAS_R_LSB_GAS_VALID_MASK) == 0) ? 0 : 1; /* 0x20 */
      g_active_gas_heat_stable = ((g_rawDataBuffer[reg2] & GAS_R_LSB_HEAT_STAB_MASK) == 0) ? 0 : 1; /* 0x10 */

/* Save the sub_meas_index_x... whole byte. */
      i2cResult = i2c_smbus_read_byte_data(fp, reg3);
      if (i2cResult < 0) return -1; /* Return device not available, serious failure here. */
      g_rawDataBuffer[reg3] = (__u8)(i2cResult); g_dataIsValid[reg3] = 1;
      g_active_sub_meas_index = g_rawDataBuffer[reg3];

      if (_debug) {
         printf("GAS STATUS: (gas_r_lsb_%d): reg[0x%02X: 0x%02X], ", _field, reg2, g_rawDataBuffer[reg2]);
         printf("gas_valid[0x%02X: bit:0x20]: %s, ", reg2, (g_active_gas_valid) ? "TRUE" : "FALSE");
         printf("heat_stable[0x%02X: bit:0x10]: %s.\n", reg2, (g_active_gas_heat_stable) ? "TRUE" : "FALSE");
      }
   }
   return 0; /* Returns 0 on success. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Sensor Raw Data Recovery.
 * Only recover the TPH registers...
 * deal with the gas registers separately.
 * Also ignore the MEAS_STATUS_x and SUB_MEAS_INDEX_x here...
 * Recover those as part of the NEW_DATA mask recovery.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int readTPHSensorRegisters(int fp, int _field, int _enable_gas, int _debug) {
   int _i2cResult = 0;
   __u8 _length   = 0x08; /* 8 registers.... do gas registers separately. */
   __u8 startReg  = PRES_0_MSB; /* assumes field zero. */

   switch(_field) {
      case 0:
      default: break;
      case 1:  startReg = PRES_1_MSB; break;
      case 2:  startReg = PRES_2_MSB; break;
   }
   _i2cResult = i2cBlockRead(fp, g_SlaveAddress, startReg, _length, g_rawDataBuffer, g_dataIsValid);
   if (_i2cResult <  0) {
      printf("readTPHSensorRegisters: Sensor Register Block Read Error: %d\n", _i2cResult);
      return -1; /* Returns -1 rather than _errorTerminate(fp, _i2cResult) for a clean fail. */
   }

   return 0; /* Returns 0 on success. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Late addition: Calculate Dew Point... June 2026.
 * From DuckDuckGo Search...
 * The Magnus Formula for calculating (in C) the dew point temperature (Td) is:
 *
 * Td = (b * Alpha(T, RH)) / (a - Alpha(T, RH))
 * Where:
 *    Alpha(T, RH) = ln(RH/100) + (a * T) / (b + T)
 *    a = 17.625
 *    b = 243.04
 * It is better to handle the errors beforehand...
 * Error handling removed...
   errno = 0;
   if (errno == EDOM) {
      printf("Domain error: log of %f is undefined (EDOM).\n", _relative_humidity);
      return 0;
   }
   if (errno == ERANGE) {
      printf("Range error: log of %f is invalid (ERANGE).\n", _relative_humidity);
      return 0;
   }
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

double calculateDewPoint(int fp, double _temperature, double _relative_humidity) {
   double a = 17.625;
   double b = 243.04;
   double alpha;
/* Yes, I know I can join all these conditions together... More useable this way. */
   if (_relative_humidity < 0)     return 0; /* LN negative: errno: EDOM. */
   if (_relative_humidity == 0)    return 0; /* LN pole error: errno: ERANGE. */
   if (_relative_humidity < 0.01)  return 0; /* Not sensible: 0.01% _relative humidity. */
   if (_relative_humidity > 100.0) return _temperature; /* Error: value > 100% relative humidity. */
   if ((_temperature < -40) || (_temperature > 85)) return 0; /* bme688 (p3) temperature operating range. */

   alpha = log(_relative_humidity / 100.0) + ((a * _temperature) / (b + _temperature));
   return (b * alpha) / (a - alpha);
} 

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Two SHM lock routines.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int _lockSHM(int fp) {
   int _status = 0;
   __u8 rL = 0x00;
   __u8 * pwL = (__u8 *)( (long)(ptr_common_shmMap) + (long)(&common_aMap.shm.writeLock) - (long)(ptr_common_aMap));

/* Use a simple exercise rather than non critical locks or mutexes...
 * Shared Memory writing is atomic and fast anyway.
 */
   __atomic_clear(pwL, __ATOMIC_RELAXED); /* Blasts any set state, just in case. */
   _status = __atomic_test_and_set(pwL, __ATOMIC_RELAXED);  /* Set the write lock. The timing is not important. */
   if (_status != 0) {
      __atomic_clear(pwL, __ATOMIC_RELAXED);
      return 0;
   }

/* Given that only a few memory values are read, use short timeout delay. */
   rL = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.readLock);
   if (rL != 0) {
      uDelay(10); /* More than enough time for a memory read to complete. */
      rL = readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.readLock);
      if (rL != 0) {
         uDelay(10); /* Just an extra short delay. */
/* Blast the readLock just in case there has been a read process termination or lockup. Allows writing to shm. */
         _status = writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.readLock, 0x00);
      }
   }
/* Continue the write. */
   return 1;
}

int _clearSHMLock(int fp) {
   int _status = 0;
   __u8 * pwL = (__u8 *)( (long)(ptr_common_shmMap) + (long)(&common_aMap.shm.writeLock) - (long)(ptr_common_aMap));
   _status = writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.readLock, 0x00);
   __atomic_clear(pwL, __ATOMIC_RELAXED);
   return _status;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * TPH and Gas Data Processing Procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688ProcessData(int fp, int _field, int _enable_gas, int _debug) { /* 0 = no debug */
   double humidity    = 0;
   double pressure    = 0;
   double temperature = 0;
   int _status = 0;

   _status = readTPHSensorRegisters(fp, _field, _enable_gas, _debug); /* Includes wait state. */
   if (_status != 0 ) return -1;

   temperature = bme688RecoverTemperature(fp, _field, 0); /* debug == 0 */
   pressure    = bme688RecoverPressure(fp,    _field, 0);
   humidity    = bme688RecoverHumidity(fp,    _field, 0);

   common_aMap.shm.humidity = humidity / 1000.0;
   common_aMap.shm.pressure = pressure / 100.0;
   common_aMap.shm.temperature = temperature / 100.0;
   g_temperature = common_aMap.shm.temperature;
   if (_enable_gas) {
      g_active_raw_gas = (__u16)(bme688GasManRecoverGas(fp, _field, _debug));
      common_aMap.shm.bme688GasMan_active_raw_gas = g_active_raw_gas;
      common_aMap.shm.bme688GasMan_active_raw_gas_range = g_active_raw_gas_range;
   }

/* Convenient to update the shmMap here rather than in the ProcessData routines.
 * A simple check rather than complexity.
 * Non critical locks... not designed to be safe, just timing convenience with external processes.
 * Mutexes unused. Memory writing will be very fast anyway.
 */
   if (_lockSHM(fp)) {
      _status  = writeshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.temperature, common_aMap.shm.temperature);
      _status |= writeshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.pressure, common_aMap.shm.pressure);
      _status |= writeshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.humidity, common_aMap.shm.humidity);
      if (_enable_gas) {
         _status |= writeshmU16(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_raw_gas, g_active_raw_gas);
         _status |= writeshmByte(ptr_common_shmMap, ptr_common_aMap,
                    &common_aMap.shm.bme688GasMan_active_raw_gas_range, (__u8)(g_active_raw_gas_range));
      } 
      if (_status < 0) printf("bme688ProcessData: Shared Memory Write Error: %d\n", _status);
      _clearSHMLock(fp);
   }
   return 0;
}  

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE: Start of the Parallel Mode reading procedures...
 *
 * There are 2 mode of operation for the gas measurement operations,
 * FORCED mode and PARALLEL mode using bme688 internal data structures
 * for control. These operations are fundamentally different and cater
 * for different types of repetitive assessments of pollutants.
 * As stated, the PARALLEL assessment procedures are user defined and
 * can vary infinitely.
 * No matter what, for the PARALLEL mode, we zero everything beforehand.
 * res_heat, idac_heat, gas_wait_shared, gas_wait_index and nb_conv
 * are all set to 0.
 *
 * Rather than try to fix range errors, these shouldn't exist so,
 * check and fail where necessary on range errors.
 *
 * FSM Profiles...
 * If PARALLEL_MODE, then tidy any initial profile sequence [index=0] alterations.
 * The remainder of the profile values are not altered.
 * PARALLEL_MODE: gas_wait_time checks, these are counters between 0..255.
 * PARALLEL_MODE: nb_conv is simply interpreted as a step multipler from 0..9.
 * The display g_style has been set from s->style.
 *
 * In FORCED_MODE, the speed of operation can be controlled by the host.
 * In PARALLEL_MODE, the device proceeds continuously...
 * and the (much faster) RPi has to synchronise and wait for the device state changes.
 * Alas, no i2c interrupt in this case. SPI interface can initiate interrupts. Pain in the neck.
 *
 * PARALLEL_MODE once started does NOT return to OFF_MODE (p17, Table 12).
 * The bme688 must be interrupted and reconfigured.
 *
 * The bme688ClearDown procedure clears all gas associated registers
 * and reconfigures the bme688 for (default non active) FORCED_MODE.
 * ClearDown is primarily aimed at stopping and removing any active
 * PARALLEL_MODE configuration.
 *
 * Reset register [0xE0:0xB6] (p36, Section 5.3.1.5) doesn't reset the configuration.
 *
 * DETECTING and MEASURING - MEAS_STATUS_MEASURING_MASK: 0x20...
 * The MODE has been set and written and there been a 50uS time delay between
 * that and reading the TPHG MEASURING status. Look for measuring status 0x20 first.
 *
 * i2c_smbus_read/write error checking here short circuits the operation.
 *
 * Field 1: reg 0x1D, reg2 0x2D, reg3 0x1E
 * Field 2: reg 0x2E, reg2 0x3E, reg3 0x2F
 * Field 3: reg 0x3F, reg2 0x4F, reg3 0x40
 *
 * Status and measuring masks...
 * New Data status... 0x80 or 0xA0...
 * MEAS_STATUS_MEASURING_MASK:     0x20
 * MEAS_STATUS_GAS_MEASURING_MASK: 0x40
 * MEAS_STATUS_NEW_DATA_MASK:      0x80
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE: Detect the Measuring status and wait until measuring is ON - mask 0x20...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmDetectMeasuringON(int fp, int _field, int _debug) {
   int  _i2cResult = 0;
   int  _counter   = GASMAN_MAX_WAIT_TIME + 16; /* 4032 is the GASMAN_MAX_WAIT_TIME. Use 4048. */
   __u8 _status    = 0x00;
   __u8 _reg       = 0x1D; /* MEAS_STATUS_0 */

   switch(_field) {
      default:
      case 0 : break;
      case 1 : _reg = MEAS_STATUS_1; break;
      case 2 : _reg = MEAS_STATUS_2; break;
   }

/* After setSensorMode, there is 50uS delay. */
   _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   _errorTerminate(fp, _i2cResult); /* Total failure error. */
   _status = (__u8)(_i2cResult); /* Should contain at least 0x20... */

/* MEAS_STATUS_MEASURING_MASK: 0x20. Will probably be immediate but check anyway.*/
   while ( ((_status & MEAS_STATUS_MEASURING_MASK) == 0) && (_counter > 0) ) {
      _counter--;
      uDelay(250); /* counter * 250uS delays should be enough. */
      _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
      _errorTerminate(fp, _i2cResult); /* Total failure error. */
      _status = (__u8)(_i2cResult);
   }
   g_rawDataBuffer[_reg] = _status; g_dataIsValid[_reg] = 1; /* Save so it can be broken up. */
   if (_counter < 1) {
      if (_debug) printf("detectMeasuringON  error, status: 0x%02X, should be 0x20 or 0xA0...\n", _status);
      return -3; /* Returns -3 indicating a detectMeasuringON error. */
   }
   if (_debug) printf("detectMeasuringON status: 0x%02X\n", _status & MEAS_STATUS_MEASURING_MASK);
   return (int)(_status) & MEAS_STATUS_MEASURING_MASK; /* Returns at least 0x20 indicating that measuring is ON. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE: Detect the GAS Measuring status and wait until ON - mask 0x40...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmDetectGasMeasuringON(int fp, int _field, int _debug) {
   int  _i2cResult = 0;
   int  _counter   = GASMAN_MAX_WAIT_TIME + 16; /* 4032 is the GASMAN_MAX_WAIT_TIME. Use 4048. */
   __u8 _status    = 0x00;
   __u8 _reg       = 0x1D; /* MEAS_STATUS_0 */

   switch(_field) {
      default:
      case 0 : break;
      case 1 : _reg = MEAS_STATUS_1; break;
      case 2 : _reg = MEAS_STATUS_2; break;
   }

/* Get the first status reading, hopefully GAS measuring has started. */
   _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   _errorTerminate(fp, _i2cResult);
   _status = (__u8)(_i2cResult);

   if (_debug) printf("pmDetectGasMeasuringON ");
   while ( ((_status & MEAS_STATUS_MEASURING_MASK) != 0) & (_counter > 0) ) {
      _counter--;
      if ((_status & MEAS_STATUS_GAS_MEASURING_MASK) == MEAS_STATUS_GAS_MEASURING_MASK) break;
      if (_debug & (_counter % 10 == 0)) { printf("."); fflush(stdout); }
      uDelay(250); /* 250uS wait time... */ 
      _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
      _errorTerminate(fp, _i2cResult);
      _status = (__u8)(_i2cResult);
   }
   g_rawDataBuffer[_reg] = _status; g_dataIsValid[_reg] = 1;
   if (_debug) printf(" %dmS\n", ((GASMAN_MAX_WAIT_TIME + 15 - _counter) / 4)); /* mS */
   if (_counter < 1) {
      printf("pmDetectGASMeasuringON error, status: 0x%02X, should be 0x40...\n", _status);
      return -4; /* detectGASMeasuring error */
   }
   return (int)(_status & MEAS_STATUS_GAS_MEASURING_MASK);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE: Detect the GAS Measuring status and wait until OFF or LOW - mask 0x40...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmDetectGasMeasuringOFF(int fp, int _field, int _debug) {
   int  _i2cResult = 0;
   int  _counter   = GASMAN_MAX_WAIT_TIME + 16; /* 4032 is the GASMAN_MAX_WAIT_TIME. Use 4048. */
   __u8 _status    = 0x00;
   __u8 _reg       = 0x1D; /* MEAS_STATUS_0 */

   switch(_field) {
      default:
      case 0 : break;
      case 1 : _reg = MEAS_STATUS_1; break;
      case 2 : _reg = MEAS_STATUS_2; break;
   }

   _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   _errorTerminate(fp, _i2cResult);
   _status = (__u8)(_i2cResult);

   if (_debug) printf("pmDetectGasMeasuringOFF");
   while ( ((_status & MEAS_STATUS_MEASURING_MASK) != 0) & (_counter > 0) ) {
      _counter--;
      if ((_status & MEAS_STATUS_GAS_MEASURING_MASK) == 0x00) break;
      if (_debug & (_counter % 10 == 0)) { printf("."); fflush(stdout); }
      uDelay(250); /* 250uS wait time... */
      _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
      _errorTerminate(fp, _i2cResult);
      _status = (__u8)(_i2cResult);
   }
   g_rawDataBuffer[_reg] = _status; g_dataIsValid[_reg] = 1; /* Save again... */
   if (_debug) printf(" %dmS\n", ((GASMAN_MAX_WAIT_TIME + 15 - _counter) / 4)); /* mS */
   if (_counter < 1) {
      printf("pmDetectGASMeasuringOFF error, status: 0x%02X, should be 0x40...\n", _status);
      return -4; /* detectGASMeasuring error */
   }
   return (int)(_status & MEAS_STATUS_GAS_MEASURING_MASK);
}  

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE: Detect New Data status and wait for New Data AVAILABLE...
 *
 * Detect New Data status... 0x80 or 0xA0...
 * detectNewDataAvailable...
 * MEAS_STATUS_MEASURING_MASK:     0x20
 * MEAS_STATUS_GAS_MEASURING_MASK: 0x40 
 * MEAS_STATUS_NEW_DATA_MASK:      0x80
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmDetectNewDataAvailable(int fp, int _field, int _debug) {
   int  _i2cResult = 0;
   int  _counter   = GASMAN_MAX_WAIT_TIME + 16; /* 4032 is the GASMAN_MAX_WAIT_TIME. Use 4048. */
   __u8 _status    = 0x00;
   __u8 _reg  = 0x1D;             /* MEAS_STATUS_0 */
   __u8 _reg2 = GAS_R_LSB_0;      /* register 0x2D */
   __u8 _reg3 = SUB_MEAS_INDEX_0; /* register 0x1E */

   switch(_field) {
      default:
      case 0 : break;
      case 1 : _reg = MEAS_STATUS_1; _reg2 = GAS_R_LSB_1; _reg3 = SUB_MEAS_INDEX_1; break;
      case 2 : _reg = MEAS_STATUS_2; _reg2 = GAS_R_LSB_2; _reg3 = SUB_MEAS_INDEX_2; break;
   }

   _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
   _errorTerminate(fp, _i2cResult);
   _status = (__u8)(_i2cResult);

   if (_debug) printf("pmDetectNewDataAvailable");
   while ( ((_status & MEAS_STATUS_MEASURING_MASK) != 0) & (_counter > 0) ) {
      _counter--;
      if ((_status & MEAS_STATUS_NEW_DATA_MASK) == MEAS_STATUS_NEW_DATA_MASK) break;
      if (_debug) { printf("."); fflush(stdout); }
      uDelay(100); /* 100uS wait time... */
      _i2cResult = i2c_smbus_read_byte_data(fp, _reg);
      _errorTerminate(fp, _i2cResult);
      _status = (__u8)(_i2cResult);
   }
   if (_counter < 1) {
      printf("pmDetectNewDataAvailable error, data not ready, status: 0x%02X, should be 0x80...\n", _status);
      return -4; /* detectNewData error. */
   }

   g_rawDataBuffer[_reg] = _status; g_dataIsValid[_reg] = 1;
   if (_debug) printf(" status: 0x%02X: 0x%02X\n", _status, _status & MEAS_STATUS_NEW_DATA_MASK);
   g_active_gas_meas_index  = g_rawDataBuffer[_reg] & 0x0F;

/* Now save REGISTER gas_r_lsb_0/1/2... gas_lsb(7:6), gas_valid_r(5), heat_stab_r(4) and gas_range_r(3:0) */
   _i2cResult = i2c_smbus_read_byte_data(fp, _reg2);
   _errorTerminate(fp, _i2cResult);
   g_rawDataBuffer[_reg2]   = (__u8)(_i2cResult); g_dataIsValid[_reg2] = 1;
   g_active_gas_valid       = ((g_rawDataBuffer[_reg2] & 0x20) == 0) ? 0 : 1; /* 0x20 */
   g_active_gas_heat_stable = ((g_rawDataBuffer[_reg2] & 0x10) == 0) ? 0 : 1; /* 0x10 */
   g_active_raw_gas_range   =   g_rawDataBuffer[_reg2] & 0x0F;                /* [3..0] */

/* Save the sub_meas_index_x... whole byte. */
   _i2cResult = i2c_smbus_read_byte_data(fp, _reg3);
   _errorTerminate(fp, _i2cResult);
   g_rawDataBuffer[_reg3]  = (__u8)(_i2cResult); g_dataIsValid[_reg3] = 1;
   g_active_sub_meas_index = g_rawDataBuffer[_reg3];

   return (int)(_status & MEAS_STATUS_NEW_DATA_MASK);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE: Prepare...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmPrepare(int fp, struct state_t * s, int _debug) {
   int i = 0;
   int _status = 0;

/* Load the entire structure regardless of probable zero values. */
   for (i = 0; i < GASMAN_BUFFER_SIZE; i++) { /* Rather than s->nb_conv... */
       _status |= __bme688GasMan_Set_gas_wait_time(fp,      i, s->p_gas_wait_time[i], 0); /* _debug = 0. */
       _status |= __bme688GasMan_Set_heater_temperature(fp, i, s->p_res_heat_temp[i], 0);
       _status |= __bme688GasMan_Set_idac(fp,               i, s->p_idac[i], 0);
   }
   g_active_step  = 0;
   g_active_field = 0; /* Always start with _field == 0. */
   g_active_gas_wait_time_shared = __bme688GasMan_Set_gas_wait_time_shared(fp, s->gas_wait_time_shared, _debug);
   g_active_nb_conv = bme688GasMan_Set_nb_conv(fp, 0, 0);
   if ((s->nb_conv > 0) && (s->nb_conv < 0x10)) g_active_nb_conv = bme688GasMan_Set_nb_conv(fp, s->nb_conv, _debug);
   g_active_idac = __bme688GasMan_Set_idac(fp, g_active_step, 0, 0);

/* Set initial g_active_res_heat_temp and g_active_gas_wait_time after initialising the structure above... */
   g_active_res_heat_temp = __bme688GasMan_Set_heater_temperature(fp, g_active_step, s->p_res_heat_temp[0], 0);
   g_active_gas_wait_time = __bme688GasMan_Set_gas_wait_time(fp, g_active_step, s->p_gas_wait_time[0], 0);

   if (debug3) {
      printf("DEBUG3: PARALLEL_MODE: preparation complete: g_active_nb_conv: %d\n", g_active_nb_conv);
      printf("TEMP: ");
      for (i = 0; i < GASMAN_BUFFER_SIZE; i++) printf("[%d: %3d] ", i, s->p_res_heat_temp[i]);
      printf("\n");
      printf("WAIT: ");
      for (i = 0; i < GASMAN_BUFFER_SIZE; i++) printf("[%d: %3d] ", i, s->p_gas_wait_time[i]);
      printf("\n\n");
      __bme688GasMan_DisplayGasConfig(fp);
      printf("\n");
   }
   return _status;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE: Main Parallel Mode starting procedure...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmBeginParallelProcess(int fp, struct state_t * s, int _enable_gas, int _debug) {
   int _status = 0;
   int _gas = 1;

   if (_enable_gas != 1) return -1;
   if (_debug) printf("\nDEBUG: *** pmBeginParallelProcess...\n");

   _status = pmPrepare(fp, s, 0); /* _debug == 0. */
   if (_gas) {
      _status = bme688GasMan_EnableRunGas(fp, _gas); /* enable == 1, disable == 0 */
      _gas = bme688GasMan_CheckRunGas(fp); /* Check for OFF. */
   }
   _status = pmProcessParallelLoop(fp, s, _gas, _debug);

/* bme688 page 17... the bme688 does NOT return to OFF_MODE after PARALLEL_MODE.
 * Must DRIVE it. Reset doesn't.
 * setSensorMode(fp, OFF_MODE, 0) and reset gas registers.
 * In this case, the default FORCED_MODE configuration is applied but not activated.
 * See bme688ClearDown(fp).
 */
   _status = bme688ClearDown(fp);
   if ((_status != 0) && _debug) printf("DEBUG: Clear Down error: %d\n", _status);

#ifndef USE_SIGACTION
   signal(SIGALRM, intHandler);
#endif

   g_enable_gas = 0; /* Set for signal ALARM timeout */
   _gas = 0;
   g_active_alarm = s->gas_cycle_time;
   alarm(g_active_alarm); /* the timeOutHandler signal is actived. */
   if (_debug) printf("*** gas alarm handler active: %d Seconds...\n", g_active_alarm);

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
* PARALLEL_MODE: Recover Data...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmProcessParallelData(int fp, struct state_t * s, int _field, int _enable_gas, int _saveIteration, int _debug) {
   double humidity    = 0;
   double pressure    = 0;
   double temperature = 0;
   int _status = 0;

   _status = readTPHSensorRegisters(fp, _field, _enable_gas, _debug); /* Includes wait state. */
   if (_status != 0 ) return -1;

   temperature = bme688RecoverTemperature(fp, _field, 0); /* debug == 0 */
   pressure    = bme688RecoverPressure(fp,    _field, 0);
   humidity    = bme688RecoverHumidity(fp,    _field, 0);

   if (_saveIteration) {
      common_aMap.shm.humidity = humidity / 1000.0;
      common_aMap.shm.pressure = pressure / 100.0;
      common_aMap.shm.temperature = temperature / 100.0;
      g_temperature = common_aMap.shm.temperature;
      if (_lockSHM(fp)) {
         _status  = writeshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.temperature, common_aMap.shm.temperature);
         _status |= writeshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.pressure, common_aMap.shm.pressure);
         _status |= writeshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.humidity, common_aMap.shm.humidity);
         _clearSHMLock(fp);
      }
   }

   if (g_active_gas_valid) {
      g_active_raw_gas = bme688GasManRecoverGas(fp, _field, _debug);
      common_aMap.shm.bme688GasMan_active_raw_gas = g_active_raw_gas;
      common_aMap.shm.bme688GasMan_active_raw_gas_range = g_active_raw_gas_range;
      if (_lockSHM(fp)) {
         _status |= writeshmU16(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_raw_gas, g_active_raw_gas);
         _status |= writeshmByte(ptr_common_shmMap, ptr_common_aMap,
                    &common_aMap.shm.bme688GasMan_active_raw_gas_range, (__u8)(g_active_raw_gas_range));
         _clearSHMLock(fp);
      }
   }
   if (_status < 0) printf("pmProcessParallelData: Shared Memory Write Error: %d\n", _status);

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * PARALLEL_MODE Main Loop procedures...
 * Strictly speaking Parallel_MODE does not finish but...
 * this procedure does a single loop of g_active_nb_conv steps.
 * The calling handler pmBeginParallelProcess terminates the mode.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int pmProcessParallelLoop(int fp, struct state_t * s, int _enable_gas, int _debug) {
   char * t = "pmProcessParallelLoop";
   int i = 0;
   int next_field  = 1;
   int loop_count  = 0;
   __u8 _status    = 0;
   int gv = 0;
   int _includeTPH = 1;

   if ((fp <= 0) || (s == NULL) || (_enable_gas == 0)) return 0; /* Basic checks. */

   g_active_field = 0;

   setSensorMode(fp, PARALLEL_MODE, _debug); /* The device must be in measuring mode... */ 
   _status = pmDetectMeasuringON(fp, g_active_field, _debug);
   if (_status < 0) return -1;
   _status = pmDetectGasMeasuringON(fp, g_active_field, _debug); /* Must be in GAS measuring mode... */ 
   if (_status < 0) return -1;
/* Gas measuring mode must be ON... */ 

/* g_active_nb_conv has already been set. This is the length of the loop. */
   loop_count = g_active_nb_conv;

   while (i < loop_count) {

      if (_debug) printf("\n...\n");
      _status = pmDetectGasMeasuringOFF(fp, next_field, _debug);
      if (_status < 0) return -1;
      _status = pmDetectGasMeasuringON(fp,  next_field, _debug);
      if (_status < 0) return -1;
      _status = pmDetectNewDataAvailable(fp, g_active_field, _debug); /* Sets g_active_gas_valid. */
      if (_status < 0) return -1;
      gv = (g_active_gas_valid) ? 1 : 0; /* Use temporary version of g_active_gas_valid. Can change. */
 
      _status = pmProcessParallelData(fp, s, g_active_field, gv, _includeTPH, _debug); /* If _includeTPH, Save this iteration. */
/*
      if (_debug) {
          printf("%s: Field: %d", t, g_active_field);
          printf(", Initial heater current idac_heat: %dmA", g_active_idac);
          printf(", Heater temperature: %3d", g_active_res_heat_temp);
          printf(", gas_wait_time_shared: %3dmS", g_active_gas_wait_time_shared);
          printf("\n");
      }
*/
      if (_debug) {
          printf("%s: g_active_gas_meas_index: %2d, g_active_sub_meas_index: %2d", t, g_active_gas_meas_index, g_active_sub_meas_index);
          printf(", VALID GAS STATUS: %s\n", (gv) ? "TRUE" : "FALSE");
      }
      if (_display) _status = bme688DisplayProcessData(fp, s, g_style, _includeTPH, gv, _debug);

      g_active_field++;
      if (g_active_field > 2) g_active_field = 0;
      next_field = g_active_field + 1;
      if (next_field > 2) next_field = 0;
      _includeTPH = 0; /* Use first TPH information for the loop. */
      i++;
   }
   g_active_field = 0;

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * End of PARALLEL_MODE procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Display...
 *
 * Style is recovered from the fsm config struct state->style in finite state mode.
 * Use -s [0..6] otherwise.
 *
 * Originally use 2 booleans to specify the types of outputs...
 *
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688DisplayProcessData(int fp, struct state_t * s, int _style, int _include_TPH, int _include_gas, int _debug) {
   int __attribute__((unused)) _status = 0;
   int _ohms = 0;
   if (_include_gas) _ohms = _convertGasOhms(g_active_raw_gas, g_active_raw_gas_range, _debug);

   switch(_style) {

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Format case, 3 floats with 2 decimal places 1 decimal, space separated.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

      case 0 :    
         if (_include_TPH) {
            printf("%.2f ", common_aMap.shm.temperature);
            printf("%.2f ", common_aMap.shm.pressure);
            printf("%.2f",  common_aMap.shm.humidity);
            if (!_include_gas) printf("\n");
         }
         if (_include_gas) printf(" %d %d\n",
                           common_aMap.shm.bme688GasMan_active_raw_gas,
                           common_aMap.shm.bme688GasMan_active_raw_gas_range);
         break;

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Default case
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

      default:
      case 1 :    
         if (_include_gas & _debug & (s == NULL)) {
            printf("Initial heater current idac_heat: %dmA", g_active_idac);
            printf(", Heater temperature: %3d", g_active_res_heat_temp);
            printf(", gas_wait_time: %3dmS", g_active_gas_wait_time);
            printf(", gas: %sVALID", (g_active_gas_valid == 1) ? "" : "NOT ");
            printf(", heat_stable: %s", (g_active_gas_heat_stable == 1) ? "TRUE" : "FALSE");
            printf("\n");
         }
         if (_include_TPH) {
            printf("Temperature: %.2f%c%c", common_aMap.shm.temperature, 0xC2, 0xB0);
            printf(", QNH: %.2fhPa",        common_aMap.shm.pressure);
            printf(", Humidity: %.2f%%",    common_aMap.shm.humidity);
            printf(", Dew Point: %.2f%c%c",
                  calculateDewPoint(fp, common_aMap.shm.temperature, common_aMap.shm.humidity), 0xC2, 0xB0);
            if (_include_gas) printf(", "); else printf("\n");
         }
         if (_include_gas) {
            printf("Raw Gas resistance: %d, Raw Gas range: %d", g_active_raw_gas, g_active_raw_gas_range);
/*          printf("Gas resistance: "); __printfcomma(_ohms); printf("%s", ohm); */
            printf("\n");
         }
         break;

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Format case, natural numbers, space separated. All data.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

      case 2 :
         printf("%d ", (int)(common_aMap.shm.temperature));
         printf("%d ", (int)(common_aMap.shm.pressure));
         printf("%d",  (int)(common_aMap.shm.humidity));
         if (_include_gas) printf(" %d", common_aMap.shm.bme688GasMan_active_raw_gas);
         if (_include_gas) printf(" %d", common_aMap.shm.bme688GasMan_active_raw_gas_range);
         printf("\n");
         break;

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Minimal Format case, comma and space separated.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

      case 3 :    
         if (_include_TPH) {
            printf("%.2f%c%c ", common_aMap.shm.temperature, 0xC2, 0xB0);
            printf("%.2fhPa ",  common_aMap.shm.pressure);
            printf("%.2f%%",    common_aMap.shm.humidity);
            if (!_include_gas) printf("\n");
         }
         if (_include_gas) { printf(" "); __printfcomma(_ohms); printf("%c%c%c\n", 0xE2, 0x84, 0xA6); }
         break;

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Debug case
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

      case 4:
         printf("\n");
         printf("aMap.shm: Temperature is %.4f%c%cC\n", common_aMap.shm.temperature, 0xC2, 0xB0);
         printf("aMap.shm: QNH         is %.4fhPa\n",  common_aMap.shm.pressure);
         printf("aMap.shm: Humidity    is %.4f%% \n",  common_aMap.shm.humidity);

         if (_include_gas) printf("aMap.shm: Gas         is %d (0x%04X), range: %d, converted: %d%c%c%c\n",
               common_aMap.shm.bme688GasMan_active_raw_gas, common_aMap.shm.bme688GasMan_active_raw_gas,
               common_aMap.shm.bme688GasMan_active_raw_gas_range,
               _ohms, 0xE2, 0x84, 0xA6);
         break;       

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Shared Memory debug case
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

      case 5:
         printf("\n");
         printf("Shared Memory ptr... PTR_COMMON_SHMMAP... is %p\n", ptr_common_shmMap);
         printf("shm --- aH: %.6f\n", readshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.humidity));
         printf("shm --- aP: %.2f\n", readshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.pressure));
         printf("shm --- aT: %.2f\n", readshmDouble(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.temperature));
         if (_include_gas)
         printf("shm --- aG: %d\n",   readshmU16(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688GasMan_active_raw_gas));
         break;

      case 6:
         _status = _extractTheStaticRegisters(fp, g_SlaveAddress);
         _status = _extractTheAltRegisters(fp, g_SlaveAddress);
         displayBME688Registers(fp);
         break;
   }

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 Finite State Machine Initialisation and Handlers.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void __fsmInitialiseStructStatePtrs() {
   int i;
   for (i = 0; i < MAXSTATECOUNT; i++) {
       ptrs[i].valid = 0;
       ptrs[i].ptr = NULL; /* Don't like uninitialised error messages. */
   }
}

struct state_t * __fsmCreateStateNode(struct state_t * _prev_state_p) {
   int i = 0;
   struct state_t * new_state_p = (struct state_t *)(malloc(sizeof(struct state_t)));
   if (new_state_p == NULL) {
      printf("__fsmCreateStateNode: malloc fail, returning NULL.\n");
      g_quit = 1;
      return NULL;
   }
   if (head == NULL) stateID = 0; else stateID++;
   new_state_p->_windex = 0; /* Per state, may be a sequence of gas_wait_times / temps /idac. */
   new_state_p->_tindex = 0;
   new_state_p->_iindex = 0;
   new_state_p->ID = stateID;
   new_state_p->state = 0;
   new_state_p->loop  = 1;
   new_state_p->loop_delay     = 1000; /* mS */
   new_state_p->sensor         = 0; /* gas off == 0, gas on == 1 */
   new_state_p->gas_wait_time_shared = 0;
   new_state_p->gas_cycle_time = MIN_GAS_CYCLE_TIME;
   new_state_p->terminate      = 0;
   new_state_p->style          = 1;
   new_state_p->use_profile    = 0; /* Use this as the number of steps used in the profile. */
   new_state_p->use_parallel   = 0;

   for (i = 0; i < GASMAN_BUFFER_SIZE; i++) {
       new_state_p->p_gas_wait_time[i] = 0; 
       new_state_p->p_res_heat_temp[i] = 0;
       new_state_p->p_idac[i]          = 0; }

   new_state_p->nb_conv        = 0;
   new_state_p->anomaly        = 0;
   new_state_p->anomalyValue1  = 0.0; /* Doesn't really matter. */
   new_state_p->anomalyValue2  = 0.0;
   new_state_p->inext          = 0;
   new_state_p->next           = NULL;
   new_state_p->inextA         = 0;
   new_state_p->nextOnAnomaly  = NULL;
   new_state_p->prev           = NULL; /* Sort the prev value later. */

/* The following next/prev links may not be true but convenient.
 * the state->next links will typically NOT be sequential.
 * Save each state pointer indexed by stateID (0...) for later back filling
 * Adjust after all the states are collected and adjust s->next...
 */
   ptrs[stateID].valid = 1; /* A convenient fudge. */
   ptrs[stateID].ptr   = new_state_p;

   return new_state_p;
}

void __fsmDivideAndConquerFiniteStates(char * key, char * value) {
   int _value = 0;

   if (strcmp(toLower(key), "state") == 0) {
      state_p = __fsmCreateStateNode(state_p); /* Save the previous state_p. */
      if (state_p == NULL) { exit(0); }

      _value = atoi(value);
      if ((_value < 0) || (_value > MAXSTATECOUNT)) {
         printf("State value %d is invalid, cannot sensibly continue.\n", _value);
         _value = 0;
         g_quit = 1;
      }
      state_p->state = _value;
      ptrs[stateID].state = _value;

      if (head == NULL) { /* Set the head node. */
         head        = state_p;
         head->prev  = state_p;
      }
   }

   if (strcmp(toLower(key), "loop") == 0) {
      _value = atoi(value);
      if ((_value < 0) || (_value > MAXLOOPCOUNT)) _value = 1; /* Can be zero for forever loop. */
      state_p->loop = _value;
   }

   if (strcmp(toLower(key), "loop_delay") == 0) {
      _value = atoi(value);
      if ((_value < 1) || (_value > MAXLOOPDELAY)) _value = 1000;
      state_p->loop_delay = _value;
   }

   if (strcmp(toLower(key), "gas_cycle_time") == 0) {
      _value = atoi(value);
      if (_value < MIN_GAS_CYCLE_TIME) _value = MIN_GAS_CYCLE_TIME;
      if (_value > MAX_GAS_CYCLE_TIME) _value = MAX_GAS_CYCLE_TIME;
      state_p->gas_cycle_time = _value;
   }

   if (strcmp(toLower(key), "gas_wait_time_shared") == 0) {
      _value = atoi(value);
      if ((_value < 1) || (_value > GASMAN_MAX_WAIT_TIME_SHARED)) _value = 0;
      state_p->gas_wait_time_shared = _value;
   }

   if (strcmp(toLower(key), "gas_wait_time") == 0) {
      _value = atoi(value);
      if ((_value < 1) || (_value > GASMAN_MAX_WAIT_TIME)) _value = 0;
      state_p->p_gas_wait_time[state_p->_windex++] = _value;
   }

   if (strcmp(toLower(key), "res_heat_temp") == 0) {
      _value = atoi(value);
      if ((_value < 1) || (_value > GASMAN_MAX_TEMPERATURE)) _value = 0;
      state_p->p_res_heat_temp[state_p->_tindex++] = _value;
   }

   if (strcmp(toLower(key), "idac") == 0) {
      _value = atoi(value);
      if ((_value < 1) || (_value > 127)) _value = 0; /* 0..0x7F. */
      state_p->p_idac[state_p->_iindex++] = _value;
   }

   if (strcmp(toLower(key), "nb_conv") == 0) {
      _value = atoi(value);
      if ((_value < 0) || (_value > GASMAN_BUFFER_SIZE)) _value = 0;
      state_p->nb_conv = _value;
   }

   if (strcmp(toLower(key), "style") == 0) {
      _value = atoi(value);
      state_p->style = 1;
      if ((_value >= 0) && (_value < STYLECOUNT)) state_p->style = _value;
   }

   if (strcmp(toLower(key), "sensor") == 0)           state_p->sensor    = (atoi(value) == 1) ? 1 : 0;
   if (strcmp(toLower(key), "terminate") == 0)        state_p->terminate = (atoi(value) == 1) ? 1 : 0;
   if (strcmp(toLower(key), "anomaly") == 0)          state_p->anomaly   = atoi(value);
   if (strcmp(toLower(key), "anomalyvalue") == 0)     state_p->anomalyValue1 = atof(value);
   if (strcmp(toLower(key), "anomalyvalue1") == 0)    state_p->anomalyValue1 = atof(value);
   if (strcmp(toLower(key), "anomalyvalue2") == 0)    state_p->anomalyValue2 = atof(value);
   if (strcmp(toLower(key), "anomalycondition") == 0) state_p->anomalyCondition = atoi(value);
   if (strcmp(toLower(key), "nextonanomaly") == 0)    state_p->inextA = atoi(value);
   if (strcmp(toLower(key), "next") == 0)             state_p->inext  = atoi(value);
   if (strcmp(toLower(key), "parallel") == 0)         state_p->use_parallel = 1;

   return;
}

/* If profile is active, then we EXPECT up to 4 tokens per string...
 * step index(0), Step Temperature(1), Step gas_wait_time or MULTIPLIER(2), idac[optional](3) if used.
 *
 * In FORCED_MODE, gas_wait_time can have any +ve value up to 4032 even though the size is 1 byte.
 * In PARALLEL_MODE, gas_wait_time is a MULTIPLIER of gas_wait_time_shared BUT...
 * the value stored in the register may be used in a FORCED_MODE operation as an explicit time
 * depending on nb_conv.
 */

int __fsmReadFiniteStateProfileConfig(char * k, char * v, char * t2, char * t3, int _debug) {
   int index  = 0;
   int value  = 0;
   int status = 0;

   if (debug3) {
      printf("\n");
      if (k  != NULL) printf("%s  ", k);  else printf("(null)  ");
      if (v  != NULL) printf("%s\t", v);  else printf("(null)\t");
      if (t2 != NULL) printf("%s\t", t2); else printf("(null)\t");
      if (t3 != NULL) printf("%s\n", t3); else printf("(null)\n");
   }

   if (k != NULL) {
      value = atoi(k); /* The first value is the step index and must be between 0..9 (GASMAN_BUFFER_SIZE - 1) */
      if ((value < 0) || (value >= GASMAN_BUFFER_SIZE)) {
         printf("__fsmReadFiniteStateProfileConfig: invalid index: %d, step zero(index = 0) is being used.\n", value);
      } else index = value; 
   }

/* Temperature... */
   state_p->p_res_heat_temp[index] = 0;
   if (v != NULL) {
      value = atoi(v);
      if ((value < 0) || (value >= GASMAN_MAX_TEMPERATURE)) {
         printf("Imported data error: gas_res_heat_temp[%d]: %d\n", index, value);
         value  =  0;
         status = -1;
      }
      state_p->p_res_heat_temp[index] = value;
   } 

/* Gas index multiplier... */
   state_p->p_gas_wait_time[index] = 0;
   if (t2 != NULL) {
      value = atoi(t2);
      if ((value < 0) || (value > GASMAN_BUFFER_SIZE)) { /* Convenient value == 10, can be up to 255. */
         printf("Imported data error: gas_multiplier[%d]: %d\n", index, value);
         value  =  0;
         status = -1;
      } 
      state_p->p_gas_wait_time[index] = value;
   }

/* idac... */
   state_p->p_idac[index] = 0;
   if (t3 != NULL) {
      value = atoi(t3);
      if ((value < 0) || (value > 127)) {
         printf("Imported data error: idac[%d]: %d\n", index, value);
         value  =  0;
         status = -1;
      } 
      state_p->p_idac[index] = value;
   }

   if (_debug) {
      printf("res_heat_temp[%d]: %3d\t", index, state_p->p_res_heat_temp[index]);
      printf("gas_multiplier[%d]: %3d\t", index, state_p->p_gas_wait_time[index]);
      printf("idac[%d]: %3d\n", index, state_p->p_idac[index]);
   }

   return status;
} 

int fsmReadStateMachineConfig(char * fname, int _debug) {
   FILE * _ptr;
   char _str[MAXSTRSIZE] = {'\0'}; /* Fixed array rather than malloc. */
   char * key;
   char * value;
   char * token;
   char * token2;
   char * token3;
   int profile = 0;
   int index   = 0;
   int i;
   char * _fname;

   if (debug3) printf("DEBUG3: Opening[r] the config file: %s...\n", fname);
   _ptr = fopen(fname, "r"); /* r or w for write */
   if (_ptr == NULL) {
      _fname = concat("./unit_tests/", fname);
      if (debug3) printf("DEBUG3: ./%s not found, trying %s...\n", fname, _fname);
      _ptr = fopen(_fname, "r"); /* r or w for write */
   } 
   if (_ptr == NULL) {
      printf("fsmReadStateMachineConfig: unable to open[r] fsmName: %s, exiting.\n", fname);
      g_quit = 1;
      return -1;
   }
   for (i = 0; i < MAXSTRSIZE; i++) _str[i] = '\0';

   while (fgets(_str, MAXSTRSIZE, _ptr) != NULL) {
      if (strlen(_str) > MAXSTRSIZE - 2) continue; /* too long, ignore the str */
      if (_str[0] == '#') continue;
      if (_str[0] == '-') continue;
      if (strchr(_str,'#') != NULL) continue;

/* Remove \n. Could use <string.h> trim(_str) functions, but easier to scan the short string. */
      _str[strlen(_str) - 1] = '\0';

/* The first mode of recovery is a simple 2 component key-value combination.
 * Additional tokens in the string are ignored.
 *
 * If the toggle "profile" is found, additional string tokens are recovered.
 * If the toggle "profile" is active, lock all other parameters out...
 * until the second profile toggle is found.
 */
      i = 0; key = NULL; value = NULL; token2 = NULL; token3 = NULL; /* Because we search on NULL, NULL. */
      token = strtok(_str, " ");
      while (token != NULL) {
         switch(i) {
            case 0 : key    = token; break;
            case 1 : value  = token; break;
            case 2 : token2 = token; break;
            case 3 : token3 = token; break;
            default: break; /* Ignore anything else. */
         }
         i++;
         token = strtok(NULL, " ");
      }
      if (key == NULL) continue;

      if (strcmp(toLower(key), "profile") == 0) {
         profile = (profile == 0) ? 1 : 0; /* toggle */
         if (profile) state_p->use_profile = 1;
         if (profile & debug3) printf("DEBUG3: Profile mode on.\n");
         continue; /* skip the rest. */
      }

      if ((!profile) & debug3) {
         printf("DEBUG3: %s\t", key);
         if (strlen(key) < 16) printf("\t");
         if (strlen(key) < 8)  printf("\t");
         if (value  != NULL) printf("%s\t", value);
         if (token2 != NULL) printf("%s\t", token2);
         if (token3 != NULL) printf("%s\t", token3);
         printf("\n");
      }

      if (!profile) __fsmDivideAndConquerFiniteStates(key, value);

/* If profile is active, then we EXPECT 4 tokens per string...
 * Step Temperature, Step gas_wait_time or MULTIPLIER, idac[optional] if used.
 *
 * In FORCED_MODE, gas_wait_time can have any +ve value up to 4032 even though the size is 1 byte.
 * In PARALLEL_MODE, gas_wait_time appears to be a MULTIPLIER of gas_wait_time_shared BUT...
 * the value stored in the register may be used in a FORCED_MODE operation depending on nb_conv.
 */
      if (profile) {
         if (__fsmReadFiniteStateProfileConfig(key, value, token2, token3, 0) < 0) g_quit = 1; /* _debug == 0 */
         state_p->use_profile = index++; /*Increment after the fact. */
      }
      for (i = 0; i < MAXSTRSIZE; i++) _str[i] = '\0';
   }
   fclose(_ptr);

   if (debug3) printf("DEBUG3: State Config file %s has been imported. Profile step count: %d, nb_conv: %d\n",
               fname, state_p->use_profile, g_active_nb_conv);

   if (g_quit != 0) return -1;

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 Finite State Machine establish the linked list.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

struct state_t * __fsmGetStatePtr(int _state) {
   int i;
   for (i = 0; i <= stateID; i++)
      if (ptrs[i].state == _state) return ptrs[i].ptr;
   return NULL;
}   

int __fsmEstablishListLinks(int _stateID, int _debug) {
   int n= 0;
   double tmp = 0;
   struct state_t * tmpptr = NULL;
   tmpptr = head;

   if (_stateID < 0) return -1; /* From global. */

/* Can't use valid as an index as the state numbers may not be contiguous.
 * Go through each stateID sequentially.
 */
   for (n = 0; n <= _stateID; n++) {
      if (ptrs[n].valid == 1) {
         state_p = ptrs[n].ptr;
         if (state_p == NULL) continue;

         if (_debug) {
            if (n == 0) printf("HEAD STATE\n");
            printf("ID: %d, state: %d, ", n, ptrs[n].state);
            printf("ptr:\t\t%p\n", (void *)(state_p));
         }
         state_p->prev = tmpptr;
         if (_debug) {
            if (state_p->prev != NULL)
                       printf("state_p->prev    ptr:\t\t%p\n", (void *)(state_p->prev));
               else    printf("state_p->prev    ptr:\t\tNULL\n");
         }
         state_p->next = __fsmGetStatePtr(state_p->inext);
         if (_debug) {
            if (state_p->next != NULL)
                       printf("state_p->next    ptr:\t\t%p, ", (void *)(state_p->next));
               else    printf("state_p->next    ptr:\t\tNULL, ");
            printf("points to state: %d, ", state_p->inext);
            if (state_p->next == NULL) printf("state not found\n"); else printf("\n");
         }
         state_p->nextOnAnomaly = __fsmGetStatePtr(state_p->inextA);
         if (_debug) {
            if (state_p->nextOnAnomaly != NULL)
                       printf("state_p->nextOnAnomaly ptr:\t%p, ", (void *)(state_p->nextOnAnomaly));
               else    printf("state_p->nextOnAnomaly ptr:\tNULL, ");
            printf("points to state: %d, ", state_p->inextA);
            if (state_p->nextOnAnomaly == NULL) printf("state not found\n"); else printf("\n");
         }
/* Tidy up any range discrepancies at the same time... */
         if (state_p->anomalyCondition == 2) { /* Range only condition. */
            if (state_p->anomalyValue1 < state_p->anomalyValue2) {
               tmp = state_p->anomalyValue1;
               state_p->anomalyValue1 = state_p->anomalyValue2;
               state_p->anomalyValue2 = tmp;
            }
         }
        tmpptr = state_p; /* Easiest way to remember the previous ptr. */
      }
   }
   if (_debug) printf("\n");

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 Finite State Machine convenience procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void fsmPrintState(const struct state_t * s) {
   if (s == NULL) return;
   printf("\n");
   if (head->state == s->state) printf("Head State:\t%d\n", head->state);
      else printf("State:\t\t%d\n", s->state);
   printf("State_p ptr:\t\t%p\n", (void *)(s));
   printf("Sensor:\t\t\t%d\n",  s->sensor);
   printf("Terminate:\t\t%s\n",  (s->terminate == 1) ? "TRUE" : "FALSE");
   printf("Display style:\t%d\n",  s->style);
   printf("Loop Count:\t\t%d\n",  s->loop);
   printf("Loop Delay:\t\t%d mS\n",  s->loop_delay);
   printf("State uses %s Mode\n", (s->use_parallel == 0) ? "FORCED" : "PARALLEL");
   printf("nb_conv:\t\t%d\n", s->nb_conv);
   printf("gas_wait_time_shared:\t%d mS\n", s->gas_wait_time_shared);
   printf("gas_wait_time[0]:\t%d mS\n", s->p_gas_wait_time[0]);
   printf("res_heat_temp[0]:\t%d%c%cC\n", s->p_res_heat_temp[0], 0xC2, 0xB0);
   printf("idac[0]:\t\t%d%c%cC\n", s->p_idac[0], 0xC2, 0xB0);
   printf("gas_cycle_time:\t\t%d Secs\n", s->gas_cycle_time);
   printf("anomaly TYPE:\t\t%d\n", s->anomaly);
   printf("anomaly value1:\t\t%.2f\n", s->anomalyValue1);
   printf("anomaly value2:\t\t%.2f\n", s->anomalyValue2);
   if (s->prev != NULL) printf("s->prev ptr:\t\t%p\n", (void *)(s->prev));
                   else printf("s->prev ptr:\t\tNULL\n");
   printf("s->inext:\t\t%d\n", s->inext);
   if (s->next != NULL) printf("s->next ptr:\t\t%p\n", (void *)(s->next));
                   else printf("s->next ptr:\t\tNULL\n");
   printf("s->inextOnAnomaly:\t%d\n", s->inextA);
   if (s->nextOnAnomaly != NULL) printf("s->nextOnAnomaly ptr:\t%p\n", (void *)(s->nextOnAnomaly));
                   else printf("s->nextOnAnomaly ptr:\tNULL\n");
   return;
}

void fsmPrintAllStates(const struct state_t * h) {
   int i;
   for (i = 0; i <= stateID; i++) {
      if (ptrs[i].ptr != NULL) {
         fsmPrintState(ptrs[i].ptr);
      }
   }
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 Finite State Machine cleanup procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void fsmPrintAndFreeList(struct state_t * currentNode, int _debug) {
   struct state_t * tempNode;
   int n = 0;

/* Use the stateID list to remove the linked list. Removes "lost" states. */
   for (n = 0; n <= stateID; n++) {
      if (_debug) printf("Removing state: %d,\t%p\n", ptrs[n].state, (void *)ptrs[n].ptr);
      tempNode = ptrs[n].ptr;
      free(tempNode);
   }

/* This is the classical linked list removal but technically, here, there can be "lost" states.
 *
 * while (currentNode != NULL) {
 *    if (_debug) printf("Removing state: %d\n", currentNode->state);
 *    tempNode    = currentNode;
 *    currentNode = currentNode->next;
 *    free(tempNode);
 * }
*/
   return;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Finite State Machine Runtime Handlers.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

void _fsmLoopSleep(int mS) {
   int _mS = 1000; /* 1000 mS, MAXLOOPDELAY #defined as 3600 Seconds. */
   if ((mS < 100) || (mS > MAXLOOPDELAY)) _mS = 1000; else _mS = mS;
   if (_mS > 5000) sleep(_mS/1000); else mDelay(_mS);
   return;
}

int _fsmEnableRunGas(int fp, int enableGas, int _debug) {
   int status = 0;
   status = bme688GasMan_EnableRunGas(fp, enableGas); /* enable == 1, disable == 0, ignore return. */
   status = bme688GasMan_CheckRunGas(fp); 
   if (_debug) {
      if (status) printf("_fsmEnableRunGas: bme688GasMan_CheckRunGas: enabled.\n");
             else printf("_fsmEnableRunGas: bme688GasMan_CheckRunGas: disabled.\n");
   }
   return status; 
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Anomalies...
 * No point hiding an anomaly within the debug framework.
 * Display the result at this stage.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __fsmAnomalyDetector(int fp, struct state_t * s, int _debug) {
/* anomalyCondition: 0 set on, 1 greater than, -1 less than, 2 beyond range [anomalyValue2 < anomalyValue1] */

   int _ohms = 0;

   switch (s->anomaly) {
      case  0 : return 0; break;
      case  1 : switch (s->anomalyCondition) {
                      case -1 : if (common_aMap.shm.temperature < s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Temperature anomaly detected, temp %.2f, limit: %.2f.\n",
                                      common_aMap.shm.temperature, s->anomalyValue1);
                                return 1;
                                break;
                                }
                      case  0 : return 0; break;
                      case  1 : if (common_aMap.shm.temperature > s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Temperature anomaly detected, temp %.2f, limit: %.2f.\n",
                                      common_aMap.shm.temperature, s->anomalyValue1);
                                return 1;
                                break;
                                }
                      case  2 : if ((common_aMap.shm.temperature > s->anomalyValue1) ||
                                    (common_aMap.shm.temperature < s->anomalyValue2)) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Temperature anomaly detected, temp %.2f, range: %.2f...%.2f.\n",
                                   common_aMap.shm.temperature, s->anomalyValue1, s->anomalyValue2);
                                return 1;
                                break;
                                }
                      default : return 0; break;
                }

      case  2 : switch (s->anomalyCondition) {
                      case -1 : if (common_aMap.shm.pressure < s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Pressure anomaly detected, pressure %.2f, limit: %.2f.\n",
                                      common_aMap.shm.pressure, s->anomalyValue1);
                                return 2;
                                break;
                                }
                      case  0 : return 0; break;
                      case  1 : if (common_aMap.shm.pressure > s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Pressure anomaly detected, pressure %.2f, limit: %.2f.\n",
                                      common_aMap.shm.pressure, s->anomalyValue1);
                                return 2;
                                break;
                                }
                      case  2 : if ((common_aMap.shm.pressure > s->anomalyValue1) ||
                                    (common_aMap.shm.pressure < s->anomalyValue2)) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Pressure anomaly detected, pressure %.2f, range: %.2f...%.2f.\n",
                                   common_aMap.shm.pressure, s->anomalyValue1, s->anomalyValue2);
                                return 2;
                                break;
                                } 
                      default : return 0; break; 
                }
   
      case  3 : switch (s->anomalyCondition) {
                      case -1 : if (common_aMap.shm.humidity < s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Humidity anomaly detected, humidity %.2f, limit: %.2f.\n",
                                      common_aMap.shm.humidity, s->anomalyValue1);
                                return 3;
                                break;
                                }
                      case  0 : return 0; break;
                      case  1 : if (common_aMap.shm.humidity > s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Humidity anomaly detected, humidity %.2f, limit: %.2f.\n",
                                      common_aMap.shm.humidity, s->anomalyValue1);
                                return 3;
                                break;
                                }
                      case  2 : if ((common_aMap.shm.humidity > s->anomalyValue1) ||
                                    (common_aMap.shm.humidity < s->anomalyValue2)) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Humidity anomaly detected, humidity %.2f, range: %.2f...%.2f.\n",
                                   common_aMap.shm.humidity, s->anomalyValue1, s->anomalyValue2);
                                return 3;
                                break;
                                } 
                      default : return 0; break; 
                }
   
      case  4 : {
                _ohms = _convertGasOhms(g_active_raw_gas, g_active_raw_gas_range, _debug);
                switch (s->anomalyCondition) {
                      case -1 : if (_ohms < s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Gas_ohms anomaly detected, _ohms %d, limit: %.2f.\n",
                                      _ohms, s->anomalyValue1);
                                return 4;
                                break;
                                }
                      case  0 : return 0; break;
                      case  1 : if (_ohms > s->anomalyValue1) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Gas_ohms anomaly detected, _ohms %d, limit: %.2f.\n",
                                      _ohms, s->anomalyValue1);
                                return 4;
                                break;
                                }
                      case  2 : if ((_ohms > s->anomalyValue1) ||
                                    (_ohms < s->anomalyValue2)) {
                                   printf("--- --- --- ---> Anomaly type: %d, ", s->anomaly);
                                   printf("Gas_ohms anomaly detected, _ohms %d, range: %.2f...%.2f.\n",
                                   _ohms, s->anomalyValue1, s->anomalyValue2);
                                return 4;
                                break;
                                } 
                      default : return 0; break; 
                   }
                }
   default : break;
   }
   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * _fsmTraverseState proper...
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 *
 * There are 2 mode of operation for the gas measurement operations,
 * FORCED mode and PARALLEL mode using bme688 internal data structures
 * for control. These operations are fundamentally different and cater
 * for different types of repetitive assessments of pollutants.
 * As stated, the PARALLEL assessment procedures are user defined and
 * can vary infinitely.
 * No matter what, for the PARALLEL mode, we zero everything beforehand.
 * res_heat, idac_heat, gas_wait and nb_conv are all set to 0.
 *
 * Rather than try to fix range errors, these shouldn't exist so,
 * check and fail where necessary on range errors.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int _fsmTraverseState(int fp, struct state_t * s, int _field, int _debug) {
   char * t = "_fsmTraverseState";
   int _status = 0;
   int __attribute__((unused)) ignore = 0;
   int _gas = 0;
   int _i2cResult = 0;
   int i;

   if (s == NULL) return 0; /* Just in case */

/* A:
 * Set the output data style if style is set.
 * Set the mode to OFF_MODE.
 * Clear any previous gas config.
 */
   if (!g_enable_g_style) if (s->style != -1) g_style = s->style;
   setSensorMode(fp, OFF_MODE, 0); /* _debug = 0 */
   _status = __bme688GasMan_Zero_Gas_Registers(fp, 0); /* Returns 0 */
   _errorTerminate(fp, _status); /* Basic parameter error only. */

/* B:
 * Use local variable _gas, g_enable_gas is set asynchronously.
 */
   _gas = ((g_enable_gas) && (s->sensor == 1)) ? 1 : 0;
   if (_debug) printf("\nState: %d: %s...\n", s->state, (_gas) ? "TPH and Gas enabled" : "TPH enabled");

/* C:
 * Prepare the basic TPH operation...
 */
   _status = bme688Prepare_TPH_Operation(fp, 0); /* _debug = 0, returns 0. */
   bme688Checkshm(fp, _debug); /* If an update has occured, notify. Also sets setSensorMode OFF. */

/* D:
 * Prepare FORCED_MODE operation if _gas is enabled.
 * bme688GasMan_Prepare_Forced_Operation sets up ALL DEFAULTS before any PARALLEL_MODE additions
 * nb_conv and g_active_gas_wait_time_shared are set to 0.
 * g_active_field = 0 and g_active_step/nb_conv = 0. 
 *
 * FORCED_MODE can use gas_wait_time[nb_conv] and res_heat_temp[nb_conv] where nb_conv = 0..9.
 * The nb_conv directive is set in the fsm config file. (FORCED_MODE or PARALLEL_MODE).
 * If PARALLEL_MODE (with the directive parallel) is specified,
 * then nb_conv is a step multiplier starting from index 0..nb_conv.
 */
   if (_gas) {
      _status = bme688GasMan_Prepare_Forced_Operation(fp, s->p_res_heat_temp[0], s->p_gas_wait_time[0], 0); /* _debug = 0 */
      _errorTerminate(fp, _status); /* Basic parameter error only. */
      bme688ClearActiveGasInfo(fp); /* Clear down any history. */
   }

/* E:
 * If the use_profile directive has been set, load the profile.
 * because we are just loading the internal registers, ignore the g_active_... variables.
 * The index 0 res_heat_temp and gas_wait_time profile entries may be overwritten by...
 * explicit res_heat_temp and gas_wait_time values following (closing) profile in the fsm config file.
 */
   if (s->use_profile > 0) { /* These will reset the g_active_... variables, reset below. */
      for (i = 0; i < s->use_profile; i++) {
          ignore = __bme688GasMan_Set_heater_temperature(fp, i, s->p_res_heat_temp[i], 0); /* _debug = 0 */
          ignore = __bme688GasMan_Set_gas_wait_time     (fp, i, s->p_gas_wait_time[i], 0);
          ignore = __bme688GasMan_Set_idac              (fp, i, s->p_idac[i],          0);
      }
   }

/* F: FORCED_MODE: Now the real work. Reset to the index 0 defaults in case the profile has been used.
 * Reset the configuration for FORCED_MODE follow E: above, using nb_conv = 0...
 * default rather than s->nb_conv.
 * If a gas_wait_time_shared has been specified, load it. Default is 0.
 * If nb_conv is not 0, set nb_conv. Can use one of the other register sets...
 * Test for zero registers and shut off gas if.
 */
   if (_gas) {
      g_active_step  = 0; /* Use default index 0 rather than s->nb_conv at this stage. */
      g_active_field = 0; /* See p41. */
      g_active_gas_wait_time_shared = __bme688GasMan_Set_gas_wait_time_shared(fp, s->gas_wait_time_shared,     0);
      g_active_gas_wait_time = __bme688GasMan_Set_gas_wait_time(fp,      g_active_field, s->p_gas_wait_time[0], 0);
      g_active_res_heat_temp = __bme688GasMan_Set_heater_temperature(fp, g_active_field, s->p_res_heat_temp[0], 0);
      g_active_idac          = __bme688GasMan_Set_idac(fp, g_active_field, s->p_idac[0], 0);
      g_active_nb_conv       = bme688GasMan_Set_nb_conv(fp, 0, 0); /* _debug = 0, set nb_conv = 0 rather than s->nb_conv. */
      if (g_active_gas_wait_time == 0) _gas = 0; /* Check. */
      if (g_active_res_heat_temp == 0) _gas = 0;

/* FORCED_MODE: If s->nb_conv != 0, set nb_conv. If PARALLEL_MODE, then nb_conv has a different interpretation. See below. */
      if ((s->nb_conv > 0) && (s->nb_conv < GASMAN_BUFFER_SIZE)) g_active_nb_conv = bme688GasMan_Set_nb_conv(fp, s->nb_conv, 0);
   }

/* G:
 * Determine _gas status and set RunGas. The mode is currently OFF_MODE.
 * RunGas activation is not dependent on FORCED_MODE or PARALLEL_MODE.
 */
   _gas = _fsmEnableRunGas(fp, _gas, 0); /* if _gas == 1 == enable, 0 == disable, _debug == 0. */

/* H:
 * Update the data buffers, do it this way to provide a short settlement timing delay.
 * If debug debug (DEBUG2) has been set, then display the overall configuration. Everything.
 * If _debug, display configuration and status of process ahead.
 */
   _status  = _extractTheStaticRegisters(fp, g_SlaveAddress);
   _status |= _extractTheAltRegisters(fp, g_SlaveAddress);
   _errorTerminate(fp, _status);
   if (debug2 && _gas) __bme688GasMan_DisplayGasConfig(fp);
   if (debug2) displayBME688Registers(fp);

/* I: FORCED_MODE
 * Switch selection...
 * FORCED_MODE or PARALLEL_MODE selection...
 * FORCED_MODE, setSensorMode, Process Data and display.
 * The display g_style has been set from s->style.
 * g_style may have been used just once but can be per state.
 */
   switch (s->use_parallel) {
      case 0 : /* This is FORCED_MODE */
             if (!_gas)  {
                g_active_res_heat_temp = 0; /* Don't reset the registers, just the globals. */
                g_active_gas_wait_time = 0;
                g_active_gas_wait_time_shared = 0;
             }
             if (_debug) {
                printf("gas: %s ACTIVE", (_gas) ? "" : "NOT");
                printf(", nb_conv: %d", g_active_nb_conv);
                printf(", idac: %d", g_active_idac);
                if (_gas) {
                   printf(", res_heat_temp: %d", g_active_res_heat_temp);
                   printf(", gas_wait_time: %d", g_active_gas_wait_time);
                   printf(", gas_wait_time_shared: %d", g_active_gas_wait_time_shared);
                   printf("\n");
                } else printf("\n");
             }

             setSensorMode(fp, FORCED_MODE, 0); /* _debug == 0 */

             _i2cResult = waitForNewDataReady(fp, g_active_field, _gas, _debug);
             if (_i2cResult < 0) {
                printf("waitForNewDataReady error: %d\n", _i2cResult);
                return _i2cResult; /* Various -ve errors. */
             }
             _status = bme688ProcessData(fp, g_active_field, _gas, _debug);
             if (_status == 0) {
                if (_display) bme688DisplayProcessData(fp, s, g_style, 1, _gas, _debug); /* _includeTPH ON. */
             } else
                printf("bme688ProcessData: Forced Mode: State: %d: Data processing error.\n", s->state);
             break;

/* J: PARALLEL_MODE...
 * Could test for _gas status etc before launching Parallel mode but...
 * If PARALLEL_MODE, then call bme688ParallelHandler.
 */
      case 1 :
             _status = pmBeginParallelProcess(fp, s, _gas, _debug);
             break;
      default : break;
   }

/* K:
 * Shutdown procedures...
 * If _gas is used, prepare and activate ALARM Interrupt timeout.
 */
   if (_gas && (!(s->use_parallel))) { /* Parallel Mode has its own timeout structure. */
       g_active_alarm = s->gas_cycle_time; /* Set the (global) Alarm timeout time. */
       g_enable_gas = 0; /* Clear */

#ifndef USE_SIGACTION
       signal(SIGALRM, intHandler);
#endif

       alarm(g_active_alarm);
       if (_debug) printf("*** gas alarm handler active: %d Seconds...\n", g_active_alarm);
   }

/* L:
 * Shutdown sequence...
 * Activate loop delay, there still may be an active TPH delay even though the gas delay has been set.
 * Explicitly set OFF_MODE and disable run_gas. 
 */
   setSensorMode(fp, OFF_MODE, 0); /* If FORCED_MODE, should be off anyway. _debug = 0 */
   _gas = _fsmEnableRunGas(fp, 0, 0); /* _gas = 0 = disable, _debug == 0. */

/* If there is an anomaly, do we delay? Under normal operations, the delay is specified. */
   _fsmLoopSleep(s->loop_delay);

   if (debug2) printf("DEBUG2: %s: State: %d completed.\n", t, s->state);

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * End of _fsmTraverseState
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * _fsmTraverseStateMachine...
 *
 * This routine traverses each of the states in preparation for the
 * single per state process above. Was going to merge the state change
 * and state handler but it is convenient to separate the functions.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int _fsmTraverseStateMachine(int fp, int _debug) {
   int status = 0;
   int finishLoop = 0;
   int anomalyFound = 0;
   int loop_counter = 0;
   struct state_t * s = NULL;
   s = head;
   if (s == NULL) return -1;

/* The state variables have been checked before loading into State Structure */
   while (s != NULL) {

      anomalyFound = 0; /* Reset every state change */
      if (s != NULL) {
         if (s->terminate) {
            printf("State: %d, terminate.\n", s->state);
            return -2; /* A wee trick. */
         } /* else printf("State: %d...\n", s->state); */
      }

      if (debug2) printf("\nDEBUG2: State: %d, Start %sfsm RunLoop\n", s->state, (s->loop == 0) ? "forever " : "");
      finishLoop = 0;
      loop_counter = s->loop;
      while ((!anomalyFound) & (!finishLoop)) {
         anomalyFound = 0; /* Reset every loop */
         status = _fsmTraverseState(fp, s, g_active_field, _debug);
         if (status < 0) return -1;
         loop_counter--;
         if (s->loop != 0) if (loop_counter <= 0) finishLoop = 1;
         if (g_quit) return 0;

/* Return anomalyFound status gives active anomaly type > 0 for interpretation if necessary. */
         anomalyFound = __fsmAnomalyDetector(fp, s, _debug);
         g_quit = g_quit || readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.terminateProcess);
      }

      if (debug2 && (!anomalyFound)) printf("DEBUG2: State: %d, fsm RunLoop Completed.\n", s->state);
      if (g_quit) return 0;

/* If an anomaly has been found within the current State,
 * then there may be a physical/electrical/logical function to perform...
 */
      if ( anomalyFound) __bme688PerformExternalAnomalyOperation(fp, s, _debug);
      if (!anomalyFound) __bme688PerformExternalOperation(fp, s, _debug);

      if (_debug) {
         if (anomalyFound == 0)
            printf("\nUser state %d->next points to %sstate: %d\n", s->state,
                  ((s->next == NULL) ? "NULL " : ""), s->inext);
         else
            printf("\nUser state %d->nextOnAnomaly points to ANOMALY %sstate: %d\n", s->state,
                  ((s->nextOnAnomaly == NULL) ? "NULL " : ""), s->inextA);
      }

      if (anomalyFound == 0) s = s->next; else s = s->nextOnAnomaly;
   }

   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * fsmStartFiniteStateMachineRunLoop
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int fsmStartFiniteStateMachineRunLoop(int fp, int _debug) {
   int status   = 0;
   int _counter = 0;

   __fsmInitialiseStructStatePtrs();
   status = fsmReadStateMachineConfig(fsmName, _debug);
   if (status != 0) return -1;
   status = __fsmEstablishListLinks(stateID, 0); /* _debug = 0 */
   if (status != 0) return -1;

   status = _fsmTraverseStateMachine(fp, _debug);
   if (_debug && (status == -2)) printf("fsmStartFiniteStateMachineRunLoop: Terminated.\n");

   if (g_active_alarm) {
      _counter = g_active_alarm + 1;
      printf("*** gas alarm handler active...\n");
      while ((g_active_alarm != 0) || (_counter == 0)) { printf("."); fflush(stdout); sleep(1); }
      printf("\n");
   }
   if (_debug) printf("Start linked list clean up...\n");
   fsmPrintAndFreeList(head, _debug);
   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Initialisation and runLoop Procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */
/*
 * The following routine simply goes through a couple of TPH cycles
 * to refresh olde data if any. Not critical.
 * This routine will fail if the device is dead or not responding.
 */

int __bme688BeginTPHCycle(int fp, int count, int _debug) { /* _debug is unused. */
   int _i2cResult = 0;
   int status = 0;
   int i = 0;
   for (i = 0; i < count; i++) {
      setSensorMode(fp, FORCED_MODE, 0);
      _i2cResult = waitForNewDataReady(fp, g_active_field, g_enable_gas, _debug);
      if (_i2cResult < 0) {
         printf("waitForNewDataReady error: %d\n", _i2cResult);
         return _i2cResult; /* Various -ve errors. */
      }
      status |= bme688ProcessData(fp, g_active_field, 0, 0); /* No gas, no debug */
   }
   _errorTerminate(fp, status);
   return 0; /* Return status is not used. */
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * Command Line runLoop Procedures.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688StartDirectRunLoop(int fp, int _debug) {
   int _i2cResult = 0;
   int _status    = 0;
   int _gas       = 0;
   int _gasInUse  = 0; /* Gas is being used in this loop regardless of the _gas state. */
   int _start     = 1;
   int _Lcounter = g_loop_counter + 1;

   if (_debug) bme688PrintHelpHint();
   if (debug2) printf("\n");

   setSensorMode(fp, OFF_MODE, 0);
   _status = bme688GasMan_EnableRunGas(fp, 0); /* enable == 1, disable == 0 */
   _status = __bme688GasMan_Zero_Gas_Registers(fp, 0); /* Even if gas is unused. */
   bme688ClearActiveGasInfo(fp);

   _status = bme688Prepare_TPH_Operation(fp, 0); /* _debug == 0, returns 0. */

/* The following routine simply goes through a couple of TPH cycles...
 * to refresh olde data if any. Not imperative.
 */
   _status = __bme688BeginTPHCycle(fp, 2, 0); /* Ignore the return. */

   if (g_enable_gas) _gas = 1; /* Use local variable _gas, g_enable_gas is set asynchronously externally. */

/* FORCED_MODE: set nb_conv = 0 only.
 * There is no PARALLEL_MODE, no option for other register sets[nb_conv=1..9].
 * Assess any gas parameters even if they aren't to be used.
 */
   if (g_active_gas_wait_time == 0) _gas = 0; /* Check. */
   if (g_active_res_heat_temp == 0) _gas = 0;
   if (g_active_nb_conv != 0) _gas = 0;

   if (_gas) {
      _status = bme688GasMan_Prepare_Forced_Operation(fp, g_active_res_heat_temp, g_active_gas_wait_time, 0);
      if (_status != 0) {
         printf("bme688GasMan_Prepare_Forced_Operation failed.\n");
         _gas = 0;
      }
   }
   g_quit = 0;
   g_enable_gas = _gas;
   _gasInUse    = _gas;
   if (debug2) printf("DEBUG2: gas %sACTIVE: g_active_gas_wait_time: %d, g_active_res_heat_temp: %d\n",
                     (_gas) ? "" : "NOT ",  g_active_gas_wait_time, g_active_res_heat_temp);
   if (debug2) printf("DEBUG2: bme688StartDirectRunLoop...\n");

   while (!g_quit) {
      if (_debug) printf("\nLoop: %d\n", _Lcounter - g_loop_counter);

/* Use local variable _gas, g_enable_gas is set asynchronously externally. */
      if (g_enable_gas && _gasInUse)   _gas = 1;
      if (g_active_gas_wait_time == 0) _gas = 0; /* Check again. */
      if (g_active_res_heat_temp == 0) _gas = 0;

      if (_gas) {
         _status = bme688GasMan_EnableRunGas(fp, _gas); /* enable == 1, disable == 0 */
         if (_status != 1) {
            printf("bme688GasMan_EnableRunGas error.\n");
            _gas = 0;
         }
         _gas = bme688GasMan_CheckRunGas(fp); 
      }

/* Interestingly, this idac at startup seems to work!! */
      if (_start && _gas) {
         _start = (g_active_res_heat_temp > 4) ? g_active_res_heat_temp / 4 : 1;
         g_active_idac = __bme688GasMan_Set_idac(fp, 0, _start, 0);
         if (debug2) printf("DEBUG2: g_active_idac: %d\n", g_active_idac);
         _start = 0;
      }
      setSensorMode(fp, FORCED_MODE, _debug);

      _i2cResult = waitForNewDataReady(fp, g_active_field, _gas, _debug);
      if (_i2cResult < 0) {
         printf("waitForNewDataReady error: %d\n", _i2cResult);
         return _i2cResult; /* Various -ve errors. */
      }

      _status = bme688ProcessData(fp, g_active_field, _gas, _debug); /* 0 = no debug */

      if (_display || (g_loop_counter == 1)) bme688DisplayProcessData(fp, NULL, g_style, 1, _gas, _debug); /* _includeTPH ON. */
      if (_status != 0) { printf("# bme688ProcessData: Data processing error.\n"); g_quit = 1; }
      bme688Checkshm(fp, _debug);

      if (g_loop_counter == 1) g_quit = 1;
      if (g_loop_counter  > 1) g_loop_counter--;
      if (!_gas) bme688LoopDelay(g_l_time);

      if (_gas) {
         setSensorMode(fp, OFF_MODE, 0);
         _status = bme688GasMan_EnableRunGas(fp, 0); /* enable == 1, disable == 0 */
         g_enable_gas = 0; /* Set for signal ALARM timeout */
         _gas = 0;
         g_active_alarm = MIN_GAS_CYCLE_TIME; /* Five second timeout if heat used. */

#ifndef USE_SIGACTION
         signal(SIGALRM, intHandler);
#endif

         alarm(g_active_alarm); /* the timeOutHandler signal is actived. */
         if (_debug) printf("*** gas alarm handler active: %d Seconds...\n", g_active_alarm);
      }
      g_quit = g_quit || readshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.terminateProcess);
   }

   if (debug2) printf("\nDEBUG2: bme688StartDirectRunLoop: Run loop completed.\n");
   writeshmByte(ptr_common_shmMap, ptr_common_aMap, &common_aMap.shm.bme688InUse, 0x00);
/* Don't reset terminateProcess as it may be used to clear associated processes. e.g. ssd1306...display */

   if (g_active_alarm) {
      if (g_style == 1) printf("*** gas alarm handler active...\n");
      while (g_active_alarm != 0) { printf("."); fflush(stdout); sleep(1); }
      printf("\n");
   }
   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * The bme688ClearDown procedure clears all gas associated registers
 * and reconfigures the bme688 for (default non active) FORCED_MODE.
 * ClearDown is primarily aimed at stopping and removing any active
 * PARALLEL_MODE configuration.
 * Reset register [0xE0:0xB6] doesn't reset the configuration.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int bme688ClearDown(int fp) {
   int _status = 0;
   _status |= setSensorMode(fp, OFF_MODE, 0);
   _status |= bme688GasMan_EnableRunGas(fp, 0); /* enable == 1, disable == 0 */
   _status |= __bme688GasMan_Zero_Gas_Registers(fp, 0); /* Even if gas is unused. */
   g_active_res_heat_temp = 100; /* Arbitrary initial values for convenience */
   g_active_gas_wait_time = 190;
   _status |= bme688GasMan_Prepare_Forced_Operation(fp, g_active_res_heat_temp, g_active_gas_wait_time, 0);
   bme688ClearActiveGasInfo(fp);
   return _status;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * __bme688PerformExternalOperation and
 * __bme688PerformExternalAnomalyOperation are functions that initiate
 * anything that may start or stop external devices such as relays etc.
 * The only constraint is that the state must exist for reference to
 * any necessary parameters and TPHG values.
 *
 * __bme688PerformExternalAnomalyOperation is activated on an Anomaly.
 * __bme688PerformExternalOperation is a status quo operation if necessary.
 * __bme688PerformExternalOperation has to be programmed in...
 * Ignored by default.
 *
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

int __bme688PerformExternalAnomalyOperation(int fp, struct state_t * s, int _debug) {
   char * t = "__bme688PerformExternalAnomalyOperation";

   if (s == NULL) return 0;
   if (_debug) printf("%s: External ANOMALY operation for state: %d started...\n", t, s->state);
/* */
   if (_debug) printf("%s: External ANOMALY operation completed.\n", t);
   return 0;
}

int __bme688PerformExternalOperation(int fp, struct state_t * s, int _debug) {
   char * t = "__bme688PerformExternalOperation";

   if (s == NULL) return 0;
   if (_debug) printf("%s: External operation for state: %d started...\n", t, s->state);
/* */
   if (_debug) printf("%s: External operation completed.\n", t);
   return 0;
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
 * bme688 Main Start and Loop.
 * Setup the signal handling.
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

/* Must be compiled in to use. Have commented out the old sa_handler. */
#ifdef USE_SIGACTION

int prepareSignal(int _debug) {
   if (_debug) printf("Using sigaction...\n");
   memset(&sa, 0, sizeof(sa));
   sa.sa_flags = 0;
   sigemptyset(&sa.sa_mask);
   sa.sa_sigaction = saHandler;   /* Set the more advanced SA_SIGINFO signal handler */
/* sa.sa_handler = intHandler; */ /* Use the first generation signal handler */

/* A lot of these are irrelevant really. Just included. */
   if (sigaction(SIGINT, &sa, NULL) == -1) {
      printf("sigaction SIGINT failure.\n"); /* Catch SIGINT (Ctrl+C) */
      return -1;
   }
   if (sigaction(SIGUSR1, &sa, NULL) == -1) {
      printf("sigaction SIGUSR1 failure.\n");
      return -1;
   }
   if (sigaction(SIGTERM, &sa, NULL) == -1) {
      printf("sigaction SIGTERM failure.\n");
      return -1;
   }
   if (sigaction(SIGTSTP, &sa, NULL) == -1) {
      printf("sigaction SIGTSTP failure.\n");
      return -1;
   }
   if (sigaction(SIGALRM, &sa, NULL) == -1) {
      printf("sigaction SIGALRM failure.\n");
      return -1;
   }
   return 0;
}

#endif

int bme688Main(int __argc, char ** __argv) {
   int status = 0;
   fp = bme688Init(__argc, __argv);
   if (fp <= 0) return -1;

#ifdef USE_SIGACTION
   if (prepareSignal(debug) == -1) runLoopInUse = 0;
#endif

   if (reset) {
      status = bme688ClearDown(fp);
      runLoopInUse = 0;
   }

   if (runLoopInUse) {
      if (fsm) {
         g_enable_gas = 1; /* Initial set anyway */
         status = fsmStartFiniteStateMachineRunLoop(fp, debug);
         if ((status != 0) && debug) printf("fsmStartFiniteStateMachineRunLoop: error, exiting.\n");
      } else {
         status = bme688StartDirectRunLoop(fp, debug); /* Now returns 0. */
      }
   }
  
   close(fp);
   if (debug) {
      if (status == 0) printf("Exit.\n");
         else printf("Exit Error Status: %d\n", status);
   }
   return 0;
}

/* End of bme688Handlers.h Header */

#endif
