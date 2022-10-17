/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_MEMORY_H_
#define _FSL_MEMORY_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.memory"
#endif

#if (__CORTEX_M == 33U)
/* The CM33 subsystem Code TCM start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_CODE_TCM_BEGIN 0xFFC0000U
/* The CM33 subsystem Code TCM end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_CODE_TCM_END 0xFFFFFFFU
/* The CM33 subsystem Code TCM alias start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_CODE_TCM_ALIAS_BEGIN 0x201C0000U
/* The CM33 subsystem Code TCM alias end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_CODE_TCM_ALIAS_END 0x201FFFFFU

/* The CM33 subsystem System TCM start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_SYSTEM_TCM_BEGIN 0x20000000U
/* The CM33 subsystem System TCM end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_SYSTEM_TCM_END 0x2003FFFFU
/* The CM33 subsystem System TCM alias start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_SYSTEM_TCM_ALIAS_BEGIN 0x20200000U
/* The CM33 subsystem System TCM alias end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M33_SYSTEM_TCM_ALIAS_END 0x2023FFFFU

#define FSL_MEM_M33_CODE_TCM_OFFSET       (FSL_MEM_M33_CODE_TCM_ALIAS_BEGIN - FSL_MEM_M33_CODE_TCM_BEGIN)
#define FSL_MEM_M33_SYSTEM_TCM_OFFSET     (FSL_MEM_M33_SYSTEM_TCM_ALIAS_BEGIN - FSL_MEM_M33_SYSTEM_TCM_BEGIN)
#define FSL_MEM_M33_SECURE_ADDRESS_OFFSET 0x10000000U

#elif (__CORTEX_M == 7U)
/* The CM7 subsystem ITCM start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_ITCM_BEGIN       0x0U
/* The CM7 subsystem ITCM end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_ITCM_END         0x7FFFFU
/* The CM7 subsystem ITCM alias start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_ITCM_ALIAS_BEGIN 0x20380000U
/* The CM7 subsystem ITCM alias end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_ITCM_ALIAS_END   0x203FFFFFU

/* The CM7 subsystem DTCM start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_DTCM_BEGIN       0x20000000U
/* The CM7 subsystem DTCM end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_DTCM_END         0x2007FFFFU
/* The CM7 subsystem DTCM alias start address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_DTCM_ALIAS_BEGIN 0x20400000U
/* The CM7 subsystem DTCM alias end address, refer to Reference Manual for detailed information */
#define FSL_MEM_M7_DTCM_ALIAS_END   0x2047FFFFU

#define FSL_MEM_M7_ITCM_OFFSET (FSL_MEM_M7_ITCM_ALIAS_BEGIN - FSL_MEM_M7_ITCM_BEGIN)
#define FSL_MEM_M7_DTCM_OFFSET (FSL_MEM_M7_DTCM_ALIAS_BEGIN - FSL_MEM_M7_DTCM_BEGIN)
#else
#error "Device is not supported by this driver!"
#endif

typedef enum _mem_direction
{
    kMEMORY_Local2DMA = 0,
    kMEMORY_DMA2Local,
} mem_direction_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
/*!
 * @brief Convert the memory map address.
 *
 * This function convert the address between system mapped address and native mapped address.
 * There maybe offset between subsystem native address and system address for some memory,
 * this funciton convert the address to different memory map.
 * @param addr address need to be converted.
 * @param direction convert direction.
 * @return the converted address
 */
