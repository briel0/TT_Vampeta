
#pragma region includes
#include <defines.hpp>
#include <move.hpp>
#include <parallel.hpp>
#include <functions.hpp>
#pragma endregion includes


void setup()
{
    setPins();
    initiateIR();
    initiateBT();

    xTaskCreatePinnedToCore(FunctionSensorTask, "SensorTask", 10000, NULL, 1, &SensorTask, 0);
    delay(500);
    // FIM ESPIRITUAL DO SETUP

    while (BT != '0')
    { // Inicio quando bluetooth recebe o char '0'
        if (SerialBT.available())
        {
            BT = SerialBT.read();
            SerialBT.println(BT);
        }
        if (BT == '<')
        { // definir para que lado será toda a programação
            direction = esq;
        }
        else if (BT == '>')
        {
            direction = dir;
        }
        else if (BT == 'D')
        {
            defense = true;
        }
        else if (BT == 'A')
        {
            defense = false;
        }
        else if (BT == 'C')
        { // char 'C' checa as estratégias
            SerialBT.println("Check");
            SerialBT.print("Estratégia: ");
            SerialBT.println(strategy);
            SerialBT.print("Direction: ");
            if (direction == dir)
                SerialBT.println("direita");
            else
                SerialBT.println("esquerda");
            SerialBT.print("modo: ");
            if (defense)
                SerialBT.println("defesa");
            else
                SerialBT.println("ataque");
        }
        else if (BT == 'S')
        { // Char 'S'teste sensor
            testSensors();
        }
        else if (BT == 'M')
        { // Char 'M'teste motor com sensor
            testMotorsWithSensors();
        }
        else if (BT == 'L')
        { // Char 'L' test loop
            testLoop();
        }
        else if (BT == '0')
        {
            
            break;
        }
        else
        {
            if (!(BT == 13 || BT == 10 || (BT > 48 && BT < 58)))
            { // Se não for ENTER ou número altera a estratégia inicial
                strategy = BT;
            }
        }
    }
    // FIM TEÓRICO DO SETUP

    bool ready = false;
    while (true)
    {
        if (IrReceiver.decode())
        {
            IrReceiver.resume();
            if (IrReceiver.decodedIRData.command == 0x0)
            {
                SerialBT.print("Pr");
                ready = true;
                for (int i = 0; i < 3; i++)
                {
                    digitalWrite(led, 1);
                    delay(50);
                    digitalWrite(led, 0);
                    delay(50);
                    if (IrReceiver.decode())
                    {
                        IrReceiver.resume();
                        if (IrReceiver.decodedIRData.command == 0x1 && ready)
                        {
                            digitalWrite(led, 0);
                            digitalWrite(stby, 1);
                            break;
                        }
                    }
                }
            }

            if (IrReceiver.decodedIRData.command == 0x1 && ready)
            {
                digitalWrite(led, 0);
                digitalWrite(stby, 1);
                break;
            }
        }
    }
    // LIMITE DO FIM DO SETUP

    SerialBT.println("COMECOUUUUU");
    variavel = millis();
    running = true;

    // controle para acelerar, não entendi como se faz 
    // int velocidadeInicial = 0;
    // int valorDelay = 10;
    // int valorDeIncremento = (255 - velocidadeInicial) / (220 / valorDelay);

    switch (strategy)
    {
     case 'a' : // frentao
       // SerialBT.print("FRENTAAO");
       for (int i =0; i < 255; i+= 10 )
       {        
            move('F',i,'F', i);
            delay(5);
       }
        move('F',255,'F', 255);
        functionDelay(300);
        break;
        //SerialBT.print("acabou loop");

    case 'b': // curvinha
        if (direction == dir)
        {
            move('F',255,'T', 255);
            delay(40);
            move('F',100,'F', 255);
            delay(500);
            move('T',255,'F', map(255, 0, 255, 0, 230));
            delay(100);
            direction = esq;
            SerialBT.println("Curvao");
        }
        else
        {
            move('T',255,'F', 255);
            delay(40);
            move('F',255,'F', 100);
            delay(500);
            move('T',255,'F', 255);
            delay(100);
            direction = dir;
            SerialBT.println("Curvao");
        }
        break; 
    
    case 'B': // curvao
        if (direction == dir)
        {
            move('F',255,'T', 255);
            delay(40);
            move('F',128,'F', 255);
            delay(500);
            move('T',255,'F', map(255, 0, 255, 0, 230));
            delay(100);
            direction = esq;
            SerialBT.println("Curvao");
        }
        else
        {
            move('T',255,'F', 255);
            delay(40);
            move('F',255,'F', 128);
            delay(500);
            move('T',255,'F', 255);
            delay(100);
            direction = dir;
            SerialBT.println("Curvao");
        }
        break;
    
    case 'e': // costas   - só gira 180º
       if (direction == esq)
        {
            move('T',255,'F', 255);
            delay(100);
            move('F',255,'F', 100);
            delay(500);
            move('F',255,'T', map(255, 0, 255, 0, 255));
            delay(100);
            direction = dir;
            SerialBT.println("Curvao");
        }
        else
        {
            move('F',255,'T', 255);
            delay(100);
            move('F',255,'F', 100);
            delay(500);
            move('F',255,'T', 255);
            delay(100);
            direction = esq;
            SerialBT.println("Curvao");
        }
        break;

    case 'k': // frentao sem delay      
        move('F',255,'F', 255);
        delay(250);
        break;

    default: 
        break;
    }
    // ESP.restart();
}

