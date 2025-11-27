#include <PS4Controller.h> // Biblioteca do controle do PS4
#include <defines.hpp>   // Definicoes globais
#include <move.hpp>      // Funcoes de movivmentacao de motores

constexpr float coefAtenuacao = 1.8; // Atenuacao da velocidade em curvas (deve ser FLOAT)
constexpr float limiteCurva = 250.0; // Velocidade limite em curvas puras (deve ser FLOAT)

volatile bool switchCxV = false;  // Switch definido para controlar MACRO usado
volatile bool velLimitada = false; // Switch definido pra limitar a velocidade do motor
int limiteVelocidade = 255;       // Limite de velocidade do motor

const char macAdress[][18] = {
    "3c:8a:1f:ad:d7:b8", //smoker (0)
    "5c:01:3b:73:fc:54", //briga (1)
    "78:1c:3c:f6:24:70", //fuego (2)
    "3c:8a:1f:ad:87:0c", //fueguito (3)
    "a0:b7:65:0f:7c:e0", //tsunami (4)
    "7c:9e:bd:fb:83:64" //vampeta (5)
};

void macro_frentao(){
    Serial.println("Frentão");
    moverMotores(255, 255);
    vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
}

void levanta_pau(){
    Serial.println("levantando pau");

    digitalWrite(PAU_POS_PIN, pauErguido);
    digitalWrite(PAU_NEG_PIN, !pauErguido);

    vTaskDelay(pdMS_TO_TICKS(500)); // Evita multiplas leituras

    digitalWrite(PAU_POS_PIN, LOW);
    digitalWrite(PAU_NEG_PIN, LOW);

    pauErguido = (pauErguido ? 0 : 1);

}

void change_speed(){
    Serial.println("Trocando velocidade");
    if(!velLimitada){
        velLimitada = true;
        limiteVelocidade = 180; // Define o limite reduzido
    }
    else{
        velLimitada = false;
        limiteVelocidade = 255; // Define o limite reduzido
    }
    vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
}

void macro_v(const Direction lado){
    if(lado == esquerda){
        Serial.println("left V");
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
    else{
        Serial.println("right V");
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
    }
}

void macro_c(const Direction lado){
    if(lado == esquerda){
        Serial.println("left C");
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(255, 120);
        vTaskDelay(pdMS_TO_TICKS(120));
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(0, 0);
    }
    else{
        Serial.println("right C");
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(120, 255);
        vTaskDelay(pdMS_TO_TICKS(120));
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(0, 0);
    }
}

void macro_curvinha(const Direction lado){
    if(lado == esquerda){
        Serial.println("curvinha pra esquerda");
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(-255, 255); // Mantem a velocidade maxima
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(255, 64);
        vTaskDelay(pdMS_TO_TICKS(200));
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(0, 0);
    }
    else{
        Serial.println("curvinha pra direita");
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(64, 255);
        vTaskDelay(pdMS_TO_TICKS(200));
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(0, 0);
    }
}

void macro_curvao(const Direction lado){
    if(!pauErguido){
        digitalWrite(PAU_POS_PIN, pauErguido);
        digitalWrite(PAU_NEG_PIN, !pauErguido);
    }
    if(lado == esquerda){
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(255, 64);
        vTaskDelay(pdMS_TO_TICKS(392));
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(38));
        digitalWrite(PAU_POS_PIN, LOW);
        digitalWrite(PAU_NEG_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(32));
        moverMotores(0, 0);
    }
    else{
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(64, 255);
        vTaskDelay(pdMS_TO_TICKS(392));
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(0, 0);
    }
}

void modoRC(){

    if(PS4.begin(macAdress[5])){
        Serial.println("Bluetooth inicializado, aguardando controle...");
        while(!PS4.isConnected()){
            Serial.println("Aguardando conexao...");
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    Serial.println("Controle conectado!");

    while (PS4.isConnected()){

        int velocidadeEsquerda = 0;
        int velocidadeDireita = 0;

        if(PS4.Cross()){
            macro_frentao();
        }
        else if(PS4.Square()){
            levanta_pau();
        }
        else if(PS4.Circle()){ //troca forma da curva
            Serial.println(switchCxV ? "switch V to C" : "switch C to V");
            vTaskDelay(pdMS_TO_TICKS(50));
            switchCxV = (switchCxV ? false : true);
            vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
        }
        else if(PS4.Triangle()){
            change_speed();
        }
        else if(PS4.R1() > 0){
            macro_curvinha(direita);
        }
        else if(PS4.L1() > 0){
            macro_curvinha(esquerda);
        }
        else if(PS4.Left() > 0){
            switchCxV ? macro_v(esquerda) : macro_c(esquerda);
        }
        else if(PS4.Right() > 0){
            switchCxV ? macro_v(direita) : macro_c(direita);
        }
        else{
            int l2 = PS4.L2Value();
            int r2 = PS4.R2Value();
            int direcao = PS4.LStickX();
            direcao *= -1;

            // Se nenhum gatilho estiver pressionado (considerando a zona morta)
            if (r2 <= 10 && l2 <= 10)
            {
                velocidadeEsquerda = 0;
                velocidadeDireita = 0;

                // Se os gatilhos estao parados, verifica se o analogico quer girar
                if (direcao < -10)
                { // Curva pra esquerda parado
                    velocidadeEsquerda = direcao * 1.5;
                    velocidadeDireita = -direcao * 1.5;
                }
                else if (direcao > 10)
                { // Curva pra direita parado
                    velocidadeEsquerda = direcao * 1.5;
                    velocidadeDireita = -direcao * 1.5;
                }
                Serial.println(velocidadeEsquerda);
                Serial.println(velocidadeDireita);
                // Se nem gatilho nem analogico estiverem ativos, as velocidades continuam 0.
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
                    velocidadeDireita = -velocidade;
                }

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