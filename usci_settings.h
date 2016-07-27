/******************************************************************************
* Copyright (c) 2014, Leo Hendrawan
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*    * Neither the name of the copyright holder(s) nor the names of its
*      contributor(s) may be used to endorse or promote products derived
*      from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CONTRIBUTOR(S) BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

/**************************************************************************//**
*
* @file     usci_settings.h
*
* @brief    header file containing the memory layout for example code
*           running function in RAM with CCS IDE
*
* @version  0.1
*
* @author   Leo Hendrawan (lhend.mss (at) gmail.com)
*
* @remark
*  - target device: MSP430G2553
*  - IDE: CCS IDE v5.5 - MSP430 CGT v4.2.1
*  - only support UART setting for UCOS16=0 - feel free to extend
*
******************************************************************************/

#ifndef _USCI_SETTINGS_H_
#define _USCI_SETTINGS_H_


//*****************************************************************************
// Include section
//*****************************************************************************


//*****************************************************************************
// Global variable declarations
//*****************************************************************************


//*****************************************************************************
// Macros (defines) and data types
//*****************************************************************************

// the following lines can be modified by user - however please keep in mind
// that UART back-channel on MSP-EXP43G2 only supports 9600 baud
#define USCI_INPUT_CLK      (8000000UL)  // in Hz
#define USCI_BAUD_RATE      (9600)

// test result (UCOS16=0)
// --------------------------------------------------------------------------
// | input freq     | baud rate   | UCBR0       | UCBR1       | UCBRS       |
// --------------------------------------------------------------------------
// | 1000000        | 9600        | 104 (0x68)  | 0 (0x00)    | 1 (0x01)    |
// | 1000000        | 19200       | 52 (0x34)   | 0 (0x00)    | 1 (0x01)    |
// | 1000000        | 38400       | 26 (0x1A)   | 0 (0x00)    | 0 (0x00)    |
// --------------------------------------------------------------------------
// | 8000000        | 9600        | 65 (0x41)   | 3 (0x03)    | 3 (0x03)    |
// | 8000000        | 19200       | 160 (0xA0)  | 1 (0x01)    | 5 (0x05)    |
// | 8000000        | 38400       | 208 (0xD0)  | 0 (0x00)    | 3 (0x03)    |
// | 8000000        | 115200      | 69 (0x45)   | 0 (0x00)    | 4 (0x04)    |
// --------------------------------------------------------------------------
// | 12000000       | 9600        | 226 (0xE2)  | 4 (0x04)    | 0 (0x00)    |
// | 12000000       | 19200       | 113 (0x71)  | 2 (0x02)    | 0 (0x00)    |
// | 12000000       | 38400       | 56 (0x38)   | 1 (0x01)    | 4 (0x04)    |
// | 12000000       | 115200      | 104 (0x68)  | 0 (0x00)    | 1 (0x01)    |
// --------------------------------------------------------------------------
// | 16000000       | 9600        | 130 (0x82)  | 6 (0x06)    | 5 (0x05)    |
// | 16000000       | 19200       | 65 (0x41)   | 3 (0x03)    | 3 (0x03)    |
// | 16000000       | 38400       | 160 (0xA0)  | 1 (0x01)    | 5 (0x05)    |
// | 16000000       | 115200      | 138 (0x8A)  | 0 (0x00)    | 7 (0x07)    |
// --------------------------------------------------------------------------


// do not change anything starting from this line unless you know what you
// are doing!

#define USCI_DIV_INT              (USCI_INPUT_CLK/USCI_BAUD_RATE)
#define USCI_BR0_VAL              (USCI_DIV_INT & 0x00FF)
#define USCI_BR1_VAL              ((USCI_DIV_INT >> 8) & 0xFF)

#define USCI_DIV_FRAC_NUMERATOR   (USCI_INPUT_CLK - (USCI_DIV_INT*USCI_BAUD_RATE))
#define USCI_DIV_FRAC_NUM_X_8     (USCI_DIV_FRAC_NUMERATOR*8)
#define USCI_DIV_FRAC_X_8         (USCI_DIV_FRAC_NUM_X_8/USCI_BAUD_RATE)

#if (((USCI_DIV_FRAC_NUM_X_8-(USCI_DIV_FRAC_X_8*USCI_BAUD_RATE))*10)/USCI_BAUD_RATE < 5)
#define USCI_BRS_VAL              (USCI_DIV_FRAC_X_8<<1)
#else
#define USCI_BRS_VAL              ((USCI_DIV_FRAC_X_8+1)<<1)
#endif


// sanity check
#if (USCI_BR0_VAL > 0xFF)
#error Unexpected UCBR0 value!
#endif

#if (USCI_BR1_VAL > 0xFF)
#error Unexpected UCBR1 value!
#endif

#if (USCI_BRS_VAL > (0x07<<1))
#error Unexpected UCBRS value!
#endif


//*****************************************************************************
// External function declarations
//*****************************************************************************

#endif /* _USCI_SETTINGS_H_ */
