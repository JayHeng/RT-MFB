/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_i3c_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.i3c_edma"
#endif

/*! @brief States for the state machine used by transactional APIs. */
enum _i3c_edma_transfer_states
{
    kIdleState = 0,
    kIBIWonState,
    kSlaveStartState,
    kSendCommandState,
    kWaitRepeatedStartCompleteState,
    kTransmitDataState,
    kReceiveDataState,
    kStopState,
    kWaitForCompletionState,
    kAddressMatchState,
};

/*! @brief Common sets of flags used by the driver. */
enum _i3c_edma_flag_constants
{
    /*! All flags which are cleared by the driver upon starting a transfer. */
    kMasterClearFlags = kI3C_MasterSlaveStartFlag | kI3C_MasterControlDoneFlag | kI3C_MasterCompleteFlag |
                        kI3C_MasterArbitrationWonFlag | kI3C_MasterSlave2MasterFlag | kI3C_MasterErrorFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kMasterDMAIrqFlags = kI3C_MasterSlaveStartFlag | kI3C_MasterControlDoneFlag | kI3C_MasterCompleteFlag |
                         kI3C_MasterArbitrationWonFlag | kI3C_MasterErrorFlag | kI3C_MasterSlave2MasterFlag,

    /*! Errors to check for. */
    kMasterErrorFlags = kI3C_MasterErrorNackFlag | kI3C_MasterErrorWriteAbortFlag |
#if !defined(FSL_FEATURE_I3C_HAS_NO_MERRWARN_TERM) || (!FSL_FEATURE_I3C_HAS_NO_MERRWARN_TERM)
                        kI3C_MasterErrorTermFlag |
#endif
                        kI3C_MasterErrorParityFlag | kI3C_MasterErrorCrcFlag | kI3C_MasterErrorReadFlag |
                        kI3C_MasterErrorWriteFlag | kI3C_MasterErrorMsgFlag | kI3C_MasterErrorInvalidReqFlag |
                        kI3C_MasterErrorTimeoutFlag,
    /*! All flags which are cleared by the driver upon starting a transfer. */
    kSlaveClearFlags = kI3C_SlaveBusStartFlag | kI3C_SlaveMatchedFlag | kI3C_SlaveBusStopFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kSlaveDMAIrqFlags = kI3C_SlaveBusStartFlag | kI3C_SlaveMatchedFlag |
                        kI3C_SlaveBusStopFlag | /*kI3C_SlaveRxReadyFlag |*/
                        kI3C_SlaveDynamicAddrChangedFlag | kI3C_SlaveReceivedCCCFlag | kI3C_SlaveErrorFlag |
                        kI3C_SlaveHDRCommandMatchFlag | kI3C_SlaveCCCHandledFlag | kI3C_SlaveEventSentFlag,

    /*! Errors to check for. */
    kSlaveErrorFlags = kI3C_SlaveErrorOverrunFlag | kI3C_SlaveErrorUnderrunFlag | kI3C_SlaveErrorUnderrunNakFlag |
                       kI3C_SlaveErrorTermFlag | kI3C_SlaveErrorInvalidStartFlag | kI3C_SlaveErrorSdrParityFlag |
                       kI3C_SlaveErrorHdrParityFlag | kI3C_SlaveErrorHdrCRCFlag | kI3C_SlaveErrorS0S1Flag |
                       kI3C_SlaveErrorOverreadFlag | kI3C_SlaveErrorOverwriteFlag,
};
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void I3C_MasterRunEDMATransfer(
    I3C_Type *base, i3c_master_edma_handle_t *handle, void *data, size_t dataSize, i3c_direction_t direction);
/*******************************************************************************
 * Code
 ******************************************************************************/
static void I3C_MasterTransferEDMACallbackRx(edma_handle_t *dmaHandle, void *param, bool transferDone, uint32_t tcds)
{
    i3c_master_edma_handle_t *i3cHandle = (i3c_master_edma_handle_t *)param;

    if (transferDone)
    {
        /* Read last data byte */
        i3cHandle->base->MCTRL |= I3C_MCTRL_RDTERM(1U);
        size_t rxCount = 0U;
        while (rxCount == 0U)
        {
            I3C_MasterGetFifoCounts(i3cHandle->base, &rxCount, NULL);
        };
        *(uint8_t *)((uint32_t)((uint32_t *)i3cHandle->transfer.data) + i3cHandle->transfer.dataSize - 1U) =
            (uint8_t)i3cHandle->base->MRDATAB;

        /* Disable I3C Rx DMA */
        i3cHandle->base->MDATACTRL &= ~I3C_MDMACTRL_DMAFB_MASK;

        i3cHandle->state = (uint8_t)kStopState;
        I3C_MasterTransferEDMAHandleIRQ(i3cHandle->base, i3cHandle);
    }
}

