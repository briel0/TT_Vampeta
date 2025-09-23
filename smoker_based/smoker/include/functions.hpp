//===============================================================================================//
//=====================================//INCLUDES E DEFINES//====================================//
//===============================================================================================//

#pragma region INCLUDES

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <defines.hpp>                        // Definicoes globais

#pragma endregion

//===============================================================================================//
//===========================================//DELAY//===========================================//
//===============================================================================================//

#pragma region DELAYS

// Funcao para o robo realizar um delay em µs sem bloquear o processamento do ESP32
void delayUs(uint32_t us) {
    uint64_t start = esp_timer_get_time();    // Obtem o tempo atual em µs
    while ((esp_timer_get_time() - start) < us) {
        taskYIELD();                          // Libera tempo de CPU para outras tarefas FreeRTOS
    }
}

// Funcao para o robo realizar um delay em ms, mas quebra-lo assim que detectar um adversario
bool delayMsSensor(unsigned long delayTime) {
    unsigned long startTime = millis();
    seeing = false;
    while (millis() - startTime < delayTime) {
        if (valueJsumoF == 1 || valueJsumoE == 1 || valueJsumoD == 1) {
            seeing = true;
            return true;                           // Interrompe se detectar adversario
        }
    }
    return false;
}

bool delayMsAttacked(unsigned long delayTime) {
    unsigned long startTime = millis();
    seeing = false;
    while (millis() - startTime < delayTime) {
        if (valueJsumoF + valueJsumoE + valueJsumoD >= 2) {
            seeing = true;
            modoLuta = ataque;
            return true;                      // Interrompe se adversario atacar
        }
    }
    return false;
}

#pragma endregion

//===============================================================================================//
//=======================================//SINAL DIGITAL//=======================================//
//===============================================================================================//

#pragma region SINAL DIGITAL

// Funcao para definir um pino como HIGH
void directWriteHigh(int pin) {
    if (pin < 32)
        GPIO.out_w1ts = ((uint32_t)1 << pin);
    else if (pin < 64)
        GPIO.out1_w1ts.val = ((uint32_t)1 << (pin - 32));
}

// Funcao para definir um pino como LOW
void directWriteLow(int pin) {
    if (pin < 32)
        GPIO.out_w1tc = ((uint32_t)1 << pin);
    else if (pin < 64)
        GPIO.out1_w1tc.val = ((uint32_t)1 << (pin - 32));
}

#pragma endregion

//===============================================================================================//
//======================================//CURVA DE BEZIER//======================================//
//===============================================================================================//

#pragma region CURVA DE BEZIER

float curvaBezierCubic(float t, float p0, float p1, float p2, float p3) {
    float u = 1.0 - t;
    return u*u*u*p0 + 3*u*u*t*p1 + 3*u*t*t*p2 + t*t*t*p3;
}

#pragma endregion

//===============================================================================================//
//===========================================//LEDs//============================================//
//===============================================================================================//

#pragma region LEDS

// Pisca o LED embutido um certo numero de vezes com um delay
void blinkLED(int times, int delayTime) {
    for (int i = 0; i < times; i++) {
        directWriteLow(LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(delayTime));
        directWriteHigh(LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(delayTime));
    }
    directWriteLow(LED_PIN);
}

// Configura os LEDs enderecaveis com o tipo de chip (WS2812), pino e ordem de cores
void setupLeds() {
    FastLED.addLeds<WS2812,LEDS_ENDERECAVEIS_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.show();
}

// Define todos os n LEDs com seus respectivos valores CRGB
void setLeds(const CRGB colors[], int n) {
    for (int i = 0; i < n; i++) {
        leds[i] = colors[i];
    }
    FastLED.show();
}

// Modo para quando enxerga o robo adversario
void AnnihilationModeLeds(){
    CRGB cores[5] = {
        CRGB(200, 0, 0),
        CRGB(200, 0, 0),
        CRGB(200, 0, 0),
        CRGB(200, 0, 0),
        CRGB(200, 0, 0)
    };

    setLeds(cores, 5);
}

// Apaga todos os LEDs
void clearLeds() {
    FastLED.clear();
    FastLED.show();
}

// Funcao para indicar o status do setup com LEDs verdes (feito) ou vermelhos (pendente)
void validaSetup(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t s4) {
    // Verde se 1, vermelho se 0
    leds[0] = s0 ? CRGB(0, 200, 0) : CRGB(200, 0, 0);
    leds[1] = s1 ? CRGB(0, 200, 0) : CRGB(200, 0, 0);
    leds[2] = s2 ? CRGB(0, 200, 0) : CRGB(200, 0, 0);
    leds[3] = s3 ? CRGB(0, 200, 0) : CRGB(200, 0, 0);
    leds[4] = s4 ? CRGB(0, 200, 0) : CRGB(200, 0, 0);

    FastLED.show();
}

