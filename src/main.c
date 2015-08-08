//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

void Delay(uint32_t nTime);

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* === (1) Enable Peripheral Clocks === */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* === (2) Configure Pins for PORTC to be output === */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* === (3) Configure SysTick Timer === */
	if(SysTick_Config(SystemCoreClock / 1000)) {	// timer interrupt = 1msec
		while (1);
	}

	while (1) {
		static int ledval = 0;

		/* === (4) toggle led === */
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, (ledval) ? Bit_SET : Bit_RESET);
		ledval = 1 - ledval;

		Delay(250);     // wait 250ms
	}
}

/* ===  (5) Timer code === */
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime) {
    TimingDelay = nTime;
    while(TimingDelay !=0);
}

void SysTick_Handler(void) {
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

//#ifdef  USE_FULL_ASSERT
//void assert_failed(uint8_t *file, uint32_t line)
//{
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    //while(1);
//}
//#endif
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
