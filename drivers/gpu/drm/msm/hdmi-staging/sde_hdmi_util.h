/*
 * Copyright (c) 2017, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _SDE_HDMI_UTIL_H_
#define _SDE_HDMI_UTIL_H_

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/debugfs.h>
#include <linux/of_device.h>
#include <linux/msm_ext_display.h>

#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include "hdmi.h"
#include "sde_kms.h"
#include "sde_connector.h"
#include "msm_drv.h"
#include "sde_hdmi_regs.h"

#ifdef HDMI_UTIL_DEBUG_ENABLE
#define HDMI_UTIL_DEBUG(fmt, args...)   SDE_ERROR(fmt, ##args)
#else
#define HDMI_UTIL_DEBUG(fmt, args...)   SDE_DEBUG(fmt, ##args)
#endif

#define HDMI_UTIL_ERROR(fmt, args...)   SDE_ERROR(fmt, ##args)

/*
 * Offsets in HDMI_DDC_INT_CTRL0 register
 *
 * The HDMI_DDC_INT_CTRL0 register is intended for HDCP 2.2 RxStatus
 * register manipulation. It reads like this:
 *
 * Bit 31: RXSTATUS_MESSAGE_SIZE_MASK (1 = generate interrupt when size > 0)
 * Bit 30: RXSTATUS_MESSAGE_SIZE_ACK  (1 = Acknowledge message size intr)
 * Bits 29-20: RXSTATUS_MESSAGE_SIZE  (Actual size of message available)
 * Bits 19-18: RXSTATUS_READY_MASK    (1 = generate interrupt when ready = 1
 *       2 = generate interrupt when ready = 0)
 * Bit 17: RXSTATUS_READY_ACK         (1 = Acknowledge ready bit interrupt)
 * Bit 16: RXSTATUS_READY      (1 = Rxstatus ready bit read is 1)
 * Bit 15: RXSTATUS_READY_NOT         (1 = Rxstatus ready bit read is 0)
 * Bit 14: RXSTATUS_REAUTH_REQ_MASK   (1 = generate interrupt when reauth is
 *   requested by sink)
 * Bit 13: RXSTATUS_REAUTH_REQ_ACK    (1 = Acknowledge Reauth req interrupt)
 * Bit 12: RXSTATUS_REAUTH_REQ        (1 = Rxstatus reauth req bit read is 1)
 * Bit 10: RXSTATUS_DDC_FAILED_MASK   (1 = generate interrupt when DDC
 *   tranasaction fails)
 * Bit 9:  RXSTATUS_DDC_FAILED_ACK    (1 = Acknowledge ddc failure interrupt)
 * Bit 8:  RXSTATUS_DDC_FAILED      (1 = DDC transaction failed)
 * Bit 6:  RXSTATUS_DDC_DONE_MASK     (1 = generate interrupt when DDC
 *   transaction completes)
 * Bit 5:  RXSTATUS_DDC_DONE_ACK      (1 = Acknowledge ddc done interrupt)
 * Bit 4:  RXSTATUS_DDC_DONE      (1 = DDC transaction is done)
 * Bit 2:  RXSTATUS_DDC_REQ_MASK      (1 = generate interrupt when DDC Read
 *   request for RXstatus is made)
 * Bit 1:  RXSTATUS_DDC_REQ_ACK       (1 = Acknowledge Rxstatus read interrupt)
 * Bit 0:  RXSTATUS_DDC_REQ           (1 = RXStatus DDC read request is made)
 *
 */

#define HDCP2P2_RXSTATUS_MESSAGE_SIZE_SHIFT         20
#define HDCP2P2_RXSTATUS_MESSAGE_SIZE_MASK          0x3ff00000
#define HDCP2P2_RXSTATUS_MESSAGE_SIZE_ACK_SHIFT     30
#define HDCP2P2_RXSTATUS_MESSAGE_SIZE_INTR_SHIFT    31

