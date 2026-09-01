
# README.md

* This document.
* Last update date August 15 2026.

--
# BME688 Sensor

* The bme688 is an advanced multi sensor system containing Temperature, Humidity and Pressure sensors along with a multi function Gas (TPHG) sensor on a single chip.

--
# Contents

* BME688 Sensor
* Contents
* Device Reference
* Raspberry Pi and i2c
* Licence
* Disclaimer
* The bme688Handlers
* Quick Build
* Quick Remove
* Common Shared Memory
* Keeping It Simple
* Preparation
* Fast i2c interface
* Basic BME688 Operation
* Additional i2c Support
* Finite State Machine (FSM) mode
* Gas and GasMan Operations
* Author

--
# Device Reference

* The reference document for the BME688 is...
 * BST-BME688-DS000-03
 * Revision 1.3 February 2024.
 * [bme688](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme688-ds000.pdf) pdf document link as of August 2026.

The BME688 Revision Document 1.3 February 2024 is incomplete. There are both actual and grammatical errors. For example, Section 3.4 describes gas conversions being performed parallelly. In addition, many parts are missing such as the descriptors and addresses associated with the low range (0x00-0x72) registers. These must interpreted from the document rather than the deliberately constrained main register tables.

--
# Raspberry Pi and i2c

* Raspberry Pi: A Raspberry Pi is used to manage the bme688 along with a range of other i2c connected devices. Really only tested with RPi4 (1Gb, 4Gb and 8Gb) and RPi5. Untested with RPi3 but should work.

* Interface: The interface is 400kb i2c (rather than SPI) and the Sparkfun version which is being used for testing, has a default i2c address 0x76. See separate section below.

* Common Shared Memory: All attached (i2c) devices communicate via shared memory. See separate section below.

* Load: On a Raspberry Pi 4 and using Raspbian Lite (Trixie at this time), the software runs flat out and the RPi is mostly bored.

* Descriptors: All i2c devices attached are described in the header file i2cDevicesHeader.h. The active device is defined in the **Main** routine, in this case, \#define BME688 . If any of the header files are altered, then the whole application should be re-compiled. See separate section below. The current i2cDevicesHeader.h file has been cut back and is basically static. New experimental device are attached occasionally.

--
# Licence

This software is released under the [BSD-3-Clause Licence](Licence.md) and a copy of the licence is provided at the top level. A copy of the said licence has been placed in each of the source files to satisfy the legal Dementors but some files, for example, the Unit Test files, are working files and therefore clean. 

--
# Disclaimer

This is a development framework and experimental software. It mostly works as designed. Hopefully there aren't too many bugs. Use at your own risk. No liability. If it crashes your Raspberry Pi (highly unlikely) or blows up your cat (more likely), it's your problem.

--
# Quick build

* Prepare a Raspberry Pi(4+) with the i2c libraries (See Preparation below) in the usual way.
* Copy the bme688.d folder to the **HOME** directory.
* Copy or move the headers folder to the **HOME** directory.
* To build the bme688 executable, run
 
```
cd bme688.d
make cleanall
make all
```
This is equivalent to

```
gcc -o bme688 bme688.c -I$HOME/headers/ -Wall -pedantic -std=c2x -D_GNU_SOURCE -li2c -lm
```

then run the executable with any options such as those presented below.
Set the link below for convenience if required.
The examples below terminate after operation.

```
./bme688 ?
./bme688 help
./bme688 registers
./bme688 -n 2
./bme688 -n 2 gas
./bme688 -n 2 gas debug

ln -s /home/(username)/bme688.d/bme688 ~/bin/bme688
```
--
### Quick Remove
To remove the whole lot, remove the directories bme688.d and headers. Nothing is saved elsewhere.

--
# Common Shared Memory

The centre of the world is a Common Shared Memory (or SHM). All interprocess communications, initialisation and raw data storage is conducted through shared memory.
Shared memory (sometime multiple) is used with a variety of sensors and displays (including SSD1306, TCA9548, TMP117, ICM20948 and others) together with the Raspberry Pi. 

