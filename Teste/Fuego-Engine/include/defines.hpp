#ifndef defines_H
#define defines_H

#include <BluetoothSerial.h>

#include <Arduino.h>
#include <IRremote.hpp>


// Definição das variáveis do Bluetooth
BluetoothSerial SerialBT;
char BT = 'f';            // char recebida pelo bluetooth12
char strategy = 'f';    // Estratégia de início da luta (qualquer caracter)

// Definição do controle Sony para infravermelho
#define DECODE_SONY


// Definição das portas alocadas para motores, sensores, sensor IR, porta stby, led, etc.
#define a1 18  // motor direito trás
#define a2 19  // motor direito frente
#define b1 17  // motor esquerdo trás
#define b2 16  // motor esquerdo frente
#define jsumoE 33
#define jsumoF 32
#define jsumoD 34
#define IR 13
#define stby 5
#define led 2

// Se estamos usando TB66 devemos habilitar as portas PWM, caso estejamos usando o drive DRV devemos comentar as portas PWM.
#define pwmA 23
#define pwmB 4


/* Definição dos valores de leitura dos sensores (necessário caso haja sensores analógicos no robô, como linha ou sharp)
#define sharpvalue 600
#define sharpinicialview 100
*/

// Definição dos valores de velocidades parametrizadas 
#define frentao 255 // VALORES QUE SERIAM INTERESSANTES SEREM COLOCADOS NO DASHBOARD
#define frentinho 180
#define curva 100 // VALORES QUE SERIAM INTERESSANTES SEREM COLOCADOS NO DASHBOARD
#define ajuste_busca 80 // Quanto maior esse número maior é a abertura da curva (a cima de 400 nem vira direito)
/* Definição do tamanho do vetor do filtro de média móvel, para sensores analógicos
#define n 10*/


/* Variáveis globais para controle do robô */
// Definição das variaveis digitais dos sensores
bool valueJsumoE, valueJsumoD,valueJsumoF;
bool running = false;    // referente ao round 
//int  valueLineD,valueLineE;
int cont = 0;


// Se der erro no Bluetooth referente à falta de alguma definição (nunca tivemos problema aqui)
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Modo defesa do robô
bool defense = false;
int contador = 0;
unsigned long variavel = 0;
bool primeiro = true;
int delay_varredura = 100;
// Enum de direção
enum Direction
{
    esq,
    dir
};
Direction direction = esq;
#endif