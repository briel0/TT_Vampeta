#include <Arduino.h>

#define PAU_POS_PIN 5
#define PAU_NEG_PIN 23

void setup(){
    Serial.begin(115200); // Comunicacao serial com baudrate 115200
    pinMode(PAU_NEG_PIN, 5);
    pinMode(PAU_POS_PIN, 23);

}

void loop(){
    digitalWrite(23, HIGH);
    digitalWrite(5, LOW);

    delay(2000);

    digitalWrite(23, LOW);
    digitalWrite(5, HIGH);

    delay(2000);

    digitalWrite(23, LOW);
    digitalWrite(5, LOW);

    delay(2000);

}