static void I3C_MasterTransferEDMACallbackTx(edma_handle_t *dmaHandle, void *param, bool transferDone, uint32_t tcds)
{
    i3c_master_edma_handle_t *i3cHandle = (i3c_master_edma_handle_t *)param;

    if (transferDone)
    {
        /* Disable I3C Tx DMA */
        i3cHandle->base->MDATACTRL &= ~I3C_MDMACTRL_DMATB_MASK;
        i3cHandle->state = (uint8_t)kStopState;

        size_t txCount = 0U;
        do
        {
            I3C_MasterGetFifoCounts(i3cHandle->base, NULL, &txCount);
        } while (txCount != 0U);
        I3C_MasterTransferEDMAHandleIRQ(i3cHandle->base, i3cHandle);
    }
}
/*!
 * brief Prepares the transfer state machine and fills in the command buffer.
 * param handle Master nonblocking driver handle.
 */
static status_t I3C_MasterInitTransferStateMachineEDMA(I3C_Type *base, i3c_master_edma_handle_t *handle)
{
    i3c_master_transfer_t *xfer = &handle->transfer;
    status_t result             = kStatus_Success;
    i3c_direction_t direction   = xfer->direction;

    /* Calculate command count and put into command buffer. */
    handle->subaddressCount = 0U;
    if (xfer->subaddressSize != 0U)
    {
        for (uint32_t i = xfer->subaddressSize; i > 0U; i--)
        {
            handle->subaddressBuffer[handle->subaddressCount++] = (uint8_t)((xfer->subaddress) >> (8U * (i - 1U)));
        }
    }

    /* Start condition shall be ommited, switch directly to next phase */
    if (xfer->dataSize == 0U)
    {
        handle->state = (uint8_t)kStopState;
    }

    /* Handle no start option. */
    if (0U != (xfer->flags & (uint32_t)kI3C_TransferNoStartFlag))
    {
        /* No need to send start flag, directly go to send command or data */
        if (xfer->subaddressSize > 0UL)
        {
            handle->state = (uint8_t)kSendCommandState;
        }
        else
        {
            if (direction == kI3C_Write)
            {
                /* Next state, send data. */
                handle->state = (uint8_t)kTransmitDataState;
            }
            else
            {
                /* Only support write with no stop signal. */
                return kStatus_InvalidArgument;
            }
        }
    }
    else
    {
        if (xfer->subaddressSize != 0U)
        {
            handle->state = (uint8_t)kSendCommandState;
        }
        else
        {
            if (handle->transfer.direction == kI3C_Write)
            {
                handle->state = (uint8_t)kTransmitDataState;
            }
            else if (handle->transfer.direction == kI3C_Read)
            {
                handle->state = (uint8_t)kReceiveDataState;
            }
            else
            {
                return kStatus_InvalidArgument;
            }
        }

        if (handle->transfer.direction == kI3C_Read)
        {
            I3C_MasterRunEDMATransfer(base, handle, xfer->data, xfer->dataSize - 1U, kI3C_Read);
        }

        if (handle->state != (uint8_t)kStopState)
        {
            /* If repeated start is requested, send repeated start. */
            if (0U != (xfer->flags & (uint32_t)kI3C_TransferRepeatedStartFlag))
            {
                result = I3C_MasterRepeatedStart(base, xfer->busType, xfer->slaveAddress, direction);
            }
            else /* For normal transfer, send start. */
            {
                result = I3C_MasterStart(base, xfer->busType, xfer->slaveAddress, direction);
            }
        }
    }

    I3C_MasterTransferEDMAHandleIRQ(base, handle);
    return result;
}

