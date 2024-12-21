#ifndef functions_H
#define functions_H

#include <defines.hpp>
#include <parallel.hpp>

/*
Filtro de média móvel para sensores analógicos, usamos ponteiros para termos apenas uma função.
long moving_average(int leitura, int* vetor)
{
    // Desloca os elementos do vetor de média móvel
    for(int i = n - 1; i > 0; i--) vetor[i] = vetor[i - 1];
    vetor[0] = leitura; // Posição inicial do vetor recebe a leitura original
    long acc = 0 ; // Acumulador para somar os pontos da média móvel
    for(int i = 0 ; i < n; i++) acc += vetor[i]; // Faz a somatória do número de pontos
    return acc / n ; // Retorna a média móvel
}
*/

void setPins()
{
    pinMode(a1, OUTPUT);
    pinMode(a2, OUTPUT);
    pinMode(b1, OUTPUT);
    pinMode(b2, OUTPUT);
    pinMode(jsumoE, INPUT);
    pinMode(jsumoF, INPUT);
    pinMode(jsumoD, INPUT);
    pinMode(led, OUTPUT);
    pinMode(stby, OUTPUT);
    pinMode(pwmA, OUTPUT);
    pinMode(pwmB, OUTPUT);
    //pinMode(lineD, INPUT); 
    //pinMode(lineE, INPUT);
    digitalWrite(stby, 0);
    return;
}

// Função que inicializa o infravermelho e testa com feedback dos LEDs.
void initiateIR()
{
    IrReceiver.begin(IR, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
    printActiveIRProtocols(&Serial);
    return;
}

// Função que inicializa o Bluetooth e imprime uma mensagem para confirmar conexão.
void initiateBT()
{
    SerialBT.begin("Fuego"); // Nome do dispositivo Bluetooth
    SerialBT.println("LIGOUUUU");
    return;
}

// Função para o robô realizar um delay, mas quebrá-lo assim que detectar um adversário.
void functionDelay(int time)
{
    int currentTime = 0;
    while(currentTime <= time / 10)
    {
        if(valueJsumoF == 1 || valueJsumoE == 1 || valueJsumoD == 1)
        {
            return;
        }
        vTaskDelay(10);
        currentTime++;
    }
}


//=====//=====// TESTES //=====//=====//

// Função com loop de teste movendo o robô na direção do sensor acionado, exceto o da frente.
void testLoop()
{
    digitalWrite(stby, 1);
    SerialBT.println("Test Loop");
    running = true;
    for (;;)
    {
        SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueJsumoE, valueJsumoF, valueJsumoD);
        if (valueJsumoF)
        {
            move('F', 0, 'F', 0);
        }
        else
        {
            if (valueJsumoD)
            {
                move('F', curva, 'F', curva/8);
                direction = dir;
            }
            else if (valueJsumoE)
            {
                move('F', curva/8, 'F', curva);
                direction = esq;
            }
            else if (direction == esq)
            {
                move('T', curva/2, 'F', curva/2);
            }
            else if (direction == dir)
            {
                move('F', curva/2, 'T', curva/2);
            }
            vTaskDelay(1);
        }
    }
}

// Função que testa os sensores imprimindo continuamente as leituras dos sensores.
void testSensors()
{
    SerialBT.println("Test Sensors");
    running = true;
    for (;;)
    {
        SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueJsumoE, valueJsumoF, valueJsumoD);
        delay(15);
    }
}

/*
Função que testa os motores movendo o robô na direção do sensor acionado.
Caso nenhum sensor seja acionado, o robô fica parado.
A função imprime continuamente as leituras dos sensores.
*/
void testMotorsWithSensors()
{
    digitalWrite(stby, 1);
    SerialBT.println("Test Motors With Sensors");
    running = true;
    for (;;)
    {
        SerialBT.printf("ESQ: %d, FEN: %d, DIR: %d\n", valueJsumoE, valueJsumoF, valueJsumoD);
        if (valueJsumoF)
        {
            move('F', frentao, 'F', frentao);
        }
        else if (valueJsumoD)
        {
            move('T', curva, 'F', curva);
        }
        else if (valueJsumoE)
        {
            move('F', curva, 'T', curva);
        }
        else
        {
            move('F', 0, 'F', 0);
        }
    }
}
#endif