//===============================================================================================//
//=====================================//INCLUDES E DEFINES//====================================//
//===============================================================================================//

#pragma region INCLUDES

#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include <defines.hpp>                        // Definicoes globais
#include <functions.hpp>                      // Funcoes auxiliares
#include <move.hpp>                           // Funcoes de movivmentacao de motores
#include <combat.hpp>                         // Loop pos estretegia inicial do modo AUTO

#pragma endregion

//===============================================================================================//
//========================================//PARAR ROBO//=========================================//
//===============================================================================================//

#pragma region PARAR ROBO

void stopRobot(void *pvParameters) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    for(;;) {
        running = false;                      // Para os loops de Defensivo e Ofensivo
        brakeMotors();                        // Trava os motores
        vTaskDelay(pdMS_TO_TICKS(1000));      // Delay para garantir a parada
        vTaskDelete(NULL);                    // Encerra esta task
    }
}

void irMonitorTask(void *pvParameters) {      // Monitora a cada 100 ms se o robo deve parar 
    for (;;) {
        if (IrReceiver.decode()) {            // Se recebe IR 3 (0x2), notifica a task de parada
            if (IrReceiver.decodedIRData.command == 0x2)
                xTaskNotifyGive(stopRobotHandle);
            IrReceiver.resume();
        }
        vTaskDelay(pdMS_TO_TICKS(100));       // 100 ms para as outras tarefas serem executadas
    }
}

#pragma endregion

//===============================================================================================//
//====================================//LEITURA DOS SENSORES//===================================//
//===============================================================================================//

//======================================//Desliga Sensores//=====================================//

#pragma region DESLIGA SENSOR

void switchSensor(void *pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (modoFurtivo) {                    // Desliga o transistor e desconecta GND dos JSumos
            GPIO.out_w1tc = ((uint32_t)1 << TRANSISTOR_SENSOR_PIN);

        } else {                              // Liga o transistor e conecta GND dos JSumos
            GPIO.out_w1ts = ((uint32_t)1 << TRANSISTOR_SENSOR_PIN);
        }
    }
}

#pragma endregion

//======================================//Leitura Sensores//=====================================//

#pragma region LEITURA SENSOR

bool IRAM_ATTR readSensors(void *arg) {
    if(!modoFurtivo) {
        portENTER_CRITICAL_ISR(&sensorMux);         // Entra em secao critica
        valueJsumoF = (GPIO.in1.val >> (JSUMO_F_PIN - 32)) & 0x1;
        valueJsumoD = (GPIO.in1.val >> (JSUMO_D_PIN - 32)) & 0x1;
        valueJsumoE = (GPIO.in1.val >> (JSUMO_E_PIN - 32)) & 0x1;
        portEXIT_CRITICAL_ISR(&sensorMux);          // Sai da secao critica
    } else {
        portENTER_CRITICAL_ISR(&sensorMux);         // Entra em secao critica
        //valueJsumoF = (GPIO.in >> SENSOR_IR_F) & 0x1;
        //valueJsumoD = (GPIO.in >> SENSOR_IR_D) & 0x1;
        //valueJsumoE = (GPIO.in >> SENSOR_IR_E) & 0x1;
        portEXIT_CRITICAL_ISR(&sensorMux);          // Sai da secao critica
    }

    if (valueJsumoE) ultimoLado = vistoEsquerda;
    else if (valueJsumoD) ultimoLado = vistoDireita;

    // Variavel para verificar se uma tarefa de maior prioridade foi despertada
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // Notifica a SensorTask
    vTaskNotifyGiveFromISR(combatLogicHandle, &xHigherPriorityTaskWoken);
    return (xHigherPriorityTaskWoken == pdTRUE);
}

#pragma endregion   

//================================//Interpreta controle remoto//=================================//

#pragma region INTERPRETA IR

void handleIRCommand(void *pvParameters) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Aguarda notificacao
    while (!(ready && IrReceiver.decodedIRData.command == 0x1)) {
        if (IrReceiver.decode()) {            // Verifica se há um comando IR recebido

            // Indica estar pronto para AUTO se recebe 1 (0x0) do controle
            if (IrReceiver.decodedIRData.command == 0x0) {
                Serial.println("Pronto");
                SerialBT.println("Pronto");
                ready = true;                 // Pronto para iniciar a movimentacao
                AnnihilationModeLeds();       // LEDs vermelhos para a sede de ser campeao
                blinkLED(3, 25);              // Pisca o LED builtin se recebe IR 1
            }

            // Inicia movimento AUTO se recebe 2 (0x1) do controle
            if (IrReceiver.decodedIRData.command == 0x1 && ready) {
                // Notifica a task de estrategia
                clearLeds();                  // Apaga LEDs enderecaveis
                xTaskNotifyGive(stratLutaHandle);
                vTaskDelete(NULL);            // Encerra esta task
            }
            IrReceiver.resume();              // Limpa o buffer IR
        }
    }
}

#pragma endregion

//===============================================================================================//
//=====================================//LOGICA DE COMBATE//=====================================//
//===============================================================================================//

#pragma region LOGICA DE COMBATE