void loop()
{   
    if(defense)
    {
        if (valueJsumoF && valueJsumoD && valueJsumoE &&
        (IrReceiver.decodedIRData.command != 0x0||
         IrReceiver.decodedIRData.command != 0x1||
         IrReceiver.decodedIRData.command != 0x2))
        {
            for (int iter = 0; iter < 255; iter += 10)
            {        
                 move('F', iter, 'F', iter);
                 delay(5);
            }
            move('F', 255, 'F', 255);
            delay(200);
        }
        // CASO NÃO FUNCIONE, DELETAR "IF" ACIMA E TORNAR "ELSE IF" ABAIXO COMO IF
        else if(valueJsumoF)
        {
            if(contador < 2048)
            {
                SerialBT.println(contador);
                move('F',0,'F',0);
                delay(10);
                contador += 10;
            }
            else
            {
                move('F',50,'F',50);
                delay(10);
                if(IrReceiver.decodedIRData.command != 0x0||IrReceiver.decodedIRData.command != 0x1||IrReceiver.decodedIRData.command != 0x2)
                {
                    if(valueJsumoF && valueJsumoD)
                    {
                        defense = false;
                    }
                    else if(valueJsumoF && valueJsumoE)
                    {
                        defense = false;
                    }
                    else if(valueJsumoF && valueJsumoE && valueJsumoD)
                    {
                        defense = false;
                    }
                    else
                    {
                        move('F',255,'F',255);
                        delay(200);
                        defense = false;
                    }
                }
            }
        }
        else if(valueJsumoE &&(IrReceiver.decodedIRData.command != 0x0||IrReceiver.decodedIRData.command != 0x1||IrReceiver.decodedIRData.command != 0x2))
        {
            move('T',100,'F',100);
            contador = 0;
            delay(10);
        }
        else if(valueJsumoD &&(IrReceiver.decodedIRData.command != 0x0||IrReceiver.decodedIRData.command != 0x1||IrReceiver.decodedIRData.command != 0x2))
        {
            move('F',40,'T',40);
            contador = 0;
            delay(10);
        }
        else if (direction== esq)
        {
            if (primeiro)
            {
                move('T',40,'F',40);
                contador = 0;
                delay(50);
                direction = dir;
                primeiro = false;
            }
            else
            {
                move('T',40,'F',40);
                contador = 0;
                delay(100);
                direction = dir; 
            }         
        }
        else if (direction== dir)
        {
            if (primeiro)
            {
                move('F',40,'T',40);
                contador = 0;
                delay(50);
                direction = esq;
                primeiro = false;
            }
            else
            {
                move('F',40,'T',40);
                contador = 0;
                delay(100);
                direction = esq;
            }
        }
    }

    else if(!defense){

        SerialBT.print("entrou no ataque");
        if(valueJsumoF){
            move('F',frentao,'F',frentao);
            variavel = millis();
            delay(10);
            
        }
        
        else
        {
            
            if (valueJsumoD)
            {   
                if (((curva)/(1+((millis()-variavel)/ajuste_busca))) < 10)
                {
                    move('F',curva,'T',curva);
                    delay(10);
                } 
                else{
                    move('F',curva,'F', ((curva)/(1+((millis()-variavel)/ajuste_busca))));
                    direction= dir;
                    delay(10);
                }
                
                
            }
            else if (valueJsumoE)
            {
                if (((curva)/(1+((millis()-variavel)/ajuste_busca))) < 10)
                {
                    move('T',curva,'F',curva);
                    delay(10);
                    
                }
                else{
                    move('F',((curva)/(1+((millis()-variavel)/ajuste_busca))),'F',curva);
                    delay(10);
                    direction= esq;
                }                
                
            }
            else if (direction== esq){
                move('T',curva,'F', curva);
                
                
            }
            else if (direction== dir){
                move('F',curva,'T', curva);
                
                
            }
        }
    }

    vTaskDelay(1);
}