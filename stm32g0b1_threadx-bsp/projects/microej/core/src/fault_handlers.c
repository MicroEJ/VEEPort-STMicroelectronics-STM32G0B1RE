/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file fault_handlers.c
 *
 * @brief This file contains the fault exception handlers.
 *
 * @details The CMSIS names are used.
 *
 * There are two modes:
 * - verbose
 * - lite
 *
 * In verbose mode, each fault exception handler will:
 * - print the stack frame,
 * - print its name,
 * - print the associated fault status register,
 * - try to make an analysis of the fault cause,
 * - make an infinite loop.
 *
 * In lite mode, each fault exception handler will:
 * - print its name,
 * - make an infinite loop.
 *
 * Define the @ref VERBOSE_MODE macro to activate verbose mode.
 */

/* Includes ______________________________________________________________________*/
#include <stdbool.h>
#include <stdio.h>

#include "stm32g0xx.h"

#include "fault_handlers.h" // will check prototypes and import public types
#include "LLMJVM.h"

/* Macros ________________________________________________________________________*/

/**
 * @brief Define this macro to if you are using Keil's ARMCC toolchain. If you are using IAR or GCC toolchains, do not define it.
 */
//#define COMPILE_WITH_ARMCC

/**
 * @brief Define this macro to enable verbose handlers. Verbose handlers will consume  more memory than lite handlers.
 */
#define VERBOSE_MODE

/**
 * @brief Make an infinite while loop.
 */
#define INFINITE_LOOP()		{while(1){};}

#ifdef VERBOSE_MODE
/**
 * @brief This macro is called as the begining of each handlers to print the stack frame.
 *
 * @details There are two stack pointers in Cortex-M processors:
 * - MSP (Main Stack Pointer)
 * - PSP (Process Stack Pointer)
 * When handling faults, we need to know which one was used before entering the faults so that
 * we can retrieve interesting information.
 *
 * To do so, we need to test the value of EXC_RETURN, which is stored into LR during exception
 * (see Section 8.1.4 EXC_RETURN). Then, we can fetch the correct stack pointer with the MRS instruction
 * (MRS = Move from special register to general register).
 *
 * NOTE: IAR's compiler doesn't like then comments are interlaced with macro definition lines.
 * Here is what the macro does:
 * - Check if bit 2 (stack return) is set in EXC_RETURN / LR
 * - If-Then condition instruction
 * - If equals, store MSP in R0
 * - Else, store PSP in RO (1st parameter)
 * - MSP is stored in R1 (2nd parameter)
 * - PSP is stored in R2 (3rd parameter)
 * - LR is stored in R3 (4th parameter)
 * - Call C function for printing
 */
#if defined(__CC_ARM)
void printer(uint32_t current_sp, uint32_t msp, uint32_t psp, uint32_t exc_return);

__asm void print_stacked_registers()
{
    TST     LR, #4
    ITE     EQ
    MRSEQ   R0, MSP
    MRSNE   R0, PSP
    MRS     R1, MSP
    MRS     R2, PSP
    MOV     R3, LR
	  LDR     R4, =__cpp(printer)
	  BX		  R4
}
#else
// Compatible with IAR for Cortex-M0+
// cppcheck-suppress [misra-c2012-8.4]: this function cannot be static because it won't be seen by assembly code during link edition.
void print_stacked_registers(void){
	__asm(
			"MOVS R7, #4 	\n"
			"MOV R6, LR 	\n"
			"TST R6, R7 	\n"
			"BNE branch_else \n"
			"MRS R0, MSP 	\n"
			"B branch_end 	\n"
			"branch_else:  	\n"
			"MRS R0, PSP  	\n"
			"branch_end:  	\n"
			"MRS R1, MSP  	\n"
			"MRS R2, PSP  	\n"
			"MOV R3, LR  	\n"
			"BL printer  	\n"
	);
}



#endif


/* Global variables  ___________________________________________________________*/
/* */

/* Private types and variables  ___________________________________________________*/


/* Private functions  ______________________________________________________________*/
// WARNING:
/**
 * @brief C function to print the stacked registers (== the stack frame) along with EXEC_RETURN.
 *
 * @warning this function cannot be static because it won't be seen by assembly code during link edition.
 * You should not called this function directly.
 */
// cppcheck-suppress [misra-c2012-8.4]: this function cannot be static because it won't be seen by assembly code during link edition.
/*static*/ void printer(uint32_t current_sp, uint32_t msp, uint32_t psp, uint32_t exc_return)
{
	  uint32_t *sp;
	printf("---------------------------------------------------------------------\n");
    // Show stack pointers
    printf("Current SP = %.8lX\n", current_sp);
    printf("MSP = %.8lX\n", msp);
    printf("PSP = %.8lX\n", psp);
    printf("\n");

    // Show stacked registers == stack frame (see section 8.1.3 and figure 12.4)
    sp = (uint32_t*) current_sp;
    printf("Stack frame: \n");
    printf("R0 =\t%.8lX\n", *sp++);
    printf("R1 =\t%.8lX\n", *sp++);
    printf("R2 =\t%.8lX\n", *sp++);
    printf("R3 =\t%.8lX\n", *sp++);
    printf("R12 =\t%.8lX\n", *sp++);
    printf("LR =\t%.8lX\n", *sp++);
    printf("PC =\t%.8lX\n", *sp++);
    printf("xPSR =\t%.8lX\n", *sp++);
    printf("\n");

    printf("EXC_RETURN (LR) = %.8lX\n", exc_return);
    printf("---------------------------------------------------------------------\n");
}

/* Public functions  _______________________________________________________________*/

/*
 * @brief Hard Fault exception handler.
 */
void HardFault_Handler(void)
{
    print_stacked_registers();
    printf("%s \n",__func__);

#ifndef MICROEJ_CORE_VALIDATION
    printf("Perform LLMJVM_dump \n");
    LLMJVM_dump();
#endif
    INFINITE_LOOP();
}


/*
 * @brief Memory Management Fault exception handler.
 */
void MemFault_Handler (void)
{
    print_stacked_registers();
    printf("%s \n",__func__);

#ifndef MICROEJ_CORE_VALIDATION
    printf("Perform LLMJVM_dump \n");
    LLMJVM_dump();
#endif

    INFINITE_LOOP();
}


/*
 * @brief Bus Fault exception handler.
 */
void BusFault_Handler (void)
{
    print_stacked_registers();
    printf("%s \n",__func__);

#ifndef MICROEJ_CORE_VALIDATION
    printf("Perform LLMJVM_dump \n");
    LLMJVM_dump();
#endif
    INFINITE_LOOP();
}


/*
 * @brief Usage Fault exception handler.
 */
void UsageFault_Handler (void)
{
    print_stacked_registers();
    printf("%s \n",__func__);

#ifndef MICROEJ_CORE_VALIDATION
    printf("Perform LLMJVM_dump \n");
    LLMJVM_dump();
#endif
    INFINITE_LOOP();
}
#else
/*
 * @brief Hard Fault exception handler.
 */
void HardFault_Handler (void)
{
	printf("%s \n",__func__);
	INFINITE_LOOP();
}


/*
 * @brief Memory Management Fault exception handler.
 */
void MemFault_Handler (void)
{
	printf("%s \n",__func__);
	INFINITE_LOOP();
}


/*
 * @brief Bus Fault exception handler.
 */
void BusFault_Handler (void)
{
	printf("%s \n",__func__);
	INFINITE_LOOP();
}


/*
 * @brief Usage Fault exception handler.
 */
void UsageFault_Handler (void)
{
	printf("%s \n",__func__);
	INFINITE_LOOP();
}

#endif