#define HDCP2P2_RXSTATUS_REAUTH_REQ_SHIFT           12
#define HDCP2P2_RXSTATUS_REAUTH_REQ_MASK             1
#define HDCP2P2_RXSTATUS_REAUTH_REQ_ACK_SHIFT    13
#define HDCP2P2_RXSTATUS_REAUTH_REQ_INTR_SHIFT    14

#define HDCP2P2_RXSTATUS_READY_SHIFT    16
#define HDCP2P2_RXSTATUS_READY_MASK                  1
#define HDCP2P2_RXSTATUS_READY_ACK_SHIFT            17
#define HDCP2P2_RXSTATUS_READY_INTR_SHIFT           18
#define HDCP2P2_RXSTATUS_READY_INTR_MASK            18

#define HDCP2P2_RXSTATUS_DDC_FAILED_SHIFT           8
#define HDCP2P2_RXSTATUS_DDC_FAILED_ACKSHIFT        9
#define HDCP2P2_RXSTATUS_DDC_FAILED_INTR_MASK       10
#define HDCP2P2_RXSTATUS_DDC_DONE                   6

/* default hsyncs for 4k@60 for 200ms */
#define HDMI_DEFAULT_TIMEOUT_HSYNC 28571

/*
 * Bits 1:0 in HDMI_HW_DDC_CTRL that dictate how the HDCP 2.2 RxStatus will be
 * read by the hardware
 */
#define HDCP2P2_RXSTATUS_HW_DDC_DISABLE             0
#define HDCP2P2_RXSTATUS_HW_DDC_AUTOMATIC_LOOP      1
#define HDCP2P2_RXSTATUS_HW_DDC_FORCE_LOOP          2
#define HDCP2P2_RXSTATUS_HW_DDC_SW_TRIGGER          3

struct sde_hdmi_tx_ddc_data {
	char *what;
	u8 *data_buf;
	u32 data_len;
	u32 dev_addr;
	u32 offset;
	u32 request_len;
	u32 retry_align;
	u32 hard_timeout;
	u32 timeout_left;
	int retry;
};

enum sde_hdmi_tx_hdcp2p2_rxstatus_intr_mask {
	RXSTATUS_MESSAGE_SIZE = BIT(31),
	RXSTATUS_READY = BIT(18),
	RXSTATUS_REAUTH_REQ = BIT(14),
};

struct sde_hdmi_tx_hdcp2p2_ddc_data {
	enum sde_hdmi_tx_hdcp2p2_rxstatus_intr_mask intr_mask;
	u32 timeout_ms;
	u32 timeout_hsync;
	u32 periodic_timer_hsync;
	u32 timeout_left;
	u32 read_method;
	u32 message_size;
	bool encryption_ready;
	bool ready;
	bool reauth_req;
	bool ddc_max_retries_fail;
	bool ddc_done;
	bool ddc_read_req;
	bool ddc_timeout;
	bool wait;
	int irq_wait_count;
	void (*link_cb)(void *data);
	void *link_data;
};

struct sde_hdmi_tx_ddc_ctrl {
	struct completion rx_status_done;
	struct dss_io_data *io;
	struct sde_hdmi_tx_ddc_data ddc_data;
	struct sde_hdmi_tx_hdcp2p2_ddc_data sde_hdcp2p2_ddc_data;
};

/* DDC */
int sde_hdmi_ddc_write(void *cb_data);
int sde_hdmi_ddc_read(void *cb_data);
int sde_hdmi_ddc_scrambling_isr(void *hdmi_display);
int _sde_hdmi_get_timeout_in_hysnc(void *hdmi_display, u32 timeout_ms);
void _sde_hdmi_scrambler_ddc_disable(void *hdmi_display);
void sde_hdmi_hdcp2p2_ddc_disable(void *hdmi_display);
int sde_hdmi_hdcp2p2_read_rxstatus(void *hdmi_display);
void sde_hdmi_ddc_config(void *hdmi_display);
int sde_hdmi_ddc_hdcp2p2_isr(void *hdmi_display);
void sde_hdmi_dump_regs(void *hdmi_display);
#endif /* _SDE_HDMI_UTIL_H_ */