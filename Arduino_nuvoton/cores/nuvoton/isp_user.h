/***************************************************************************//**
 * @file     isp_user.h
 * @brief    ISP Command header file
 * @version  0x32
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef ISP_USER_H
#define ISP_USER_H
#if defined(__M460MINIMA__)
//#include "HardwareSerial.h"
#ifdef __cplusplus
extern "C" {
#endif
#define FW_VERSION                          0xA2

#include "fmc_user.h"

#define CMD_UPDATE_APROM            0x000000A0
#define CMD_UPDATE_CONFIG           0x000000A1
#define CMD_READ_CONFIG             0x000000A2
#define CMD_ERASE_ALL               0x000000A3
#define CMD_SYNC_PACKNO             0x000000A4
#define CMD_GET_FWVER               0x000000A6
#define CMD_RUN_APROM               0x000000AB
#define CMD_RUN_LDROM               0x000000AC
#define CMD_RESET                   0x000000AD
#define CMD_CONNECT                 0x000000AE
#define CMD_DISCONNECT              0x000000AF
#define CMD_TRIGGER_HIDISP          0x000000D0

#define CMD_GET_DEVICEID            0x000000B1

#define CMD_UPDATE_DATAFLASH        0x000000C3
#define CMD_WRITE_CHECKSUM          0x000000C9
#define CMD_GET_FLASHMODE           0x000000CA

#define CMD_RESEND_PACKET           0x000000FF

#define V6M_AIRCR_VECTKEY_DATA      0x05FA0000UL
#define V6M_AIRCR_SYSRESETREQ       0x00000004UL

extern void GetDataFlashInfo(uint32_t *pu32Addr, uint32_t *pu32Size);
extern uint32_t GetApromSize(void);
extern int ParseCmd(uint8_t *pu8Buffer, uint8_t u8len);
extern uint32_t g_u32ApromSize, g_u32DataFlashAddr, g_u32DataFlashSize;

extern __attribute__((aligned(4))) uint8_t g_u8UsbRcvBuff[];
extern __attribute__((aligned(4))) uint8_t g_au8ResponseBuff[64];

#ifdef __cplusplus
}
#endif
#endif//defined(__M460MINIMA__)
#endif  /* ISP_USER_H */
