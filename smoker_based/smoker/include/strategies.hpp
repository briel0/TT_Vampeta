//===============================================================================================//
//=====================================//INCLUDES E DEFINES//====================================//
//===============================================================================================//

#pragma region INCLUDES

#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <defines.hpp>                        // Definicoes globais
#include <functions.hpp>                      // Funcoes auxiliares
#include <move.hpp>                           // Funcoes de movivmentacao de motores

#define maxIteracoesW          10             // WOOD | Maximo de iteracoes 
#define maxIteracoesS          6              // SLOW | Maximo de iteracoes

volatile int numIteracoes      = 0;           // WOOD + SLOW | Iteracao de busca atual

#pragma endregion

//===============================================================================================//
//========================================//ESTRATEGIAS//========================================//
//===============================================================================================//

#pragma region ESTATEGIAS

/*
!INFO | Estrutura das estrategias sequenciais
-----------------------------------
A estrutura das estrategias sequenciais deve seguir o formato
const StrategyStep nome[] = {
    {   a,    b,    c}, 
    {   d,    e,    f}, 
    [...]
    {   0,    0,    0}                        // Indica que a sequencia acabou
};

*/

#pragma endregion

//==========================================//FRENTAO//==========================================//

#pragma region FRENTAO

// Vai para frente (3/4 dohyo) em alta velocidade
const StrategyStep frentao[] = {
    {  48,   48,    4}, 
    {  60,   60,    4}, 
    {  72,   72,    4}, 
    {  84,   84,    4}, 
    {  96,   96,    4},
    { 128,  128,    8}, 
    { 160,  160,    8}, 
    { 196,  196,    8}, 
    { 228,  228,   12}, 
    { 255,  255,  108},
    {   0,    0,    0}
};

#pragma endregion

//======================================//FRENTE SEM DELAY//=====================================//

#pragma region FRENTE SEM DELAY

// Vai para frente (3/4 dohyo) em alta velocidade e sem curva de aceleracao
const StrategyStep frenteSemDelay[] = {
    { 255,  255,  200},
    {   0,    0,    0}
};

#pragma endregion

//=========================================//FRENTINHO//=========================================//

#pragma region FRENTINHO

// Vai para frente (1/4 dohyo) em baixa velocidade e sem curva de aceleracao
const StrategyStep frentinho[] = {
    { 127,  127,  200},
    {   0,    0,    0}
};

#pragma endregion

//===========================================//CURVAO//===========================================//

#pragma region CURVAO

// Curva aberta (3/4 dohyo)
const StrategyStep curvaoEsquerda[] = {
    {-255,  255,   65}, 
    { 255,  120,  400},
    {   0,    0,    0}
};

const StrategyStep curvaoDireita[] = {
    { 255, -255,   65}, 
    { 120,  255,  400},
    {   0,    0,    0}
};

#pragma endregion

//==========================================//CURVINHA//=========================================//

#pragma region CURVINHA

// Curva fechada (3/4 dohyo)
const StrategyStep curvinhaEsquerda[] = {
    {-255,  255,   40}, 
    { 255,  120,  400},
    {   0,    0,    0}
};

const StrategyStep curvinhaDireita[] = {
    { 255, -255,   40}, 
    { 120,  255,  400},
    {   0,    0,    0}
};

#pragma endregion

//============================================//EM V//===========================================//

#pragma region EM V

// Movimentacao em V (3/4 dohyo)
const StrategyStep emVEsquerda[] = {
    {-255,  255,   50}, 
    { 255,  255,  225},
    { 255, -255,  155},
    { 255,  255,  125},
    {   0,    0,    0}
};

const StrategyStep emVDireita[] = {
    { 255, -255,   50}, 
    { 255,  255,  225},
    {-255,  255,  155},
    { 255,  255,  125},
    {   0,    0,    0}
};

#pragma endregion

//===========================================//COSTAS//==========================================//

#pragma region COSTAS

// Movimentacao em V (3/4 dohyo)
const StrategyStep costasEsquerda[] = {
    {-255,  255,  150},
    {   0,    0,    0}
};

const StrategyStep costasDireita[] = {
    { 255, -255,  150},
    {   0,    0,    0}
};

#pragma endregion

//=========================================//WOODPECKER//========================================//

#pragma region WOODPECKER

void buscaWoodpecker() {
    if (modoFurtivo) xTaskNotifyGive(swSensorHandle);

    // Delay depois para haste abaixar enquanto anda
    moverMotores(-200, -200);
    if (hasteAbaixada) xTaskNotifyGive(openServoHandle);
    vTaskDelay(pdMS_TO_TICKS(300));

    modoFurtivo = false; xTaskNotifyGive(swSensorHandle);

    do {                              // Iterativo ate ver adversario ou maxIteracoes
        moverMotores(200, 200);  if (delayMsSensor(75)) return;
        moverMotores(0, 0);      if (delayMsSensor(75)) return;
        moverMotores(-200, 200); if (delayMsSensor(75)) return;
        moverMotores(200, -200); if (delayMsSensor(150)) return;
        moverMotores(0, 0);      if (delayMsSensor(150)) return;
        numIteracoes++;
    } while(!seeing || numIteracoes < maxIteracoesW);

    // Defesa para ter cuidado ja que nao encontrou o adversario dentro do limite de it
    if (numIteracoes > maxIteracoesW) modoLuta = defesa;

    // Modo de luta de Giro mantem JSumos desligados
    if (modoLuta == giro) { 
        modoFurtivo = true; 
        xTaskNotifyGive(swSensorHandle); 
    }
}

