#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define LED_PIN PB5       // LED on pin PB5
#define BUTTON_PIN PB4    // Button on pin PB4

void setup() {
    Uart_Init();
    DDRB |= (1 << LED_PIN);       // Set LED pin as output
    DDRB &= ~(1 << BUTTON_PIN);   // Set BUTTON_PIN as input and not or
    PORTB |= (1 << BUTTON_PIN);   // Enable pull-up resistor on BUTTON_PIN
}

void loop() {
    int buttonState = !(PINB & (1 << BUTTON_PIN));  // 1 if pressed, 0 if released
    
    if (Uart_IsDataAvailable()) {
        char command = Uart_ReadData();
        if (command == '1') {
            PORTB |= (1 << LED_PIN);
            Uart_SendString("LED ON\n", 7);
        } else if (command == '0') {
            PORTB &= ~(1 << LED_PIN);
            Uart_SendString("LED OFF\n", 8);
        }
    }
    
    if (buttonState) {
        Uart_SendString("button_pressed\n", 15);
    } else {
        Uart_SendString("button_released\n", 16);
    }
    
    _delay_ms(100);  // Debounce delay
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