By definition, when created, there is **NO STRUCTURE to shared memory**, only a size. Shared memory is just an allocated blob of run\-time main memory.

A common (union) data structure, in this case named **common_shmMap**, is logically overlay-ed on the shared memory. **All processes using that shared memory must use the same definition**. It can be a lot of fun if they don't.

For these applications and related processes, a 4K byte shared memory space is created (4096 (2\*\*12) bytes treated as 32 (2\*\*5) banks of 128bytes (2\*\*7)) and initialised to 0x00.

When a variable is accessed within the shared memory, it is simply an implicit length in bytes, a pointer to the variable together with a run\-time pointer to the shared memory. For example, int rawTemperature is a location starting at the (runtime pointer) shm address and offset address to the location of rawTemperature and going for 4 bytes. A double is 8 bytes. The structure of any variable is system dependent. This is very useful.

#### Common Shared Memory Simple Utilities ####

To manage, watch and test shared memory, a number of separate much smaller routines have been created. None of these routine need to be used. They are mostly informational. The shmC... functions are typically generic shared memory functions while the shmCbme688_... are orientated specifically to the bme688.

These routines are not normally used, and some functions are replicated.

The shmC... prefix refers to use of the C common shared memory. Other shared memory versions (A and B) are not used here.

```
shmCPrepare
```
Simply connects to or create a C common shared memory. Displays creation status and the first local memory block and common shared memory block.
The shared memory is left active after this utility terminates.

```
shmCbme688Prepare
```
This is effectively the same as above but populates a few of the bme688 related variables in the common shared memory. Will fail if the shared memory cannot be accessed. Displays status (0 for success) and exits.
Again the shared memory is left active.

```
shmCRelease
```
Zeroes and releases the current common shared memory. Nothing more.

```
shmCDisplay [n] where 0 < n < 33
```
Displays common shared memory banks between 0 and 31. Really only sensible to look at the first block in this case.

```
shmCbme688DisplayResults
```
More comprehensive display of common shared memory bank 0 (only) with offsets to expected bme688 and bme280 Temperature, Pressure, Humidity and (bme688) Gas results.

```
i2cDetect [adapter: default 1] [extend:0 or 1]
```
A local variation of the standard utility i2cdetect. Reads the device registers 0x08-0x7F compared to the standard i2cdetect range 0x08-0x77. There is an implied adapter address option, but 1 (/dev/i2c-1) is the default. The extend option (1) starts the display from device register 0x00. None of the awareness protections in i2cdetect exist in i2cDetect.

```
stop1 [1 or 0]
```
A simple routine to shutdown properly and terminate all associated shared processes.

--
# Keeping It Simple

* With the exception of the main procedure and the shared memory utilities above, all software described is contained in the file bme688Handlers.h.
 
* Everything is written in C. The software has been written and designed to be readable and therefore sometimes a little verbose. No hidden tricks, obfuscating code or short unreadable naming tricks are used.

* Although C pointers are used extensively, each is carefully named and the paths included where necessary. The use of system level libraries and the reason for a library inclusion is explained in the i2cIncludes.h file. For example, the definition for __u8 etc. in \<linux/i2c\-dev.h\>. Other convenient routines (many no longer used or used elsewhere but remain for interest) are contained in ./headers/i2cConvenients.h.

* No separate third party libraries are used.

* The procedures in bme688Handlers.h run on the Raspberry Pi and provide a variety of ways to configure and recover sensor data from the bme688 and save the results to a **Shared Memory** system for other processes such as SSD1306 displays to access. A GUI version of Raspbian can be used but all the software below runs from the command line. Processes to recover TPHG data from shared memory and display it using gnuplots are separate.

* All procedures in bme688Handlers.h are mostly documented (as best) step by step. The procedures are designed to be a framework of routines to be used together. Many can be used as standalone procedures but most bme688 related procedures are not as a series of global variables are used.

