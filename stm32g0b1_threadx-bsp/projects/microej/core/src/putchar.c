/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>

#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_def.h"

/* Defines -------------------------------------------------------------------*/

#define UART_TRANSMIT_TIMEOUT 0xFFFF

// USARTx clock resources
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

// USARTx RX/TX Pins
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF1_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF1_USART2


/* Global --------------------------------------------------------------------*/

UART_HandleTypeDef UartHandle;

// cppcheck-suppress [misra-c2012-8.9]: Global variable necessary to do only one init.
static int putchar_initialized = 0;

/* Private API ---------------------------------------------------------------*/

static void PUTCHAR_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	// enable GPIO TX/RX clock
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();

	// Enable USARTx clock
	USARTx_CLK_ENABLE();

	// UART TX GPIO pin configuration
	GPIO_InitStruct.Pin       = USARTx_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USARTx_TX_AF;

	HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

	// UARTx RX GPIO pin configuration
	GPIO_InitStruct.Pin = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;

	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

	UartHandle.Instance        = USARTx;
	UartHandle.Init.BaudRate   = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		while(1){}; // error
	}

}

static int PUTCHAR(int ch)
{
    if (!putchar_initialized)
    {
        PUTCHAR_init();
        putchar_initialized = 1;
    }

    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, UART_TRANSMIT_TIMEOUT);

    return ch;
}

/* Public functions ----------------------------------------------------------*/

// cppcheck-suppress [misra-c2012-21.2]: this reserved identifier is overriden on purpose.
int fputc(int ch, FILE *f)
{
	(void)(f);

	return PUTCHAR(ch);
}

int _write(int file, char *data, int len)
{
	(void)(file);
    int bytes_written;

    for (bytes_written = 0; bytes_written < len; bytes_written++)
    {
    	// cppcheck-suppress [misra-c2012-17.7]: returned value not used for performance purpose to avoid unnecessary checks
        PUTCHAR(*data);
        data++;
    }

    return bytes_written;
}
