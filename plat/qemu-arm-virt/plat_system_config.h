/*
 * Platform defaults for the QEMU Arm virt.
 *
 * Copyright (C) 2023-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */


// kernel log uses UART_0, so we can use UART_1 for i/o test
#define UART_IO     UART_1
