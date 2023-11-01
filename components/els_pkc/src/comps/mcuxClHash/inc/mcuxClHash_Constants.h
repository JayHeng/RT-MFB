/*--------------------------------------------------------------------------*/
/* Copyright 2020-2023 NXP                                                  */
/*                                                                          */
/* NXP Confidential. This software is owned or controlled by NXP and may    */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms. If you do not agree to be bound by the applicable license */
/* terms, then you may not retain, install, activate or otherwise use the   */
/* software.                                                                */
/*--------------------------------------------------------------------------*/

/** @file  mcuxClHash_Constants.h
 *  @brief Constants for use with the mcuxClHash component */

#ifndef MCUXCLHASH_CONSTANTS_H_
#define MCUXCLHASH_CONSTANTS_H_

#include <mcuxClConfig.h> // Exported features flags header

/**
 * @defgroup mcuxClHash_Constants mcuxClHash_Constants
 * @brief Constants of @ref mcuxClHash component
 * @ingroup mcuxClHash
 * @{
 */

/**
 * @defgroup MCUXCLHASH_STATUS_ MCUXCLHASH_STATUS_
 * @brief Return code definitions
 * @ingroup mcuxClHash_Constants
 * @{
 */
#define MCUXCLHASH_STATUS_OK                                  ((mcuxClHash_Status_t) 0x06662E03u )                  ///< Hash operation successful
#define MCUXCLHASH_STATUS_COMPARE_EQUAL                       ((mcuxClHash_Status_t) 0x06662E07u)                   ///< Hash operation and comparison of result successful
#define MCUXCLHASH_COMPARE_EQUAL                              MCUXCLHASH_STATUS_COMPARE_EQUAL                       ///< \deprecated Replaced by MCUXCLHASH_STATUS_COMPARE_EQUAL
#define MCUXCLHASH_STATUS_FAILURE                             ((mcuxClHash_Status_t) 0x06665330u )                  ///< Hash operation failed
#define MCUXCLHASH_FAILURE                                    MCUXCLHASH_STATUS_FAILURE                             ///< \deprecated Replaced by MCUXCLHASH_STATUS_FAILURE
#define MCUXCLHASH_STATUS_INVALID_PARAMS                      ((mcuxClHash_Status_t) 0x066653F8u )                  ///< Hash function called with invalid parameters
#define MCUXCLHASH_STATUS_EXPORT_STATE_NOT_MULTIPLE_OF_BLOCK  ((mcuxClHash_Status_t) 0x06665338u)                   ///< Export on state, for which a NON-multiple of the blocksize has been hashed
#define MCUXCLHASH_EXPORT_STATE_NOT_MULTIPLE_OF_BLOCK         MCUXCLHASH_STATUS_EXPORT_STATE_NOT_MULTIPLE_OF_BLOCK  ///< \deprecated Replaced by MCUXCLHASH_STATUS_EXPORT_STATE_NOT_MULTIPLE_OF_BLOCK
#define MCUXCLHASH_STATUS_COMPARE_NOT_EQUAL                   ((mcuxClHash_Status_t) 0x06668930u)                   ///< Hash operation succeeded, but comparison of result failed
#define MCUXCLHASH_COMPARE_NOT_EQUAL                          ((mcuxClHash_Status_t) 0x06668930u)                   ///< \deprecated Replaced by MCUXCLHASH_STATUS_COMPARE_NOT_EQUAL
#define MCUXCLHASH_STATUS_FAULT_ATTACK                        ((mcuxClHash_Status_t) 0x0666F0F0u )                  ///< Fault attack (unexpected behavior) detected
/**@}*/

/**
 * @defgroup MCUXCLHASH_OUTPUT_SIZE_ MCUXCLHASH_OUTPUT_SIZE_
 * @brief Defines for digest sizes
 * @ingroup mcuxClHash_Constants
 * @{
 */
#define MCUXCLHASH_OUTPUT_SIZE_MD5                  (16U) ///< MD5 output size: 128 bit (16 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_1                (20U) ///< SHA-1 output size: 160 bit (20 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_224              (28U) ///< SHA-224 output size: 224 bit (28 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_256              (32U) ///< SHA-256 output size: 256 bit (32 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_384              (48U) ///< SHA-384 output size: 384 bit (48 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_512              (64U) ///< SHA-512 output size: 512 bit (64 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_512_224          (28U) ///< SHA-512/224 output size: 224 bit (28 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA_512_256          (32U) ///< SHA-512/256 output size: 256 bit (32 bytes)
/**@}*/


/**
 * @defgroup MCUXCLHASH_OUTPUT_SIZE_ MCUXCLHASH_OUTPUT_SIZE_
 * @brief Defines for digest sizes
 * @ingroup mcuxClHash_Constants
 * @{
 */
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_224        (28uL)  ///< SHA3-224        output size:  224 bit (28 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_256        (32uL)  ///< SHA3-256        output size:  256 bit (32 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_384        (48uL)  ///< SHA3-384        output size:  384 bit (48 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_512        (64uL)  ///< SHA3-512        output size:  512 bit (64 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_SHAKE_128  (168uL) ///< SHA3-SHAKE 128  output size: 1344 bit (168 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_SHAKE_256  (136uL) ///< SHA3-SHAKE 256  output size: 1088 bit (136 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_CSHAKE_128 (168uL) ///< SHA3-CSHAKE 128 output size: 1344 bit (168 bytes)
#define MCUXCLHASH_OUTPUT_SIZE_SHA3_CSHAKE_256 (136uL) ///< SHA3-CSHAKE 256 output size: 1088 bit (136 bytes)
#define MCUXCLHASH_MAX_OUTPUT_SIZE             (MCUXCLHASH_OUTPUT_SIZE_SHA3_SHAKE_128) ///< Maximum output size
/**@}*/





/**@}*/

#endif /* MCUXCLHASH_CONSTANTS_H_ */
