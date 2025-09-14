//===============================================================================================//
//=====================================//INCLUDES E DEFINES//====================================//
//===============================================================================================//

#pragma region INCLUDES

#ifndef MOVE_H
#define MOVE_H

#include <defines.hpp>                        // Definicoes globais
#include <functions.hpp>                      // Funcoes auxiliares

#pragma endregion

//===============================================================================================//
//========================================//MOVIMENTACAO//=======================================//
//===============================================================================================//

#pragma region MOVIMENTACAO

//=======================================//Mover Motores//=======================================//

#pragma region SINAL PWM

// Define o duty cycle (velocidade) para os motores esquerdo e direito
void setMotors(int velocidadeEsquerdaMotor, int velocidadeDireitaMotor) {

    portENTER_CRITICAL(&motorMux);

    // Logica para o Motor Esquerdo
    if (velocidadeEsquerdaMotor >= 0) {
        // Gira para frente: PWM no pino POS, pino NEG em 0
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_POS_CHANNEL, velocidadeEsquerdaMotor);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_NEG_CHANNEL, 0);
    } else {                                  // Inverte o valor para ser positivo
        // Gira para tras: pino POS em 0, PWM no pino NEG
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_POS_CHANNEL, 0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_NEG_CHANNEL, -velocidadeEsquerdaMotor);
    }

    // Logica para o Motor Direito
    if (velocidadeDireitaMotor >= 0) {
        // Gira para frente: PWM no pino POS, pino NEG em 0
        ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_POS_CHANNEL, velocidadeDireitaMotor);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_NEG_CHANNEL, 0);
    } else {                                  // Inverte o valor para ser positivo
        // Gira para tras: pino POS em 0, PWM no pino NEG
        ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_POS_CHANNEL, 0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_NEG_CHANNEL, -velocidadeDireitaMotor);
    }

    // Aplica as alteracoes de duty cycle em todos os canais
    // Isso garante que as mudancas ocorram de forma sincrona
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEFT_POS_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEFT_NEG_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, RIGHT_POS_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, RIGHT_NEG_CHANNEL);

    portEXIT_CRITICAL(&motorMux);

    // Envia informacoes da movimentacao para debug via Bluetooth e Serial
    SerialBT.printf("MOTOR -> D: %d E: %d\n", novaVD, novaVE);
    Serial.printf("MOTOR -> D: %d E: %d\n", novaVD, novaVE);
}

#pragma endregion

//=======================================//Parar Motores//=======================================//

#pragma region PARAR PWM

// Para os motores zerando o PWM e depois os pinos
void stopMotors() {
    portENTER_CRITICAL(&motorMux);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_POS_CHANNEL, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_NEG_CHANNEL, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_POS_CHANNEL, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_NEG_CHANNEL, 0);

    ledc_update_duty(LEDC_LOW_SPEED_MODE, RIGHT_POS_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, RIGHT_NEG_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEFT_POS_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEFT_NEG_CHANNEL);

    portEXIT_CRITICAL(&motorMux);

    delayUs(2 * PWM_ZERO_DELAY);              // Pequeno delay para garantir que os PWMs zerem
}

void brakeMotors() {
    portENTER_CRITICAL(&motorMux);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_POS_CHANNEL, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, RIGHT_NEG_CHANNEL, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_POS_CHANNEL, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEFT_NEG_CHANNEL, 0);

    ledc_update_duty(LEDC_LOW_SPEED_MODE, RIGHT_POS_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, RIGHT_NEG_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEFT_POS_CHANNEL);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEFT_NEG_CHANNEL);

    portEXIT_CRITICAL(&motorMux);

    vTaskDelay(pdMS_TO_TICKS(1)); 
}

#pragma endregion

//==============================//Processar Movimento dos Motores//==============================//

#pragma region PROCESSAR COMANDO

