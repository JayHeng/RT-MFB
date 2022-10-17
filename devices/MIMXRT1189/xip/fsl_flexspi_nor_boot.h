/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FLEXSPI_NOR_BOOT_H__
#define __FLEXSPI_NOR_BOOT_H__

#include <stdint.h>
#include "board.h"
#include "fsl_common.h"

/*! @name Driver version */
/*@{*/
/*! @brief XIP_DEVICE driver version 2.0.4. */
#define FSL_XIP_DEVICE_DRIVER_VERSION (MAKE_VERSION(2, 0, 4))
/*@}*/

/*************************************
 *  Container
 *************************************/
/* Container header */
#define CNT_TAG_HEADER 0x87
#define CNT_SIZE       (uint16_t)(sizeof(container))
#define CNT_VERSION    0x00
#define CNT_NUM_IMG    1
#define CNT_FUSE_VER   0
#define CNT_SW_VER     0
#define CNT_FLAGS      0x00000000 // Container not authenticated

#define IMG_FLAGS 0x00000213 // Non-encrypted, SHA512, CM33, Executable

#define SGNBK_TAG     0x90
#define SGNBK_SIZE    (uint16_t)(sizeof(sign_block))
#define SGNBK_VERSION 0x00

typedef struct __attribute__((packed)) _cnt_hdr_
{
    uint8_t version;
    uint16_t length;
    uint8_t tag;

    uint32_t flags;

    uint16_t sw_ver;
    uint8_t fuse_ver;
    uint8_t num_images;

    uint16_t sign_blk_offset;
    uint16_t reserved1;
} cnt_hdr;

typedef struct __attribute__((packed)) _img_entry_
{
    uint32_t offset;
    uint32_t size;
    uint32_t load_addr;
    uint32_t reserved1;
    uint32_t entry;
    uint32_t reserved2;
    uint32_t flags;
    uint32_t metadata;
    uint8_t hash[64];
    uint8_t iv[32];
} image_entry;

typedef struct __attribute__((packed)) _sign_block_
{
    uint8_t version;
    uint16_t length;
    uint8_t tag;

    uint16_t cert_offset;
    uint16_t srk_offset;
    uint16_t sign_offset;
    uint16_t blob_offset;
    uint32_t reserved1;
    uint8_t data[0];
} sign_block;

typedef struct __attribute__((packed)) _container_
{
    cnt_hdr hdr;
    image_entry array[CNT_NUM_IMG];
    sign_block sign_block;
} container;

/* Set resume entry */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Reset_Handler[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)Reset_Handler)
#define BOOT_IMAGE_BASE     ((uint32_t)FLASH_BASE)
#define BOOT_IMAGE_SIZE     ((uint32_t)FLASH_SIZE)
#define DCD_DATA_ADDRESS    dcd_data
#elif defined(__MCUXPRESSO)
extern uint32_t ResetISR[];
// extern uint32_t __boot_hdr_start__[];
// extern uint32_t __boot_hdr_end__[];
// extern uint32_t __boot_hdr_conf__[];
// extern uint32_t __boot_hdr_xmcd__[];
// extern uint32_t __boot_hdr_container__[];
extern uint32_t app_image_offset[];
extern uint32_t _boot_loadaddr[];
extern uint32_t _boot_size[];
extern uint32_t _image_start[];
extern uint32_t _image_size[];
extern uint32_t _image_loadaddr[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)ResetISR)
#define IMAGE_LOAD_ADDRESS  ((uint32_t)_image_loadaddr)
#define IMAGE_SIZE          ((uint32_t)_image_size)
#define BOOT_IMAGE_BASE     ((uint32_t)_boot_loadaddr)
#define BOOT_IMAGE_SIZE     ((uint32_t)_boot_size)
#define DCD_DATA_ADDRESS    ((uint32_t)__boot_hdr_xmcd__)
#elif defined(__ICCARM__)
extern uint32_t Reset_Handler[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)Reset_Handler)
#define BOOT_IMAGE_BASE     ((uint32_t)FLASH_BASE)
#define BOOT_IMAGE_SIZE     ((uint32_t)FLASH_SIZE)
#define DCD_DATA_ADDRESS    dcd_data
#elif defined(__GNUC__)
extern uint32_t Reset_Handler[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)Reset_Handler)
#define BOOT_IMAGE_BASE     ((uint32_t)FLASH_BASE)
#define BOOT_IMAGE_SIZE     ((uint32_t)FLASH_SIZE)
#define DCD_DATA_ADDRESS    dcd_data
#endif
#if defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1)
#if defined(XIP_BOOT_HEADER_DCD_ENABLE) && (1 == XIP_BOOT_HEADER_DCD_ENABLE)
#define DCD_ADDRESS DCD_DATA_ADDRESS
#else
#define DCD_ADDRESS 0
#endif
#endif

#if __CORTEX_M == 7
#define FLASH_BASE FlexSPI1_AMBA_BASE
#elif __CORTEX_M == 33
#define FLASH_BASE FlexSPI1_ALIAS_BASE
#endif

#if defined(BOARD_FLASH_SIZE)
#define FLASH_SIZE BOARD_FLASH_SIZE
#else
#error "Please define macro BOARD_FLASH_SIZE"
#endif

/* External Variables */
#if defined(XIP_BOOT_HEADER_DCD_ENABLE) && (1 == XIP_BOOT_HEADER_DCD_ENABLE)
extern const uint8_t dcd_data[];
#endif

#endif /* __FLEXSPI_NOR_BOOT_H__ */
