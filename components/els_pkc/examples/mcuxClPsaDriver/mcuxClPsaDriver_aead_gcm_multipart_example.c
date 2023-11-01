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

#include <mcuxClCore_Examples.h> // Defines for example return codes and assertions
#include <mcuxClExample_ELS_Helper.h>
#include <mcuxClEls.h> // Interface to the entire mcuxClEls component
#include <mcuxClSession.h> // Interface to the entire mcuxClSession component
#include <mcuxClKey.h> // Interface to the entire mcuxClKey component
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h> // Code flow protection
#include <mcuxClToolchain.h> // memory segment definitions
#include <stdbool.h>  // bool type for the example's return code
#include <mcuxClPsaDriver.h>
#include <mcuxClAes.h> // Interface to AES-related definitions and types

#define LIFETIME_INTERNAL PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_VOLATILE, PSA_KEY_LOCATION_EXTERNAL_STORAGE)
#define LIFETIME_EXTERNAL PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_VOLATILE, PSA_KEY_LOCATION_LOCAL_STORAGE)

bool mcuxClPsaDriver_aead_gcm_multipart_example(void)
{
    /** Key for the AES encryption. */
    const uint8_t aes128_key[MCUXCLAES_BLOCK_SIZE] = {
        0x2BU, 0x7EU, 0x15U, 0x16U,
        0x28U, 0xAEU, 0xD2U, 0xA6U,
        0xABU, 0xF7U, 0x15U, 0x88U,
        0x09U, 0xCFU, 0x4FU, 0x3CU
    };

    /** IV of the AES encryption. */
    const uint8_t aes128_iv[12u] = {
        0xF8U, 0xD2U, 0x68U, 0x76U,
        0x81U, 0x6FU, 0x0FU, 0xBAU,
        0x86U, 0x2BU, 0xD8U, 0xA3U
    };

    /** Plaintext input for the AES encryption. */
    const uint8_t msg_plain[MCUXCLAES_BLOCK_SIZE] = {
        0x6BU, 0xC1U, 0xBEU, 0xE2U,
        0x2EU, 0x40U, 0x9FU, 0x96U,
        0xE9U, 0x3DU, 0x7EU, 0x11U,
        0x73U, 0x93U, 0x17U, 0x2AU
    };

    /** Additional authenticated data. */
    const uint8_t msg_adata[MCUXCLAES_BLOCK_SIZE] = {
        0xCAU, 0xEAU, 0x07U, 0x26U,
        0x62U, 0xE2U, 0x20U, 0x06U,
        0x2DU, 0x45U, 0x46U, 0x41U,
        0x5EU, 0xFFU, 0xFAU, 0xD2U
    };

    /** Expected ciphertext output of the AES-GCM encryption. */
    const uint8_t msg_enc_expected[MCUXCLAES_BLOCK_SIZE] = {
        0x4FU, 0x74U, 0x2DU, 0xF6U,
        0x9DU, 0x1CU, 0x03U, 0x6BU,
        0x56U, 0xBCU, 0xC2U, 0x81U,
        0x5FU, 0xDAU, 0x8DU, 0x6DU
    };

    /** Expected authentication tag output. */
    const uint8_t msg_tag_expected[MCUXCLAES_BLOCK_SIZE] = {
        0xB2U, 0xC5U, 0xCFU, 0xC3U,
        0xF2U, 0x8CU, 0x9FU, 0x78U,
        0xFCU, 0x25U, 0xBCU, 0x10U,
        0xC9U, 0xCAU, 0xFFU, 0xD5U
    };

    /* Example sizes for multiple calls for aad and input. */
    size_t input_size_part_1 = MCUXCLAES_BLOCK_SIZE - 6u;
    size_t input_size_part_2 = MCUXCLAES_BLOCK_SIZE - input_size_part_1;

    /** Initialize ELS, Enable the ELS **/
    if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /****************/
    /** Encryption **/
    /****************/

    /* Set up PSA key attributes. */
    psa_key_attributes_t attributes = {
        .core = {                                // Core attributes
            .type = PSA_KEY_TYPE_AES,            // Key is for AES operations
            .bits = 0U,                          // No key bits
            .lifetime = LIFETIME_EXTERNAL,       // Volatile (RAM), Local Storage (plain) key
            .id = 0U,                            // ID zero
            .policy = {
                .usage = PSA_KEY_USAGE_ENCRYPT,  // Key may be used for encryption
                .alg = PSA_ALG_GCM,              // Key may be used for AEAD GCM mode only
                .alg2 = PSA_ALG_NONE
            },
            .flags = 0u},                        // No flags
        .domain_parameters = NULL,               // No domain parameters
        .domain_parameters_size = 0u
    };

    /* Initialize the operation with all zeros */
    psa_aead_operation_t operation = psa_aead_operation_init();

    /* Call the AEAD setup operation */
    psa_status_t result = psa_driver_wrapper_aead_encrypt_setup(
        &operation,           // psa_mac_operation_t *operation,
        &attributes,          // const psa_key_attributes_t *attributes,
        aes128_key,           // const uint8_t *key_buffer,
        sizeof(aes128_key),   // size_t key_buffer_size,
        PSA_ALG_GCM           // psa_algorithm_t alg
    );

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD operation to set the lengths */
    result = psa_driver_wrapper_aead_set_lengths(
        &operation,           // psa_aead_operation_t *operation
        sizeof(msg_adata),    // size_t ad_length
        sizeof(msg_plain)     // size_t plaintext_length
    );

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD operation to set the nonce */
    result = psa_driver_wrapper_aead_set_nonce(
        &operation,           // psa_aead_operation_t *operation
        aes128_iv,            // const uint8_t *nonce
        sizeof(aes128_iv)     // size_t nonce_length
    );

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD update operation for the aad - part 1 */
    result = psa_driver_wrapper_aead_update_ad(
        &operation,           // psa_aead_operation_t *operation
        msg_adata,            // const uint8_t *input
        input_size_part_1     // size_t input_length
    );
    
     /* Update remaining ad bytes */
    operation.ad_remaining -= input_size_part_1;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD update operation for the aad - part 2 */
    result = psa_driver_wrapper_aead_update_ad(
        &operation,                     // psa_aead_operation_t *operation
        msg_adata + input_size_part_1,  // const uint8_t *input
        input_size_part_2               // size_t input_length
    );

    /* Update remaining ad bytes */
    operation.ad_remaining -= input_size_part_2;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Variable for the output length of the update/finish operations */
    size_t output_length = 0u;

    /* Output buffer for the AEAD update/finish operations */
    uint8_t output_enc[sizeof(msg_plain)];

    /* Call the AEAD update operation for the input - part 1 */
    result = psa_driver_wrapper_aead_update(
        &operation,           // psa_aead_operation_t *operation
        msg_plain,            // const uint8_t *input
        input_size_part_1,    // size_t input_length
        output_enc,           // const uint8_t *output
        sizeof(output_enc),   // size_t output_size
        &output_length        // size_t *output_length
    );

    /* Update remaining bytes*/
    operation.body_remaining -= input_size_part_1;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length - no output was return  */
    if(0u != output_length)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD update operation for the input - part 2 */
    result = psa_driver_wrapper_aead_update(
        &operation,                          // psa_aead_operation_t *operation
        msg_plain + input_size_part_1,       // const uint8_t *input
        input_size_part_2,                   // size_t input_length
        output_enc + output_length,          // const uint8_t *output
        sizeof(output_enc) - output_length,  // size_t output_size
        &output_length                       // size_t *output_length
    );

    /* Update remaining bytes */
    operation.body_remaining -= input_size_part_2;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length */
    if((input_size_part_1 + input_size_part_2) != output_length)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    size_t tag_length = 0u;

    /* Output buffer for the AEAD finish operation */
    uint8_t tag[PSA_ALG_AEAD_GET_TAG_LENGTH(PSA_ALG_GCM)];

    /* Call the AEAD finish operation */
    result = psa_driver_wrapper_aead_finish(
        &operation,                          // psa_aead_operation_t *operation
        output_enc + (input_size_part_1 + input_size_part_2),          // const uint8_t *ciphertext
        sizeof(output_enc) - (input_size_part_1 + input_size_part_2),  // size_t ciphertext_size
        &output_length,                      // size_t *ciphertext_length
        tag,                                 // uint8_t *tag
        sizeof(tag),                         // size_t tag_size
        &tag_length                          // size_t *tag_length
    );

    /* Check the output length - no output was return */
    if(0u != output_length)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output lengths */
    if((sizeof(msg_tag_expected) != tag_length))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the content of the encrypted output data */
    if(!mcuxClCore_assertEqual(output_enc, msg_enc_expected, sizeof(msg_plain)))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the content of the tag */
    if(!mcuxClCore_assertEqual(tag, msg_tag_expected, sizeof(msg_tag_expected)))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /****************/
    /** Decryption **/
    /****************/

    /* Update key usage */
    attributes.core.policy.usage = PSA_KEY_USAGE_DECRYPT;  // Key may be used for decryption

    /* reset output length */
    output_length = 0u;

    /* Initialize the operation with all zeros */
    operation = psa_aead_operation_init();

    /* Call the AEAD setup operation */
    result = psa_driver_wrapper_aead_decrypt_setup(
        &operation,           // psa_mac_operation_t *operation,
        &attributes,          // const psa_key_attributes_t *attributes,
        aes128_key,           // const uint8_t *key_buffer,
        sizeof(aes128_key),   // size_t key_buffer_size,
        PSA_ALG_GCM           // psa_algorithm_t alg
    );

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD operation to set the lengths */
    result = psa_driver_wrapper_aead_set_lengths(
        &operation,           // psa_aead_operation_t *operation
        sizeof(msg_adata),    // size_t ad_length
        sizeof(msg_plain)     // size_t plaintext_length
    );

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD operation to set the nonce */
    result = psa_driver_wrapper_aead_set_nonce(
        &operation,           // psa_aead_operation_t *operation
        aes128_iv,            // const uint8_t *nonce
        sizeof(aes128_iv)     // size_t nonce_length
    );

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD update operation for the aad - part 1 */
    result = psa_driver_wrapper_aead_update_ad(
        &operation,           // psa_aead_operation_t *operation
        msg_adata,            // const uint8_t *input
        input_size_part_1     // size_t input_length
    );

    /* Update remaining ad bytes */
    operation.ad_remaining -= input_size_part_1;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD update operation for the aad - part 2 */
    result = psa_driver_wrapper_aead_update_ad(
        &operation,                     // psa_aead_operation_t *operation
        msg_adata + input_size_part_1,  // const uint8_t *input
        input_size_part_2               // size_t input_length
    );

    /* Update remaining ad bytes */
    operation.ad_remaining -= input_size_part_2;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Output buffer for the AEAD update/verify operations */
    uint8_t output_dec[sizeof(msg_plain)];

    /* Call the AEAD update operation for the input - part 1 */
    result = psa_driver_wrapper_aead_update(
        &operation,           // psa_aead_operation_t *operation
        output_enc,           // const uint8_t *input
        input_size_part_1,    // size_t input_length
        output_dec,           // const uint8_t *output
        sizeof(output_dec),   // size_t output_size
        &output_length        // size_t *output_length
    );

    /* Update remaining bytes*/
    operation.body_remaining -= input_size_part_1;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length */
    if(0u != output_length)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD update operation for the input - part 2 */
    result = psa_driver_wrapper_aead_update(
        &operation,                          // psa_aead_operation_t *operation
        output_enc + input_size_part_1,      // const uint8_t *input
        input_size_part_2,                   // size_t input_length
        output_dec + output_length,          // const uint8_t *output
        sizeof(output_dec) - output_length,  // size_t output_size
        &output_length                       // size_t *output_length
    );

    /* Update remaining bytes*/
    operation.body_remaining -= input_size_part_2;

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length */
    if((input_size_part_1 + input_size_part_2) != output_length)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Call the AEAD verify operation */
    result = psa_driver_wrapper_aead_verify(
        &operation,                          // psa_aead_operation_t *operation
        output_dec + (input_size_part_1 + input_size_part_2),          // const uint8_t *plaintext
        sizeof(output_dec) - (input_size_part_1 + input_size_part_2),  // size_t plaintext_size
        &output_length,                      // size_t *plaintext_length
        tag,                                 // uint8_t *tag
        sizeof(tag)                          // size_t tag_length
    );

    /* Check the return value */
    if(PSA_SUCCESS != result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the output length - no output was return */
    if(0u != output_length)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }


    /* Check the content of the encrypted output data */
    if(!mcuxClCore_assertEqual(output_dec, msg_plain, sizeof(msg_plain)))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Success */
    return MCUXCLEXAMPLE_STATUS_OK;
}
bool nxpClPsaDriver_aead_gcm_multipart_example(void)
{
    bool result = mcuxClPsaDriver_aead_gcm_multipart_example();
    return result;
}