void processarMovimento(int velocidadeEsquerdaMotor, int velocidadeDireitaMotor) {
    static int ultimaVE = 0;
    static int ultimaVD = 0;

    // Caso (0, 0): aplica freio ativo
    if (velocidadeEsquerdaMotor == 0 && velocidadeDireitaMotor == 0) {
        brakeMotors();
        SerialBT.printf("FREIO -> D: %d E: %d\n", velocidadeDireitaMotor, velocidadeEsquerdaMotor);
        Serial.printf("FREIO -> D: %d E: %d\n", velocidadeDireitaMotor, velocidadeEsquerdaMotor);

    // Caso de troca de polaridade: zera PWM antes de inverter
    } else if (
        (ultimaVE > 0 && velocidadeEsquerdaMotor < 0) || 
        (ultimaVE < 0 && velocidadeEsquerdaMotor > 0) ||
        (ultimaVD > 0 && velocidadeDireitaMotor < 0) || 
        (ultimaVD < 0 && velocidadeDireitaMotor > 0)
    ) {
        stopMotors();
        setMotors(velocidadeEsquerdaMotor, velocidadeDireitaMotor);

    // Caso normal (continuidade de movimento): mantem fluidez
    } else {
        setMotors(velocidadeEsquerdaMotor, velocidadeDireitaMotor);
    }

    // Atualiza os valores das ultimas velocidades
    ultimaVE = velocidadeEsquerdaMotor;
    ultimaVD = velocidadeDireitaMotor;
}

#pragma endregion

//=======================================//Funcao Chamada//======================================//

#pragma region FUNCAO GLOBAL

void movePau(bool indo) {

    int velocidadePau = 255;

    if(indo){
        digitalWrite(23, HIGH);
        digitalWrite(5, LOW);
    }
    else{
        digitalWrite(23, LOW);
        digitalWrite(5, HIGH);
    }


}

void moverMotores(int velocidadeEsquerdaMotor, int velocidadeDireitaMotor) {
    static int ultimaVE = 0;
    static int ultimaVD = 0;

    // So notifica a processarMovimento se a velocidade realmente mudou
    if (velocidadeEsquerdaMotor != ultimaVE || 
        velocidadeDireitaMotor != ultimaVD) {
        
        novaVE = velocidadeEsquerdaMotor;
        novaVD = velocidadeDireitaMotor;

        processarMovimento(novaVE, novaVD);
    }

    // Atualiza os valores das ultimas velocidades
    ultimaVE = velocidadeEsquerdaMotor;
    ultimaVD = velocidadeDireitaMotor;
}

#pragma endregion

//=========================================//Servomotor//========================================//

#pragma region SERVOMOTOR

// Converte um angulo (0-180) para o valor de duty cycle correspondente
uint32_t angleToDuty(int angle) {
    long pulse_width_us = map(angle, 0, 180, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
    long period_us = 1000000 / SERVO_FREQ_HZ;
    uint32_t duty = (uint32_t)(((double)pulse_width_us / 
        (double)period_us) * (double)(1 << SERVO_RESOLUTION_BITS));
    return duty;
}

void openServo(void *pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, SERVO_LEDC_CHANNEL, angleToDuty(120));
        ledc_update_duty(LEDC_LOW_SPEED_MODE, SERVO_LEDC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(150));
        ledc_set_duty(LEDC_LOW_SPEED_MODE, SERVO_LEDC_CHANNEL, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, SERVO_LEDC_CHANNEL);
        Serial.println("//=====//SERVO ABERTO//=====//");
    }
}

void closeServo(void *pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, SERVO_LEDC_CHANNEL, angleToDuty(0));
        ledc_update_duty(LEDC_LOW_SPEED_MODE, SERVO_LEDC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(150));
        Serial.println("//=====//SERVO FECHADO//=====//");
    }
}

#pragma endregion
#pragma endregion

//===============================================================================================//
//===========================================//SETUP//===========================================//
//===============================================================================================//

//=====================================//Setup dos motores//=====================================//

#pragma region SETUP MOTORES