static void I3C_MasterRunEDMATransfer(
    I3C_Type *base, i3c_master_edma_handle_t *handle, void *data, size_t dataSize, i3c_direction_t direction)
{
    edma_transfer_config_t xferConfig;
    uint32_t address;
    bool isEnableTxDMA = false;
    bool isEnableRxDMA = false;
    uint32_t width;

    handle->transferCount = dataSize;

    switch (direction)
    {
        case kI3C_Write:
            address = (uint32_t)&base->MWDATAB1;
            EDMA_PrepareTransfer(&xferConfig, data, sizeof(uint8_t), (uint32_t *)address, sizeof(uint8_t), 1, dataSize,
                                 kEDMA_MemoryToPeripheral);
            (void)EDMA_SubmitTransfer(handle->txDmaHandle, &xferConfig);
            EDMA_StartTransfer(handle->txDmaHandle);
            isEnableTxDMA = true;
            width         = 2U;
            break;

        case kI3C_Read:
            address = (uint32_t)&base->MRDATAB;
            EDMA_PrepareTransfer(&xferConfig, (uint32_t *)address, sizeof(uint8_t), data, sizeof(uint8_t), 1, dataSize,
                                 kEDMA_PeripheralToMemory);
            (void)EDMA_SubmitTransfer(handle->rxDmaHandle, &xferConfig);
            EDMA_StartTransfer(handle->rxDmaHandle);
            isEnableRxDMA = true;
            width         = 1U;
            break;

        default:
            /* This should never happen */
            assert(0);
            break;
    }

    I3C_MasterEnableDMA(base, isEnableTxDMA, isEnableRxDMA, width);
}

static status_t I3C_MasterRunTransferStateMachineEDMA(I3C_Type *base, i3c_master_edma_handle_t *handle, bool *isDone)
{
    uint32_t status;
    uint32_t errStatus;
    status_t result = kStatus_Success;
    i3c_master_transfer_t *xfer;
    size_t rxCount      = 0;
    bool state_complete = false;

    /* Set default isDone return value. */
    *isDone = false;

    /* Check for errors. */
    status = (uint32_t)I3C_MasterGetPendingInterrupts(base);
    I3C_MasterClearStatusFlags(base, status);

    i3c_master_state_t masterState = I3C_MasterGetState(base);
    errStatus                      = I3C_MasterGetErrorStatusFlags(base);
    result                         = I3C_MasterCheckAndClearError(base, errStatus);
    if (kStatus_Success != result)
    {
        return result;
    }

    if (0UL != (status & (uint32_t)kI3C_MasterSlave2MasterFlag))
    {
        if (handle->callback.slave2Master != NULL)
        {
            handle->callback.slave2Master(base, handle->userData);
        }
    }

    if ((0UL != (status & (uint32_t)kI3C_MasterSlaveStartFlag)) && (handle->transfer.busType != kI3C_TypeI2C))
    {
        handle->state = (uint8_t)kSlaveStartState;
    }

    if ((masterState == kI3C_MasterStateIbiRcv) || (masterState == kI3C_MasterStateIbiAck))
    {
        handle->state = (uint8_t)kIBIWonState;
    }

    if (handle->state == (uint8_t)kIdleState)
    {
        return result;
    }

    if (handle->state == (uint8_t)kIBIWonState)
    {
        /* Get fifo counts and compute room in tx fifo. */
        rxCount = (base->MDATACTRL & I3C_MDATACTRL_RXCOUNT_MASK) >> I3C_MDATACTRL_RXCOUNT_SHIFT;
    }

    /* Get pointer to private data. */
    xfer = &handle->transfer;

    while (!state_complete)
    {
        /* Execute the state. */
        switch (handle->state)
        {
            case (uint8_t)kSlaveStartState:
                /* Emit start + 0x7E */
                I3C_MasterEmitRequest(base, kI3C_RequestAutoIbi);
                handle->state  = (uint8_t)kIBIWonState;
                state_complete = true;
                break;

            case (uint8_t)kIBIWonState:
                if (masterState == kI3C_MasterStateIbiAck)
                {
                    handle->ibiType = I3C_GetIBIType(base);
                    if (handle->callback.ibiCallback != NULL)
                    {
                        handle->callback.ibiCallback(base, handle, handle->ibiType, kI3C_IbiAckNackPending);
                    }
                    else
                    {
                        I3C_MasterEmitIBIResponse(base, kI3C_IbiRespNack);
                    }
                }

                /* Make sure there is data in the rx fifo. */
                if (0UL != rxCount)
                {
                    if ((handle->ibiBuff == NULL) && (handle->callback.ibiCallback != NULL))
                    {
                        handle->callback.ibiCallback(base, handle, kI3C_IbiNormal, kI3C_IbiDataBuffNeed);
                    }
                    uint8_t tempData = (uint8_t)base->MRDATAB;
                    if (handle->ibiBuff != NULL)
                    {
                        handle->ibiBuff[handle->ibiPayloadSize++] = tempData;
                    }
                    rxCount--;
                    break;
                }
                else if (0UL != (status & (uint32_t)kI3C_MasterCompleteFlag))
                {
                    handle->ibiType    = I3C_GetIBIType(base);
                    handle->ibiAddress = I3C_GetIBIAddress(base);
                    state_complete     = true;
                    result             = kStatus_I3C_IBIWon;
                }
                else
                {
                    state_complete = true;
                }
                break;

            case (uint8_t)kSendCommandState:
                /* Calculate command count and put into command buffer. */
                if (xfer->dataSize == 0U)
                {
                    *isDone = true;
                }

                I3C_MasterRunEDMATransfer(base, handle, handle->subaddressBuffer, handle->subaddressCount, kI3C_Write);

                if ((xfer->direction == kI3C_Read) || (0UL == xfer->dataSize))
                {
                    if (0UL == xfer->dataSize)
                    {
                        handle->state = (uint8_t)kWaitForCompletionState;
                    }
                    else
                    {
                        /* xfer->dataSize != 0U, xfer->direction = kI3C_Read */
                        handle->state = (uint8_t)kWaitRepeatedStartCompleteState;
                    }
                }
                else
                {
                    /* Next state, transfer data. */
                    handle->state = (uint8_t)kTransmitDataState;
                }

                state_complete = true;
                break;

            case (uint8_t)kWaitRepeatedStartCompleteState:
                /* We stay in this state until the maste complete. */
                if (0UL != (status & (uint32_t)kI3C_MasterCompleteFlag))
                {
                    handle->state = (uint8_t)kReceiveDataState;
                    /* Send repeated start and slave address. */
                    result = I3C_MasterRepeatedStart(base, xfer->busType, xfer->slaveAddress, kI3C_Read);
                }

                state_complete = true;
                break;

            case (uint8_t)kTransmitDataState:
                I3C_MasterRunEDMATransfer(base, handle, xfer->data, xfer->dataSize, kI3C_Write);
                handle->state = (uint8_t)kWaitForCompletionState;

                state_complete = true;
                break;

            case (uint8_t)kReceiveDataState:
                /* Do DMA read. */
                handle->state = (uint8_t)kWaitForCompletionState;

                state_complete = true;
                break;

            case (uint8_t)kWaitForCompletionState:
                /* We stay in this state until the maste complete. */
                if (0UL != (status & (uint32_t)kI3C_MasterCompleteFlag))
                {
                    handle->state = (uint8_t)kStopState;
                }
                else
                {
                    state_complete = true;
                }
                break;

            case (uint8_t)kStopState:
                /* Only issue a stop transition if the caller requested it. */
                if (0UL == (xfer->flags & (uint32_t)kI3C_TransferNoStopFlag))
                {
                    if (xfer->busType == kI3C_TypeI3CDdr)
                    {
                        I3C_MasterEmitRequest(base, kI3C_RequestForceExit);
                    }
                    else
                    {
                        I3C_MasterEmitRequest(base, kI3C_RequestEmitStop);
                    }
                }
                *isDone        = true;
                state_complete = true;
                break;

            default:
                assert(false);
                break;
        }
    }
    return result;
}

