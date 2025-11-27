#ifndef PROFILE_H
#define PROFILE_H

const char* macAdress = "88:13:bf:68:c4:70";

constexpr int IR_PIN = 13;

constexpr int RIGHT_POS_PIN = 19;
constexpr int RIGHT_NEG_PIN = 18;
constexpr int LEFT_POS_PIN = 17;
constexpr int LEFT_NEG_PIN = 16;

constexpr double turnfactor = 1.5;

constexpr int PWM_ZERO_DELAY = 30; //Tempo para Ponte H limpar PWM (µs)

#endif