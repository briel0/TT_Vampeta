#include <Arduino.h>
#include <IRremote.hpp>
#include <PS4Controller.h>

#include "profile.hpp"
#include "actions.hpp"

void setup(){
    Serial.begin(115200);
    pinMode(RIGHT_POS_PIN, OUTPUT);
    pinMode(RIGHT_NEG_PIN, OUTPUT);
    pinMode(LEFT_POS_PIN, OUTPUT);
    pinMode(LEFT_NEG_PIN, OUTPUT);
}

void loop(){
    while(!PS4.begin(macAdress)){
        Serial.println("Aguardando conexão...");
        delay(500);
    }
    Serial.println("Controle conectado!");

    while(PS4.isConnected()){
        int steer = PS4.LStickX();
        int r2 = PS4.R2Value();
        int l2 = PS4.L2Value();
        steer *= -1;

        if(r2 > 10 || l2 > 10){
            frente_padrao(steer, r2, l2, turnfactor);
        }
        else if(steer < -STICK_TRIGGER || steer > STICK_TRIGGER) {
            curva(steer, turnfactor);
        }
    }

    Serial.println("Controle desconectado!");
}