void I3C_MasterTransferCreateHandleEDMA(I3C_Type *base,
                                        i3c_master_edma_handle_t *handle,
                                        const i3c_master_edma_callback_t *callback,
                                        void *userData,
                                        edma_handle_t *rxDmaHandle,
                                        edma_handle_t *txDmaHandle)
{
    uint32_t instance;

    assert(NULL != handle);

    /* Clear out the handle. */
    (void)memset(handle, 0, sizeof(*handle));

    /* Look up instance number */
    instance = I3C_GetInstance(base);

    handle->base        = base;
    handle->txDmaHandle = txDmaHandle;
    handle->rxDmaHandle = rxDmaHandle;
    handle->callback    = *callback;
    handle->userData    = userData;

    /* Save this handle for IRQ use. */
    s_i3cMasterHandle[instance] = handle;

    /* Set irq handler. */
    s_i3cMasterIsr = I3C_MasterTransferEDMAHandleIRQ;

    EDMA_SetCallback(handle->rxDmaHandle, I3C_MasterTransferEDMACallbackRx, handle);
    EDMA_SetCallback(handle->txDmaHandle, I3C_MasterTransferEDMACallbackTx, handle);

    /* Clear all flags. */
    I3C_MasterClearErrorStatusFlags(base, (uint32_t)kMasterErrorFlags);
    I3C_MasterClearStatusFlags(base, (uint32_t)kMasterClearFlags);
    /* Reset fifos. These flags clear automatically. */
    base->MDATACTRL |= I3C_MDATACTRL_FLUSHTB_MASK | I3C_MDATACTRL_FLUSHFB_MASK;

    /* Enable NVIC IRQ, this only enables the IRQ directly connected to the NVIC.
     In some cases the I3C IRQ is configured through INTMUX, user needs to enable
     INTMUX IRQ in application code. */
    (void)EnableIRQ(kI3cIrqs[instance]);

    /* Clear internal IRQ enables and enable NVIC IRQ. */
    I3C_MasterEnableInterrupts(base, (uint32_t)kMasterDMAIrqFlags);
}