* Makefiles are used to compile into a local directory. Or you can use gcc directly. The c2x standard is used. C11 will show comment errors. No files are written into system areas. Several i2c related system libraries are preinstalled as described in the **Preparation** below.

<center><b>bme688Handlers.h Procedure Naming...</b></center>

 * Local procedures are typically begin with an \_ (underscore) character while major procedures typically begin with **bme688**...
 * Gas handling procedures typically begin with **bme688GasMan\_**...
 * Low level procedures are typically named **\_compensate**... for example while...
 * PARALLEL_MODE specific procedures begin with pm, for example **pmBeginParallelProcess**...
 * The Finite State Machine procedures begin with **_fsm**... 

 * The most important variable is of course the device file pointer, **fp**. If the device is not found, everything stops. The SETTER software (see separate section below) runs without looking for the device.

* May 2026 - Altered signal handling to use sigaction. Online documentation for sigaction is SOOO olde. Hopeless. AI no help. The bme688Handlers use simple traditional signal handling. Works. Timing alarms and user interrupt clean shutdown handling. Multiple user interrupt handling (^C ^C) is not handled and should be avoided. Unclean. Sigaction is active but calls the standard intHandler() from the saHandler() routine.

--
# Preparation

* The first and most obvious stage is to prepare a Raspberry Pi.
* Any version can be used but a RPi4 or faster with at least 1Gb of RAM is suggested. I haven't tried RPi3 or RPiZero. An RPi5 won't function any better than the RPi4 because of the bme688 design in this case.
* A reasonably fast SDRAM card 16Gb or better is suggested.
* Download a recent 64bit version of Raspbian Lite, currently Trixie, in the usual way and prepare the SDRAM. Strictly speaking, only Raspbian Lite is required but you can install a GUI version if you so desire. On a Mac Terminal screen, run

```
sudo dd bs=16m if=./2026-06-18-raspios-trixie-arm64-lite.img of=/dev/rdisk2
```

* or use balenaEtcher or similar.
 
* Install and boot the RPi.

```
sudo dpkg-reconfigure console-setup
```

* (For example, try UTF-8 -> Guess -> Terminus -> 12x24 framebuffer...
to set console font size from the default. Works for 27" and 32" monitors)

* Once started, log in and run the command line routine...

```
sudo raspi-config
```

* to set the host name (for example RPi44), wifi connection, enable interface->i2c and interface->ssh. All other options are you own. Reboot as required.
Run the following command (or ip -4 a (enter)) to find your local ip address for a local ssh connection and then complete all sensible updates...

```
ifconfig
```

```
sudo apt update
sudo apt upgrade -y
sync
sudo apt dist-upgrade
sudo apt autoremove
sync
```

