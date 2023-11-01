/*--------------------------------------------------------------------------*/
/* Copyright 2022-2023 NXP                                                  */
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

#include "common.h"

#include <mcuxClEls.h> // Interface to the entire mcuxClEls component
#include <mcuxClSession.h> // Interface to the entire mcuxClSession component
#include <mcuxClKey.h> // Interface to the entire mcuxClKey component
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h> // Code flow protection
#include <mcuxClToolchain.h> // memory segment definitions
#include <stdbool.h>  // bool type for the example's return code
#include <mcuxClPsaDriver.h>
#include <mcuxClCore_Examples.h>
#include <mcuxClExample_ELS_Helper.h>

#define LIFETIME_INTERNAL PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_VOLATILE, PSA_KEY_LOCATION_EXTERNAL_STORAGE)
#define LIFETIME_EXTERNAL PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_VOLATILE, PSA_KEY_LOCATION_LOCAL_STORAGE)

bool mcuxClPsaDriver_cipher_multipart_CTR(void)
{
    /** Values from NIST Special Publication 800-38A 2001 - F.5.1 CTR-AES128.Encrypt */
    /** Key for the AES encryption. */
    uint8_t const aes128_key[16u] = {   0x2Bu, 0x7Eu, 0x15u, 0x16u,
                                        0x28u, 0xAEu, 0xD2u, 0xA6u,
                                        0xABu, 0xF7u, 0x15u, 0x88u,
                                        0x09u, 0xCFu, 0x4Fu, 0x3Cu};

    /** IV of the AES encryption. */
    uint8_t const aes128_iv[16u] = {    0xF0u, 0xF1u, 0xF2u, 0xF3u,
                                        0xF4u, 0xF5u, 0xF6u, 0xF7u,
                                        0xF8u, 0xF9u, 0xFAu, 0xFBu,
                                        0xFCu, 0xFDu, 0xFEu, 0xFFu};

    /** Plaintext input for the AES encryption. */
    uint8_t const aes128_input[32u] = { 0x6Bu, 0xC1u, 0xBEu, 0xE2u,
                                        0x2Eu, 0x40u, 0x9Fu, 0x96u,
                                        0xE9u, 0x3Du, 0x7Eu, 0x11u,
                                        0x73u, 0x93u, 0x17u, 0x2Au,
                                        0xAEu, 0x2Du, 0x8Au, 0x57u,
                                        0x1Eu, 0x03u, 0xACu, 0x9Cu,
                                        0x9Eu, 0xB7u, 0x6Fu, 0xaCu,
                                        0x45u, 0xAFu, 0x8Eu, 0x51u};

    /** Expected ciphertext output of the AES encryption. */
    uint8_t const aes128_result[32u] = {0x87u, 0x4Du, 0x61u, 0x91u,
                                        0xB6u, 0x20u, 0xE3u, 0x26u,
                                        0x1Bu, 0xEFu, 0x68u, 0x64u,
                                        0x99u, 0x0Du, 0xB6u, 0xCEu,
                                        0x98u, 0x06u, 0xF6u, 0x6Bu,
                                        0x79u, 0x70u, 0xFDu, 0xFFu,
                                        0x86u, 0x17u, 0x18u, 0x7Bu,
                                        0xB9u, 0xFFu, 0xFDu, 0xFFu};

    /* Output buffer for the AES operation */
    uint8_t aes128_output[sizeof(aes128_result)];

    /** Initialize ELS, Enable the ELS **/
    if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Set up PSA key attributes. */
    psa_key_attributes_t attributes = {
        .core = {                               // Core attributes
            .type = PSA_KEY_TYPE_AES,           // Key is for AES operations
            .bits = 0U,                         // No key bits
            .lifetime = LIFETIME_EXTERNAL,      // Volatile (RAM), Local Storage (plain) key
            .id = 0U,                           // ID zero
            .policy = {
                .usage = PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, // Key may be used for encryption and decryption
                .alg = PSA_ALG_CTR,             // Key may be used for CTR mode
                .alg2 = PSA_ALG_NONE},
            .flags = 0u},                       // No flags
        .domain_parameters = NULL,              // No domain parameters
        .domain_parameters_size = 0u};

    /* Variable for the output length of the encryption operation */
    size_t output_length = 0u;

    psa_cipher_operation_t operation;

    /* Call the encryption operation */
    psa_status_t result = psa_driver_wrapper_cipher_encrypt_setup(
        &operation,                         // psa_operation_driver_context_t *operation,
        &attributes,                        // const psa_key_attributes_t *attributes,
        aes128_key,                         // const uint8_t *key_buffer
        MCUXCLELS_CIPHER_KEY_SIZE_AES_128,   // size_t key_buffer_size
        PSA_ALG_CTR             // psa_algorithm_t alg
    );

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    result = psa_driver_wrapper_cipher_set_iv(
        &operation,        // psa_cipher_operation_t *operation,
        aes128_iv,         // const uint8_t *iv,
        sizeof(aes128_iv)  // size_t iv_length
    );

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    result = psa_driver_wrapper_cipher_update(
        &operation,                         // psa_operation_driver_context_t *operation,
        aes128_input,                       // const uint8_t *input
        sizeof(aes128_input),               // size_t input_length
        aes128_output,                      // uint8_t *output
        sizeof(aes128_output),              // size_t output_size
        &output_length);                    // size_t *output_length

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length */
    if(output_length != sizeof(aes128_output)) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    result = psa_driver_wrapper_cipher_finish(
        &operation,                         // psa_cipher_operation_t *operation,
        aes128_output,                      // uint8_t *output,
        sizeof(aes128_output),              // size_t output_size,
        &output_length                      // size_t *output_length
        );

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length - no output was return */
    if(0u != output_length)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }


    /* Check the content */
    for (size_t i = 0U; i < sizeof(aes128_result); i++)
    {
        if (aes128_output[i] != aes128_result[i]) // Expect that the resulting encrypted msg matches our initial message
        {
            return MCUXCLEXAMPLE_STATUS_ERROR;
        }
    }