/*!
 * brief Performs a non-blocking DMA transaction on the I2C/I3C bus.
 *
 * param base The I3C peripheral base address.
 * param handle Pointer to the I3C master driver handle.
 * param transfer The pointer to the transfer descriptor.
 * retval #kStatus_Success The transaction was started successfully.
 * retval #kStatus_I3C_Busy Either another master is currently utilizing the bus, or a non-blocking
 *      transaction is already in progress.
 */
status_t I3C_MasterTransferEDMA(I3C_Type *base, i3c_master_edma_handle_t *handle, i3c_master_transfer_t *transfer)
{
    assert(NULL != handle);
    assert(NULL != transfer);
    assert(transfer->subaddressSize <= sizeof(transfer->subaddress));
    i3c_master_state_t masterState = I3C_MasterGetState(base);
    bool checkDdrState             = false;

    /* Return busy if another transaction is in progress. */
    if (handle->state != (uint8_t)kIdleState)
    {
        return kStatus_I3C_Busy;
    }

    /* Return an error if the bus is already in use not by us. */
    checkDdrState = (transfer->busType == kI3C_TypeI3CDdr) ? (masterState != kI3C_MasterStateDdr) : true;
    if ((masterState != kI3C_MasterStateIdle) && (masterState != kI3C_MasterStateNormAct) && checkDdrState)
    {
        return kStatus_I3C_Busy;
    }

    /* Disable I3C IRQ sources while we configure stuff. */
    I3C_MasterDisableInterrupts(
        base, ((uint32_t)kMasterDMAIrqFlags | (uint32_t)kI3C_MasterRxReadyFlag | (uint32_t)kI3C_MasterTxReadyFlag));

    /* Save transfer into handle. */
    handle->transfer = *transfer;

    /* Configure IBI response type. */
    base->MCTRL &= ~I3C_MCTRL_IBIRESP_MASK;
    base->MCTRL |= I3C_MCTRL_IBIRESP(transfer->ibiResponse);

    /* Clear all flags. */
    I3C_MasterClearErrorStatusFlags(base, (uint32_t)kMasterErrorFlags);
    I3C_MasterClearStatusFlags(base, (uint32_t)kMasterClearFlags);
    /* Reset fifos. These flags clear automatically. */
    base->MDATACTRL |= I3C_MDATACTRL_FLUSHTB_MASK | I3C_MDATACTRL_FLUSHFB_MASK;

    /* Generate commands to send. */
    (void)I3C_MasterInitTransferStateMachineEDMA(base, handle);

    /* Enable I3C internal IRQ sources. NVIC IRQ was enabled in CreateHandle() */
    I3C_MasterEnableInterrupts(base, (uint32_t)(kMasterDMAIrqFlags));

    if (transfer->busType == kI3C_TypeI2C)
    {
        I3C_MasterDisableInterrupts(base, (uint32_t)kI3C_MasterSlaveStartFlag);
    }

    return kStatus_Success;
}

void I3C_MasterTransferEDMAHandleIRQ(I3C_Type *base, void *i3cHandle)
{
    i3c_master_edma_handle_t *handle = (i3c_master_edma_handle_t *)i3cHandle;

    bool isDone;
    status_t result;

    /* Don't do anything if we don't have a valid handle. */
    if (NULL == handle)
    {
        return;
    }

    result = I3C_MasterRunTransferStateMachineEDMA(base, handle, &isDone);

    if (handle->state == (uint8_t)kIdleState)
    {
        return;
    }

    if (isDone || (result != kStatus_Success))
    {
        /* XXX need to handle data that may be in rx fifo below watermark level? */

        /* XXX handle error, terminate xfer */
        if ((result == kStatus_I3C_Nak) || (result == kStatus_I3C_IBIWon))
        {
            I3C_MasterEmitRequest(base, kI3C_RequestEmitStop);
        }

        /* Set handle to idle state. */
        handle->state = (uint8_t)kIdleState;

        /* Invoke IBI user callback. */
        if ((result == kStatus_I3C_IBIWon) && (handle->callback.ibiCallback != NULL))
        {
            handle->callback.ibiCallback(base, handle, handle->ibiType, kI3C_IbiReady);
            handle->ibiPayloadSize = 0;
        }

        /* Invoke callback. */
        if (NULL != handle->callback.transferComplete)
        {
            handle->callback.transferComplete(base, handle, result, handle->userData);
        }
    }
}

