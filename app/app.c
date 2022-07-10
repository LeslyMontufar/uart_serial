/*
 * Lesly Montúfar
 *
 * app.c
 *
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "cbf.h"
#include "shell.h"
#include "app.h"
#include "hw.h"

#define APP_DEBOUNCING_TIME_MS 	50
#define APP_UART_BUFFER_MAX 	64

static bool app_started = false;
extern uint32_t delay;

void app_button_interrupt(void){

	static uint32_t debouncing_time_ms = 0;
	if(!app_started)
		return;

	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
//		delay = (delay == 1000)? 100 : 1000;
		if(delay == 1000){
//			hw_uart_tx((uint8_t*)"led 1 on\n",9);
			hw_uart_tx((uint8_t*)"set timer 50\n",13);
		}
		else{
			delay = 1000;
//			hw_uart_tx((uint8_t*)"led 1 off\n",10);
		}
		debouncing_time_ms = hw_tick_ms_get();
	}
}
void app_tick_1ms(void){
	if(!app_started)
		return;
}

void app_init(void){
	shell_init();
	app_started = true;
}

void app_loop(void){

}