#pragma endregion

//========================================//SLOW SEARCH//========================================//

#pragma region SLOW SEARCH

void slowSearch() {
    if (modoFurtivo) xTaskNotifyGive(swSensorHandle);

    if (direction == esquerda) {      // Esquerda

        // Delay depois para haste abaixar enquanto anda
        moverMotores(200, -200);
        if (hasteAbaixada) xTaskNotifyGive(openServoHandle);
        vTaskDelay(pdMS_TO_TICKS(75));
        moverMotores(-200, -200); vTaskDelay(pdMS_TO_TICKS(300));

        modoFurtivo = false; xTaskNotifyGive(swSensorHandle);

        do { // Iterativo ate ver adversario ou maxIteracoes
            moverMotores(200, 200);  if (delayMsSensor(150)) return;
            moverMotores(0, 0);      if (delayMsSensor(75)) return;
            moverMotores(-200, 200); if (delayMsSensor(75)) return;
            moverMotores(200, -200); if (delayMsSensor(150)) return;
            moverMotores(-200, 200); if (delayMsSensor(75)) return;
            moverMotores(0, 0);      if (delayMsSensor(150)) return;
            numIteracoes++;
        } while(!seeing || numIteracoes < maxIteracoesS);

    } else { // Direita

        // Delay depois para haste abaixar enquanto anda
        moverMotores(-200, 200);
        if (hasteAbaixada) xTaskNotifyGive(openServoHandle);
        vTaskDelay(pdMS_TO_TICKS(75));
        moverMotores(-200, -200); vTaskDelay(pdMS_TO_TICKS(300));

        modoFurtivo = false; xTaskNotifyGive(swSensorHandle);

        do { // Iterativo ate ver adversario ou maxIteracoes
            moverMotores(200, 200);  if (delayMsSensor(150)) return;
            moverMotores(0, 0);      if (delayMsSensor(75)) return;
            moverMotores(200, -200); if (delayMsSensor(75)) return;
            moverMotores(-200, 200); if (delayMsSensor(150)) return;
            moverMotores(200, -200); if (delayMsSensor(75)) return;
            moverMotores(0, 0);      if (delayMsSensor(150)) return;
            numIteracoes++;
        } while(!seeing || numIteracoes < maxIteracoesS);
    }

    // Defesa para ter cuidado ja que nao encontrou o adversario dentro do limite de iteracoes
    if (numIteracoes > maxIteracoesS) modoLuta = defesa;

    // Modo de luta de Giro mantem JSumos desligados
    if (modoFurtivo && (modoLuta != giro)) { 
        modoFurtivo = false; 
        xTaskNotifyGive(swSensorHandle); 
    }
}

#pragma endregion

//===============================================================================================//
//====================================//EXECUTAR ESTRATEGIA//====================================//
//===============================================================================================//

#pragma region EXECUTAR

void executarEstrategia(const StrategyStep strategySequence[]) {
    if (hasteAbaixada) xTaskNotifyGive(openServoHandle);
    if (modoFurtivo) xTaskNotifyGive(swSensorHandle);

    for (int i = 0; strategySequence[i].delayMs > 0; ++i) {
        moverMotores(strategySequence[i].speedLeft, strategySequence[i].speedRight);
        vTaskDelay(pdMS_TO_TICKS(strategySequence[i].delayMs));
    }

    if (modoFurtivo && (modoLuta != giro)) {
        modoFurtivo = false;
        xTaskNotifyGive(swSensorHandle);
    }
    
    moverMotores(0, 0);
}

#pragma endregion

//===============================================================================================//
//===================================//SELECIONAR ESTRATEGIA//===================================//
//===============================================================================================//

#pragma region SELECIONAR