* Create /etc/sudoers.d/010_pi-nopasswd and put in... (yes I know that the username pi shouldn't be used but its convenient!)

```
pi ALL=(ALL) NOPASSWD: ALL  
```
```
sudo shutdown -r now  
```

* Reconnect and...

```
sudo apt install i2c-tools  
sudo apt-get install libi2c-dev   
sudo apt-get install samba  
```

* Samba is an optional convenience. It can be used to transfer files between Apple Macs (both High Sierra and Tahoe) and other Linux systems. Everything is relatively private and fast. In /etc/samba/smb.conf, my global options are...

```
[global]
   workgroup...
   
------- LOCAL ADDITION -------
security = user
encrypt passwords = true
# smb v4.13 or earlier
smb encrypt = required
unix extensions = no
allow insecure wide links = yes
veto files = /._*/.DS_Store/
------- LOCAL ADDITION -------
```

* and further down in the Share Definitions section...
(If you like, disable [homes] and all associated options.
Simplifies specifically named shares across multiple RPis)

```
[RPi5Trixie]       <--- arbitrary name for example...
    comment = pi profiles  
    path = /home/pi  
    guest ok = yes
    writeable = yes
    browseable = yes
    create mask = 0644]
    directory mask = 0755
    follow symlinks = yes
    wide links = yes
```

* Once you have Samba working, set the smb password(s) for the user(s), pi in this case...

```
sudo smbpasswd -a pi (enter)
```

--
# Fast i2c interface...

* Now, to use a fast i2c interface, edit the file /boot/firmware/config.txt and add...

```
# i2c  
dtparam=i2c=on,i2c_baudrate=400000
```

* There appears to be whole pile of variations of this exercise to achieve the same result. This works at the moment (Trixie 2026) but check.

--
# Basic bme688 Operations...

* The bme688 has 3 modes of operation...
  * OFF\_MODE... (ignore)
  * FORCED\_MODE... 
  * PARALLEL\_MODE.

* The bme688Handlers software below also has several operating modes...
  * An Individual Command Line RUNNING the bme688 device directly... 
  * An Individual Command Line running a **SETTER** mode operation, then terminates.
  * **Finite State Machine (fsm)** mode running either FORCED\_MODE and/or PARALLEL\_MODE. See separate section below.

* The Individual Command Line format (in BNF with [ ]s) is...

```
bme688 [-n nn] [gas] [debug] [debug] [debug] [nodisplay] [-a adapterAddress][-b slaveAddress]...
[-d][-D][-h][-p][-s][-t] [0,1,2,4,8(default),16] [-v] [-i] [-gn] [-gt] [-gw] [-l] and others, see help...
```

* The bme688 Temperature Pressure Humidity (TPH) without gas is the default operation. A simplest command would be...

```
bme688 (enter)
```

*  The output TPH (no gas) data display style set to 1 (default).
*  loop_count is 1 (default).
*  loop_time is 1000mS(default).
* A variation of the above command example would be...
 
```
bme688 [-n 2] [-s 0] gas \[debug] (enter)
```

* gas: the keyword **gas** used to activate a FORCED_MODE gas operation.
* There is a default 10 second TIMEOUT for each gas operation.
* The gas timeout is asynchronous and does not affect the TPH loop timeout.
* If -n loop\_counter x (multiplied by) -l loop\_delay is greater than the gas timeout, then the FORCED_MODE gas operation repeats.

<center><b>SETTER OPERATIONS</b></center>

* There are several pre-processing requirements before a gas operation can start, these are defined using **SETTER** operations below.

* All SETTER commands terminate after the option is set. The values are stored in bme688 section of SHARED MEMORY and loaded into the bme688 when a RUN command is executed. 

* SETTER GasMan: active\_res\_heat and active\_wait\_time MUST be set prior to any gas operation. There are simple default settings for res\_heat\_temp (100 degrees) and gas\_wait\_time (190mS) used for testing purposes.
 * A SETTER command example would be bme688 -gw 200 -gt 100 (enter)
 * A RUN    command example would be bme688 -n 8 gas [-s 1] [debug] (enter)
 * -h  : set over sampling rate 1x 2x 4x 8x 16x for humidity sensor in shared memory, then terminate.
 * -p  : set over sampling rate 1x 2x 4x 8x 16x for pressure sensor in shared memory, then terminate.
 * -t  : set over sampling rate 1x 2x 4x 8x 16x for temperature sensor in shared memory, then terminate.
 * -i  : set the IIR Filter Coefficient 0, 1(default) 3, 7, 15, 31, 63, 127 in shared memory, then terminate.
 * -d  : set default osrs for all sensors in shared memory, then terminate.
 * -D  : set minimum osrs for all sensors in shared memory, then terminate.
 * -gn : set GasMan nb_conv (0..9), then terminate.
 * -gt : set GasMan current active_res_heat in degrees Celsius (0..MAXTEMP), then terminate.
 * -gw : set GasMan current active_gas_wait in mS (0..4032), then terminate.
 * -l  : set Loop time mS or Seconds for runLoop in shared memory, then terminate. Use 0.5(mS), 62.5, 125, 250, 500, 1000(mS)(default) or 1(s), 10, 20 (as used in bme280).
 * -n  : nn set loop mode [0 for infinite or nn seconds] and proceeds to the runLoop Use bme688 -n 0 to run continuously (Forced mode with cycle time 1Hz)...
 * -s  : style: set the output display style [0..6], default(1). style is NOT saved and must be set for each run.
 * -v  : Display all current device registers, then terminate.
 * registers : Display all current device registers, then terminate.
 * -?  : Display this help summary
 * ?   : Display this help summary
 * Arguments are processed in argument order. All other arguments are ignored.

* There are 3 debug levels...
 * Use bme688 [...] debug (enter) to run in DEBUG mode.
 * Use bme688 [...] debug debug (enter) to run in additional information DEBUG mode.
 * Use bme688 [...] debug debug debug (enter) to run in extreme detail DEBUG mode.
 * Use bme688 nodisplay : to stop showing the results as they are received. Final results shown.
 
* For example...
 * Use bme688 -gw 200 -gt 100 (enter) to set a basic gas setting, then...
 * Use bme688 -n -6 -s 1 gas (enter) for example to get a result.
 * Use stop2 or ^C for a controlled interrupt...
  
```
bme688 -n 2 (enter) will display...
Temperature: 18.38°, QNH: 1025.61hPa, Humidity: 60.85%, Dew Point: 10.69°
Temperature: 18.37°, QNH: 1025.61hPa, Humidity: 60.82%, Dew Point: 10.67°
```

--
# Additional i2c Support...
 
* Use ``` i2cdetect -y 1 ``` to find the i2c device
* or use ``` bme688 scan ``` (only adapter /dev/i2c-1) to see a local variation of i2cdetect -y 1
* Use ``` bme688 [-s 6] or [-v] or [registers] ``` to see ALL the current device registers. The green registers are the active static registers.
* There are other i2c scanning (i2cDetect) routines. See other sources.

--
# Finite State Machine (FSM) mode
 
* Given that the bme688 can run either continuously \(PARALLEL\_MODE) or under a FORCED\_MODE and thus driven by an external controller, the Raspberry Pi provides a configurable and controllable FSM handler. All **States** are defined in a file with various options to configure and operate the bme688.

* Although this is called a Finite State Machine, it is not an academic theoretical FS machine or Turing machine. It uses a user defined **config file** which define states each with a (non critical) state number and a series of options defined for each state. The NEXT state is used if an ANOMALY is detected or a loop timeout. ANOMALY routines are entered to control other hardware or processes if required. There are a range of ANOMALY types based on each sensor. The FSM steps from state to state as required. If there is an error the whole machine simply stops, shuts down the bme688 and terminates. No attempt is made to correct any runtime errors or restart.

* It is very easy to create a (silly) gas state that fails. Range errors are detected at config load time but even so, a configuration can be used to violate the bme688 and potentially destroy or ruin it. For example, setting a gas\_wait\_time of 4032mS \(GASMAN\_MAX\_WAIT\_TIME\_SHARED) and a res\_heat\_temp of 360 (GASMAN\_MAX\_TEMPERATURE) repeatably would be foolish and would potentially destroy the device. Don't use silly configurations.
 
* FSM mode takes priority over individual [with or without gas] Mode. The command line format to select Finite State Machine and optionally the config fileName is...

```
bme688 [fsm] [-f fsmName] [-s 0..6] [debug] [debug] [debug] (enter)
```

to select Finite State Machine mode and optionally the config fileName...

* ALL parameters are set in the [default] EDITABLE TEXT file **bme688FiniteStateConfiguration.txt**. Use bme688 fsm (enter) for this default.

* There are a series of TEST fsmName config files in the **./tests/** directory.

* fsm mode: The -s [0..6] selects the display output style. 0 is minimal, 1 is the default.
For example, use...

```
 bme688 -f 2State6LoopsStopOnAnomaly.txt (enter)
```

The example file (./tests/2State6LoopsStopOnAnomaly.txt) is...

```
# --- --- --- ---
# LOOP2x6...
# Test file to loop 6 times, jump to second state and loop again, then terminate.
# Control C to interrupt.
# Terminate on a TEMPERATURE ABOVE anomaly.
# Terminate on a PRESSURE ABOVE ANOMALY.
# --- --- --- ---
# All line containing the # character are ignored no matter where the character is.
# Copy and past each state group and adjust the values to suit.
# Rename each group State numbers. If there is a duplicate state number,
# the second is used and the first is ignored.
# The state numbers can any unique value but its not vital.
# The state keyword is used to create a new linked list node.
#
# state 1 <- unique integer between 0 and say 64 (upper limit is alterable)
# sensor 0 <- 0 is a TPH operation,  1 is a TPHG operation. Default/error is 0.
# loop 4 <- repeat this state n times before s->next state unless an anomaly.
# loop_delay 1000 <- loop cycle time in mS. Minimum set to 250mS. No sensible max.
# next 2 <- next STATE as defined by the state variable. If invalid, everything stops.
# prev 0 <- linked list state caller.
#
# anomalyValue 9999 <- value recovered in ohms to trigger an nextOnAnomaly state change.
# anomalyValue1 <- low range value. 
# anomalyValue2 <- upper range value. 
#
# anomaly 1 <- Type of anomaly detected for this State. T(1)P(2)H(3)G(4) values.
# anomaly 1 temperature anomaly
# anomaly 2 pressure anomaly
# anomaly 3 humidity anomaly
# anomaly 4 gas anomaly
# anomaly 0 and anything else ignored
#
# anomalyCondition 1 greater than, -1 less than, 2 outside range anomalyValue1 and anomalyValue2
# nextOnAnomaly 1 <- Alternate NEXT state after an anomaly. If invalid, stop.
#
# gas_wait_time 0 <- wait time defined between 0 and 4032mS as per document.
# res_heat_temp 0 <- heater temperature between 0 and MAXTEMP (350 at the moment)
- <- convenient state definition separator, could also use #
#
# If any option is missing, a default (usually 0) is set.
# The default loop_delay is 1000mS.
# The default loop count is 1 completed cycle, then termination.
# The default s->next is NULL which terminates.
# The default s->nextOnAnomaly is NULL which terminates.
# The default sensor operation is TPH only.
#
# OPTIONAL parameters...
#
# There are 3 modes of operation, sleep, forced or parallel mode.
# FORCED_MODE is always the default operating mode unless the...
# descriptor "parallel" is included AFTER THE DESCRIPTOR profile.
#
# Can specify the starting register set in forced mode using nb_conv
# or the numeric sequence required in parallel mode.
# nb_conv 0..9. nb_conv default is 0.
#
# sensor (see above) is also an optional directive. The default(0) is a TPH (no gas) operation.
# style <- 0..5, selects the display format required. 1 is the default for detailed display.
-
state 1
sensor 0
loop 6
loop_delay 1000
next 2
anomaly 1
anomalyCondition 1
anomalyValue 18
nextOnAnomaly 3
style 1
-
state 2
sensor 0
loop 6
loop_delay 1000
next 3
anomaly 2
anomalyCondition 1
anomalyValue 1026
nextOnAnomaly 3
style 1
-
state 3
terminate 1
-
```

Here is another example, this time using the FSM with a Parallel Mode operation...

```
#
# PARALLEL Test.
#
# All line containing the # character are ignored no matter where the character is.
# Copy and past each state group and adjust the values to suit.
# Rename each group State numbers. If there is a duplicate state number,
# the second is used and the first is ignored.
# The state numbers can any unique value but its not vital.
# The state keyword is used to create a new linked list node.
#
# state 1 <- unique integer between 0 and say 64 (upper limit is alterable)
# sensor 0 <- 0 is a TPH operation,  1 is a TPHG operation. Default/error is 0.
# loop 4 <- repeat this state n times before s->next state unless an anomaly.
# loop_delay 1000 <- loop cycle time in mS. Minimum set to 250mS. No sensible max.
# next 2 <- next STATE as defined by the state variable. If invalid, everything stops.
# prev 0 <- linked list state caller.
#
# anomalyValue 9999 <- value recovered in ohms to trigger an nextOnAnomaly state change.
# anomaly 1 <- Type of anomaly detected for this State. T(1)P(2)H(3)G(4) values.
# anomaly 0 and anything else ignored
# anomaly 1 temperature anomaly
# anomaly 2 pressure anomaly
# anomaly 3 humidity anomaly
# anomaly 4 gas anomaly
# anomalyCondition 1 greater than, -1 less than, 2 outside range anomalyValue1 and anomalyValue2
# nextOnAnomaly 1 <- Alternate NEXT state after an anomaly. If invalid, stop.
#
# gas_wait_time 0 <- wait time defined between 0 and 4032mS as per document.
# res_heat_temp 0 <- heater temperature between 0 and MAXTEMP (350 at the moment)
- <- convenient state definition separator, could also use #
#
# If any option is missing, a default (usually 0) is set.
# The default loop_delay is 1000mS.
# The default loop count is 1 completed cycle, then termination.
# The default s->next is NULL which terminates.
# The default s->nextOnAnomaly is NULL which terminates.
# The default sensor operation is TPH only.
#
# OPTIONAL parameters...
#
# There are 3 modes of operation, sleep, forced or parallel mode.
# FORCED_MODE is always the default operating mode unless the...
# descriptor "parallel" is included AFTER THE DESCRIPTOR profile.
#
# Can specify the starting register set in forced mode using nb_conv
# or the numeric sequence required in parallel mode.
# nb_conv 0..9. nb_conv default is 0.
#
# sensor (see above) is also an optional directive. The default(0) is a TPH (no gas) operation.
-
state 1
sensor 0
loop 2
loop_delay 1000
next 6
nextOnAnomaly 6
anomaly 1
anomalyValue1 31
anomalyCondition 1
# gas_wait_time 0 <- Resets any previous entry as the values are stored in shared memory.
# res_heat_temp 0 <- Resets any previous entry as the values are stored in shared memory.
gas_wait_time_shared 0
gas_wait_time 0
res_heat_temp 0
nb_conv 0
style 1
-
state 4
sensor 1
loop 2
loop_delay 1000
next 6
nextOnAnomaly 6
anomaly 1
anomalyValue1 32
anomalyCondition 1
res_heat_temp 100
gas_wait_time 250
gas_cycle_time 12
style 1
-
state 3
terminate 1
-
state 5
terminate 1
-
State 6
sensor 1
loop 1
loop_delay 1000
next 5
nextOnAnomaly 5
# gas anomaly 4
anomaly 4
anomalyCondition 10000
# gas_wait_time_shared 100 in mS rather than the internal timersteps... 0.477mS
# gas_wait_time_shared 190 This matches the gas_wait_time values in 1mS units.
gas_wait_time_shared 190
gas_cycle_time 12
# nb_conv 3 ... 0..2 ... 0 and 1 mean 1 step. 2 == 2.
nb_conv 6
style 1
profile
# The format required is index, temp, wait(0..4032) or multiplier(0..255), idac
0 100 1 0
1 100 2 0
2 100 1 0
3 100 2 0
4 100 1 0
5 100 1 0
6 0 0 0
7 0 0 0
8 0 0 0
9 0 0 0
profile
parallel
-
```

--
# Gas and GasMan Operations...

The gas sensor on the bme688 is a heating sensor that operates when specific res\_heat\_temp (registers 0x54 to 0x63) and gas\_wait\_times (registers 0x64 to 0x6C) are set and run\_gas (register 0x71:5) is enabled. Optionally idac_heat (registers 0x50 to 0x59) can be set with initial values.
 
Unfortunately, the recovered raw gas values are not linear and several parameters are required for further processing of the gas information.

**Raw Gas Data Registers**

Table 17 shows the locations of the 2 part raw ADC values and the associated gas\_range registers and the status bits.

* Field 0: ADC: 0x2C << 2 + 0x2D:7,6 >> 6 for lsb, range 0x2D:3-0
* Field 1: ADC: 0x3D << 2 + 0x3E:7,6 >> 6 for lsb, range 0x3E:3-0
* Field 2: ADC: 0x4E << 2 + 0x4F:7,6 >> 6 for lsb, range 0x4F:3-0

(Documentation error: Incorrect descriptor for gas\_r\_lsb(0) Field 0: register 0x2D)

The lsb raw data registers also contain the gas status bits...

* Field 0: 0x2D: gas\_valid: bit 5, heat\_stable: bit 4 
* Field 1: 0x3E: gas\_valid: bit 5, heat\_stable: bit 4
* Field 2: 0x4F: gas\_valid: bit 5, heat\_stable: bit 4

The same lsb raw data registers also contain the gas_range value...

* Field 0: 0x2D: bits 3-0
* Field 1: 0x3E: bits 3-0
* Field 2: 0x4F: bits 3-0 

All these have to be stripped out and tested at the specific times. This is straightforward but annoying. Not until the Data Ready flags (Field 0:0x1D:7, Field 1:0x2E:7 and Field 2:0x3F:7) are valid are the values above stable.

On top of all that, given the polling interface for this i2c system, one has to assess not only the valid\_data bit, but successful gas_measuring and measuring status bits
(0:0x1D:6,5, 1:0x2E:6,5, 2:0x3F:6,5) before recovering any raw data. The raw data bits cannot be saved and reused.

Once that is achieved, the stable raw gas values and the associated gas\_range\_r values can be recovered. Be aware, the gas\_range\_r value is not set at stable time and new data available time but at new_data available only.

After that, an external algorithm (Document page 28, Section 3.7.1) takes over the raw gas values to convert the raw result into a resistance value in OHMS. This is rather a strange algorithm depending on on 3 calibration parameters stored in undocumented registers (0xEB, 0xEC, 0xED and 0xEE) not included in the main register memory map (Document pages 33 and 34, Section 5.2) but indirectly referenced in Document page 28, Table 16.

In addition, to set the res\_heat\_temp registers, the res\_heat\_range registers at location 0x00 and 0x02 are also required. Again, these registers are undocumented and unreferenced in the global table but are specified in Document page 28, Table 16, res\_heat\_range (0x02:5,4) and res\_heat\_val (0x00)

The use of location 0x00 is definitely a design afterthought as normally for i2c devices, the low registers are used for identification purposes.

**Raw Gas Results**

Given all that, it was decided that the raw data and the gas range would be the only data saved in the **shared memory**. Additional processing such as display or the post processing algorithm can be done at anytime following the raw data recovery. Thus, in this case, the conversion is not done unless the option to display the OHMS value is selected. (Error on pg. 29 for the input values required, ADC... see 0.)

It was noted that the OHMS algorithm results swing wildly in value. It is assumed that the values are required for the internal BOSCH Air Quality Sensor API (see Document) and which is not available here.

For interest and testing, approximate(!) Parallel Mode algorithms as visually presented in the bme688 Document Figure 1, p. 16 and Figure 5, p. 22 have been created and stored in the tests directory. **These are experimental only**, but seem to work. Again they cannot be tested against either the internal API or with real air samples as I don't have access to such polluted air samples as suggested in Document p. 11 Table 6.

--
# End Statement

There are a huge number of operating options for the bme688 and hopefully they are covered in the settings. Use at your own risk. Hopefully, there won't be too many bugs and not too many errors in this document!

--
# Author

* Author: Bruce Stephens.
* Country of origin: Australia.
* Most Recent Update: August 2026.
* Version Number: soon...
* Errors : bruce @ eigenspaces.com.au

--