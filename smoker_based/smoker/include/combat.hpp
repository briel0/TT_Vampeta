#pragma region INCLUDES

#ifndef COMBAT_H
#define COMBAT_H

#include <defines.hpp> // Definicoes globais
#include <move.hpp>    // Funcoes de movivmentacao de motores

#define velUltimoLado 86    // Velocidade de giro para o ultimo lado visto
#define velLadoAtual 172    // Velocidade de giro para o lado atual
#define velFrenteRapida 192 // Velocidade de avanco rapido
#define velFrenteLenta 48   // Velocidade de avanco lento
#define acrescimo 32        // Acrescimo de velocidade para um dos motores

#define maxAvancosIteracao 5 // BUSCA | Limite de avancos por iteracao

#define maxLeituraFrente 5   // QUEBRADO | Maximo de leituras com o sensorF
#define maxTempoAndando 3000 // QUEBRADO | Maximo de tempo andando para frente

int avancosIteracao = 0; // BUSCA | Conta os avancos da iteracao
int avancosTotais = 0;   // BUSCA | Conta os avancos totais

int leituraFrente = 0;       // QUEBRADO | Qtde de leituras feitas com o sensorF
bool primeiraSambada = true; // QUEBRADO | Primeiro passo da movimentacao frontal

#pragma endregion

//===============================================================================================//
//========================================//MODO ATAQUE//========================================//
//===============================================================================================//

#pragma region MODO ATAQUE

void modoAtaque()
{
    if (valueJsumoF)
    { // Se o inimigo esta a frente
        moverMotores(velFrenteRapida, velFrenteRapida);
        return;
    }
    else if (valueJsumoE && !valueJsumoF && !valueJsumoD)
    { // Se o inimigo esta a esquerda
        moverMotores(-velLadoAtual, velLadoAtual);
        return;
    }
    else if (valueJsumoD && !valueJsumoF && !valueJsumoE)
    { // Se o inimigo esta a direita
        moverMotores(velLadoAtual, -velLadoAtual);
        return;
    }
    else
    {
        switch (ultimoLado)
        { // Procura o inimigo no ultimo lado visto
        case vistoEsquerda:
            moverMotores(-velUltimoLado, velUltimoLado);
            return;

        case vistoDireita:
            moverMotores(velUltimoLado, -velUltimoLado);
            return;

        default: // Se nunca viu, gira para tentar encontrar
            moverMotores(-velUltimoLado, velUltimoLado);
            return;
        }
    }
}

#pragma endregion

//===============================================================================================//
//========================================//MODO DEFESA//========================================//
//===============================================================================================//

#pragma region MODO DEFESA

void modoDefesa()
{
    if (valueJsumoF)
    { // Se o inimigo esta a frente
        moverMotores(velFrenteLenta, velFrenteLenta);
        return;
    }
    else if (valueJsumoE && !valueJsumoF && !valueJsumoD)
    { // Se o inimigo esta a esquerda
        moverMotores(-velLadoAtual, velLadoAtual);
        return;
    }
    else if (valueJsumoD && !valueJsumoF && !valueJsumoE)
    { // Se o inimigo esta a direita
        moverMotores(velLadoAtual, -velLadoAtual);
        return;
    }
    else
    {
        switch (ultimoLado)
        { // Procura o inimigo no ultimo lado visto
        case vistoEsquerda:
            moverMotores(-velUltimoLado, velUltimoLado);
            return;

        case vistoDireita:
            moverMotores(velUltimoLado, -velUltimoLado);
            return;

        default: // Se nunca viu, gira para tentar encontrar
            moverMotores(-velUltimoLado, velUltimoLado);
            return;
        }
    }
}

#pragma endregion

//===============================================================================================//
//=========================================//MODO GIRO//=========================================//
//===============================================================================================//

#pragma region MODO GIRO

