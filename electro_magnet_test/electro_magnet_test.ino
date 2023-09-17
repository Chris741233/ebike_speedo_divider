/******************************************************************
Created with PROGRAMINO IDE for Arduino - 05.09.2023 | 18:02:21
Project     :
Libraries   :
Author      : Chris74
Description : test mini electromagnet to simul tachymeter (test with Sigma counter)

Source of this code  : Github: https://github.com/Chris741233/ebike_speedo_divider 
License              : MIT


diagram (with 220 Ohm resistor)
http://www.robotsforfun.com/webpages/electromagnet.html

north and south pole
https://kitbook.tornadodesign.com/electromagnet-poles/

// test conso. electromagnet 10*10 6V 0.2KG 2N with trans. 2N2222, 220 Ohm resistor and diode : +5V 0.21A / ~1.1W 
https://fr.aliexpress.com/item/32824124231.html?spm=a2g0o.order_list.order_list_main.47.21ef5e5bT2UGA4&gatewayAdapt=glo2fra


******************************************************************/



// -------- GPIO Arduino Nano --------------

#define MAGNET_PIN 2
#define LED_PIN    13  // LED_BUILTIN



// -------- SETTING CONSTANTS ------------------------
/*
Sigma bike counter
26'   = 2085
27'   = 2195
27.5' = 2180
28'   = 2200
29'   = 2300
700C  = 2095
*/

const int WHEELE_CIRC = 2085; // circ. in mm
const int SPEED = 25;         // desired Km/h


// *** Calculation, dont modif ! RPM = (kmh*1000)/(circ/1000)/60
const float RPM = (SPEED*1000.0)/(WHEELE_CIRC/1000.0)/60.0;
const int DELAY_TIME = 60/RPM * 1000;   // delay between magnet pulse
const int PULSE_TIME = 30;              // 30ms or 50ms is Ok


// -------- GLOBALES VARIABLES ----------------------

unsigned long start = 0;  // timer



// -------- MAIN  ------------------------

void setup()
{
    // --- Serial
    Serial.begin(115200);
    
    pinMode(MAGNET_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    digitalWrite(MAGNET_PIN, LOW); 
    digitalWrite(LED_PIN, LOW); 
    
} // endsetup


void loop()
{
    
    run_simul();
    
    // test conso (see up)
    // digitalWrite(MAGNET_PIN, HIGH);
    
} // endloop


// ------------- FUNCTIONS ---------------------------

void run_simul() {
    // -- simul spedd, low signal (magnet off)
    start = millis();
    while (millis() - start <= DELAY_TIME - PULSE_TIME) {
        digitalWrite(MAGNET_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        //Serial.println(DELAY_TIME);
    }
    // -- simul speed, high signal (magnet on)
    start = millis();
    while (millis() - start <= PULSE_TIME) {
        digitalWrite(MAGNET_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);
    }
    
} //endfunc
