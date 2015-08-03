/*
    This file is a part of the wiringBone library
    Copyright (C) 2015 Abhraneel Bera

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/


#include "PRU1.hp"

.macro INIT
  // Clear all registers
  ZERO &register, SIZE(register)

  // Clear all output bits
  MOV OUTPUT, 0x0

  // Initialize starting time
  LBCO CURRENT_TIME, IEP, COUNT, 4

  // Initialize PREV_TOGGLE (for input) and NEXT_TOGGLE (for output) with current time
  MOV register.P8_45_toggle, CURRENT_TIME
  MOV register.P8_46_toggle, CURRENT_TIME
  MOV register.P8_43_toggle, CURRENT_TIME
  MOV register.P8_44_toggle, CURRENT_TIME
  MOV register.P8_41_toggle, CURRENT_TIME
  MOV register.P8_42_toggle, CURRENT_TIME
  MOV register.P8_39_toggle, CURRENT_TIME
  MOV register.P8_40_toggle, CURRENT_TIME
  MOV register.P8_27_toggle, CURRENT_TIME
  MOV register.P8_29_toggle, CURRENT_TIME
  MOV register.P8_28_toggle, CURRENT_TIME
  MOV register.P8_30_toggle, CURRENT_TIME
  MOV register.P8_21_toggle, CURRENT_TIME
  MOV register.P8_20_toggle, CURRENT_TIME
  MOV register.P9_26_toggle, CURRENT_TIME

.endm

.macro PROCESS_OUTPUT
.mparam PIN, TON_OFFSET, TOFF_OFFSET, NEXT_TOGGLE

  // Check for toggle
  SUB TEMP, NEXT_TOGGLE, CURRENT_TIME
  QBBC procees_output_end, TEMP.t31

  // Load Ton and T
  LBCO T_ON, RAM, TON_OFFSET, 4
  LBCO T_OFF, RAM, TOFF_OFFSET, 4

  // Check full high and full low condition
  QBEQ out_low, T_ON, 0
  QBEQ out_high, T_OFF, 0

  // Process low if pin already high, else process high
  QBBS out_low, PIN

  out_high:
    // Next toggle after Ton time
    ADD NEXT_TOGGLE, NEXT_TOGGLE, T_ON

    // Check if pin already set
    QBBS procees_output_end, PIN

    // Set pin
    SET PIN

    JMP procees_output_end

  out_low:
    // Next toggle after Toff time
    ADD NEXT_TOGGLE, NEXT_TOGGLE, T_OFF

    // Check if pin already cleared
    QBBC procees_output_end, PIN

    // Clear pin
    CLR PIN

procees_output_end:
.endm

.macro PROCESS_INPUT
.mparam PIN, TON_OFFSET, TOFF_OFFSET, PREV_TOGGLE, LAST_STATE, CURRENT_STATE

  // Process high if pin is high, else process low
  QBBS in_high, CURRENT_STATE

  in_low:
    // Skip if previous state is low
    QBBC process_timeout, LAST_STATE

    // Store Ton
    SUB T_ON, CURRENT_TIME, PREV_TOGGLE
    SBCO T_ON, RAM, TON_OFFSET, 4

    // Set previous toggle == current time
    MOV PREV_TOGGLE, CURRENT_TIME

    JMP process_input_end

  in_high:
    // Skip if previous state is high
    QBBS process_timeout, LAST_STATE

    // Store Toff
    SUB T_OFF, CURRENT_TIME, PREV_TOGGLE
    SBCO T_OFF, RAM, TOFF_OFFSET, 4

    // Set previous toggle == current time
    MOV PREV_TOGGLE, CURRENT_TIME

    JMP process_input_end

process_timeout:
  // Check if (current time - previous toggle time) has reached timeout
  SUB TEMP, CURRENT_TIME, PREV_TOGGLE
  SUB TEMP, T_TIMEOUT, TEMP
  QBBC process_input_end, TEMP.t31

  // Set Ton and Toff values to zero
  MOV TEMP, 0
  SBCO TEMP, RAM, TON_OFFSET, 4
  SBCO TEMP, RAM, TOFF_OFFSET, 4

  // Set previous toggle == current time
  // Though toggle not occured, still to avoid any garbage reading when pulse is restored
  MOV PREV_TOGGLE, CURRENT_TIME

process_input_end:

.endm

.macro PROCESS_FAILSAFE
.mparam LOOP_START_TIME

  // Store approx. loop execution time in TEMP
  LBCO TEMP, IEP, COUNT, 4
  SUB TEMP, TEMP, LOOP_START_TIME

  // Add loop execution time to Watchdog
  LBCO T_TIMEOUT, RAM, WATCHDOG, 4
  ADD T_TIMEOUT, T_TIMEOUT, TEMP

  // Check if failsafe occured
  MOV TEMP, WATCHDOG_TIMEOUT
  SUB TEMP, TEMP, T_TIMEOUT
  QBBC process_failsafe_end, TEMP.t31

  // Set failsafe Ton and Toff values to respective pins
  LBCO T_ON, RAM, FAILSAFE_T_ON, 8

  SBCO T_ON, RAM, P8_45_TON, 8
  SBCO T_ON, RAM, P8_46_TON, 8
  SBCO T_ON, RAM, P8_43_TON, 8
  SBCO T_ON, RAM, P8_44_TON, 8
  SBCO T_ON, RAM, P8_41_TON, 8
  SBCO T_ON, RAM, P8_42_TON, 8
  SBCO T_ON, RAM, P8_39_TON, 8
  SBCO T_ON, RAM, P8_40_TON, 8
  SBCO T_ON, RAM, P8_27_TON, 8
  SBCO T_ON, RAM, P8_29_TON, 8
  SBCO T_ON, RAM, P8_28_TON, 8
  SBCO T_ON, RAM, P8_30_TON, 8
  SBCO T_ON, RAM, P8_21_TON, 8
  SBCO T_ON, RAM, P8_20_TON, 8

  // Reset the Watchdog value
  MOV TEMP, 0x0
  SBCO TEMP, RAM, WATCHDOG, 4

process_failsafe_end:
  // Store incremented Watchdog value
  SBCO T_TIMEOUT, RAM, WATCHDOG, 4

.endm

.origin 0

INIT
PROCESS_MAIN:

  // Load enable and mode bits
  LBCO ENABLE, RAM, PIN_ENABLE, 4
  LBCO MODE, RAM, PIN_MODE, 4

  // Load current time
  LBCO CURRENT_TIME, IEP, COUNT, 4

  // Load pulse read timeout
  LBCO T_TIMEOUT, RAM, TIMEOUT, 4

  // Load current state
  MOV CURRENT_INPUT_FRAME, INPUT

  // P8_45

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_45, P8_45_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_45_OUT, P8_45_MODE

    // Process output
    PROCESS_OUTPUT P8_45_OUT, P8_45_TON, P8_45_TOFF, P8_45_NEXT_TOGGLE
    JMP SKIP_P8_45

SKIP_P8_45_OUT:
    // Process input
    PROCESS_INPUT P8_45_IN, P8_45_TON, P8_45_TOFF, P8_45_PREV_TOGGLE, P8_45_LAST_STATE, P8_45_CURRENT_STATE

SKIP_P8_45:

  // P8_46

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_46, P8_46_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_46_OUT, P8_46_MODE

    // Process output
    PROCESS_OUTPUT P8_46_OUT, P8_46_TON, P8_46_TOFF, P8_46_NEXT_TOGGLE
    JMP SKIP_P8_46

SKIP_P8_46_OUT:
    // Process input
    PROCESS_INPUT P8_46_IN, P8_46_TON, P8_46_TOFF, P8_46_PREV_TOGGLE, P8_46_LAST_STATE, P8_46_CURRENT_STATE

SKIP_P8_46:

  // P8_43

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_43, P8_43_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_43_OUT, P8_43_MODE

    // Process output
    PROCESS_OUTPUT P8_43_OUT, P8_43_TON, P8_43_TOFF, P8_43_NEXT_TOGGLE
    JMP SKIP_P8_43

SKIP_P8_43_OUT:
    // Process input
    PROCESS_INPUT P8_43_IN, P8_43_TON, P8_43_TOFF, P8_43_PREV_TOGGLE, P8_43_LAST_STATE, P8_43_CURRENT_STATE

SKIP_P8_43:

  // P8_44

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_44, P8_44_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_44_OUT, P8_44_MODE

    // Process output
    PROCESS_OUTPUT P8_44_OUT, P8_44_TON, P8_44_TOFF, P8_44_NEXT_TOGGLE
    JMP SKIP_P8_44

SKIP_P8_44_OUT:
    // Process input
    PROCESS_INPUT P8_44_IN, P8_44_TON, P8_44_TOFF, P8_44_PREV_TOGGLE, P8_44_LAST_STATE, P8_44_CURRENT_STATE

SKIP_P8_44:

  // P8_41

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_41, P8_41_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_41_OUT, P8_41_MODE

    // Process output
    PROCESS_OUTPUT P8_41_OUT, P8_41_TON, P8_41_TOFF, P8_41_NEXT_TOGGLE
    JMP SKIP_P8_41

SKIP_P8_41_OUT:
    // Process input
    PROCESS_INPUT P8_41_IN, P8_41_TON, P8_41_TOFF, P8_41_PREV_TOGGLE, P8_41_LAST_STATE, P8_41_CURRENT_STATE

SKIP_P8_41:

  // P8_42

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_42, P8_42_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_42_OUT, P8_42_MODE

    // Process output
    PROCESS_OUTPUT P8_42_OUT, P8_42_TON, P8_42_TOFF, P8_42_NEXT_TOGGLE
    JMP SKIP_P8_42

SKIP_P8_42_OUT:
    // Process input
    PROCESS_INPUT P8_42_IN, P8_42_TON, P8_42_TOFF, P8_42_PREV_TOGGLE, P8_42_LAST_STATE, P8_42_CURRENT_STATE

SKIP_P8_42:

  // P8_39

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_39, P8_39_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_39_OUT, P8_39_MODE

    // Process output
    PROCESS_OUTPUT P8_39_OUT, P8_39_TON, P8_39_TOFF, P8_39_NEXT_TOGGLE
    JMP SKIP_P8_39

SKIP_P8_39_OUT:
    // Process input
    PROCESS_INPUT P8_39_IN, P8_39_TON, P8_39_TOFF, P8_39_PREV_TOGGLE, P8_39_LAST_STATE, P8_39_CURRENT_STATE

SKIP_P8_39:

  // P8_40

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_40, P8_40_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_40_OUT, P8_40_MODE

    // Process output
    PROCESS_OUTPUT P8_40_OUT, P8_40_TON, P8_40_TOFF, P8_40_NEXT_TOGGLE
    JMP SKIP_P8_40

SKIP_P8_40_OUT:
    // Process input
    PROCESS_INPUT P8_40_IN, P8_40_TON, P8_40_TOFF, P8_40_PREV_TOGGLE, P8_40_LAST_STATE, P8_40_CURRENT_STATE

SKIP_P8_40:

  // P8_27

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_27, P8_27_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_27_OUT, P8_27_MODE

    // Process output
    PROCESS_OUTPUT P8_27_OUT, P8_27_TON, P8_27_TOFF, P8_27_NEXT_TOGGLE
    JMP SKIP_P8_27

SKIP_P8_27_OUT:
    // Process input
    PROCESS_INPUT P8_27_IN, P8_27_TON, P8_27_TOFF, P8_27_PREV_TOGGLE, P8_27_LAST_STATE, P8_27_CURRENT_STATE

SKIP_P8_27:

  // P8_29

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_29, P8_29_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_29_OUT, P8_29_MODE

    // Process output
    PROCESS_OUTPUT P8_29_OUT, P8_29_TON, P8_29_TOFF, P8_29_NEXT_TOGGLE
    JMP SKIP_P8_29

SKIP_P8_29_OUT:
    // Process input
    PROCESS_INPUT P8_29_IN, P8_29_TON, P8_29_TOFF, P8_29_PREV_TOGGLE, P8_29_LAST_STATE, P8_29_CURRENT_STATE

SKIP_P8_29:

  // P8_28

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_28, P8_28_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_28_OUT, P8_28_MODE

    // Process output
    PROCESS_OUTPUT P8_28_OUT, P8_28_TON, P8_28_TOFF, P8_28_NEXT_TOGGLE
    JMP SKIP_P8_28

SKIP_P8_28_OUT:
    // Process input
    PROCESS_INPUT P8_28_IN, P8_28_TON, P8_28_TOFF, P8_28_PREV_TOGGLE, P8_28_LAST_STATE, P8_28_CURRENT_STATE

SKIP_P8_28:

  // P8_30

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_30, P8_30_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_30_OUT, P8_30_MODE

    // Process output
    PROCESS_OUTPUT P8_30_OUT, P8_30_TON, P8_30_TOFF, P8_30_NEXT_TOGGLE
    JMP SKIP_P8_30

SKIP_P8_30_OUT:
    // Process input
    PROCESS_INPUT P8_30_IN, P8_30_TON, P8_30_TOFF, P8_30_PREV_TOGGLE, P8_30_LAST_STATE, P8_30_CURRENT_STATE

SKIP_P8_30:

  // P8_21

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_21, P8_21_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_21_OUT, P8_21_MODE

    // Process output
    PROCESS_OUTPUT P8_21_OUT, P8_21_TON, P8_21_TOFF, P8_21_NEXT_TOGGLE
    JMP SKIP_P8_21

SKIP_P8_21_OUT:
    // Process input
    PROCESS_INPUT P8_21_IN, P8_21_TON, P8_21_TOFF, P8_21_PREV_TOGGLE, P8_21_LAST_STATE, P8_21_CURRENT_STATE

SKIP_P8_21:

  // P8_20

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_20, P8_20_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_20_OUT, P8_20_MODE

    // Process output
    PROCESS_OUTPUT P8_20_OUT, P8_20_TON, P8_20_TOFF, P8_20_NEXT_TOGGLE
    JMP SKIP_P8_20

SKIP_P8_20_OUT:
    // Process input
    PROCESS_INPUT P8_20_IN, P8_20_TON, P8_20_TOFF, P8_20_PREV_TOGGLE, P8_20_LAST_STATE, P8_20_CURRENT_STATE

SKIP_P8_20:

  // P9_26

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_26, P9_26_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBC SKIP_P9_26, P9_26_MODE

    // Process input
    PROCESS_INPUT P9_26_IN, P9_26_TON, P9_26_TOFF, P9_26_PREV_TOGGLE, P9_26_LAST_STATE, P9_26_CURRENT_STATE

SKIP_P9_26:

  // Process Failsafe
  PROCESS_FAILSAFE CURRENT_TIME

  //Set last frame = current frame
  MOV LAST_INPUT_FRAME, CURRENT_INPUT_FRAME

JMP PROCESS_MAIN
