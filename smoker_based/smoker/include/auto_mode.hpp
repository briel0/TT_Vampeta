//===============================================================================================//
//=====================================//INCLUDES E DEFINES//====================================//
//===============================================================================================//

#pragma region INCLUDES

#ifndef AUTO_MODE_H
#define AUTO_MODE_H

#include <defines.hpp>                        // Definicoes globais
#include <functions.hpp>                      // Funcoes auxiliares
#include <sensor_task.hpp>                    // Funcoes de sensoreamento
#include <strategies.hpp>                     // Estrategias iniciais do modo AUTO

#pragma endregion

//===============================================================================================//
//====================================//INTERPRETA BLUETOOTH//===================================//
//===============================================================================================//

#pragma region INTERPRETA BT

void translateBT() {
    while (true) {
        char receivedChar;
        // Atualiza os LEDs com o status dos sensores
        indicarSensores(valueJsumoE, valueJsumoF, valueJsumoD);

        if (xQueueReceive(btQueue, &receivedChar, pdMS_TO_TICKS(10))) {
            if (isConfiguring) {
                if (receivedChar == '\n') {
                    btBuffer[bufferIndex] = '\0'; // Finaliza a string
                    if (strcmp(btBuffer, ".") == 0) {
                        isConfiguring = false;
                        SerialBT.println("Configuracao finalizada.");
                        Serial.println("Configuracao finalizada.");
                        customStrategy[customStrategyCount].speedLeft = 0;
                        customStrategy[customStrategyCount].speedRight = 0;
                        customStrategy[customStrategyCount].delayMs = 0;
                    } else {
                        parseAndStoreStep(btBuffer);
                    }
                    bufferIndex = 0;          // Limpa o buffer
                } else if (receivedChar >= 32) {
                    if (bufferIndex < BT_BUFFER_SIZE - 1) {
                        btBuffer[bufferIndex++] = receivedChar;
                    } else {
                        Serial.println("Erro: Buffer cheio!");
                        bufferIndex = 0;
                    }
                }
            } else {                          // Se NÃO está configurando
                if (receivedChar == '0') {    // Fim dos comandos BT
                    Serial.println("//=====//FIM DOS COMANDOS//=====//");
                    SerialBT.println("//=====//FIM DOS COMANDOS//=====//");
                    vTaskDelay(pdMS_TO_TICKS(500));
                    break;                    // Retorna para a modoAUTO
                } else if (receivedChar == 'z') {
                    isConfiguring = true;
                    customStrategyCount = 0;
                    bufferIndex = 0;
                    SerialBT.println("Modo de configuracao iniciado");
                    SerialBT.println("Envie passo (vel_esq,vel_dir,delay) ou '.' para finalizar");
                    Serial.println("Modo de configuracao iniciado");
                } else {
                    definicoesBaseBT(receivedChar);

                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

#pragma endregion

//===============================================================================================//
//===================================//ESCOLHA DA ESTRATEGIA//===================================//
//===============================================================================================//

#pragma region ESCOLHA ESTRATEGIA

void modoAUTO() {
    setupAUTOTask();

    btQueue = xQueueCreate(BT_QUEUE_LENGTH, sizeof(char));

    Serial.println("//=====//Setup AUTO feita//=====//");
    SerialBT.register_callback(bt_callback);

    vTaskDelay(pdMS_TO_TICKS(500));           // Pequeno atraso para evitar sobrecarregar a CPU

    SerialBT.begin("Smoker");
    Serial.printf("Bluetooth iniciado. Tentando conectar");
    validaSetup(1,1,1,1,1);                   // 0 vermelhos e 5 verdes
    vTaskDelay(pdMS_TO_TICKS(500));           // Pequeno atraso

    
    while (!SerialBT.hasClient()) {
        Serial.printf(".");
        vTaskDelay(pdMS_TO_TICKS(100)); // Espera em pequenos incrementos
    }
    Serial.println();
    Serial.println("Cliente Bluetooth conectado! Iniciando comunicacao");
    SerialBT.println("//=====//Bluetooth conectado!//=====//");
    printCommands();

    translateBT();                            // Interpreta o char recebido por BT
    inicializado = true;                      // Indica que a configuracao acabou e o loop reinicia
    xTaskNotifyGive(IRCommandHandle);         // Notifica a moveTask
}

#pragma endregion

//===============================================================================================//
//=====================================//FINALIZA O ARQUIVO//====================================//
//===============================================================================================//

#endif