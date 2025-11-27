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
    // LEDs enderecaveis
    setupLeds();                // Configura os LEDs enderecaveis
    validaSetup(0, 0, 0, 0, 0); // 5 vermelhos e 0 verdes

    // Comunicacao por IR
    IrReceiver.begin(IR_RECIEVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
    printActiveIRProtocols(&Serial);
    Serial.println("//=====//Setup IR feita//=====//");
    validaSetup(1, 0, 0, 0, 0); // 4 vermelhos e 1 verde

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
                validaSetup(1, 1, 1, 1, 1); // 1 vermelho e 4 verdes
            }

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

    vTaskSuspend(NULL); // Suspende o loop pra nao consumir CPU desnecessaria
}