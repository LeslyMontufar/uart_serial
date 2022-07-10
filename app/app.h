/*
 * app.h
 *
 *  Created on: Jun 20, 2022
 *      Author: lesly
 */

#ifndef APP_H_
#define APP_H_

void app_tick_1ms(void);
void app_init(void);
void app_loop(void);
void app_button_interrupt(uint8_t pin_it);
void app_delay_toggle(void);

#endif /* APP_H_ */
