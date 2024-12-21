#ifndef parallel_H
#define parallel_H
#include <defines.hpp>
#include <move.hpp>

TaskHandle_t SensorTask;

// Função atrelada ao segundo núcleo do ESP32.
void FunctionSensorTask(void *pvParameters)
{
    // SerialBT.print("FunctionSensorTask running on core ");
    // SerialBT.println(xPortGetCoreID());
    for (;;)
    {
        if (running)
        {
            if (IrReceiver.decode())
            {
                IrReceiver.resume();
                if (IrReceiver.decodedIRData.command == 0x2)
                {   move('F', 0, 'F', 0);
                    ESP.restart();
                }
            }
            valueJsumoF = digitalRead(jsumoF);
            valueJsumoD = digitalRead(jsumoD);
            valueJsumoE = digitalRead(jsumoE);
            // Exemplo de implementação sensor linha com média movel.
            //valueLineD = moving_average(analogRead(lineD), vectorlineD);
            //valueLineE = moving_average(analogRead(lineE), vectorlineE);

            if (valueJsumoE)
            {
                direction = esq;
            }
            else if (valueJsumoD)
            {
                direction = dir;
            }
             else if (valueJsumoF)
            {
                
                variavel = millis();
            }
             else 
            {
                variavel = millis();
            }

        }
        vTaskDelay(1);
    }
}
#endif