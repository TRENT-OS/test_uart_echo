/*
 * UART test
 *
 * Copyright (C) 2023-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "OS_Error.h"
#include "OS_Dataport.h"
#include "lib_io/FifoDataport.h"
#include "lib_debug/Debug.h"

#include <camkes.h>

#include <string.h>

//---------------------------------------------------------------------------

typedef struct {
    FifoDataport*   uart_input_fifo; // FIFO in dataport shared with the UART driver
    uint8_t*        uart_output_fifo;
} uart_ctx_t;

uart_ctx_t uart_ctx = { 0 };


//---------------------------------------------------------------------------

void uart_write(uart_ctx_t *ctx, char* bytes, size_t amount) {
    // Copy data into uart ouput buffer
    memcpy(ctx->uart_output_fifo, bytes, amount);

    // Notfiy the uart driver about the data
    uart_rpc_write(amount);
}

void uart_read(uart_ctx_t *ctx) {
    FifoDataport* fifo = ctx->uart_input_fifo;

    void *buffer = NULL;
    // Get buffer with the available data
    size_t avail = FifoDataport_getContiguous(fifo, &buffer);

    uart_write(ctx, buffer, avail);

    // Remove the data in the buffer from the fifo
    FifoDataport_remove(fifo, avail);
}

static void data_available_callback(void * ctx_ptr) {
    //Debug_LOG_DEBUG("Uart event callback triggered");
    uart_ctx_t *ctx = ctx_ptr;
    
    int err;
    
    uart_read(ctx);
    
    err = uart_event_reg_callback((void *) &data_available_callback, (void *) ctx);
    if (err)  {
        Debug_LOG_ERROR("uart_event_reg_callback failed, code: %d", err);
    }
}

//---------------------------------------------------------------------------
void pre_init(void)
{
    Debug_LOG_DEBUG("pre_init");
}


//---------------------------------------------------------------------------
void post_init(void)
{
    uart_ctx_t *ctx = &uart_ctx;

    // init function to register callback
    OS_Dataport_t input_port  = OS_DATAPORT_ASSIGN(uart_input_port);
    OS_Dataport_t output_port = OS_DATAPORT_ASSIGN(uart_output_port);

    ctx->uart_input_fifo  = (FifoDataport *) OS_Dataport_getBuf(input_port);
    ctx->uart_output_fifo = (uint8_t *) OS_Dataport_getBuf(output_port);

    int err = uart_event_reg_callback((void *) &data_available_callback, (void *) ctx);
    if (err) {
        Debug_LOG_ERROR("uart_event_reg_callback failed, code: %d", err);
    }
    Debug_LOG_DEBUG("uart callback registered!");
}