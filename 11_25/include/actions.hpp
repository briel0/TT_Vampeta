#ifndef ACTIONS_H
#define ACTIONS

#include <Arduino.h>
#include <math.h>
#include <enum.hpp>
#include "move.hpp"

constexpr int STICK_TRIGGER = 10;

void macro_frentao() {
    Serial.println("Frentão");
    set_move(255, 255);
    vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
}

void macro_v(const direction lado) {
    if (lado == left) {
        Serial.println("left V");
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(40));
        set_move(255, 255);
        vTaskDelay(pdMS_TO_TICKS(150));
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(130));
        set_move(255, 255);
        vTaskDelay(pdMS_TO_TICKS(100));
        set_move(-10, -10);
        set_move(0, 0);
    } else {
        Serial.println("right V");
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(40));
        set_move(255, 255);
        vTaskDelay(pdMS_TO_TICKS(150));
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(130));
        set_move(255, 255);
        vTaskDelay(pdMS_TO_TICKS(100));
        set_move(-10, -10);
        set_move(0, 0);
    }
}

void macro_c(const direction lado) {
    if (lado == left) {
        Serial.println("left C");
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(50));
        set_move(255, 120);
        vTaskDelay(pdMS_TO_TICKS(120));
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(50));
        set_move(0, 0);
    } else {
        Serial.println("right C");
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(50));
        set_move(120, 255);
        vTaskDelay(pdMS_TO_TICKS(120));
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(50));
        set_move(0, 0);
    }
}

void macro_curvinha(const direction lado) {
    if (lado == left) {
        Serial.println("curvinha pra esquerda");
        vTaskDelay(pdMS_TO_TICKS(50));
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(70));
        set_move(255, 64);
        vTaskDelay(pdMS_TO_TICKS(200));
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(70));
        set_move(0, 0);
    } else {
        Serial.println("curvinha pra direita");
        vTaskDelay(pdMS_TO_TICKS(50));
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(70));
        set_move(64, 255);
        vTaskDelay(pdMS_TO_TICKS(200));
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(70));
        set_move(0, 0);
    }
}

void macro_curvao(const direction lado) {
    if (lado == left) {
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(90));
        set_move(255, 100);
        vTaskDelay(pdMS_TO_TICKS(392));
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(70));
        set_move(0, 0);
    } else {
        set_move(255, -255);
        vTaskDelay(pdMS_TO_TICKS(90));
        set_move(100, 255);
        vTaskDelay(pdMS_TO_TICKS(392));
        set_move(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(70));
        set_move(0, 0);
    }
}

void frente_padrao(int steer, int r2, int l2, double turnfactor){
    int forward = 0;
    if(r2 > STICK_TRIGGER){
        forward = map(r2, 10, 255, 0, 255);
    }
    else if(l2 > STICK_TRIGGER){
        forward = -map(l2, 10, 255, 0, 255);
    }
    
    int turn = 0;
    if(abs(steer) > STICK_TRIGGER){
        turn = (steer * turnfactor);
    }

    int velocidadeEsquerda = forward + turn;
    int velocidadeDireita   = forward - turn;

    set_move(velocidadeEsquerda, velocidadeDireita);
}

void curva(int turn, double turnfactor){
    set_move(turn * turnfactor, -turn * turnfactor);
}

#endif