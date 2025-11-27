#pragma region INCLUDES

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
        analogWrite(LEFT_POS_PIN, velocidadeEsquerdaMotor);
        analogWrite(LEFT_NEG_PIN, 0);
    } else {                                  // Inverte o valor para ser positivo
        // Gira para tras: pino POS em 0, PWM no pino NEG
        analogWrite(LEFT_POS_PIN, 0);
        analogWrite(LEFT_NEG_PIN, -velocidadeEsquerdaMotor);
    }

    // Logica para o Motor Direito
    if (velocidadeDireitaMotor >= 0) {
        // Gira para frente: PWM no pino POS, pino NEG em 0
        analogWrite(RIGHT_POS_PIN, velocidadeDireitaMotor);
        analogWrite(RIGHT_NEG_PIN, 0);
    } else {                                  // Inverte o valor para ser positivo
        // Gira para tras: pino POS em 0, PWM no pino NEG
        analogWrite(RIGHT_POS_PIN, 0);
        analogWrite(RIGHT_NEG_PIN, -velocidadeDireitaMotor);
    }

    // Aplica as alteracoes de duty cycle em todos os canais
    // Isso garante que as mudancas ocorram de forma sincrona
    portEXIT_CRITICAL(&motorMux);

    // Envia informacoes da movimentacao para debug via Bluetooth e Serial
    //SerialBT.printf("MOTOR -> D: %d E: %d\n", novaVD, novaVE);
    //Serial.printf("MOTOR -> D: %d E: %d\n", novaVD, novaVE);
}

#pragma endregion

//=======================================//Parar Motores//=======================================//

#pragma region PARAR PWM

// Para os motores zerando o PWM e depois os pinos
void stopMotors() {
    portENTER_CRITICAL(&motorMux);

    analogWrite(RIGHT_POS_PIN, 0);
    analogWrite(RIGHT_NEG_PIN, 0);
    analogWrite(LEFT_POS_PIN, 0);
    analogWrite(LEFT_NEG_PIN, 0);

    portEXIT_CRITICAL(&motorMux);

    delayUs(2 * PWM_ZERO_DELAY);              // Pequeno delay para garantir que os PWMs zerem
}

void brakeMotors() {
    portENTER_CRITICAL(&motorMux);

    analogWrite(RIGHT_POS_PIN, 0);
    analogWrite(RIGHT_NEG_PIN, 0);
    analogWrite(LEFT_POS_PIN, 0);
    analogWrite(LEFT_NEG_PIN, 0);

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
        //SerialBT.printf("FREIO -> D: %d E: %d\n", velocidadeDireitaMotor, velocidadeEsquerdaMotor);
        //Serial.printf("FREIO -> D: %d E: %d\n", velocidadeDireitaMotor, velocidadeEsquerdaMotor);

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

void moverMotores(int velocidadeEsquerdaMotor, int velocidadeDireitaMotor) {
    static int ultimaVE = 0;
    static int ultimaVD = 0;

    // So notifica a processarMovimento se a velocidade realmente mudou
    if(velocidadeEsquerdaMotor != ultimaVE || velocidadeDireitaMotor != ultimaVD){  
        novaVE = velocidadeEsquerdaMotor;
        novaVD = velocidadeDireitaMotor;
        processarMovimento(novaVE, novaVD);
    }

    // Atualiza os valores das ultimas velocidades
    ultimaVE = velocidadeEsquerdaMotor;
    ultimaVD = velocidadeDireitaMotor;
}