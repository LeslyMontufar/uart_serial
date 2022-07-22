# UART com interrupção e buffer circulares ![Language grade: C](https://img.shields.io/badge/language-C-blue)

Autora: Lesly Montúfar

Uso da porta serial UART do STM32F1xx em loop back com um buffer circular para implementar um console via porta serial, capaz de receber comandos do PC para ligar/desligar leds e ler o estado da chave do usuário.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Crie uma nova configuração no CubeMX, usando clock externo e barramentos internos com valores
máximos possíveis de clock. Depois, escolha uma UART e a configure para um baud rate de 115200,
8 bits de dados, sem paridade e um stop bit. Configure também os GPIOs relacionados aos LEDs e
ao botão do usuário.

Crie um programa que implemente um console via serial, capaz de receber comandos do PC para
ligar/desligar leds e ler o estado da chave do usuário.

Os comandos devem ser criados em forma textual, com finalizador de linha dado por “\n” (código
10, new line), da seguinte forma:


| Comando | Descrição |
|---------|-----------|
| `led <n> <on\|off>` | Liga ou desliga um dos leds da placa. O led deve ser especificado como 1, 2, etc.|
| `bot <n>` | Realiza a leitura de um determinado pino de GPIO, mapeado como entrada para realizar a simulação de um botão.  |
| `help` | Envia instruções de ajuda, listando os comados suportados e maneira de usar. |

O programa deve considerar que os dados são recebidos via interrupção da serial, sendo que
cada byte recebido deve ser armazenados num buffer circular para posterior tratamento pela rotina de
decodificação e resposta, chamada do seu loop principal. O envio da resposta pode ser feito de forma
mais direta, através da rotina de transmissão do HAL.

Lembre-se que, ao encontrar o caractere “\n”, você deve entender que um novo comando chegou e a
rotina de tratamento será acionada. Apesar de você poder tratar os dados recebidos manualmente,
funções como scanf() podem ajudar na leitura dos valores recebidos.

## Desenvolvimento

