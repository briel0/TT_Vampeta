//#ifndef RC_MODE_H
//#define RC_MODE_H 

#include <PS4Controller.h> // Biblioteca do controle do PS4
#include <defines.hpp>   // Definicoes globais
#include <functions.hpp> // Funcoes auxiliares
#include <move.hpp>      // Funcoes de movivmentacao de motores

constexpr float coefAtenuacao = 1.8; // Atenuacao da velocidade em curvas (deve ser FLOAT)
constexpr float coefReverse = 0.9;   // Coeficiente para balancear a re (deve ser FLOAT)
constexpr float limiteCurva = 127.0; // Velocidade limite em curvas puras (deve ser FLOAT)

volatile bool switchCxV = false;  // Switch definido para controlar MACRO usado
volatile bool velLimitada = true; // Switch definido pra limitar a velocidade do motor
int limiteVelocidade = 180;       // Limite de velocidade do motor
volatile int r2 = 0;              // Valor do gatilho direito do PS4 (R2)
volatile int l2 = 0;              // Valor do gatilho esquerdo do PS4 (L2)
volatile int direcao = 0;         // Valor do direcional esquerdo do PS4 em X (LStickX)

const char macAdress[][18] = {
    "3c:8a:1f:ad:d7:b8", //smoker (0)
    "5c:01:3b:73:fc:54", //briga (1)
    "78:1c:3c:f6:24:70", //fuego (2)
    "3c:8a:1f:ad:87:0c", //fueguito (3)
    "a0:b7:65:0f:7c:e0" //tsunami (4)
};

void modoRC(){

    CRGB cores[5] = {
        CRGB(0, 200, 0), CRGB(0, 200, 0), CRGB(0, 200, 0),CRGB(0, 200, 0), CRGB(0, 200, 0)
    };

    // Tenta conectar ao controle PS4 e debuga conexao pelo Serial Monitor
    if(PS4.begin(macAdress[2])){
        Serial.println("Bluetooth inicializado, aguardando controle...");
        while(!PS4.isConnected()){
            setLeds(cores, 5);
            Serial.println("Aguardando conexao...");
            vTaskDelay(pdMS_TO_TICKS(500)); // Pequeno atraso
            clearLeds();
        }
    }

    Serial.println("Controle conectado!");
    PS4.setLed(0, 200, 0); // Define a cor do LED para verde
    setLeds(cores, 5);

    while (PS4.isConnected()){

        int velocidadeEsquerda = 0; // Velocidade do motor esquerdo
        int velocidadeDireita = 0;  // Velocidade do motor direito

        if(PS4.Cross()){
            moverMotores(255, 255);
            //continue
            //por que nao tem delay aqui?
        }
        else if(PS4.Square()){

            vTaskDelay(pdMS_TO_TICKS(50));

            if(pauErguido){
                Serial.println("levantando pau");
            }
            else{
                Serial.println("abaixando pau");
            }

            movePau(pauErguido);
            pauErguido ^= 1;
            vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
        }
        if(PS4.Circle()){ //troca modo de alguma coisa
            vTaskDelay(pdMS_TO_TICKS(50));
            switchCxV ^= 1; //inverte
            vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
        }
#pragma region LIMITADOR
        else if(PS4.Triangle() && !velLimitada){
            velLimitada = true;
            limiteVelocidade = 180;         // Define o limite reduzido
            vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
        }
        else if (PS4.Triangle()){
            velLimitada = false;
            limiteVelocidade = 255;         // Restaura o limite maximo
            vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras

#pragma endregion

            //===============================================================================================//
            //==========================================//MACROS//===========================================//
            //===============================================================================================//

            //=========================================//Curvinhas//=========================================//

#pragma region MACRO CURVINHAS
        }
        else if (PS4.R1() > 0)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            moverMotores(255, -255); // Mantem a velocidade maxima
            vTaskDelay(pdMS_TO_TICKS(70));
            moverMotores(64, 255);
            vTaskDelay(pdMS_TO_TICKS(200));
            moverMotores(-255, 255);
            vTaskDelay(pdMS_TO_TICKS(70));
            moverMotores(0, 0);
        }
        else if (PS4.L1() > 0)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            moverMotores(-255, 255); // Mantem a velocidade maxima
            vTaskDelay(pdMS_TO_TICKS(70));
            moverMotores(255, 64);
            vTaskDelay(pdMS_TO_TICKS(200));
            moverMotores(255, -255);
            vTaskDelay(pdMS_TO_TICKS(70));
            moverMotores(0, 0);

#pragma endregion

            //========================================//Macros em V//========================================//

#pragma region MACRO EM V
        }
        else if (PS4.Left() > 0 && switchCxV)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            xTaskNotifyGive(openServoHandle);
            moverMotores(-255, 255);
            vTaskDelay(pdMS_TO_TICKS(40));
            moverMotores(255, 255);
            vTaskDelay(pdMS_TO_TICKS(150));
            moverMotores(255, -255);
            vTaskDelay(pdMS_TO_TICKS(130));
            moverMotores(255, 255);
            vTaskDelay(pdMS_TO_TICKS(100));
            moverMotores(-10, -10);
            moverMotores(0, 0);
        }
        else if (PS4.Right() > 0 && switchCxV)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            xTaskNotifyGive(openServoHandle);
            moverMotores(255, -255);
            vTaskDelay(pdMS_TO_TICKS(40));
            moverMotores(255, 255);
            vTaskDelay(pdMS_TO_TICKS(150));
            moverMotores(-255, 255);
            vTaskDelay(pdMS_TO_TICKS(130));
            moverMotores(255, 255);
            vTaskDelay(pdMS_TO_TICKS(100));
            moverMotores(-10, -10);
            moverMotores(0, 0);