/*!
 * brief Get master transfer status during a dma non-blocking transfer
 *
 * param base I3C peripheral base address
 * param handle pointer to i2c_master_edma_handle_t structure
 * param count Number of bytes transferred so far by the non-blocking transaction.
 */
status_t I3C_MasterTransferGetCountEDMA(I3C_Type *base, i3c_master_edma_handle_t *handle, size_t *count)
{
    assert(handle != NULL);

    if (NULL == count)
    {
        return kStatus_InvalidArgument;
    }

    /* Catch when there is not an active transfer. */
    if (handle->state == (uint8_t)kIdleState)
    {
        *count = 0;
        return kStatus_NoTransferInProgress;
    }

    /* There is no necessity to disable interrupts as we read a single integer value */
    i3c_direction_t dir = handle->transfer.direction;

    if (dir == kI3C_Read)
    {
        *count = handle->transferCount -
                 1U * EDMA_GetRemainingMajorLoopCount(handle->rxDmaHandle->base, handle->rxDmaHandle->channel);
    }
    else
    {
        *count = handle->transferCount -
                 1U * EDMA_GetRemainingMajorLoopCount(handle->txDmaHandle->base, handle->txDmaHandle->channel);
    }

    return kStatus_Success;
}

/*!
 * brief Abort a master dma non-blocking transfer in a early time
 *
 * param base I3C peripheral base address
 * param handle pointer to i2c_master_edma_handle_t structure
 */
void I3C_MasterTransferAbortEDMA(I3C_Type *base, i3c_master_edma_handle_t *handle)
{
    if (handle->state != (uint8_t)kIdleState)
    {
        EDMA_AbortTransfer(handle->txDmaHandle);
        EDMA_AbortTransfer(handle->rxDmaHandle);

        I3C_MasterEnableDMA(base, false, false, 0);

        /* Reset fifos. These flags clear automatically. */
        base->MDATACTRL |= I3C_MDATACTRL_FLUSHTB_MASK | I3C_MDATACTRL_FLUSHFB_MASK;

        /* Send a stop command to finalize the transfer. */
        (void)I3C_MasterStop(base);

        /* Reset handle. */
        handle->state = (uint8_t)kIdleState;
    }
}

static void I3C_SlaveTransferEDMACallback(edma_handle_t *dmaHandle, void *param, bool transferDone, uint32_t tcds)
{
    i3c_slave_edma_handle_t *i3cHandle = (i3c_slave_edma_handle_t *)param;

    if (transferDone)
    {
        /* Simply disable dma enablement */
        if (i3cHandle->txDmaHandle == dmaHandle)
        {
            i3cHandle->base->SDMACTRL &= ~I3C_SDMACTRL_DMATB_MASK;
        }
        else
        {
            i3cHandle->base->SDMACTRL &= ~I3C_SDMACTRL_DMAFB_MASK;
        }
    }
}

/*!
 * brief Create a new handle for the I3C slave DMA APIs.
 *
 * The creation of a handle is for use with the DMA APIs. Once a handle
 * is created, there is not a corresponding destroy handle. If the user wants to
 * terminate a transfer, the I3C_SlaveTransferAbortDMA() API shall be called.
 *
 * For devices where the I3C send and receive DMA requests are OR'd together, the @a txDmaHandle
 * parameter is ignored and may be set to NULL.
 *
 * param base The I3C peripheral base address.
 * param handle Pointer to the I3C slave driver handle.
 * param callback User provided pointer to the asynchronous callback function.
 * param userData User provided pointer to the application callback data.
 * param rxDmaHandle Handle for the DMA receive channel. Created by the user prior to calling this function.
 * param txDmaHandle Handle for the DMA transmit channel. Created by the user prior to calling this function.
 */