void combatLogicTask(void *pvParameters) {
    for (;;) {                                // Define a logica de combate que sera ativada
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if(!running) continue;                   // Trava de seguranca da logica
        
//================================//Condicoes de mudanca de modo//===============================//

        // GERAL | Se dois ou mais sensores detectam o inimigo, desativa o modo furtivo e ataca
        // BUSCA | Se os avancos forem suficientes, desativa o modo furtivo e ataca
        if ((valueJsumoF + valueJsumoE + valueJsumoD >= 2) ||  
            (avancosIteracao > maxAvancosIteracao) || (avancosTotais > 5 * maxAvancosIteracao)
        ) {

            modoLuta = ataque;                // Entra no modo de Ataque
            modoFurtivo = false;              // Desativa o modo furtivo
            xTaskNotifyGive(swSensorHandle);  // Notifica a task para trocar os sensores
        }

//=======================================//Seleciona Modo//======================================//

        switch (modoLuta) {
            case ataque:
                modoAtaque();                 // Ataca o adversario de forma rapida
                break;
            case defesa:
                modoDefesa();                 // Ataca o adversario devagar
                break;
            case giro:
                modoGiro();                   // Acompanha o adversario sem andar pra frente
                break;
            case busca:
                modoBusca();                  // FSM | Acompanha o adversario com passos
                break;
            case quebrado:
                modoQuebrado();               // FSM | Acompanha o adversario em ziguezague
                break;
        }
    }
}

/*
!INFO | FSM (Finite State Machine) Cooperativa Nao-Bloqueante
-----------------------------------
A logica dos modos complexos e uma Maquina de Estados Finitos (FSM) cooperativa. A transicao entre
os estados e baseada em tempo ('millis()') ao inves de delays, garantindo que a funcao de modo 
retorne imediatamente a cada tick

Isso mantem a latencia do loop de controle principal proxima de zero, permitindo preempcao 
instantanea da estrategia atual por uma de maior prioridade baseada em novas leituras dos sensores
*/

#pragma endregion

//===============================================================================================//
//===========================================//SETUP//===========================================//
//===============================================================================================//

//===========================================//Timer//===========================================//

#pragma region SETUP TIMER

void setupTimer() {
    // Configurar o timer (grupo 0, timer 0)
    timer_config_t config = {
        .alarm_en = TIMER_ALARM_EN,           // Habilita o alarme
        .counter_en = TIMER_PAUSE,            // O contador comeca pausado
        .intr_type = TIMER_INTR_LEVEL,        // Tipo de interrupcao
        .counter_dir = TIMER_COUNT_UP,        // Contagem progressiva
        .auto_reload = TIMER_AUTORELOAD_EN,   // Recarrega o contador no alarme
        .divider = 80                         // Prescaler de 80
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);

    // Definir o valor inicial e o valor do alarme
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 300);
    
    // timer_isr_callback_add permite especificar o nucleo alocando de flags de interrupcao
    timer_isr_callback_add(
        TIMER_GROUP_0, 
        TIMER_0,                              // Timer usado
        readSensors,                          // Funcao chamada
        NULL,                                 // Argumentos para a ISR
        ESP_INTR_FLAG_IRAM                    // Flag para ISR rapida
    );

    // 4. Habilitar a interrupção e iniciar o timer
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_start(TIMER_GROUP_0, TIMER_0);
}

void timerInitTask(void *pvParameters) {
    setupTimer();                             // Funcao de configuracao criada no mesmo nucleo (0)
    vTaskDelete(NULL);                        // Auto-deleta
}

#pragma endregion

//======================================//Task de alocacao//=====================================//

#pragma region TASK ALOCACAO

void setupSensorTask() {

    xTaskCreatePinnedToCore(
        stopRobot,                            // Funcao da tarefa
        "StopRobot",                          // Nome da tarefa
        256 * 16,                             // Tamanho da pilha
        nullptr,                              // Parametros
        16,                                   // Prioridade 16
        &stopRobotHandle,                     // Handle
        PRO_CPU_NUM                           // Nucleo 0 onde a tarefa sera executada
    );

    xTaskCreatePinnedToCore(
        irMonitorTask,                        // Funcao da tarefa
        "IR_Monitor",                         // Nome da tarefa
        256 * 16,                             // Tamanho da pilha
        nullptr,                              // Parametros
        16,                                   // Prioridade 16
        NULL,                                 // Handle
        PRO_CPU_NUM                           // Nucleo 0 onde a tarefa sera executada
    );
    
    xTaskCreatePinnedToCore(
        switchSensor,                         // Funcao da tarefa
        "SwitchSensor",                       // Nome da tarefa
        256 * 16,                             // Tamanho da pilha
        nullptr,                              // Parametros
        15,                                   // Prioridade 15
        &swSensorHandle,                      // Handle
        PRO_CPU_NUM                           // Nucleo 0 onde a tarefa sera executada
    );

    xTaskCreatePinnedToCore(
        handleIRCommand,                      // Funcao da tarefa
        "HandleIRCommand",                    // Nome da tarefa
        256 * 16,                             // Tamanho da pilha
        nullptr,                              // Parametros
        14,                                   // Prioridade 14
        &IRCommandHandle,                     // Handle
        APP_CPU_NUM                           // Nucleo 1 onde a tarefa sera executada
    ); 

    xTaskCreatePinnedToCore(
        combatLogicTask,                      // Funcao da tarefa
        "SensorTask",                         // Nome da tarefa
        256 * 16,                             // Tamanho da pilha
        nullptr,                              // Parametros
        14,                                   // Prioridade 14
        &combatLogicHandle,                   // Handle
        APP_CPU_NUM                           // Nucleo 1 onde a tarefa sera executada
    );

    xTaskCreatePinnedToCore(
        timerInitTask,                        // Funcao da tarefa
        "TimerInit",                          // Nome
        256 * 8,                              // Tamanho da pilha
        NULL,                                 // Parametros
        14,                                   // Prioridade 14
        NULL,                                 // Handle
        PRO_CPU_NUM                           // Nucleo 0 onde a tarefa sera executada
    );

    vTaskDelay(pdMS_TO_TICKS(100));
}

#pragma endregion

//===============================================================================================//
//=====================================//FINALIZA O ARQUIVO//====================================//
//===============================================================================================//

#endif