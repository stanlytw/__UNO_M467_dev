/***************************************************************************//**
 * @file     fmc_user.h
 * @brief    FMC driver header file
 * @version  2.0.0
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef FMC_USER_H
#define FMC_USER_H
#if defined(__M460MINIMA__)
//#include "HardwareSerial.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "targetdev.h"

// #define FMC_CONFIG0_ADDR        (FMC_CONFIG_BASE)       /*!< CONFIG 0 Address */
// #define FMC_CONFIG1_ADDR        (FMC_CONFIG_BASE + 4)   /*!< CONFIG 1 Address */

#define Config0         FMC_CONFIG_BASE
#define Config1         (FMC_CONFIG_BASE+4)

#define ISPGO           0x01

/*---------------------------------------------------------------------------------------------------------*/
/* Define parameter                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*  FMC Macro Definitions                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define _FMC_ENABLE_CFG_UPDATE()   (FMC->ISPCTL |=  FMC_ISPCTL_CFGUEN_Msk) /*!< Enable CONFIG Update Function  */
#define _FMC_DISABLE_CFG_UPDATE()  (FMC->ISPCTL &= ~FMC_ISPCTL_CFGUEN_Msk) /*!< Disable CONFIG Update Function */


int FMC_Read_User(uint32_t u32Addr, uint32_t *data);
int FMC_Erase_User(uint32_t u32Addr);
void ReadData(uint32_t addr_start, uint32_t addr_end, uint32_t *data);
void WriteData(uint32_t addr_start, uint32_t addr_end, uint32_t *data);
int EraseAP(uint32_t addr_start, uint32_t size);
void UpdateConfig(uint32_t *data, uint32_t *res);

#ifdef __cplusplus
}
#endif
#endif//defined(__M460MINIMA__)
#endif  /* FMC_USER_H */
