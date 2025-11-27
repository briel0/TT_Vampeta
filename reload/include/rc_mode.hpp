#include <PS4Controller.h> // Biblioteca do controle do PS4
#include <defines.hpp>     // Definicoes globais
#include <move.hpp>        // Funcoes de movivmentacao de motores

constexpr float coefAtenuacao = 1.8; // Atenuacao da velocidade em curvas (deve ser FLOAT)
constexpr float limiteCurva = 250.0; // Velocidade limite em curvas puras (deve ser FLOAT)

const int STICK_TRIGGER = 10;

volatile bool pauErguido = false;
int limiteVelocidade = 255;      // Limite de velocidade do motor

void macro_frentao() {
    Serial.println("Frentão");
    moverMotores(255, 255);
    vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
}

void levanta_pau() {
    Serial.println("levantando pau");

    digitalWrite(PAU_POS_PIN, pauErguido);
    digitalWrite(PAU_NEG_PIN, !pauErguido);

    vTaskDelay(pdMS_TO_TICKS(500)); // Evita multiplas leituras

    digitalWrite(PAU_POS_PIN, LOW);
    digitalWrite(PAU_NEG_PIN, LOW);

    pauErguido = (pauErguido ? 0 : 1);
}

void change_speed() {
    Serial.println("Trocando velocidade");
    limiteVelocidade = 255 ? 180 : 255;
    vTaskDelay(pdMS_TO_TICKS(200)); // Evita multiplas leituras
}

void macro_v(const direction lado) {
    if (lado == left) {
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
    } else {
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

void macro_c(const direction lado) {
    if (lado == left) {
        Serial.println("left C");
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(255, 120);
        vTaskDelay(pdMS_TO_TICKS(120));
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(0, 0);
    } else {
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

void macro_curvinha(const direction lado) {
    if (lado == left) {
        Serial.println("curvinha pra esquerda");
        vTaskDelay(pdMS_TO_TICKS(50));
        moverMotores(-255, 255); // Mantem a velocidade maxima
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(255, 64);
        vTaskDelay(pdMS_TO_TICKS(200));
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(70));
        moverMotores(0, 0);
    } else {
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

void macro_curvao(const direction lado) {
    if (!pauErguido) {
        digitalWrite(PAU_POS_PIN, pauErguido);
        digitalWrite(PAU_NEG_PIN, !pauErguido);
    }
    if (lado == left) {
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(90));
        moverMotores(255, 100);
        vTaskDelay(pdMS_TO_TICKS(392));
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(38));
        digitalWrite(PAU_POS_PIN, LOW);
        digitalWrite(PAU_NEG_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(32));
        moverMotores(0, 0);
    } else {
        moverMotores(255, -255);
        vTaskDelay(pdMS_TO_TICKS(90));
        moverMotores(100, 255);
        vTaskDelay(pdMS_TO_TICKS(392));
        moverMotores(-255, 255);
        vTaskDelay(pdMS_TO_TICKS(38));
        digitalWrite(PAU_POS_PIN, LOW);
        digitalWrite(PAU_NEG_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(32));
        moverMotores(0, 0);
    }
}

void frente_padrao() {

    int l2 = PS4.L2Value();
    int r2 = PS4.R2Value();
    int direcao = PS4.LStickX();
    int velocidadeEsquerda = 0, velocidadeDireita = 0;
    direcao *= -1;

    if (r2 > 10) { // Gatilho direito (frente)
        int velocidade = map(r2, 10, 255, 0, limiteVelocidade);
        velocidadeEsquerda = velocidade;
        velocidadeDireita = velocidade;
    }
    else if (l2 > 10) { // Gatilho esquerdo (re)
        int velocidade = map(l2, 10, 255, 0, limiteVelocidade);
        velocidadeEsquerda = -velocidade;
        velocidadeDireita = -velocidade;
    }

    if (direcao < -10) {
        if (r2 > 10) {
            velocidadeEsquerda *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
        }
        else {
            velocidadeDireita *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
        }
    }
    else if (direcao > 10) {
        if (r2 > 10) {
            velocidadeDireita *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
        }
        else {
            velocidadeEsquerda *= (1.0 - (abs(direcao) * coefAtenuacao) / 255.0);
        }
    }
    moverMotores(velocidadeEsquerda, velocidadeDireita);
}

void curva(){
    int direcao = PS4.LStickX();
    direcao *= -1;
    int velocidadeEsquerda = 0, velocidadeDireita = 0;
    velocidadeEsquerda = direcao * 1.5; 
    velocidadeDireita = -direcao * 1.5;
    moverMotores(velocidadeEsquerda, velocidadeDireita);
}

const char macAdress[][18] = {
    "3c:8a:1f:ad:d7:b8", // smoker (0)
    "5c:01:3b:73:fc:54", // briga (1)
    "78:1c:3c:f6:24:70", // fuego (2)
    "3c:8a:1f:ad:87:0c", // fueguito (3)
    "a0:b7:65:0f:7c:e0", // tsunami (4)
    "7c:9e:bd:fb:83:64"  // vampeta (5)
};

void modoRC() {

    if (PS4.begin(macAdress[5])) {
        Serial.println("Bluetooth inicializado, aguardando controle...");
        while (!PS4.isConnected()) {
            Serial.println("Aguardando conexao...");
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    Serial.println("Controle conectado!");

    while (PS4.isConnected()) {

        if (PS4.Cross()) {
            macro_frentao();
        }
        else if (PS4.Square()) {
            levanta_pau();
        }
        else if (PS4.Triangle()) {
            change_speed();
        }
        else if (PS4.R1() > 0) {
            macro_curvinha(right);
        }
        else if (PS4.L1() > 0) {
            macro_curvinha(left);
        }
        else if (PS4.Left() > 0) {
            macro_curvao(left);
        }
        else if (PS4.Right() > 0) {
            macro_curvao(right);
        }
        else {
            int direcao = PS4.LStickX();
            direcao *= -1;

            if(PS4.R2() > 10 || PS4.L2() > 10){
                frente_padrao();
            }
            else if(direcao < -STICK_TRIGGER || direcao > STICK_TRIGGER){
                curva();
            }
          
        }
    }
}