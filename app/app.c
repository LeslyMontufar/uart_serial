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

#define SERIAL_TX(texto)		hw_uart_tx((uint8_t*)texto,sizeof(texto))

static cbf_t cbf;
static uint8_t cbf_area[APP_UART_BUFFER_MAX];
static bool app_started = false;
extern uint32_t delay;

void app_button_interrupt(void){

	static uint32_t debouncing_time_ms = 0;
	if(!app_started)
		return;

	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		if(delay == 1000){
			SERIAL_TX("set timer 50\n"); //13
		}
		else{
			SERIAL_TX("set timer 1000\n"); //15
		}
		debouncing_time_ms = hw_tick_ms_get();
	}
}

void app_button2_interrupt(void){

	static uint32_t debouncing_time_ms = 0;
	if(!app_started)
		return;

	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		debouncing_time_ms = hw_tick_ms_get();
	}
}

void app_tick_1ms(void){
	if(!app_started)
		return;
}

void app_init(void){
	cbf_init(&cbf,cbf_area,APP_UART_BUFFER_MAX);
	hw_uart_init(&cbf);
	shell_init();
	app_started = true;
}


void app_loop(void){

}