1. Pelos vídeos da aula entendi `cbf.c`,`cbf.h`,`shell.c`, `shell.h`.
2. `hw` (uart2) faz o papel de quem transmite para o `shell` (uart1), que recebe a msg por interrupção em `shell_uart_interrupt`. `shell`(uart) executa uma ação e devolve uma resposta para o `hw` (uart2).
3. `shell_uart_interrupt` chama `shell_add_byte`, que chama `shell_process()` quando msg completa é recebida, que é quando é recebido um '\n'. Em `shell_process()` é processado a msg recebida de `hw` (uart2).
```
void shell_process(void){
	uint32_t argc;
	uint8_t *argv[SHELL_MAX_ARGS];
	bool error = true;

	if(shell_parse_args(shell_ctrl.cmd, shell_ctrl.size, &argc, (uint8_t **)argv, SHELL_MAX_ARGS)){
		if(argc == 3){
			if((strncmp("set", (char*)argv[0],3) == 0) && (strncmp("time", (char*)argv[1],4) == 0)){
				int time_ms = 0;

				if(sscanf((char*)argv[2], "%d",&time_ms) == 1){
					if(time_ms > 0){
						delay = time_ms;
						shell_uart_tx((char*)"ok\n",3);
						error = false;
					}
				}
			}
			else if(strncmp("led", (char*)argv[0],3) == 0){
				int n = 0;

				if(sscanf((char*)argv[1], "%d",&n)){
					if(strncmp("on", (char*)argv[2],2) == 0){
						hw_led_n_state_set(n,true);
					}
					else if(strncmp("off", (char*)argv[2],3) == 0){
						hw_led_n_state_set(n,false);
					}
					if((strncmp("on", (char*)argv[2],2)== 0) || (strncmp("off", (char*)argv[2],3)==0)){
						shell_ctrl.size = sprintf((char*)shell_ctrl.cmd, "%s %d %s\n", argv[0],n,argv[2]);
						shell_uart_tx(shell_ctrl.cmd, shell_ctrl.size);
						error = false;
					}
				}
			}
		}else if(argc == 2){
			if((strncmp("get", (char*)argv[0],3) == 0) && (strncmp("time", (char*)argv[1],4) == 0)){
				shell_ctrl.size = sprintf((char*)shell_ctrl.cmd, "time %u\n",(unsigned int)delay);
				shell_uart_tx(shell_ctrl.cmd, shell_ctrl.size);
				error = false;
			}
			else if(strncmp("bot", (char*)argv[0],3) == 0){
				int n = 0;

				if(sscanf((char*)argv[1], "%d",&n)){
					if(hw_button_n_state_get(n)){
						shell_ctrl.size = sprintf((char*)shell_ctrl.cmd, "%s %d on\n", argv[0],n);
						shell_uart_tx(shell_ctrl.cmd, shell_ctrl.size);
						error = false;
					}else {
						shell_ctrl.size = sprintf((char*)shell_ctrl.cmd, "%s %d off\n", argv[0],n);
						shell_uart_tx(shell_ctrl.cmd, shell_ctrl.size);
						error = false;
					}
				}
			}
		}
		else if(argc == 1){
			if(strncmp("help", (char*)argv[0],4) == 0){
				shell_uart_tx((uint8_t*)"led <n> <on|off>: liga ou desliga um dos leds da placa, n = identificacao do led; bot <n>: Realiza a leitura de um determinado pino de GPIO, n = identificacao do led; help: Lista os comados suportados e maneira de usar.\n",220);
				error = false;
			}
		}
	}

	if(error){
		shell_uart_tx((uint8_t*)"error\n",6);
		error = false;
	}
}
```
4. Para realizar um teste, o botão 1 que funciona por interrupção, envia uma msg de mudar delay em que o led 1 pisca.
```
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
```
5. E, o botão 2 que funciona por interrupção, envia uma msg para ligar ou desliar o led 2.
```
void app_button2_interrupt(void){

	static uint32_t debouncing_time_ms = 0;
	if(!app_started)
		return;

	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		if(hw_led_n_state_get(2))
			SERIAL_TX("led 2 off\n");
		else
			SERIAL_TX("led 2 on\n");
		debouncing_time_ms = hw_tick_ms_get();
	}
}
```
6. Os comandos funcionam e fazem o esperado: led 1 (PC13), led 2 (led verde) e led 3 (led vermelho). Iniciei com todos os led ligados, ou seja output setado como Low, que na configuração push pull é o estado em haverá ddp!=0 e corrente passando pelo led.

![Circuito](https://github.com/LeslyMontufar/uart_serial/blob/main/img/circuito.jpeg)

7. Para verificar a msg chegando em `hw` (uart2), adicionei um led, que acende e apaga a cada caractere recebido pela porta serial do uart2.
```
void hw_uart2_interrupt(void){
	uint8_t c;
	uint32_t sr;
	USART_TypeDef *h = huart2.Instance;

	sr = h->SR;
	while(sr & (UART_FLAG_ORE | UART_FLAG_PE | UART_FLAG_FE | UART_FLAG_NE)){
		sr = h->SR;
		c = h->DR;
	}

	// se sem erros e com dado recebido ... pegar o dado e colocar no buffer
	if(sr & UART_FLAG_RXNE){
		c = h->DR;

		if(hw_uart_cbf)
			cbf_put(hw_uart_cbf,c);
	}
  
	// led 3 que liga e desliga a cada caractere
	if(hw_led_n_state_get(3))
		hw_led_n_state_set(3,false);
	else
		hw_led_n_state_set(3,true);
}
```

8. No debug, é possível ver os caracteres chegando em `hw` (uart2).
![hw_rx_funcionando](https://github.com/LeslyMontufar/uart_serial/blob/main/img/led_2_off_recebido.png)
