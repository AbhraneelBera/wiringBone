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


#include "PRU0.hp"

.macro INIT
  // Clear all registers
  ZERO &register, SIZE(register)

  // Clear all output bits
  MOV OUTPUT, 0x0

  // Initialize starting time
  LBCO CURRENT_TIME, IEP, COUNT, 4

  // Initialize PREV_TOGGLE (for input) and NEXT_TOGGLE (for output) with current time
  MOV register.P9_31_toggle, CURRENT_TIME
  MOV register.P9_29_toggle, CURRENT_TIME
  MOV register.P9_30_toggle, CURRENT_TIME
  MOV register.P9_28_toggle, CURRENT_TIME
  MOV register.P9_42_toggle, CURRENT_TIME
  MOV register.P9_27_toggle, CURRENT_TIME
  MOV register.P9_41_toggle, CURRENT_TIME
  MOV register.P9_25_toggle, CURRENT_TIME
  MOV register.P8_12_toggle, CURRENT_TIME
  MOV register.P8_11_toggle, CURRENT_TIME
  MOV register.P8_16_toggle, CURRENT_TIME
  MOV register.P8_15_toggle, CURRENT_TIME
  MOV register.P9_24_toggle, CURRENT_TIME

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
  MOV TEMP, 0x0
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

  SBCO T_ON, RAM, P9_31_TON, 8
  SBCO T_ON, RAM, P9_29_TON, 8
  SBCO T_ON, RAM, P9_30_TON, 8
  SBCO T_ON, RAM, P9_28_TON, 8
  SBCO T_ON, RAM, P9_42_TON, 8
  SBCO T_ON, RAM, P9_27_TON, 8
  SBCO T_ON, RAM, P9_41_TON, 8
  SBCO T_ON, RAM, P9_25_TON, 8
  SBCO T_ON, RAM, P8_12_TON, 8
  SBCO T_ON, RAM, P8_11_TON, 8

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

  // P9_31

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_31, P9_31_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_31_OUT, P9_31_MODE

    // Process output
    PROCESS_OUTPUT P9_31_OUT, P9_31_TON, P9_31_TOFF, P9_31_NEXT_TOGGLE
    JMP SKIP_P9_31

SKIP_P9_31_OUT:
    // Process input
    PROCESS_INPUT P9_31_IN, P9_31_TON, P9_31_TOFF, P9_31_PREV_TOGGLE, P9_31_LAST_STATE, P9_31_CURRENT_STATE

SKIP_P9_31:

  // P9_29

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_29, P9_29_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_29_OUT, P9_29_MODE

    // Process output
    PROCESS_OUTPUT P9_29_OUT, P9_29_TON, P9_29_TOFF, P9_29_NEXT_TOGGLE
    JMP SKIP_P9_29

SKIP_P9_29_OUT:
    // Process input
    PROCESS_INPUT P9_29_IN, P9_29_TON, P9_29_TOFF, P9_29_PREV_TOGGLE, P9_29_LAST_STATE, P9_29_CURRENT_STATE

SKIP_P9_29:

  // P9_30

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_30, P9_30_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_30_OUT, P9_30_MODE

    // Process output
    PROCESS_OUTPUT P9_30_OUT, P9_30_TON, P9_30_TOFF, P9_30_NEXT_TOGGLE
    JMP SKIP_P9_30

SKIP_P9_30_OUT:
    // Process input
    PROCESS_INPUT P9_30_IN, P9_30_TON, P9_30_TOFF, P9_30_PREV_TOGGLE, P9_30_LAST_STATE, P9_30_CURRENT_STATE

SKIP_P9_30:

  // P9_28

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_28, P9_28_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_28_OUT, P9_28_MODE

    // Process output
    PROCESS_OUTPUT P9_28_OUT, P9_28_TON, P9_28_TOFF, P9_28_NEXT_TOGGLE
    JMP SKIP_P9_28

SKIP_P9_28_OUT:
    // Process input
    PROCESS_INPUT P9_28_IN, P9_28_TON, P9_28_TOFF, P9_28_PREV_TOGGLE, P9_28_LAST_STATE, P9_28_CURRENT_STATE