void setupLEDC() {
    // Configurar o Timer do LEDC (um timer pode servir para varios canais)
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,    // Modo de velocidade baixa (para PWM comum)
        .duty_resolution = (ledc_timer_bit_t)PWM_RESOLUTION,  // Resolucao do duty cycle (8 bits)
        .timer_num = LEDC_TIMER_3,            // Usa o Timer 3 do LEDC
        .freq_hz = PWM_FREQ,                  // Frequencia do PWM em Hz
        .clk_cfg = LEDC_AUTO_CLK              // Configuracao automatica do clock
    };
    ledc_timer_config(&ledc_timer);           // Aplica a configuracao do timer

    // Configurar cada pino/canal individualmente
    // Array com as configuracoes para simplificar
    ledc_channel_config_t ledc_channels[6] = {
        { .gpio_num = RIGHT_POS_PIN,          // Canal para motor direito, sentido positivo
          .speed_mode = LEDC_LOW_SPEED_MODE, 
          .channel = RIGHT_POS_CHANNEL, 
          .timer_sel = LEDC_TIMER_3, 
          .duty = 0, 
          .hpoint = 0 },

        { .gpio_num = RIGHT_NEG_PIN,          // Canal para motor direito, sentido negativo
          .speed_mode = LEDC_LOW_SPEED_MODE, 
          .channel = RIGHT_NEG_CHANNEL, 
          .timer_sel = LEDC_TIMER_3, 
          .duty = 0, 
          .hpoint = 0 },

        { .gpio_num = LEFT_POS_PIN,           // Canal para motor esquerdo, sentido positivo
          .speed_mode = LEDC_LOW_SPEED_MODE, 
          .channel = LEFT_POS_CHANNEL,  
          .timer_sel = LEDC_TIMER_3, 
          .duty = 0,
          .hpoint = 0 },
          
        { .gpio_num = LEFT_NEG_PIN,           // Canal para motor esquerdo, sentido negativo
          .speed_mode = LEDC_LOW_SPEED_MODE, 
          .channel = LEFT_NEG_CHANNEL,  
          .timer_sel = LEDC_TIMER_3, 
          .duty = 0, 
          .hpoint = 0 },

        { .gpio_num = PAU_NEG_PIN,           // Canal para motor esquerdo, sentido negativo
          .speed_mode = LEDC_LOW_SPEED_MODE, 
          .channel = PAU_NEG_CHANNEL,  
          .timer_sel = LEDC_TIMER_3, 
          .duty = 0, 
          .hpoint = 0 },

        { .gpio_num = PAU_POS_PIN,           // Canal para motor esquerdo, sentido negativo
          .speed_mode = LEDC_LOW_SPEED_MODE, 
          .channel = PAU_POS_CHANNEL,  
          .timer_sel = LEDC_TIMER_3, 
          .duty = 0, 
          .hpoint = 0 }

    };
    
    // Aplica a configuracao de cada canal
    for (int i = 0; i < 6; i++) {
        ledc_channel_config(&ledc_channels[i]);
    }
}

#pragma endregion

//===================================//Setup dos servomotores//==================================//

#pragma region SETUP SERVOMOTOR

void setupLEDC_Servo() {
    // Configurar o Timer do LEDC (um timer pode servir para varios canais)
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,    // Modo de velocidade baixa (para PWM comum)
        .duty_resolution = (ledc_timer_bit_t)SERVO_RESOLUTION_BITS,  // Resolucao do duty cycle (8 bits)
        .timer_num = SERVO_TIMER,            // Usa o Timer 3 do LEDC
        .freq_hz = SERVO_FREQ_HZ,                  // Frequencia do PWM em Hz
        .clk_cfg = LEDC_AUTO_CLK              // Configuracao automatica do clock
    };
    ledc_timer_config(&ledc_timer);           // Aplica a configuracao do timer

    ledc_channel_config_t canal_servo = {
        .gpio_num   = SERVOMOTOR_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = SERVO_LEDC_CHANNEL,
        .timer_sel  = SERVO_TIMER,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&canal_servo);
}

#pragma endregion

//======================================//Task de alocacao//=====================================//

#pragma region TASK ALOCACAO

void setupMoveTask() {

    setupLEDC();
    setupLEDC_Servo();

    xTaskCreatePinnedToCore(
        openServo,                            // Funcao da tarefa
        "OpenServo",                          // Nome da tarefa
        256 * 16,                             // Tamanho da pilha
        nullptr,                              // Parametros
        15,                                   // Prioridade 15
        &openServoHandle,                     // Handle
        PRO_CPU_NUM                           // Nucleo 0 onde a tarefa sera executada
    );

    xTaskCreatePinnedToCore(
        closeServo,                           // Funcao da tarefa
        "CloseServo",                         // Nome da tarefa
        256 * 16,                             // Tamanho da pilha
        nullptr,                              // Parametros
        15,                                   // Prioridade 15
        &closeServoHandle,                    // Handle
        PRO_CPU_NUM                           // Nucleo 0 onde a tarefa sera executada
    );
}

#pragma endregion

//===============================================================================================//
//=====================================//FINALIZA O ARQUIVO//====================================//
//===============================================================================================//

#endif