void modoGiro()
{
    if (valueJsumoF)
    { // Se ve oponente a frente, para
        moverMotores(0, 0);
        return;
    }
    else if (valueJsumoE && !valueJsumoF && !valueJsumoD)
    { // Gira para a esquerda
        moverMotores(-velLadoAtual, velLadoAtual);
        return;
    }
    else if (valueJsumoD && !valueJsumoF && !valueJsumoE)
    { // Gira para a direita
        moverMotores(velLadoAtual, -velLadoAtual);
        return;
    }
    else
    {
        switch (ultimoLado)
        { // Procura o inimigo no ultimo lado visto
        case vistoEsquerda:
            moverMotores(-velUltimoLado, velUltimoLado);
            return;

        case vistoDireita:
            moverMotores(velUltimoLado, -velUltimoLado);
            return;

        default: // Se nunca viu, gira para tentar encontrar
            moverMotores(-velUltimoLado, velUltimoLado);
            return;
        }
    }
}

#pragma endregion

//===============================================================================================//
//========================================//MODO BUSCA//=========================================//
//===============================================================================================//

#pragma region MODO BUSCA

void modoBusca()
{
    static int passo_busca = 0;
    static unsigned long tempo_inicio_passo = 0;

    // Logica de curva executada a cada pulso
    if (valueJsumoE && !valueJsumoD)
    {
        moverMotores(-velLadoAtual, velLadoAtual);
        passo_busca = 0;     // Reinicia a logica de busca
        avancosIteracao = 0; // Reinicia a contagem de avancos da iteracao
        return;
    }
    else if (valueJsumoD && !valueJsumoE)
    {
        moverMotores(velLadoAtual, -velLadoAtual);
        passo_busca = 0;     // Reinicia a logica de busca
        avancosIteracao = 0; // Reinicia a contagem de avancos da iteracao
        return;
    }
    else if (valueJsumoF && !valueJsumoD && !valueJsumoE)
    {
        // Maquina de estados se nao houver curva
        switch (passo_busca)
        {
        case 0: // Passo 1: Avancar por 50ms
            moverMotores(velFrenteLenta, velFrenteLenta);
            tempo_inicio_passo = millis();
            passo_busca = 1;
            break;

        case 1: // Passo 2: Esperar os 50ms terminarem
            if (millis() - tempo_inicio_passo > 50)
            {
                passo_busca = 2;
            }
            break;

        case 2: // Passo 3: Parar por 500ms
            moverMotores(0, 0);
            tempo_inicio_passo = millis();
            passo_busca = 3;
            break;

        case 3: // Passo 4: Esperar os 500ms terminarem
            if (millis() - tempo_inicio_passo > 500)
            {
                passo_busca = 0;   // Reinicia a sequencia
                avancosIteracao++; // Acresce a contagem de avancos da iteracao
                avancosTotais++;   // Acresce a contagem de avancos totais
            }
            break;
        }
    }
    else
    { // Se nao ve o inimigo
        switch (ultimoLado)
        {
        case vistoDireita: // Procura o inimigo no ultimo lado visto
            moverMotores(velUltimoLado, -velUltimoLado);
            passo_busca = 0;     // Reinicia a logica de busca
            avancosIteracao = 0; // Reinicia a contagem de avancos da iteracao
            break;

        default: // Caso nao veja na direita, gira para a esquerda
            moverMotores(-velUltimoLado, velUltimoLado);
            passo_busca = 0;     // Reinicia a logica de busca
            avancosIteracao = 0; // Reinicia a contagem de avancos da iteracao
            break;
        }
    }
}

#pragma endregion

//===============================================================================================//
//=======================================//MODO QUEBRADO//=======================================//
//===============================================================================================//

// OBS: O modo quebrado nao esta quebrado!!! Movimentacao em angulos quebrados (zigzag)

#pragma region MODO QUEBRADO

