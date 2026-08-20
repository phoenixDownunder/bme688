# --- --- --- ---
# TEST FILES...
# The files in this test directory are simply TEST and EXAMPLE files...
# for the bme688 operating in FSM mode.
#
# Most normally terminate but can be stopped with a control Control C interrupt.
# --- --- --- ---
#
# All lines containing the # character are ignored no matter where the character is.
- <- convenient state definition separator, could also use #
# 
# To use, copy and past each state group and adjust the values to suit.
# Rename each group State numbers. If there is a duplicate state number,
# the second is used and the first is ignored.
#
# The state numbers can any unique value but its not vital.
# A state keyword is used to create a new linked list node.
#
# state 1 <- unique integer between 0 and say 64 (MAXSTATECOUNT upper limit is alterable)
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
# anomaly 1 or 2 or 3 or 4 <-- Type of anomaly detected for this State. T(1)P(2)H(3)G(4).
# anomaly 1 temperature anomaly in degree Celcius C.
# anomaly 2 pressure anomaly in HECTOPASCALS HPa.
# anomaly 3 humidity anomaly as a percentage between 0 and 0.999
# anomaly 4 gas anomaly in ohms.
# anomaly 0 and anything else ignored
#
# anomalyCondition -1 or 1 or 2
# anomalyCondition 1 greater than, -1 less than, 2 outside range anomalyValue1 and anomalyValue2
# nextOnAnomaly 1 <- Alternate NEXT state after an anomaly. If invalid, stop.
#
# gas_wait_time 0 <- wait time defined between 0 and 4032mS as per document.
# res_heat_temp 0 <- heater temperature between 0 and GASMAN_MAX_TEMPERATURE (currently 360C)
#
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
# The BME688 has 3 modes of operation, sleep, forced or parallel mode.
# FORCED_MODE is always the default operating mode unless the...
# descriptor "parallel" is included AFTER THE DESCRIPTOR profile.
# PARALLEL_MODE...
# NOTE AGAIN: descriptor "parallel" is included AFTER THE DESCRIPTOR profile.
#
# Must also specify the starting register set in forced mode using nb_conv
# or the numeric sequence count required in parallel mode.
# nb_conv 0..9. nb_conv default is 0.
#
# style <- 0..5, selects the display format required. 1 is the default for detailed display.
#
# EXAMPLE: 2 working States plus terminator state(3)
-
# State number and automatically the START state regardless of state number.
state 1
# sensor 0 means the TPH mode only, no G gas operation.
sensor 0
# loop 6 means to loop 6 times before branching to the NEXT state unless anomaly.
loop 6
# loop_delay 1000 Each loop has a loop time of 1000mS (Minimum is 250mS)
loop_delay 1000
# next 2             <-- the next state number this state will normally branch to.
next 2
# anomaly 1          <-- means a TEMPERATURE anomaly.
anomaly 1
# anomalyCondition 1 <-- means to branch (to anomaly address) if this temperature is exceeded.
anomalyCondition 1
# anomalyValue 18    <-- In this case, branch if temperature of 18C is exceeded.
anomalyValue 18
# nextOnAnomaly 3    <-- The next state (anomaly state) if an anomaly is detected.
nextOnAnomaly 3
# style 1 A simple directive to change the default output style, used for data handling and debug.
style 1
-
- <- as above, convenient state definition separator, could also use #
-
state 2
sensor 0
loop 6
loop_delay 1000
next 3
# anomaly 2          <--  means PRESSURE anomaly.
anomaly 2
anomalyCondition 1
# anomalyValue 1026  <-- In this case, branch if pressure (QNH) is above 1026HPa.
anomalyValue 1026
nextOnAnomaly 3
style 1
-
state 3
# terminate 1. This is a terminate state only. Anything else is ignored.
terminate 1
-
