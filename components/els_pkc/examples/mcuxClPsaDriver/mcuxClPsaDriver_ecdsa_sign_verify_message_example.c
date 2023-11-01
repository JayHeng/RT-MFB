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

#include <mcuxClSession.h> // Interface to the entire mcuxClSession component
#include <mcuxCsslFlowProtection.h> // Code flow protection
#include <mcuxClPsaDriver.h>
#include <mcuxClCore_Examples.h>
#include <mcuxClExample_ELS_Helper.h>

/**
 * @brief ECC private key used in signature.
 */
static const uint8_t privateKeyBuffer[PSA_BITS_TO_BYTES(256u)] __attribute__ ((aligned (4))) = {
  0x8F, 0xDF, 0x49, 0xC9, 0xDF, 0x2C, 0x3B, 0x91, 0x78, 0x5F, 0xE2, 0xF9, 0xF0, 0x1F, 0x94, 0x6D,
  0x07, 0x1A, 0x1C, 0xB3, 0x21, 0x1E, 0x8F, 0xD7, 0xA6, 0x2B, 0xEE, 0xF5, 0x48, 0x3E, 0xF7, 0x22
};

/**
 * @brief ECC public key used in verification.
 */
static const uint8_t publicKeyBuffer[2u * PSA_BITS_TO_BYTES(256u) + 1u] __attribute__ ((aligned (4))) = {
  0x04,
  0x55, 0xAB, 0x4C, 0x42, 0x0E, 0x4B, 0x95, 0x79, 0x8D, 0x82, 0xC6, 0x29, 0x75, 0x99, 0x9A, 0x8F,
  0xBB, 0xFA, 0xD9, 0x72, 0xCB, 0x00, 0x68, 0x9B, 0x8A, 0xFA, 0x58, 0x6C, 0x55, 0xD5, 0xAE, 0xCE,
  0xFC, 0xE3, 0x04, 0xBE, 0x0F, 0xE9, 0x11, 0xED, 0xFA, 0x83, 0x07, 0x5D, 0x29, 0x9D, 0x0F, 0x2B,
  0xA1, 0x1B, 0xB9, 0x65, 0x83, 0xCC, 0x99, 0x92, 0x03, 0xEB, 0x71, 0xED, 0x72, 0xA7, 0x63, 0xA8
};

/**
 * @brief Input message to be signed
 */
static const uint8_t message[] __attribute__ ((aligned (4))) = {
  0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,  0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
  0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,  0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x70,
  0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,  0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x80
};

/**
 * @brief Signature
 */
static uint8_t signature[PSA_SIGN_OUTPUT_SIZE(PSA_KEY_TYPE_ECC_KEY_PAIR_BASE, 256u, PSA_ALG_ECDSA(PSA_ALG_SHA_384))] __attribute__ ((aligned (4))) = {0};

/*
 *Example of ECDSA signature generation and verification for:
 * - key size 256
 * - SHA-384
 * - signature scheme PSA_ALG_ECDSA(PSA_ALG_SHA_384)
 */
bool mcuxClPsaDriver_ecdsa_sign_verify_message_example(void)
{

  /** Initialize ELS, Enable the ELS **/
  if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
  {
      return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  /*
   * Sign hash: PSA_ALG_ECDSA(PSA_ALG_SHA_384), SHA_384
   */

  /* Set up PSA key attributes. */
  const psa_key_attributes_t sign_attributes = {
    .core = {                                                                                                                // Core attributes
      .type = PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),                                                             // Curve secp256r1
      .bits = 256u,                                                                                                          // Key size in bits
      .lifetime = PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_VOLATILE, PSA_KEY_LOCATION_LOCAL_STORAGE), // Volatile (RAM), Local Storage for private key
      .id = 0U,                                                                                                              // ID zero
      .policy = {
        .usage = PSA_KEY_USAGE_SIGN_MESSAGE,                                                                                 // Key may be used for signing a message
        .alg = PSA_ALG_ECDSA(PSA_ALG_SHA_384),                                                                               // ECDSA signature scheme without specifying a hash algorithm
        .alg2 = PSA_ALG_NONE},
      .flags = 0U},                                                                                                          // No flags
    .domain_parameters = NULL,
    .domain_parameters_size = 0U};

  size_t signature_length;

  psa_status_t sign_status = psa_driver_wrapper_sign_message(
    &sign_attributes,               //const psa_key_attributes_t *attributes,
    privateKeyBuffer,               //const uint8_t *key_buffer,
    sizeof(privateKeyBuffer),       //size_t key_buffer_size,
    PSA_ALG_ECDSA(PSA_ALG_SHA_384), //psa_algorithm_t alg,
    message,                        //const uint8_t *input,
    sizeof(message),                //size_t input_length,
    signature,                      //uint8_t *signature,
    sizeof(signature),              //size_t signature_size,
    &signature_length               //size_t *signature_length
    );

  /* Check the return value */
  if(sign_status != PSA_SUCCESS)
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  /* Check the signature length */
  if(signature_length != PSA_SIGN_OUTPUT_SIZE(PSA_KEY_TYPE_ECC_KEY_PAIR_BASE, 256u, PSA_ALG_ECDSA(PSA_ALG_SHA_384)))
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  /*
   * Verify hash: PSA_ALG_ECDSA(PSA_ALG_SHA_384), SHA_384
   */

  /* Set up PSA key attributes. */
  const psa_key_attributes_t verify_attributes = {
    .core = {                                                                                                                // Core attributes
      .type = PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1),                                                           // Curve secp256r1
      .bits = 256u,                                                                                                          // Key bits
      .lifetime = PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_VOLATILE, PSA_KEY_LOCATION_LOCAL_STORAGE), // Volatile (RAM), Local Storage for public key
      .id = 0U,                                                                                                              // ID zero
      .policy = {
        .usage = PSA_KEY_USAGE_VERIFY_MESSAGE,                                                                               // Key may be used for verify a message
        .alg = PSA_ALG_ECDSA(PSA_ALG_SHA_384),                                                                               // ECDSA signature scheme without specifying a hash algorithm
        .alg2 = PSA_ALG_NONE},
      .flags = 0U},                                                                                                          // No flags
    .domain_parameters = NULL,
    .domain_parameters_size = 0U};

  psa_status_t verify_status = psa_driver_wrapper_verify_message(
    &verify_attributes,             //const psa_key_attributes_t *attributes,
    publicKeyBuffer,                //const uint8_t *key_buffer,
    sizeof(publicKeyBuffer),        //size_t key_buffer_size,
    PSA_ALG_ECDSA(PSA_ALG_SHA_384), //psa_algorithm_t alg,
    message,                        //const uint8_t *input,
    sizeof(message),                //size_t input_length,
    signature,                      //const uint8_t *signature,
    sizeof(signature)               //size_t signature_length
    );

  /* Check the return value */
  if(verify_status != PSA_SUCCESS)
  {
    return MCUXCLEXAMPLE_STATUS_ERROR;
  }

  /* Return */
  return MCUXCLEXAMPLE_STATUS_OK;
}

bool nxpClPsaDriver_ecdsa_sign_verify_message_example(void)
{
    bool result = mcuxClPsaDriver_ecdsa_sign_verify_message_example();
    return result;
}