/* Call the decryption operation */
    output_length = 0u;

    result = psa_driver_wrapper_cipher_decrypt_setup(
        &operation,                         // psa_operation_driver_context_t *operation,
        &attributes,                        // const psa_key_attributes_t *attributes,
        aes128_key,                         // const uint8_t *key_buffer
        MCUXCLELS_CIPHER_KEY_SIZE_AES_128,   // size_t key_buffer_size
        PSA_ALG_CTR             // psa_algorithm_t alg
    );

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    result = psa_driver_wrapper_cipher_set_iv(
        &operation,        // psa_cipher_operation_t *operation,
        aes128_iv,         // const uint8_t *iv,
        sizeof(aes128_iv)  // size_t iv_length
    );

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    result = psa_driver_wrapper_cipher_update(
        &operation,                         // psa_operation_driver_context_t *operation,
        aes128_result,                      // const uint8_t *input
        sizeof(aes128_result),              // size_t input_length
        aes128_output,                      // uint8_t *output
        sizeof(aes128_output),              // size_t output_size
        &output_length);                    // size_t *output_length

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length */
    if(output_length != sizeof(aes128_output)) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    result = psa_driver_wrapper_cipher_finish(
        &operation,                         // psa_cipher_operation_t *operation,
        aes128_output,                      // uint8_t *output,
        sizeof(aes128_output),              // size_t output_size,
        &output_length                      // size_t *output_length
        );

    /* Check the return value */
    if(result != PSA_SUCCESS) {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the content */
    for (size_t i = 0U; i < sizeof(aes128_output); i++)
    {
        if (aes128_output[i] != aes128_input[i]) // Expect that the resulting decrypted msg matches our initial message
        {
            return MCUXCLEXAMPLE_STATUS_ERROR;
        }
    }

    /* Return */
    return MCUXCLEXAMPLE_STATUS_OK;
}
bool nxpClPsaDriver_cipher_multipart_CTR(void)
{
    bool result = mcuxClPsaDriver_cipher_multipart_CTR();
    return result;
}
