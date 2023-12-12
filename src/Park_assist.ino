// PROJECT      :   Parking Sensor
// Designed by  :   FireFli
// REV          :   1.00
// Date         :   December 2023

#include "../lib/SparkFun_VL53L1X_Arduino_Library/src/SparkFun_VL53L1X.h"
#include <Wire.h>

#define Red_Pin D2
#define Green_Pin D3
#define Blue_Pin D4

//Set up dintance thresholds (mm)

int far = 1500;
int near = 750;
int stop = 300;

int Distance;

int ledState = LOW;  // ledState used to set the LED

unsigned long previousMillis = 0;
unsigned interval = 0;


VL53L1X sensor;

void setup() {
    
    Wire.begin();
    delay(200);
    
    pinMode(Red_Pin, OUTPUT);
    pinMode(Green_Pin, OUTPUT);
    pinMode(Blue_Pin, OUTPUT);
    
    digitalWrite (Red_Pin, HIGH); 
    digitalWrite (Blue_Pin, LOW);
    digitalWrite (Green_Pin, LOW);
    
    delay(500);
    
    digitalWrite (Red_Pin, LOW); 
    digitalWrite (Blue_Pin, HIGH);
    digitalWrite (Green_Pin, LOW);
    
    delay(500);
    
    digitalWrite (Red_Pin, LOW); 
    digitalWrite (Blue_Pin, LOW);
    digitalWrite (Green_Pin, HIGH);
    
    delay(500);
    
    digitalWrite (Red_Pin, LOW); 
    digitalWrite (Blue_Pin, LOW);
    digitalWrite (Green_Pin, LOW);
    
    delay(500);
    
}


void ToF1() {
    
    Wire.setClock(400000);                                          // use 400 kHz I2C
    sensor.init(0x29);
    
    if (!sensor.init(0x29)) {
        Particle.publish("Could not find a valid ToF1 on bus 0, check wiring!");            //  Debug
        while (1);                                                                          //  Debug
            } //else { Particle.publish("Tof1 Device OK");                                  //  Debug
        //}

    sensor.setDistanceMode(VL53L1X::Long);
    sensor.setMeasurementTimingBudget(50000);
    sensor.startContinuous(50);                                                             // timing budget

    //Particle.publish("ToF1: " + String(sensor.read()) + "mm", PRIVATE);                   // Publish Results in mm
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    Distance = sensor.read();
    
}

void LED_PANEL() {
    
    if (Distance >= (far)) {
        digitalWrite (Red_Pin, LOW); 
        digitalWrite (Blue_Pin, LOW);
        digitalWrite (Green_Pin, HIGH);
    
    } else if (Distance <= (far) && Distance >= (near)) {
        interval = 250;
        FlashYellow();

    } else if (Distance < (near) && Distance >= (stop)) {
        interval = 100;
        FlashRed();
        
    } else if (Distance <= (stop)) {

        digitalWrite (Red_Pin, HIGH); 
        digitalWrite (Blue_Pin, LOW);
        digitalWrite (Green_Pin, LOW);
    }
}


void FlashYellow() {
    
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

    if (ledState == LOW) {
        
        digitalWrite (Red_Pin, HIGH); 
        digitalWrite (Blue_Pin, LOW);
        digitalWrite (Green_Pin, HIGH);
        
        ledState = HIGH;
        
    } else {
            
            digitalWrite (Red_Pin, LOW); 
            digitalWrite (Blue_Pin, LOW);
            digitalWrite (Green_Pin, LOW);
            
            ledState = LOW;
            }
    }
}

void FlashRed() {
    
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

    if (ledState == LOW) {
        
        digitalWrite (Red_Pin, HIGH); 
        digitalWrite (Blue_Pin, LOW);
        digitalWrite (Green_Pin, LOW);
        
        ledState = HIGH;
        
    } else {
            
            digitalWrite (Red_Pin, LOW); 
            digitalWrite (Blue_Pin, LOW);
            digitalWrite (Green_Pin, LOW);
            
            ledState = LOW;
            }
    }
}

void loop() {
    
    ToF1();
    LED_PANEL();
    
    delay(100);
}