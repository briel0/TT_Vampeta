#include <Arduino.h>

#define PAU_POS_PIN 5
#define PAU_NEG_PIN 23

void setup(){
    Serial.begin(115200); // Comunicacao serial com baudrate 115200
    pinMode(PAU_NEG_PIN, OUTPUT);
    pinMode(PAU_POS_PIN, OUTPUT);
}

void loop(){
    Serial.println("Subindo");
    digitalWrite(23, HIGH);
    digitalWrite(5, LOW);

    delay(4000);

    Serial.println("Descendo");

    digitalWrite(23, LOW);
    digitalWrite(5, HIGH);

    delay(4000);

    digitalWrite(23, LOW);
    digitalWrite(5, LOW);

    delay(2000);

}