void I3C_SlaveTransferCreateHandleEDMA(I3C_Type *base,
                                       i3c_slave_edma_handle_t *handle,
                                       i3c_slave_edma_callback_t callback,
                                       void *userData,
                                       edma_handle_t *rxDmaHandle,
                                       edma_handle_t *txDmaHandle)
{
    uint32_t instance;

    assert(NULL != handle);

    /* Clear out the handle. */
    (void)memset(handle, 0, sizeof(*handle));

    /* Look up instance number */
    instance = I3C_GetInstance(base);

    handle->base        = base;
    handle->txDmaHandle = txDmaHandle;
    handle->rxDmaHandle = rxDmaHandle;
    handle->callback    = callback;
    handle->userData    = userData;

    /* Save this handle for IRQ use. */
    s_i3cSlaveHandle[instance] = handle;

    /* Set irq handler. */
    s_i3cSlaveIsr = I3C_SlaveTransferEDMAHandleIRQ;

    EDMA_SetCallback(handle->rxDmaHandle, I3C_SlaveTransferEDMACallback, handle);
    EDMA_SetCallback(handle->txDmaHandle, I3C_SlaveTransferEDMACallback, handle);

    /* Clear internal IRQ enables and enable NVIC IRQ. */
    I3C_SlaveDisableInterrupts(base, (uint32_t)kSlaveDMAIrqFlags);

    /* Enable NVIC IRQ, this only enables the IRQ directly connected to the NVIC.
     In some cases the I3C IRQ is configured through INTMUX, user needs to enable
     INTMUX IRQ in application code. */
    (void)EnableIRQ(kI3cIrqs[instance]);

    /* Enable IRQ. */
    I3C_SlaveEnableInterrupts(base, (uint32_t)kSlaveDMAIrqFlags);
}

static void I3C_SlavePrepareTxEDMA(I3C_Type *base, i3c_slave_edma_handle_t *handle)
{
    edma_transfer_config_t txConfig;
    uint32_t *txFifoBase            = (uint32_t *)&base->SWDATAB;
    i3c_slave_edma_transfer_t *xfer = &handle->transfer;

    EDMA_PrepareTransfer(&txConfig, xfer->txData, 1, (void *)txFifoBase, 1, 1, xfer->txDataSize,
                         kEDMA_MemoryToPeripheral);
    (void)EDMA_SubmitTransfer(handle->txDmaHandle, &txConfig);
    EDMA_StartTransfer(handle->txDmaHandle);
}

static void I3C_SlavePrepareRxEDMA(I3C_Type *base, i3c_slave_edma_handle_t *handle)
{
    edma_transfer_config_t rxConfig;
    uint32_t *rxFifoBase            = (uint32_t *)&base->SRDATAB;
    i3c_slave_edma_transfer_t *xfer = &handle->transfer;

    EDMA_PrepareTransfer(&rxConfig, (void *)rxFifoBase, 1, xfer->rxData, 1, 1, xfer->rxDataSize,
                         kEDMA_PeripheralToMemory);
    (void)EDMA_SubmitTransfer(handle->rxDmaHandle, &rxConfig);
    EDMA_StartTransfer(handle->rxDmaHandle);
}

/*!
 * brief Prepares for a non-blocking DMA-based transaction on the I3C bus.
 *
 * The API will do DMA configuration according to the input transfer descriptor, and the data will be transferred when
 * there's bus master requesting transfer from/to this slave. So the timing of call to this API need be aligned
 * with master application to ensure the transfer is executed as expected.
 * Callback specified when the @a handle was created is invoked when the transaction has completed.
 *
 * param base The I3C peripheral base address.
 * param handle Pointer to the I3C slave driver handle.
 * param transfer The pointer to the transfer descriptor.
 * param eventMask Bit mask formed by OR'ing together #i3c_slave_transfer_event_t enumerators to specify
 *      which events to send to the callback. The transmit and receive events is not allowed to be enabled.
 * retval kStatus_Success The transaction was started successfully.
 * retval #kStatus_I3C_Busy Either another master is currently utilizing the bus, or another DMA
 *      transaction is already in progress.
 * retval #kStatus_Fail The transaction can't be set.
 */