// Funcao para indicar o status dos sensores JSumo (esquerda, frente, direita) usando LEDs
void indicarSensores(bool esquerda, bool frente, bool direita) {
    leds[0] = CRGB::Black;                    // LED 1 apagado
    leds[4] = CRGB::Black;                    // LED 5 apagado

    // Decide entre roxo (128, 0, 128) se 1 ou laranja (200, 128, 0) se 0
    leds[1] = esquerda ? CRGB(128, 0, 128) : CRGB(200, 128, 0);  
    leds[2] = frente   ? CRGB(128, 0, 128) : CRGB(200, 128, 0);  
    leds[3] = direita  ? CRGB(128, 0, 128) : CRGB(200, 128, 0); 

    FastLED.show();
}

#pragma endregion
#pragma endregion

//===============================================================================================//
//========================================//VALIDAR AUTO//=======================================//
//===============================================================================================//

#pragma region VALIDAR AUTO

//===================================//Comandos e verificacao//==================================//

// Funcao de Callback: chamada pelo driver Bluetooth quando chegam dados
void bt_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_DATA_IND_EVT) {        // Nos interessa apenas o evento de dados recebidos
    while(SerialBT.available()){              // Enquanto houver dados disponiveis no buffer
      char receivedChar = SerialBT.read();    // Le um caractere
      // Envia o caractere para a fila para ser processado pela tarefa modoAutonomo
      // O '0' no final significa que ele espera se a fila estiver cheia
      xQueueSend(btQueue, &receivedChar, (TickType_t)0);
    }
  }
}

//===================================//Comandos selecionados//===================================//

#pragma region SELECIONADOS

void printCommands() {
    SerialBT.printf(
        "Caractere  | Funcao\n"
        "-----------|---------------------------------------\n"
        "'f'        | Estrategia Frentao\n"
        "'k'        | Estrategia Frentao sem Delay\n"
        "'i'        | Estrategia Frentinho\n"
        "'c'        | Estrategia Curvao\n"
        "'u'        | Estrategia Curvinha\n"
        "'v'        | Estrategia em V\n"
        "'b'        | Estrategia Costas\n"
        "'d'        | Defensivo puro\n"
        "'w'        | Busca Woodpecker\n"
        "'s'        | Slow Search\n"
        "-----------|---------------------------------------\n"
        "'<'        | Movimento para o lado Esquerdo\n"
        "'>'        | Movimento para o lado Direito\n"
        "-----------|---------------------------------------\n"
        "'H'        | Abaixa a haste\n"
        "'L'        | Levanta a haste\n"
        "-----------|---------------------------------------\n"
        "'D'        | Ativa o modo de defesa\n"
        "'A'        | Ativa o modo de ataque\n"
        "'B'        | Ativa o modo de busca\n"
        "'G'        | Ativa o modo de giro\n"
        "'Q'        | Ativa o modo quebrado\n"
        "-----------|---------------------------------------\n"
        "'S'        | Ativa o modo furtivo\n"
        "'O'        | Desativa o modo furtivo\n"
        "-----------|---------------------------------------\n"
        "'z'        | Inicia configuracao personalizada\n"
        "'.'        | Finaliza configuracao personalizada\n"
        "'y'        | Seleciona configuracao personalizada\n"
        "-----------|---------------------------------------\n"
        "'E'        | Conferir estrategia\n"
        "'0'        | Finaliza configuracao\n"
        "-----------|---------------------------------------\n"
        "\n"
        "Botao IR   | Funcao\n"
        "-----------|---------------------------------------\n"
        "'1'        | Verifica se recebe sinal\n"
        "'2'        | Inicia o robo\n"
        "'3'        | Para o robo\n"
        "-----------|---------------------------------------\n"
    );
    return;
}

#pragma endregion

//====================================//Verifica Estrategia//====================================//

#pragma region RETORNO BT

void verificarEstrategia(char comando) {
    switch (comando) {
        case 'f': SerialBT.println("//=====//FRENTAO SELECIONADO//=====//"); break;
        case 'k': SerialBT.println("//=====//FRENTAO SEM DELAY SELECIONADO//=====//"); break;
        case 'i': SerialBT.println("//=====//FRENTINHO SELECIONADO//=====//"); break;
        case 'c': SerialBT.println("//=====//CURVAO SELECIONADO//=====//"); break;
        case 'u': SerialBT.println("//=====//CURVINHA SELECIONADA//=====//"); break;
        case 'v': SerialBT.println("//=====//EM V SELECIONADO//=====//"); break;
        case 'b': SerialBT.println("//=====//COSTAS SELECIONADO//=====//"); break;
        case 'd': SerialBT.println("//=====//DEFENSIVO PURO SELECIONADO//=====//"); break;
        case 'y': SerialBT.println("//=====//PERSONALIZADA SELECIONADA//=====//"); break;
        case 'w': SerialBT.println("//=====//BUSCA WOODPECKER SELECIONADA//=====//"); break;
        case 's': SerialBT.println("//=====//SLOW SEARCH SELECIONADA//=====//"); break;
        default: break;
    }
}