void modoQuebrado()
{
    static int passo_quebrado = 0;
    static unsigned long tempo_inicio_passo = 0;

    // Logica de curva executada a cada pulso
    if (valueJsumoE && !valueJsumoD)
    {
        moverMotores(-velLadoAtual, velLadoAtual);
        passo_quebrado = 0;     // Reinicia a logica do modo quebrado
        leituraFrente = 0;      // Reinicia a contagem de leituras com sensor F
        primeiraSambada = true; // Indica que voltou para o primeiro movimento
        return;
    }
    else if (valueJsumoD && !valueJsumoE)
    {
        moverMotores(velLadoAtual, -velLadoAtual);
        passo_quebrado = 0;     // Reinicia a logica do modo quebrado
        leituraFrente = 0;      // Reinicia a contagem de leituras com sensor F
        primeiraSambada = true; // Indica que voltou para o primeiro movimento
        return;
    }
    else if (valueJsumoF && !valueJsumoD && !valueJsumoE)
    {
        // Maquina de estados se nao houver curva
        switch (passo_quebrado)
        {
        case 0: // Passo 1: Primeira sambada esquerda
            moverMotores(-velFrenteLenta, (velFrenteLenta + acrescimo));
            tempo_inicio_passo = millis();
            passo_quebrado = 1;
            break;

        case 1: // Passo 2: Esperar a primeira sambada esquerda
            if (millis() - tempo_inicio_passo > 40)
            {
                passo_quebrado = 2;
            }
            break;

        case 2: // Passo 3: Sambada direita
            moverMotores((velFrenteLenta + acrescimo), -velFrenteLenta);
            tempo_inicio_passo = millis();
            passo_quebrado = 3;
            break;

        case 3: // Passo 4: Esperar a sambada direita
            if (millis() - tempo_inicio_passo > 80)
            {
                passo_quebrado = 4;
            }
            break;

        case 4: // Passo 5: Sambada esquerda
            moverMotores(-velFrenteLenta, (velFrenteLenta + acrescimo));
            tempo_inicio_passo = millis();
            passo_quebrado = 5;
            break;

        case 5: // Passo 6: Esperar a sambada esquerda
            if (millis() - tempo_inicio_passo > 80)
            {
                if (leituraFrente > maxLeituraFrente)
                {
                    passo_quebrado = 6;
                }
                else
                    modoLuta = ataque;
            }
            break;

        case 6: // Passo 7: Se avancou menos de x vezes, avancar
            if (leituraFrente > maxLeituraFrente)
            {
                moverMotores(velFrenteLenta, velFrenteLenta);
                tempo_inicio_passo = millis();
                passo_quebrado = 7;
            }
            else
                modoLuta = ataque; // Entra no modo de ataque
            // Essa logica indica que o adversario esta alinhado, pois curvas zeram a contagem
            break;

        case 7: // Passo 8: Tempo de avanco
            if (millis() - tempo_inicio_passo > 50)
            {
                leituraFrente++;
                passo_quebrado = 8;
            }
            break;

        case 8: // Passo 9: Parar
            moverMotores(0, 0);
            tempo_inicio_passo = millis();
            passo_quebrado = 9;
            break;

        case 9: // Passo 4: Esperar parado
            if (millis() - tempo_inicio_passo > 500)
            {
                passo_quebrado = 6; // Reinicia a sequencia de avanco
            }
            break;
        }
    }
    else
    { // Se nao ve o inimigo
        switch (ultimoLado)
        {
        case vistoDireita: // Procura o inimigo no ultimo lado visto
            moverMotores(velUltimoLado, -velUltimoLado);
            passo_quebrado = 0;     // Reinicia a logica do modo quebrado
            leituraFrente = 0;      // Reinicia a contagem de leituras com sensor F
            primeiraSambada = true; // Indica que voltou para o primeiro movimento
            break;

        default: // Caso nao veja na direita, gira para a esquerda
            moverMotores(-velUltimoLado, velUltimoLado);
            passo_quebrado = 0;     // Reinicia a logica do modo quebrado
            leituraFrente = 0;      // Reinicia a contagem de leituras com sensor F
            primeiraSambada = true; // Indica que voltou para o primeiro movimento
            break;
        }
    }
}

#pragma endregion

//===============================================================================================//
//=====================================//FINALIZA O ARQUIVO//====================================//
//===============================================================================================//

#endif