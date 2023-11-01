/*--------------------------------------------------------------------------*/
/* Copyright 2021-2023 NXP                                                  */
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

#include <mcuxClCore_Examples.h> // Defines and assertions for examples
#include <mcuxClExample_ELS_Helper.h>
#include <mcuxClExample_Session_Helper.h>
#include <mcuxClExample_Key_Helper.h>
#include <mcuxClEls.h> // Interface to the entire mcuxClEls component
#include <mcuxClSession.h> // Interface to the entire mcuxClSession component
#include <mcuxClKey.h> // Interface to the entire mcuxClKey component
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h> // Code flow protection
#include <mcuxClToolchain.h> // memory segment definitions
#include <mcuxClAes.h> // Interface to AES-related definitions and types
#include <mcuxClCipher.h> // Interface to the entire mcuxClCipher component
#include <mcuxClCipherModes.h> // Interface to the entire mcuxClCipherModes component
#include <mcuxClExample_RNG_Helper.h>

/** Key for the AES encryption. */
static uint8_t aes128_key[MCUXCLAES_BLOCK_SIZE] = {
    0x2Bu, 0x7Eu, 0x15u, 0x16u,
    0x28u, 0xAEu, 0xD2u, 0xA6u,
    0xABu, 0xF7u, 0x15u, 0x88u,
    0x09u, 0xCFu, 0x4Fu, 0x3Cu
};

/** IV of the AES encryption. */
static uint8_t aes128_iv[MCUXCLAES_BLOCK_SIZE] = {
    0xF8u, 0xD2u, 0x68u, 0x76u,
    0x81u, 0x6Fu, 0x0Fu, 0xBAu,
    0x86u, 0x2Bu, 0xD8u, 0xA3u,
    0x2Du, 0x04u, 0x67u, 0xC3u
};

/** Plaintext input for the AES encryption. */
static uint8_t const msg_plain[MCUXCLAES_BLOCK_SIZE] = {
    0x6Bu, 0xC1u, 0xBEu, 0xE2u,
    0x2Eu, 0x40u, 0x9Fu, 0x96u,
    0xE9u, 0x3Du, 0x7Eu, 0x11u,
    0x73u, 0x93u, 0x17u, 0x2Au
};

/** Expected ciphertext output of the AES encryption. */
static uint8_t const msg_enc_expected[MCUXCLAES_BLOCK_SIZE] = {
    0xCAu, 0xEAu, 0x07u, 0x26u,
    0x62u, 0xE2u, 0x20u, 0x06u,
    0x2Du, 0x45u, 0x46u, 0x41u,
    0x5Eu, 0xFFu, 0xFAu, 0xD2u
};

MCUXCLEXAMPLE_FUNCTION(mcuxClCipherModes_CBC_Multipart_ELS_example)
{
    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/

    /** Initialize ELS, MCUXCLELS_RESET_DO_NOT_CANCEL **/
    if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }


    /* Initialize session */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    /* Allocate and initialize session */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLCIPHER_MAX_AES_CPU_WA_BUFFER_SIZE + MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE, 0u);

    /* Initialize the PRNG */
    MCUXCLEXAMPLE_INITIALIZE_PRNG(session);

    /* Initialize key */
    uint32_t keyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t) &keyDesc;

    /* Set key properties. */
    mcuxClEls_KeyProp_t key_properties;

    key_properties.word.value = 0u;
    key_properties.bits.ksize = MCUXCLELS_KEYPROPERTY_KEY_SIZE_128;
    key_properties.bits.kactv = MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE;

    /* Load key. */
    uint32_t dstData[8];
    //Initializes a key handle, Set key properties and Load key.
    if(!mcuxClExample_Key_Init_And_Load(session,
                                       key,
                                       mcuxClKey_Type_Aes128,
                                       (mcuxCl_Buffer_t) aes128_key,
                                       sizeof(aes128_key),
                                       &key_properties,
                                       dstData, MCUXCLEXAMPLE_CONST_EXTERNAL_KEY))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Create a buffer for the context */
    uint8_t ctxBuf[MCUXCLCIPHER_AES_CONTEXT_SIZE];
    mcuxClCipher_Context_t * const ctx = (mcuxClCipher_Context_t *) ctxBuf;

    /* Declare message buffer and size. */
    uint8_t msg_enc[MCUXCLAES_BLOCK_SIZE];
    uint32_t msg_enc_size = 0u;

    /**************************************************************************/
    /* Init                                                                   */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_init, token_init, mcuxClCipher_init(
    /* mcuxClSession_Handle_t session:          */ session,
    /* mcuxClCipher_Context_t * const pContext: */ ctx,
    /* mcuxClKey_Handle_t key:                  */ key,
    /* mcuxClCipher_Mode_t mode:                */ mcuxClCipher_Mode_AES_CBC_Enc_NoPadding,
    /* mcuxCl_InputBuffer_t pIv:                */ aes128_iv,
    /* uint32_t ivLength:                      */ sizeof(aes128_iv)
    ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_init) != token_init) || (MCUXCLCIPHER_STATUS_OK != result_init))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();


    /**************************************************************************/
    /* Process                                                                */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_proc, token_proc, mcuxClCipher_process(
    /* mcuxClSession_Handle_t session:         */ session,
    /* mcuxClCipher_Context_t * const pContext:*/ ctx,
    /* mcuxCl_InputBuffer_t pIn:               */ msg_plain,
    /* uint32_t inLength:                     */ sizeof(msg_plain),
    /* mcuxCl_Buffer_t pOut:                   */ msg_enc,
    /* uint32_t * const pOutLength:           */ &msg_enc_size
    ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_process) != token_proc) || (MCUXCLCIPHER_STATUS_OK != result_proc))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();


    /**************************************************************************/
    /* Finish                                                                 */
    /**************************************************************************/

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result_fin, token_fin, mcuxClCipher_finish(
    /* mcuxClSession_Handle_t session:         */ session,
    /* mcuxClCipher_Context_t * const pContext:*/ ctx,
    /* mcuxCl_Buffer_t pOut:                   */ msg_enc,
    /* uint32_t * const pOutLength:           */ &msg_enc_size
    ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCipher_finish) != token_fin) || (MCUXCLCIPHER_STATUS_OK != result_fin))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    if(msg_enc_size != sizeof(msg_enc_expected))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Check the result of the encryption, compare it against the reference */
    if(!mcuxClCore_assertEqual(msg_enc, msg_enc_expected, msg_enc_size))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }


    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/

    /* Flush the key. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClKey_flush(session, key));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != token) || (MCUXCLKEY_STATUS_OK != result))
    {
        return MCUXCLEXAMPLE_STATUS_FAILURE;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /** Destroy Session and cleanup Session **/
    if(!mcuxClExample_Session_Clean(session))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /** Disable the ELS **/
    if(!mcuxClExample_Els_Disable())
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}
