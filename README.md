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


