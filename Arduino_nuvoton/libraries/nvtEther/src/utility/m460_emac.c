/**************************************************************************//**
 * @file     m460_emac.c
 * @version  V3.00
 * @brief    M460 EMAC driver source file
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2022 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "string.h"
#include "m460_emac.h"
#include "m460_mii.h"
#include "ethernetif.h"
#include "memp.h"

#include <lwip/pbuf.h>
#include <lwip/sys.h>
#include <stdio.h>
#include <stdarg.h>

static synopGMACdevice GMACdev = {0};
static DmaDesc tx_desc[TRANSMIT_DESC_SIZE] __attribute__((aligned(32))) = {0};
static DmaDesc rx_desc[RECEIVE_DESC_SIZE] __attribute__((aligned(32))) = {0};
static PKT_FRAME_T tx_buf[TRANSMIT_DESC_SIZE] __attribute__((aligned(32))) = {0};
static PKT_FRAME_T rx_buf[RECEIVE_DESC_SIZE] __attribute__((aligned(32))) = {0};

static void EMAC_ModuleInit(void)
{
    SYS_ResetModule(EMAC0_RST);

    /* Enable EMAC module clock */
    CLK_EnableModuleClock(EMAC0_MODULE);

    /* Configure pins for EMAC module */
#if 0    
    SET_EMAC0_RMII_MDC_PE8();
    SET_EMAC0_RMII_MDIO_PE9();
    SET_EMAC0_RMII_TXD0_PE10();
    SET_EMAC0_RMII_TXD1_PE11();
    SET_EMAC0_RMII_TXEN_PE12();
    SET_EMAC0_RMII_REFCLK_PC8();
    SET_EMAC0_RMII_RXD0_PC7();
    SET_EMAC0_RMII_RXD1_PC6();
    SET_EMAC0_RMII_CRSDV_PA7();
    SET_EMAC0_RMII_RXERR_PA6();
    SET_EMAC0_PPS_PB6();
#else
    SET_EMAC0_RMII_MDC_PB11();
    SET_EMAC0_RMII_MDIO_PB10();
    SET_EMAC0_RMII_TXD0_PB9();
    SET_EMAC0_RMII_TXD1_PB8();
    SET_EMAC0_RMII_TXEN_PB7();
    SET_EMAC0_RMII_REFCLK_PB5();
    SET_EMAC0_RMII_RXD0_PB4();
    SET_EMAC0_RMII_RXD1_PB3();
    SET_EMAC0_RMII_CRSDV_PB2();
    SET_EMAC0_RMII_RXERR_PB1();
    SET_EMAC0_PPS_PB6();

#endif    
}