#pragma endregion

//===============================================================================================//
//======================================//DEFINICOES BASE//======================================//
//===============================================================================================//

#pragma region CONFERIR STRAT

void checkStrategy() {
    SerialBT.printf("-> Estrategia: ");
    SerialBT.printf("%c\n", strategy);

    SerialBT.printf("-> Direcao: ");
    if (direction == direita) SerialBT.printf("DIREITA\n");
    else SerialBT.printf("ESQUERDA\n");

    SerialBT.printf("-> Modo: ");
    if (modoLuta == defesa) SerialBT.printf("DEFESA\n");
    else if (modoLuta == ataque) SerialBT.printf("ATAQUE\n");
    else if (modoLuta == busca) SerialBT.printf("BUSCA\n");
    else if (modoLuta == giro) SerialBT.printf("GIRO\n");

    SerialBT.printf("-> Sensor: ");
    if (modoFurtivo) SerialBT.printf("DESATIVADO\n");
    else SerialBT.printf("ATIVADO\n");

    SerialBT.printf("-> Haste: ");
    if (hasteAbaixada) SerialBT.printf("ABAIXADA\n");
    else SerialBT.printf("LEVANTADA\n");
}

#pragma endregion

//===================================//Comandos Disponiveis//====================================//

#pragma region DISPONIVEIS

void definicoesBaseBT(char comando) {
    switch (comando) {
        case '?':
            printCommands();
            break;

#pragma endregion

//=========================================//Direcoes//==========================================//

#pragma region DIRECOES

        case '<':
            direction = esquerda;
            SerialBT.printf("//=====//DIRECAO ESQUERDA//=====//\n");
            break;
        case '>':
            direction = direita;
            SerialBT.printf("//=====//DIRECAO DIREITA//=====//\n");
            break;

#pragma endregion

//==========================================//Hastes//===========================================//

#pragma region HASTES

        case 'H':
            hasteAbaixada = true;
            SerialBT.printf("//=====//HASTE ABAIXADA//=====//\n");
            break;
        case 'L':
            hasteAbaixada = false;
            SerialBT.printf("//=====//HASTE LEVANTADA//=====//\n");
            break;

#pragma endregion

//=======================================//Modo de Luta//========================================//

#pragma region MODO LUTA

        case 'D':
            modoLuta = defesa;
            SerialBT.printf("//=====//DEFESA ATIVADA//=====//\n");
            break;
        case 'A':
            modoLuta = ataque;
            SerialBT.printf("//=====//ATAQUE ATIVADO//=====//\n");
            break;
        case 'B':
            modoLuta = busca;
            SerialBT.printf("//=====//BUSCA ATIVADA//=====//\n");
            break;
        case 'G':
            modoLuta = giro;
            SerialBT.printf("//=====//GIRO ATIVADO//=====//\n");
            break;
        case 'Q':
            modoLuta = quebrado;
            SerialBT.printf("//=====//QUEBRADO ATIVADO//=====//\n");
            break;

#pragma endregion

//=========================================//Sensores//==========================================//

#pragma region SENSORES

        case 'S':
            modoFurtivo = true;
            SerialBT.printf("//=====//MODO FURTIVO ATIVADO//=====//\n");
            break;
        case 'O':
            modoFurtivo = false;
            SerialBT.printf("//=====//MODO FURTIVO DESATIVADO//=====//\n");
            break;

#pragma endregion

//========================================//Finalizacao//========================================//

#pragma region FINALIZACAO

        case 'E':
            SerialBT.printf("//=====//CONFERIR ESTRATEGIA//=====//\n");
            checkStrategy();
            break;

        case '0':
            SerialBT.printf("//=====//FIM DOS COMANDOS//=====//\n");
            checkStrategy();
            break;

        default:                              // Define estrategiaLutaBT se nao for ENTER ou numero
            if (!(comando == 13 || comando == 10 || (comando > 48 && comando < 58))) {
                verificarEstrategia(comando);
                strategy = comando;
            }
    }
}

#pragma endregion

//===============================================================================================//
//=====================================//FINALIZA O ARQUIVO//====================================//
//===============================================================================================//

#endif