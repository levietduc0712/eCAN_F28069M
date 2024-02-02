/*
 * can.c
 *
 *   Copyright (C) 2024,  LE VIET DUC
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

// **************************************************************************
// the includes

#include<stdlib.h>
#include "sw/drivers/can/src/32b/f28x/f2806x/can.h"


// **************************************************************************
// the defines

//! \brief Define to allow protected register writes
//!
#define  ENABLE_PROTECTED_REGISTER_WRITE_MODE  asm(" EALLOW")

//! \brief Define to disable protected register writes
//!
#define  DISABLE_PROTECTED_REGISTER_WRITE_MODE asm(" EDIS")

// **************************************************************************
// the globals


// **************************************************************************
// the functions


ECAN_Handle ECAN_init()
{

    ECAN_Handle handle = (ECAN_Handle)calloc(1, sizeof(ECAN_Obj));

    if(handle == NULL){
        return (ECAN_Handle)NULL;
    }

    handle->ECanaRegs = (ECAN_REGS_t*)ECANA_REGS_ADDR;
    handle->ECanaMboxes = (ECAN_MBOXES_t*)ECANA_MBOX_ADDR;
    handle->ECanaLAMRegs = (LAM_REGS_t*)ECANA_LAM_ADDR;
    handle->ECanaMOTORegs = (MOTO_REGS_t*)ECANA_MOTO_ADDR;
    handle->ECanaMOTSRegs = (MOTS_REGS_t*)ECANA_MOTS_ADDR;

    return handle;
}


void ECAN_setTXIO(ECAN_Handle handle)
{

  struct ECAN_REGS ECanaShadow;

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  ECanaShadow.CANTIOC.all = handle->ECanaRegs->CANTIOC.all;
  ECanaShadow.CANTIOC.bit.TXFUNC = 1;
  handle->ECanaRegs->CANTIOC.all = ECanaShadow.CANTIOC.all;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_setRXIO(ECAN_Handle handle)
{

  struct ECAN_REGS ECanaShadow;

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  ECanaShadow.CANRIOC.all = handle->ECanaRegs->CANRIOC.all;
  ECanaShadow.CANRIOC.bit.RXFUNC = 1;
  handle->ECanaRegs->CANRIOC.all = ECanaShadow.CANRIOC.all;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_Mode(ECAN_Handle handle, SCB_Bit_e mode)
{

  struct ECAN_REGS ECanaShadow;

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  ECanaShadow.CANMC.all = handle->ECanaRegs->CANMC.all;
  ECanaShadow.CANMC.bit.SCB = mode;
  handle->ECanaRegs->CANMC.all = ECanaShadow.CANMC.all;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_clearMSGID(ECAN_Handle handle)
{

  volatile struct MBOX *mbox = (&(handle->ECanaMboxes->MBOX0));

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  /* Disable all Mailboxes  */
  handle->ECanaRegs->CANME.all = 0x00000000;        // Required before writing the MSGIDs

    int i = 0;
    for(i = 0; i < 32; i++)
    {
      mbox->MSGID.all = 0x00000000;
      mbox->MSGCTRL.all = 0x00000000;
      mbox->MDL.all = 0x00000000;
      mbox->MDH.all = 0x00000000;
      *mbox++;
    }

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_clearCANTA(ECAN_Handle handle)
{

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  /* Clear all Transmission-Acknowledge Register bits */
  handle->ECanaRegs->CANTA.all = 0xFFFFFFFF;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_clearCANRMP(ECAN_Handle handle)
{

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  /* Clear all Received-Message-Pending Register bits */
  handle->ECanaRegs->CANRMP.all = 0xFFFFFFFF;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_clearCANGIF0(ECAN_Handle handle)
{

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  /* Clear all Global Interrupt Flag 0 */
  handle->ECanaRegs->CANGIF0.all = 0xFFFFFFFF;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_clearCANGIF1(ECAN_Handle handle)
{

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  /* Clear all Global Interrupt Flag 1 */
  handle->ECanaRegs->CANGIF1.all = 0xFFFFFFFF;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_setBitrate(ECAN_Handle handle)
{

  struct ECAN_REGS ECanaShadow;

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  ECanaShadow.CANMC.all = handle->ECanaRegs->CANMC.all;
  ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
  handle->ECanaRegs->CANMC.all = ECanaShadow.CANMC.all;

  // Wait until the CPU has been granted permission to change the configuration registers
  do
  {
    ECanaShadow.CANES.all = handle->ECanaRegs->CANES.all;
  } while(ECanaShadow.CANES.bit.CCE != 1 );       // Wait for CCE bit to be set..

  ECanaShadow.CANBTC.all = 0;

  ECanaShadow.CANBTC.bit.BRPREG = 5;
  ECanaShadow.CANBTC.bit.TSEG2REG = 2;
  ECanaShadow.CANBTC.bit.TSEG1REG = 10;


  handle->ECanaRegs->CANBTC.all = ECanaShadow.CANBTC.all;

  ECanaShadow.CANMC.all = handle->ECanaRegs->CANMC.all;
  ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
  handle->ECanaRegs->CANMC.all = ECanaShadow.CANMC.all;

  // Wait until the CPU no longer has permission to change the configuration registers
  do
  {
    ECanaShadow.CANES.all = handle->ECanaRegs->CANES.all;
  } while(ECanaShadow.CANES.bit.CCE != 0 );       // Wait for CCE bit to be  cleared..


  handle->ECanaRegs->CANME.all = 0;        // Required before writing the MSGIDs

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_enablerx(ECAN_Handle handle)
{

  struct ECAN_REGS ECanaShadow;

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  /* Clear all Global Interrupt Flag 1 */
  ECanaShadow.CANMD.all = 0;
  ECanaShadow.CANMD.bit.MD16 = 1;
  handle->ECanaRegs->CANMD.all = ECanaShadow.CANMD.all;

  ECanaShadow.CANME.all = 0;
  ECanaShadow.CANME.bit.ME16 = 1;
  handle->ECanaRegs->CANME.all = ECanaShadow.CANME.all;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}


void ECAN_configMasterReg(ECAN_Handle handle, CCR_Bit_e CCR_t, PDR_Bit_e PDR_t, DBO_Bit_e DBO_t, WUBA_Bit_e WUBA_t, CDR_Bit_e CDR_t, ABO_Bit_e ABO_t, SRES_Bit_e SRES_t, MBNR_Bit_e MBNR_t)
{

  struct ECAN_REGS ECanaShadow;

  ENABLE_PROTECTED_REGISTER_WRITE_MODE;

  /* config Master Control Register  */
  ECanaShadow.CANMC.all = handle->ECanaRegs->CANMC.all;
  ECanaShadow.CANMC.bit.CCR = CCR_t;
  ECanaShadow.CANMC.bit.PDR = PDR_t;
  ECanaShadow.CANMC.bit.DBO = DBO_t;
  ECanaShadow.CANMC.bit.WUBA = WUBA_t;
  ECanaShadow.CANMC.bit.CDR = CDR_t;
  ECanaShadow.CANMC.bit.ABO = ABO_t;
  ECanaShadow.CANMC.bit.SRES = SRES_t;
  ECanaShadow.CANMC.bit.MBNR = MBNR_t;
  handle->ECanaRegs->CANMC.all = ECanaShadow.CANMC.all;

  DISABLE_PROTECTED_REGISTER_WRITE_MODE;

}
