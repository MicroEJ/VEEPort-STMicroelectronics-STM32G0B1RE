/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "microej_time.h"
#include "microej.h"
#include "stm32g0xx_hal.h"
#include "tx_api.h"

/* Defines -------------------------------------------------------------------*/

#define TIMx                           TIM2
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()

// TIMER definition
#define TIMER_CHANNEL TIM_CHANNEL_1

#define MAX_TIMER_VALUE 0xFFFFFFFFu
#define TC_CLOCK 1000000    // clock frequency in Hertz

/* Globals -------------------------------------------------------------------*/

// this value is updated at each interrupt
static int64_t software_counter;

/* TIM handle declaration */
static TIM_HandleTypeDef TimHandle;

/** Offset in ms from system time to application time */
static uint64_t microej_application_time_offset = 0;

/* Private functions ---------------------------------------------------------*/

static int64_t timer_get_counter_value(void)
{
	int64_t tc = __HAL_TIM_GET_COUNTER(&TimHandle);
	return tc & (int64_t) 0xffffffffu;
}

static int64_t time_hardware_timer_getTicks(void)
{
	return software_counter + timer_get_counter_value();
}

/*
 * An interrupt can occur between the reading of software_counter
 * and timer_get_counter_value(). So the value of software_counter is not
 * coherent with the value of timer_get_counter_value().
 * Compute twice the ticks value. At least one will be correct.
 */
static int64_t time_hardware_timer_getAndCheckTicks(void)
{
	int64_t val1 = time_hardware_timer_getTicks();
	int64_t val2 = time_hardware_timer_getTicks();

	return (val2 < val1) ? val1 : val2;
}

static void timer_set_interrupt_value(int64_t itVal)
{
	__HAL_TIM_SET_COMPARE(&TimHandle, TIMER_CHANNEL, (uint32_t)itVal);
	__HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_CC1);
}

static int64_t timer_get_max_counter_value(void)
{
	return MAX_TIMER_VALUE;
}

static int64_t timer_get_max_value(void)
{
	// get the max value which can be used for an interrupt
	// return the max value of the timer divided by 2
	// if the interrupt is initialize to the max value of timer, an overflow will occurs

	// cppcheck-suppress [misra-c2012-10.1]: This use case enter in the rule 10.1 exception of MISRA (false positive)
	return timer_get_max_counter_value() >> 1; // / 2
}

/*
 * Increment the soft timer value
 */
static void timer_update_software_counter(void)
{
	int64_t currentTimerValue = timer_get_counter_value();
	software_counter += currentTimerValue;
}

/*
 * Reconfigure interrupt value
 */
static void timer_configure_next_interrupt(void)
{
	timer_set_interrupt_value(timer_get_max_value());
}

static void timer_set_counter_value(int64_t timeVal)
{
	__HAL_TIM_SET_COUNTER(&TimHandle, (uint32_t) timeVal);
}

/* Interrupt functions -------------------------------------------------------*/

void TIM2_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC1);

	// increment the soft timer value and reset the hardware timer to 0
	timer_update_software_counter();
	timer_set_counter_value(0);

	timer_configure_next_interrupt();
}

/* Public functions ----------------------------------------------------------*/

void microej_time_init(void)
{
	TIM_OC_InitTypeDef sConfig;
	uint32_t uwPrescalerValue = 0;

	// initialize fields
	software_counter = 0;
	TimHandle.Instance = TIMx;

	// initialize hardware timer
	{
		// enable timer clock
		TIMx_CLK_ENABLE();

		// Compute the prescaler value to have TIMER counter clock equal to TC_CLOCK Hz
		uwPrescalerValue = (uint32_t)((SystemCoreClock) / TC_CLOCK) - (uint32_t) 1;

		// Initialize TIMER peripheral as follows:
		//   - Period = TC_CLOCK - 1
		//   - Prescaler = ((SystemCoreClock / 2)/TC_CLOCK) - 1
		//   - ClockDivision = 1
		//   - Counter direction = Up
		TimHandle.Init.Period            = MAX_TIMER_VALUE;
		TimHandle.Init.Prescaler         = uwPrescalerValue;
		TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
		TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
		TimHandle.Init.RepetitionCounter = 0;

		if (HAL_TIM_OC_Init(&TimHandle) != HAL_OK) {
			while(1){}; // error
		}

		// configure the Output Compare channel 1
		sConfig.OCMode       = TIM_OCMODE_ACTIVE;
		sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
		sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH /*not used*/;
		sConfig.Pulse        = MAX_TIMER_VALUE;
		sConfig.OCIdleState  = TIM_OCIDLESTATE_SET /*not used*/;
		sConfig.OCNIdleState = TIM_OCNIDLESTATE_SET /*not used*/;

		if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
			while(1){}; // error
		}

		// initialize the timer value to 0
		timer_set_counter_value(0);
		// initialize the interrupt
		timer_configure_next_interrupt();

		// start timer channel 1 in Output compare mode
		if (HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK) {
			while(1){}; // error
		}
	}

	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0x00);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

int64_t microej_time_get_current_time(uint8_t isPlatformTime)
{
	/*
	 * /!\
	 * isPlatformTime == true when ej.bon.Util.platformTimeMillis
	 * isPlatformTime == false when java.lang.System.currentTimeMillis
	 * /!\
	 */

	int64_t systemTime = time_hardware_timer_getAndCheckTicks() / 1000;
	return ((uint8_t) MICROEJ_TRUE == isPlatformTime) ? systemTime : ( systemTime + (int64_t) microej_application_time_offset );
}

int64_t microej_time_get_time_nanos(void)
{
	return time_hardware_timer_getAndCheckTicks() * 1000;
}

void microej_time_set_application_time(int64_t t)
{
	int64_t currentTime = (int64_t) microej_time_get_current_time(MICROEJ_TRUE);
	microej_application_time_offset = t - currentTime;
}

/*
 * Convert a time in milliseconds in ticks.
 * Result is round up to the next tick (ie. converting back the resulting ticks
 * to milliseconds results in a value greater than or equals to given time).
 * Return a saturated number of ticks or 0 if given time is negative.
 */
int64_t microej_time_time_to_tick(int64_t time)
{
	int64_t output;
	int64_t ticks64;
	uint32_t ticks;
	uint8_t overflows = MICROEJ_TRUE;

	if(0 >= time){
		overflows = MICROEJ_FALSE;
		output = 0;
	}else{
		int64_t mticks = time * (int64_t)TX_TIMER_TICKS_PER_SECOND; // milli-ticks
		// Check for no overflow
		if ( (mticks >= 0) ) {
			ticks64 = (mticks + 999LL) / 1000LL;
			// Check for no overflow
			if ( (ticks64 >= 0) ) {
				ticks = (uint32_t)ticks64;
				// Check for no overflow
				if ( (ticks == (uint32_t)ticks64) ) {
					output = (int64_t)ticks;
					overflows = MICROEJ_FALSE;
				}
			}
		}
	}

	if ((uint8_t) MICROEJ_TRUE == overflows ) {
		output = (int64_t)UINT32_MAX;
	}

	return output;
}