uint8_t EMAC_Open(uint8_t *macaddr)
{
    int32_t ret = 0;
    uint32_t i;

    EMAC_ModuleInit();

    /* Attach the device to MAC struct This will configure all the required base addresses
      such as Mac base, configuration base, phy base address(out of 32 possible phys) */
    synopGMAC_attach(&GMACdev, (EMAC_BASE + MACBASE), (EMAC_BASE + DMABASE), DEFAULT_PHY_BASE, macaddr);

    synopGMAC_reset(&GMACdev);

    memcpy((void *)&GMACdev.mac_addr[0], (void *)&macaddr[0], 6);
    nvtEthernet_printf("Set MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                       macaddr[0], macaddr[1],
                       macaddr[2], macaddr[3],
                       macaddr[4], macaddr[5]);

    /* Lets read the version of ip in to device structure */
    synopGMAC_read_version(&GMACdev);

    /* Check for Phy initialization */
    if (SystemCoreClock >= 250000000)
        synopGMAC_set_mdc_clk_div(&GMACdev, GmiiCsrClk5);
    else if (SystemCoreClock >= 150000000)
        synopGMAC_set_mdc_clk_div(&GMACdev, GmiiCsrClk4);
    else if (SystemCoreClock >= 100000000)
        synopGMAC_set_mdc_clk_div(&GMACdev, GmiiCsrClk1);
    else if (SystemCoreClock >= 60000000)
        synopGMAC_set_mdc_clk_div(&GMACdev, GmiiCsrClk0);
    else if (SystemCoreClock >= 35000000)
        synopGMAC_set_mdc_clk_div(&GMACdev, GmiiCsrClk3);
    else
        synopGMAC_set_mdc_clk_div(&GMACdev, GmiiCsrClk2);

    GMACdev.ClockDivMdc = synopGMAC_get_mdc_clk_div(&GMACdev);

    if ((ret = mii_check_phy_init(&GMACdev)) < 0)
    {
        nvtEthernet_printf("emac:: Init PHY FAIL.\n");
        nvtEthernet_printf("\n??? Check the PHY device...\n");
        return 0;
    }

    /* Set up the tx and rx descriptor queue/ring */
    synopGMAC_setup_tx_desc_queue(&GMACdev, &tx_desc[0], TRANSMIT_DESC_SIZE, RINGMODE);
    synopGMAC_init_tx_desc_base(&GMACdev);  // Program the transmit descriptor base address in to DmaTxBase addr
    TR("DmaTxBaseAddr = %08x\n", synopGMACReadReg(GMACdev.DmaBase, DmaTxBaseAddr));

    synopGMAC_setup_rx_desc_queue(&GMACdev, &rx_desc[0], RECEIVE_DESC_SIZE, RINGMODE);
    synopGMAC_init_rx_desc_base(&GMACdev); // Program the transmit descriptor base address in to DmaTxBase addr
    TR("DmaTxBaseAddr = %08x\n", synopGMACReadReg(GMACdev.DmaBase, DmaRxBaseAddr));

    synopGMAC_dma_bus_mode_init(&GMACdev, (DmaBurstLength32 | DmaDescriptorSkip0 | DmaDescriptor8Words));
    synopGMAC_dma_control_init(&GMACdev, (DmaStoreAndForward | DmaTxSecondFrame | DmaRxThreshCtrl128));

    /* Initialize the mac interface */
    synopGMAC_mac_init(&GMACdev);
    synopGMAC_promisc_enable(&GMACdev);

    synopGMAC_pause_control(&GMACdev); // This enables the pause control in Full duplex mode of operation

#if (LWIP_USING_HW_CHECKSUM == 1)
    /* IPC Checksum offloading is enabled for this driver. Should only be used if Full Ip checksumm offload engine is configured in the hardware */
    synopGMAC_enable_rx_chksum_offload(&GMACdev);    // Enable the offload engine in the receive path
    synopGMAC_rx_tcpip_chksum_drop_enable(&GMACdev); // This is default configuration, DMA drops the packets if error in encapsulated ethernet payload
#endif

    for (i = 0; i < RECEIVE_DESC_SIZE; i ++)
    {
        synopGMAC_set_rx_qptr(&GMACdev, (u32)&rx_buf[i], PKT_FRAME_BUF_SIZE, 0);
    }

    /* Enable interrupt */
    synopGMAC_clear_interrupt(&GMACdev);
    synopGMAC_disable_interrupt_all(&GMACdev);
    synopGMAC_enable_interrupt(&GMACdev, DmaIntEnable);

    /* Enable DMA */
    synopGMAC_enable_dma_rx(&GMACdev);
    synopGMAC_enable_dma_tx(&GMACdev);

    synopGMAC_set_mac_addr(&GMACdev, GmacAddr0High, GmacAddr0Low, &GMACdev.mac_addr[0]);

    if (GMACdev.Speed == SPEED10)
        synopGMAC_set_mode(&GMACdev, 2); // 1: 100Mbps, 2: 10Mbps
    else
        synopGMAC_set_mode(&GMACdev, 1); // 1: 100Mbps, 2: 10Mbps

    NVIC_EnableIRQ(EMAC0_TXRX_IRQn);

    return 1;
}

/*----------------------------------------------------------------------------
  EMAC IRQ Handler
 *----------------------------------------------------------------------------*/