static inline uint32_t MEMORY_ConvertMemoryMapAddress(uint32_t addr, mem_direction_t direction)
{
    uint32_t dest;

    switch (direction)
    {
        case kMEMORY_Local2DMA:
        {
#if (__CORTEX_M == 33U)
            if (((addr >= FSL_MEM_M33_CODE_TCM_BEGIN) && (addr <= FSL_MEM_M33_CODE_TCM_END)) ||
                ((addr >= (FSL_MEM_M33_CODE_TCM_BEGIN + FSL_MEM_M33_SECURE_ADDRESS_OFFSET)) &&
                 (addr <= (FSL_MEM_M33_CODE_TCM_END + FSL_MEM_M33_SECURE_ADDRESS_OFFSET))))
            {
                dest = addr + FSL_MEM_M33_CODE_TCM_OFFSET;
            }
            else if (((addr >= FSL_MEM_M33_SYSTEM_TCM_BEGIN) && (addr <= FSL_MEM_M33_SYSTEM_TCM_END)) ||
                     ((addr >= (FSL_MEM_M33_SYSTEM_TCM_BEGIN + FSL_MEM_M33_SECURE_ADDRESS_OFFSET)) &&
                      (addr <= (FSL_MEM_M33_SYSTEM_TCM_END + FSL_MEM_M33_SECURE_ADDRESS_OFFSET))))
            {
                dest = addr + FSL_MEM_M33_SYSTEM_TCM_OFFSET;
            }
#elif (__CORTEX_M == 7U)
            /* 
             * For CM7, ITCM has valid memory 0, but for EDMA, 0 is also pointer NULL, which has specific
             * meaning. Considering we never use ITCM 0 as EDMA buffer, address translation for 0 is ignored
             */
            if ((addr > 0) && (addr <= FSL_MEM_M7_ITCM_END))
            {
                dest = addr + FSL_MEM_M7_ITCM_OFFSET;
            }
            else if ((addr >= FSL_MEM_M7_DTCM_BEGIN) && (addr <= FSL_MEM_M7_DTCM_END))
            {
                dest = addr + FSL_MEM_M7_DTCM_OFFSET;
            }
#endif
            else
            {
                dest = addr;
            }
            break;
        }
        case kMEMORY_DMA2Local:
        {
#if (__CORTEX_M == 33U)
            if (((addr >= FSL_MEM_M33_CODE_TCM_ALIAS_BEGIN) && (addr <= FSL_MEM_M33_CODE_TCM_ALIAS_END)) ||
                ((addr >= (FSL_MEM_M33_CODE_TCM_ALIAS_BEGIN + FSL_MEM_M33_SECURE_ADDRESS_OFFSET)) &&
                 (addr <= (FSL_MEM_M33_CODE_TCM_ALIAS_END + FSL_MEM_M33_SECURE_ADDRESS_OFFSET))))
            {
                dest = addr - FSL_MEM_M33_CODE_TCM_OFFSET;
            }
            else if (((addr >= FSL_MEM_M33_SYSTEM_TCM_ALIAS_BEGIN) && (addr <= FSL_MEM_M33_SYSTEM_TCM_ALIAS_END)) ||
                     ((addr >= (FSL_MEM_M33_SYSTEM_TCM_ALIAS_BEGIN + FSL_MEM_M33_SECURE_ADDRESS_OFFSET)) &&
                      (addr <= (FSL_MEM_M33_SYSTEM_TCM_ALIAS_END + FSL_MEM_M33_SECURE_ADDRESS_OFFSET))))
            {
                dest = addr - FSL_MEM_M33_SYSTEM_TCM_OFFSET;
            }
#elif (__CORTEX_M == 7U)
            if ((addr >= FSL_MEM_M7_ITCM_ALIAS_BEGIN) && (addr <= FSL_MEM_M7_ITCM_ALIAS_END))
            {
                dest = addr - FSL_MEM_M7_ITCM_OFFSET;
            }
            else if ((addr >= FSL_MEM_M7_DTCM_ALIAS_BEGIN) && (addr <= FSL_MEM_M7_DTCM_ALIAS_END))
            {
                dest = addr - FSL_MEM_M7_DTCM_OFFSET;
            }
#endif
            else
            {
                dest = addr;
            }
            break;
        }
        default:
            dest = addr;
            break;
    }

    return dest;
}
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _FSL_MEMORY_H_ */
