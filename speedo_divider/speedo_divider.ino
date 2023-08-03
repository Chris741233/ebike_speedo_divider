/******************************************************************
Created with PROGRAMINO IDE for Arduino - 01.08.2023 | 13:57:45

Author      : Chris74

Description : E-bike hacking, speedometer divider (signal / 2) 
- expl: if 30Km/h real, ebike read 15Km/h
Choose diagram reed switch (2 wire) or hall sensor (3 wire, see docs)

Source of this code  : Github: https://github.com/Chris741233/ebike_speedo_divider 
License              : MIT

--------------------

Board                 : tested with Digispark Attiny85 
Arduino IDE (1.8.19)  : Digistump AVR Board, ArminJo version  1.7.5
Board firmware        : 2.6                      
Clock                 : you can choose 16MHz no USB and Recommended (6586 byte avail.)
Micronucleus          : v2.5 
Install board, see    : https://github.com/ArminJo/DigistumpArduino

Note: by default Digispark board are burned with old 1.6.x firmware, its also Ok with correct IDE config


Power via USB or External Source (regulated 5v or 7-35v, 12v or less recommended with automatic selection)
On-board 500ma 5V Regulator

******************************************************************/

// Requires headers for AVR defines and ISR function
#include <avr/io.h>
#include <avr/interrupt.h>


#define LED_PIN 1       // Digispark LED_BUILTIN (PB1) and ebike controler, speed input
#define INTERRUPT_PIN 2 // INT0 -> PB2, ebike speed sensor (wheel)

#define BUTTON_PIN 0    // PB0, button On/Off divider (not mandatory)


volatile byte i = 0;
volatile long rev = 0;  // Revolution Count: Volatile obligatoire pour interupt si échange avec loop ! (long max 2,147,483,647)


// -- ISR Interrupt Attiny85
ISR(INT0_vect)
{
    i = digitalRead(INTERRUPT_PIN);
    if (i==0) rev++; // 0 or low = contact speedo ON
}


void setup() 
{
    
    // test internal Led
    pinMode(LED_PIN, OUTPUT);         // Internal Led and  controler
    digitalWrite(LED_PIN, HIGH);      // Blink to show it's booting up and your LED is wired correctly
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
    
    
    
    // Code here is the key piece of configuring and enabling the interrupt 
    cli();                            // Disable interrupts during setup
    
    GIMSK |= (1 << INT0);   // enabling the INT0 (external interrupt) 
    MCUCR |= (1 << ISC00);  // CHANGE mode
    
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);  // speed sensor interrupt
    pinMode(BUTTON_PIN, INPUT_PULLUP);     // Button On/Off speedo divider
    
    sei();                                 // last line of setup - enable interrupts after setup
    
} // end setup


void loop()
{
    byte button = digitalRead(BUTTON_PIN); // read state of button On/Off (speedo divider)
    
    // send signal only when pair revolution (rev%2=0)
    if (button==1) {
        if (i==0 && rev%2==0) digitalWrite(LED_PIN, HIGH); // i==0 or low = contact speedo ON  
        else digitalWrite(LED_PIN, LOW);
    }
    // if button divider is off, send all signal (normal mode)
    else {
        if (i==0) digitalWrite(LED_PIN, HIGH); 
        else digitalWrite(LED_PIN, LOW);
    }
} // endloop