SKIP_P9_28:

  // P9_42

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_42, P9_42_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_42_OUT, P9_42_MODE

    // Process output
    PROCESS_OUTPUT P9_42_OUT, P9_42_TON, P9_42_TOFF, P9_42_NEXT_TOGGLE
    JMP SKIP_P9_42

SKIP_P9_42_OUT:
    // Process input
    PROCESS_INPUT P9_42_IN, P9_42_TON, P9_42_TOFF, P9_42_PREV_TOGGLE, P9_42_LAST_STATE, P9_42_CURRENT_STATE

SKIP_P9_42:

  // P9_27

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_27, P9_27_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_27_OUT, P9_27_MODE

    // Process output
    PROCESS_OUTPUT P9_27_OUT, P9_27_TON, P9_27_TOFF, P9_27_NEXT_TOGGLE
    JMP SKIP_P9_27

SKIP_P9_27_OUT:
    // Process input
    PROCESS_INPUT P9_27_IN, P9_27_TON, P9_27_TOFF, P9_27_PREV_TOGGLE, P9_27_LAST_STATE, P9_27_CURRENT_STATE

SKIP_P9_27:

  // P9_41

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_41, P9_41_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_41_OUT, P9_41_MODE

    // Process output
    PROCESS_OUTPUT P9_41_OUT, P9_41_TON, P9_41_TOFF, P9_41_NEXT_TOGGLE
    JMP SKIP_P9_41

SKIP_P9_41_OUT:
    // Process input
    PROCESS_INPUT P9_41_IN, P9_41_TON, P9_41_TOFF, P9_41_PREV_TOGGLE, P9_41_LAST_STATE, P9_41_CURRENT_STATE

SKIP_P9_41:

  // P9_25

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_25, P9_25_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P9_25_OUT, P9_25_MODE

    // Process output
    PROCESS_OUTPUT P9_25_OUT, P9_25_TON, P9_25_TOFF, P9_25_NEXT_TOGGLE
    JMP SKIP_P9_25

SKIP_P9_25_OUT:
    // Process input
    PROCESS_INPUT P9_25_IN, P9_25_TON, P9_25_TOFF, P9_25_PREV_TOGGLE, P9_25_LAST_STATE, P9_25_CURRENT_STATE

SKIP_P9_25:

  // P8_12

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_12, P8_12_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_12, P8_12_MODE

    // Process output
    PROCESS_OUTPUT P8_12_OUT, P8_12_TON, P8_12_TOFF, P8_12_NEXT_TOGGLE

SKIP_P8_12:

  // P8_11

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_11, P8_11_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBS SKIP_P8_11, P8_11_MODE

    // Process output
    PROCESS_OUTPUT P8_11_OUT, P8_11_TON, P8_11_TOFF, P8_11_NEXT_TOGGLE

SKIP_P8_11:

  // P8_16

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_16, P8_16_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBC SKIP_P8_16, P8_16_MODE

    // Process input
    PROCESS_INPUT P8_16_IN, P8_16_TON, P8_16_TOFF, P8_16_PREV_TOGGLE, P8_16_LAST_STATE, P8_16_CURRENT_STATE

SKIP_P8_16:

  // P8_15

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P8_15, P8_15_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBC SKIP_P8_15, P8_15_MODE

    // Process input
    PROCESS_INPUT P8_15_IN, P8_15_TON, P8_15_TOFF, P8_15_PREV_TOGGLE, P8_15_LAST_STATE, P8_15_CURRENT_STATE

SKIP_P8_15:

  // P9_24

  // If pin enabled (1) then process, else skip
  QBBC SKIP_P9_24, P9_24_ENABLE

  // Check pin mode, 0 for out, 1 for in
  QBBC SKIP_P9_24, P9_24_MODE

    // Process input
    PROCESS_INPUT P9_24_IN, P9_24_TON, P9_24_TOFF, P9_24_PREV_TOGGLE, P9_24_LAST_STATE, P9_24_CURRENT_STATE

SKIP_P9_24:

  // Process Failsafe
  PROCESS_FAILSAFE CURRENT_TIME

  //Set last frame = current frame
  MOV LAST_INPUT_FRAME, CURRENT_INPUT_FRAME

JMP PROCESS_MAIN