void EMAC0_IRQHandler(void)
{
    u32 interrupt, dma_status_reg;
    s32 status;
    u32 u32GmacIntSts;
    u32 u32GmacDmaIE = DmaIntEnable;

    // Check GMAC interrupt
    u32GmacIntSts = synopGMACReadReg(GMACdev.MacBase, GmacInterruptStatus);
    if (u32GmacIntSts & GmacTSIntSts)
    {
        GMACdev.synopGMACNetStats.ts_int = 1;
        status = synopGMACReadReg(GMACdev.MacBase, GmacTSStatus);
        if (!(status & (1 << 1)))
        {
            TR("TS alarm flag not set??\n");
        }
        else
        {
            TR("TS alarm!!!!!!!!!!!!!!!!\n");
        }
    }

    synopGMACWriteReg(GMACdev.MacBase, GmacInterruptStatus, u32GmacIntSts);

    dma_status_reg = synopGMACReadReg(GMACdev.DmaBase, DmaStatus);
    if (dma_status_reg == 0)
    {
        TR("dma_status ==0 \n");
        return;
    }

    if (dma_status_reg & GmacPmtIntr)
    {
        TR("%s:: Interrupt due to PMT module\n", __FUNCTION__);
        synopGMAC_powerup_mac(&GMACdev);
    }

    if (dma_status_reg & GmacLineIntfIntr)
    {
        TR("%s:: Interrupt due to GMAC LINE module\n", __FUNCTION__);
    }

    interrupt = synopGMAC_get_interrupt_type(&GMACdev);
    TR("%s:Interrupts to be handled: 0x%08x  %08x\n", __func__, interrupt, synopGMAC_get_ie(&GMACdev));

    if (interrupt & synopGMACDmaError)
    {
        TR("%s::Fatal Bus Error Inetrrupt Seen\n", __FUNCTION__);
        synopGMAC_disable_dma_tx(&GMACdev);
        synopGMAC_disable_dma_rx(&GMACdev);

        synopGMAC_take_desc_ownership_tx(&GMACdev);
        synopGMAC_take_desc_ownership_rx(&GMACdev);

        synopGMAC_init_tx_rx_desc_queue(&GMACdev);

        synopGMAC_reset(&GMACdev);

        synopGMAC_set_mac_addr(&GMACdev, GmacAddr0High, GmacAddr0Low, &GMACdev.mac_addr[0]);
        synopGMAC_dma_bus_mode_init(&GMACdev, DmaBurstLength32 | DmaDescriptorSkip0/*DmaDescriptorSkip2*/ | DmaDescriptor8Words);
        synopGMAC_dma_control_init(&GMACdev, DmaStoreAndForward | DmaTxSecondFrame | DmaRxThreshCtrl128);
        synopGMAC_init_rx_desc_base(&GMACdev);
        synopGMAC_init_tx_desc_base(&GMACdev);
        synopGMAC_mac_init(&GMACdev);
        synopGMAC_enable_dma_rx(&GMACdev);
        synopGMAC_enable_dma_tx(&GMACdev);
    }

    if ((interrupt & synopGMACDmaRxNormal) ||
            (interrupt & synopGMACDmaRxAbnormal))
    {
        if (interrupt & synopGMACDmaRxNormal)
        {
            //static uint32_t u32RXCount = 0;
            //nvtEthernet_printf("%s:: Rx Normal(%d) \n", __FUNCTION__, u32RXCount++);
            ethernetif_isr_cb();
        }
        if (interrupt & synopGMACDmaRxAbnormal)
        {
            TR0("%s::Abnormal Rx Interrupt Seen %08x\n", __FUNCTION__, dma_status_reg);
            if (GMACdev.GMAC_Power_down == 0)
            {
                GMACdev.synopGMACNetStats.rx_over_errors++;
                u32GmacDmaIE &= ~DmaIntRxAbnMask;
                synopGMAC_resume_dma_rx(&GMACdev);
            }
        }
    }

    if (interrupt & synopGMACDmaRxStopped)
    {
        TR("%s::Receiver stopped seeing Rx interrupts\n", __FUNCTION__); //Receiver gone in to stopped state
        if (GMACdev.GMAC_Power_down == 0)   // If Mac is not in powerdown
        {
            GMACdev.synopGMACNetStats.rx_over_errors++;
            synopGMAC_enable_dma_rx(&GMACdev);
        }
    }

    if (interrupt & synopGMACDmaTxNormal)
    {
        TR("%s::Finished Normal Transmission \n", __FUNCTION__);
        synop_handle_transmit_over(&GMACdev);//Do whatever you want after the transmission is over
    }

    if (interrupt & synopGMACDmaTxAbnormal)
    {
        TR("%s::Abnormal Tx Interrupt Seen\n", __FUNCTION__);
        if (GMACdev.GMAC_Power_down == 0)   // If Mac is not in powerdown
        {
            synop_handle_transmit_over(&GMACdev);
        }
    }

    if (interrupt & synopGMACDmaTxStopped)
    {
        TR("%s::Transmitter stopped sending the packets\n", __FUNCTION__);
        if (GMACdev.GMAC_Power_down == 0)    // If Mac is not in powerdown
        {
            synopGMAC_disable_dma_tx(&GMACdev);
            synopGMAC_take_desc_ownership_tx(&GMACdev);
            synopGMAC_enable_dma_tx(&GMACdev);
            TR("%s::Transmission Resumed\n", __FUNCTION__);
        }
    }

    /* Enable the interrrupt before returning from ISR */
    synopGMAC_enable_interrupt(&GMACdev, u32GmacDmaIE);
}

uint32_t EMAC_ReceivePkt(PKT_FRAME_T **ppsPktFrame)
{
    return synop_handle_received_data(&GMACdev, ppsPktFrame);
}

void EMAC_ReceivePkt_Free(PKT_FRAME_T *psPktFrame)
{
    synopGMAC_set_rx_qptr(&GMACdev, (u32)psPktFrame, PKT_FRAME_BUF_SIZE, 0);
}

uint8_t *EMAC_AllocatePktBuf(void)
{
    u32 index = GMACdev.TxNext;

    return &tx_buf[index].au8Buf[0];
}

int32_t EMAC_TransmitPkt(uint8_t *pbuf, uint32_t len)
{
    u32 offload_needed;

    if ((pbuf != NULL) && (len > 0) &&
            !synopGMAC_is_desc_owned_by_dma(GMACdev.TxNextDesc))
    {

#if (LWIP_USING_HW_CHECKSUM == 1)
        offload_needed = 1;
#else
        offload_needed = 0;
#endif
        return synopGMAC_xmit_frames(&GMACdev, pbuf, len, offload_needed, 0);
    }

    return -1;
}