void estrategiaLutaBT(char comando) {
    switch(strategy) {

//==================================//Estrategias Sequenciais//==================================//

        // Frentao
        case 'f':
            Serial.println("//=====//FRENTAO INICIADO//=====//");
            executarEstrategia(frentao);
            break;

        // Frente sem delay
        case 'k':
            Serial.println("//=====//FRENTE SEM DELAY INICIADA//=====//");
            executarEstrategia(frenteSemDelay);
            break;

        // Frentinho
        case 'i':
            SerialBT.println("//=====//FRENTINHO INICIADO//=====//");
            executarEstrategia(frentinho);
            break;

        // Curvao
        case 'c':
            SerialBT.println("//=====//CURVAO INICIADO//=====//");
            if (direction == esquerda) executarEstrategia(curvaoEsquerda);
            else if (direction == direita) executarEstrategia(curvaoDireita);
            break;

        // Curvinha
        case 'u':
            SerialBT.println("//=====//CURVINHA INICIADA//=====//");
            if (direction == esquerda) executarEstrategia(curvinhaEsquerda);
            else if (direction == direita) executarEstrategia(curvinhaDireita);
            break;

        // Em V
        case 'v':                             // Movimento em forma de V
            SerialBT.println("//=====//EM V INICIADA//=====//");
            if (direction == esquerda) executarEstrategia(emVEsquerda);
            else if (direction == direita) executarEstrategia(emVDireita);
            break;

        // Costas
        case 'b':                             // Gira 180 graus
            SerialBT.println("//=====//COSTAS (180 GRAUS) INICIADO//=====//");
            if (direction == esquerda) executarEstrategia(costasEsquerda);
            else if (direction == direita) executarEstrategia(costasDireita);
            break;

//==================================//Estrategias Complexas//==================================//

        // Busca Woodpecker
        case 'w':
            SerialBT.println("//=====//BUSCA WOODPECKER INICIADA//=====//");
            buscaWoodpecker();
            break;

        // Slow Search
        case 's':
            SerialBT.println("//=====//SLOW SEARCH INICIADA//=====//");
            slowSearch();
            break;
 
//======================================//Casos especiais//======================================//

        // Estrategia personalizada
        case 'y':
            SerialBT.println("//=====//ESTRATEGIA PERSONALIZADA INICIADA//=====//");
            executarEstrategia(customStrategy);
            break;

        // Defensivo puro
        case 'd':                             // Inicia somente defensivo
            SerialBT.println("//=====//DEFENSIVO PURO INICIADO//=====//");
            if (hasteAbaixada) xTaskNotifyGive(openServoHandle);
            if (modoFurtivo) xTaskNotifyGive(swSensorHandle);
            moverMotores(0, 0);
            break;

        // Caso padrao para nao ficar sem fazer nada
        default:                              // Caractere invalido
            SerialBT.println("//=====//ESTRATEGIA INVALIDA: INICIANDO DEFENSIVO//=====//");
            if (hasteAbaixada) xTaskNotifyGive(openServoHandle);
            if (modoFurtivo) xTaskNotifyGive(swSensorHandle);
            moverMotores(0, 0);
            break;
    }
}

#pragma endregion
 
//===============================================================================================//
//===================================//CONFIG ESTRATEGIA NOVA//==================================//
//===============================================================================================//

#pragma region PERSONALIZACAO

void parseAndStoreStep(const char* buffer) {
    int vel_esq, vel_dir, delay_ms;           // Variaveis para armazenar os valores parseados

    // Usa sscanf para parsear os tres inteiros separados por virgula
    int num_parsed = sscanf(buffer, "%d,%d,%d", &vel_esq, &vel_dir, &delay_ms);

    if (num_parsed != 3) {                    // Se nao tem 3 valores, o formato esta incorreto
        SerialBT.println("Formato invalido! Use: vel_esq,vel_dir,delay");
        Serial.println("Formato invalido! Use: vel_esq,vel_dir,delay");
        return;
    }

    if (customStrategyCount < (MAX_STEPS - 1)) {    // Verifica se ha espaco disponivel no array
        customStrategy[customStrategyCount].speedLeft = vel_esq;
        customStrategy[customStrategyCount].speedRight = vel_dir;
        customStrategy[customStrategyCount].delayMs = delay_ms;
        customStrategyCount++;                // Incrementa o contador de passos
        
        Serial.printf("Passo adicionado: E:%d, D:%d, Delay:%dms\n", vel_esq, vel_dir, delay_ms);
        SerialBT.printf("Passo adicionado: E:%d, D:%d, Delay:%dms\n", vel_esq, vel_dir, delay_ms);
    } else {
        SerialBT.println("Limite de passos da estratégia atingido!");
        Serial.println("Limite de passos da estratégia atingido!");
    }
}

#pragma endregion
 
//===============================================================================================//
//====================================//TASK DA ESTRATEGIA//=====================================//
//===============================================================================================//

#pragma region TASKS

void estrategiaTask(void *pvParameters) {
    for(;;) {
        // Aguarda ser chamado
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        estrategiaLutaBT(strategy);           // Executa a estratégia
        running = true;                       // Ativa o robo
    }
}

void setupAUTOTask() {
    xTaskCreatePinnedToCore(
        estrategiaTask,                       // Funcao da tarefa
        "EstrategiaTask",                     // Nome da tarefa
        256 * 32,                             // Tamanho da pilha
        nullptr,                              // Parametros
        14,                                   // Prioridade 14
        &stratLutaHandle,                     // Handle
        APP_CPU_NUM                           // Nucleo 1 onde a tarefa sera executada
    );   
}

#pragma endregion
 
//===============================================================================================//
//=====================================//FINALIZA O ARQUIVO//====================================//
//===============================================================================================//

#endif