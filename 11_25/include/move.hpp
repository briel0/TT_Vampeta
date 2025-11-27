#ifndef MOVE_H
#define MOVE_H

#include <Arduino.h>

#include "profile.hpp"

/*
talvez seja interessante colocar um portEnter_CRITICAL
nas funções de movimentação
quando estivermos trabalhando no auto
*/

inline int lastLS;
inline int lastRS;

void set_move(int leftSpeed, int rightSpeed);
void stop_motors();
void set_motors(int leftSpeed, int rightSpeed);
void brake_motors();

void set_move(int leftSpeed, int rightSpeed){

    if(leftSpeed == lastLS && rightSpeed == lastRS){
        return;
    }

    if((leftSpeed * lastLS) < 0 || (rightSpeed * lastRS) < 0){
        //se estiver trocando a polaridade (sentido) para o movimento anterior antes
        //testar para frenagem passiva caso dê errado
        brake_motors();
    }

    set_motors(leftSpeed, rightSpeed);

}

void stop_motors(){
    analogWrite(RIGHT_POS_PIN, 0);
    analogWrite(RIGHT_NEG_PIN, 0);
    analogWrite(LEFT_POS_PIN, 0);
    analogWrite(LEFT_NEG_PIN, 0);
}

void brake_motors(){
    stop_motors();
    delayMicroseconds(PWM_ZERO_DELAY);
}

void set_motors(int leftSpeed, int rightSpeed){

    /*
    com um exemplo feito no papel essa lógica é evidente
    confira com os pares de LS e RS: (1, 1), (1, -1), (-1, 1), (-1, -1)
    */

    analogWrite(LEFT_POS_PIN, max(leftSpeed, 0));
    analogWrite(LEFT_NEG_PIN, max(-leftSpeed, 0));

    analogWrite(RIGHT_POS_PIN, max(rightSpeed, 0));
    analogWrite(RIGHT_NEG_PIN, max(-rightSpeed, 0));
}

#endif