#include <defines.hpp>     // Definicoes globais
#include <functions.hpp>   // Funcoes auxiliares
#include <rc_mode.hpp>     // Interpretacao do controle de PS4 no modo RC

#define LED_BUILTIN 2 

void setup(){
    Serial.begin(115200); // Comunicacao serial com baudrate 115200

    pinMode(LED_PIN, OUTPUT);
    directWriteLow(LED_PIN);
    
    pinMode(IR_RECIEVE_PIN, INPUT);

    pinMode(PAU_NEG_PIN, OUTPUT);
    pinMode(PAU_POS_PIN, OUTPUT);

    pinMode(RIGHT_NEG_PIN, OUTPUT);
    pinMode(RIGHT_POS_PIN, OUTPUT);

    pinMode(LEFT_NEG_PIN, OUTPUT);
    pinMode(LEFT_POS_PIN, OUTPUT);

}

void __init__(){

    // Comunicacao por IR
    IrReceiver.begin(IR_RECIEVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
    printActiveIRProtocols(&Serial);

    Serial.println("//=====//Setup IR feita//=====//");
    Serial.println("//=====//Selecione modo de luta//=====//");

    while (selecionouModo == naoSelecionado)
    {
        if (IrReceiver.decode()){ 
            // Inicia modo RC recebe 9 (0x8) do controle
            if (IrReceiver.decodedIRData.command == 0x8)
            {
                selecionouModo = radioControlado;
                IrReceiver.resume(); // Limpa o buffer IR
                Serial.println("//=====//Modo RC selecionado//=====//");
            }

            //Algum dia nessa parte o código vai mandar pro auto (ou não).

            IrReceiver.resume(); // Limpa o buffer IR
        }
    }
}

void loop()
{
    if (!inicializado)
    { // Configuracao dos modos de luta
        Serial.println("//=====//Modo RC selecionado//=====//");
        modoRC();
    }

    vTaskSuspend(NULL); // Suspende o loop pra nao consumir CPU desnecessaria, checar se é necessário
}