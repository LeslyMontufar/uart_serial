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
#include "app.h"
#include "hw.h"


#define APP_DEBOUNCING_TIME_MS 	50
#define APP_UART_BUFFER_MAX		64

static cbf_t cbf;
static uint8_t cbf_area[APP_UART_BUFFER_MAX];
static bool app_started = false;

void app_button_interrupt(void){

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
	app_started = true;
}

void app_loop(void){
	static uint32_t loop_counter = 0;
	static uint8_t tx_buffer[APP_UART_BUFFER_MAX];
	static uint8_t rx_buffer[APP_UART_BUFFER_MAX];
	uint8_t data;
	uint32_t tx_size;
	uint32_t rx_size;
	cbf_status_t status;

	tx_size = snprintf((char *)tx_buffer, APP_UART_BUFFER_MAX-1, "loop %lu\n", loop_counter++);

	hw_uart_tx(tx_buffer, tx_size);

	hw_delay_ms(10);
	rx_size = 0;

	while(true){
		status = cbf_get(&cbf,&data);
		if(status == CBF_EMPTY || (rx_size >= APP_UART_BUFFER_MAX))
			break;
		rx_buffer[rx_size++] = data;
	}

	if(strncmp((char*)tx_buffer, (char*) rx_buffer,tx_size) == 0){
		// duas piscadas - OK
		hw_led_state_set(true);
		hw_delay_ms(100);
		hw_led_state_set(false);
		hw_delay_ms(100);

		hw_led_state_set(true);
		hw_delay_ms(100);
		hw_led_state_set(false);
		hw_delay_ms(100);
	} else{
		// uma piscada longa - falha
		hw_led_state_set(true);
		hw_delay_ms(300);
		hw_led_state_set(false);
		hw_delay_ms(300);
	}

	hw_delay_ms(1000);
}