#pragma endregion

            //========================================//Macros em C//========================================//

#pragma region MACRO EM C
        }
        else if (PS4.Left() > 0 && !switchCxV)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            xTaskNotifyGive(openServoHandle);
            moverMotores(-255, 255);
            vTaskDelay(pdMS_TO_TICKS(50));
            moverMotores(255, 120);
            vTaskDelay(pdMS_TO_TICKS(120));
            moverMotores(255, -255);
            vTaskDelay(pdMS_TO_TICKS(50));
            moverMotores(0, 0);
        }
        else if (PS4.Right() > 0 && !switchCxV)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            xTaskNotifyGive(openServoHandle);
            moverMotores(255, -255);
            vTaskDelay(pdMS_TO_TICKS(50));
            moverMotores(120, 255);
            vTaskDelay(pdMS_TO_TICKS(120));
            moverMotores(-255, 255);
            vTaskDelay(pdMS_TO_TICKS(50));
            moverMotores(0, 0);

#pragma endregion

            //===============================================================================================//
            //=====================================//MOVIMENTACAO LIVRE//====================================//
            //===============================================================================================//

            //========================================//Curvas Puras//=======================================//

#pragma region CURVAS PURAS
        }
        else
        {
            r2 = PS4.R2Value();
            l2 = PS4.L2Value();
            direcao = PS4.LStickX();

            // Se nenhum gatilho estiver pressionado (considerando a zona morta)
            if (r2 <= 10 && l2 <= 10)
            {
                velocidadeEsquerda = 0;
                velocidadeDireita = 0;

                // Se os gatilhos estao parados, verifica se o analogico quer girar
                if (direcao < -10)
                { // Curva pra esquerda parado
                    velocidadeEsquerda = -int(limiteCurva * (abs(direcao) / limiteCurva));
                    velocidadeDireita = int(limiteCurva * (abs(direcao) / limiteCurva));
                }
                else if (direcao > 10)
                { // Curva pra direita parado
                    velocidadeEsquerda = int(limiteCurva * (abs(direcao) / limiteCurva));
                    velocidadeDireita = -int(limiteCurva * (abs(direcao) / limiteCurva));
                }
                // Se nem gatilho nem analogico estiverem ativos, as velocidades continuam 0.

#pragma endregion

                //=======================================//Gatilhos Puros//======================================//

#pragma region GATILHOS PUROS
            }
            else
            { // Pelo menos um gatilho pressionado

                // Define a velocidade base (frente ou re)
                if (r2 > 10)
                { // Gatilho direito (frente)
                    int velocidade = map(r2, 10, 255, 0, limiteVelocidade);
                    velocidadeEsquerda = velocidade;
                    velocidadeDireita = velocidade;
                }
                else
                { // Gatilho esquerdo (re)
                    int velocidade = map(l2, 10, 255, 0, limiteVelocidade);
                    velocidadeEsquerda = -velocidade;
                    velocidadeDireita = -int(coefReverse * velocidade);
                }

#pragma endregion

                //=====================================//Gatilhos e Curvas//=====================================//

#pragma region GATILHOS E CURVAS

                // Aplica a curva sobre a velocidade existente
                if (direcao < -10)
                { // Curva pra esquerda em movimento
                    // Reduz a velocidade da roda interna (esquerda para frente, direita para re)
                    if (r2 > 10)
                        velocidadeEsquerda *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                    else
                        velocidadeDireita *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                }
                else if (direcao > 10)
                { // Curva pra direita em movimento
                    // Reduz a velocidade da roda interna (direita para frente, esquerda para re)
                    if (r2 > 10)
                        velocidadeDireita *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                    else
                        velocidadeEsquerda *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
                }
            }
            moverMotores(velocidadeEsquerda, velocidadeDireita);
        }
    }
}