status_t I3C_SlaveTransferEDMA(I3C_Type *base,
                               i3c_slave_edma_handle_t *handle,
                               i3c_slave_edma_transfer_t *transfer,
                               uint32_t eventMask)
{
    assert(NULL != handle);
    assert(NULL != transfer);

    bool txDmaEn = false, rxDmaEn = false;
    uint32_t width;

    if (handle->isBusy)
    {
        return kStatus_I3C_Busy;
    }
    /* Clear all flags. */
    I3C_SlaveClearErrorStatusFlags(base, (uint32_t)kSlaveErrorFlags);
    I3C_SlaveClearStatusFlags(base, (uint32_t)kSlaveClearFlags);
    /* Reset fifos. These flags clear automatically. */
    base->SDATACTRL |= I3C_SDATACTRL_FLUSHTB_MASK | I3C_SDATACTRL_FLUSHFB_MASK;

    handle->transfer = *transfer;

    /* Set up event mask. */
    handle->eventMask = eventMask;

    if ((transfer->txData != NULL) && (transfer->txDataSize != 0U))
    {
        I3C_SlavePrepareTxEDMA(base, handle);
        txDmaEn = true;
        width   = 2U;
    }

    if ((transfer->rxData != NULL) && (transfer->rxDataSize != 0U))
    {
        I3C_SlavePrepareRxEDMA(base, handle);
        rxDmaEn = true;
        width   = 1U;
    }

    if (txDmaEn || rxDmaEn)
    {
        I3C_SlaveEnableDMA(base, txDmaEn, rxDmaEn, width);
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

void I3C_SlaveTransferEDMAHandleIRQ(I3C_Type *base, void *i3cHandle)
{
    uint32_t flags;
    uint32_t errFlags;
    i3c_slave_edma_transfer_t *xfer;

    i3c_slave_edma_handle_t *handle = (i3c_slave_edma_handle_t *)i3cHandle;
    /* Check for a valid handle in case of a spurious interrupt. */
    if (NULL == handle)
    {
        return;
    }

    xfer = &handle->transfer;

    /* Get status flags. */
    flags    = I3C_SlaveGetStatusFlags(base);
    errFlags = I3C_SlaveGetErrorStatusFlags(base);

    /* Clear status flags. */
    I3C_SlaveClearStatusFlags(base, flags);

    if (0UL != (errFlags & (uint32_t)kSlaveErrorFlags))
    {
        xfer->event            = (uint32_t)kI3C_SlaveCompletionEvent;
        xfer->completionStatus = I3C_SlaveCheckAndClearError(base, errFlags);

        if ((0UL != (handle->eventMask & (uint32_t)kI3C_SlaveCompletionEvent)) && (NULL != handle->callback))
        {
            handle->callback(base, xfer, handle->userData);
        }
        return;
    }

    if (0UL != (flags & (uint32_t)kI3C_SlaveEventSentFlag))
    {
        xfer->event = (uint32_t)kI3C_SlaveRequestSentEvent;
        if (handle->ibiData != NULL)
        {
            size_t count = 0U;
            while (count < handle->ibiDataSize)
            {
                base->SCTRL = (base->SCTRL & ~I3C_SCTRL_IBIDATA_MASK) | I3C_SCTRL_IBIDATA(handle->ibiData[count]) |
                              I3C_SCTRL_EVENT(1U);
                count++;
            }
        }

        /* Reset IBI data buffer. */
        handle->ibiData = NULL;

        if ((0UL != (handle->eventMask & xfer->event)) && (NULL != handle->callback))
        {
            handle->callback(base, xfer, handle->userData);
        }
    }

    if (0UL != (flags & (uint32_t)kI3C_SlaveReceivedCCCFlag))
    {
        handle->isBusy = true;
        xfer->event    = (uint32_t)kI3C_SlaveReceivedCCCEvent;
        if ((0UL != (handle->eventMask & xfer->event)) && (NULL != handle->callback))
        {
            handle->callback(base, xfer, handle->userData);
        }
    }

    if (0UL != (flags & (uint32_t)kI3C_SlaveBusStopFlag))
    {
        if (handle->isBusy == true)
        {
            xfer->event            = (uint32_t)kI3C_SlaveCompletionEvent;
            xfer->completionStatus = kStatus_Success;
            handle->isBusy         = false;

            if ((0UL != (handle->eventMask & xfer->event)) && (NULL != handle->callback))
            {
                handle->callback(base, xfer, handle->userData);
            }
            I3C_SlaveTransferAbortEDMA(base, handle);
        }
        else
        {
            return;
        }
    }

    if (0UL != (flags & (uint32_t)kI3C_SlaveMatchedFlag))
    {
        xfer->event    = (uint32_t)kI3C_SlaveAddressMatchEvent;
        handle->isBusy = true;
        if ((0UL != (handle->eventMask & (uint32_t)kI3C_SlaveAddressMatchEvent)) && (NULL != handle->callback))
        {
            handle->callback(base, xfer, handle->userData);
        }
    }
}

/*!
 * brief Abort a slave dma non-blocking transfer in a early time
 *
 * param base I3C peripheral base address
 * param handle pointer to i3c_slave_edma_handle_t structure
 */
void I3C_SlaveTransferAbortEDMA(I3C_Type *base, i3c_slave_edma_handle_t *handle)
{
    if (handle->isBusy != false)
    {
        EDMA_AbortTransfer(handle->txDmaHandle);
        EDMA_AbortTransfer(handle->rxDmaHandle);

        I3C_SlaveEnableDMA(base, false, false, 0);

        /* Reset fifos. These flags clear automatically. */
        base->SDATACTRL |= I3C_SDATACTRL_FLUSHTB_MASK | I3C_SDATACTRL_FLUSHFB_MASK;
    }
}
