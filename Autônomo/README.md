# Autônomo (v1.0)
 Mini Sumô da Tamandutech - ARRUELA!

 ```
                   _____ ____________________ ____ ______________.____       _____
                  /  _  \\______   \______   \    |   \_   _____/|    |     /  _  \
                 /  /_\  \|       _/|       _/    |   /|    __)_ |    |    /  /_\  \
                /    |    \    |   \|    |   \    |  / |        \|    |___/    |    \
                \____|__  /____|_  /|____|_  /______/ /_______  /|_______ \____|__  /
                        \/       \/        \/                 \/         \/       \/






                                    ....::-==+++++++==--::....
                                  .-+***********+******+++++***+-.
                             .-+*******+++***+***+*++++++++++++++++++-.
                         .:+************+***+*+++++++++++++++++++++++**+=:.
                      .:=*#************************+++*******++++++++++++**=:.
                    .=*#***************************************+*+++++*+++****-.
                 ..+#*************************************+*******+****+*******#=..
                .*#***************************************+**********+************+.
              .+##********************************************+*******+*************=.
             -###**********************+=-::.:++****+-...:--=++++*********************:
           .+##*******************++=:.       -+**+++.       .:-=+*********************=.
          :###**###************++-.           .+++++-            .:=+********************.
         :%#****#*#*********++=.                                    .-++*****************#.
        -###***************+-..                                       .:+*****************#:
       -######***##******+=.                                            .-+****************#:
      :########*********+-.                                               .=*****************.
     .##########*******+.                                                  .=*****************.
    .*############****=.                                                     -+**************#=
    -%###########****+.                                                      .-+**************#:
   .############****+.                                                        .=+**************+
   -%############***-                                                          .+**************#:
   *##############*+.                                                           :**************#=
  .%#############**-                                                            .+***************.
  :%#############**.                                                             -**************#.
  =%#############*+                                                              :**************#-
  =%########*####*=                                                              .**************#-
  +%#############*=                                                              .***************-
  =%#############*+                                                              :**************#-
  -%##############*.                                                             -**************#:
  :%##############*:                                                             =**************#.
  .################=                                                            .****************.
   +%##############*:                                                           =***************=
   :%###############+.                                                         :***************#:
    *%###############=.                                                       .+***************+
    :%###############*-                                                      .+***************#.
     =################*=                                                    .+****************=
     .*%################=.                                                 :+****************+.
      .##################+:.                                             .-******************.
       :##################*=:.                                         .:+******************.
        .###################*+..                                     .:+*******************.
         .####################*+:                                  .=*********************.
          .+%####################*+-.                           .=+*********************=.
           .-#######################*+=-:.                ..:-=+***********************-.
             .*#########################***+=---::::::--=++++************************=.
               -#####################################*******************************:
                 -##############################**********************************:
                  .-################################***************************+:.
                    .:+###############################***********************=:.
                       .-+#############################*******************=-.
                          .-+*#########################***************+=:.
                              .-+######################************=:.
                                ....:=*#################*****+-:...
                                       ....::::------::::....
 ```

## Pastas e Arquivos
 ```
 Autonomo (AUTO)
 |--include
    |- main.hpp
 |--lib
    |--engine
       |- engine.cpp
       |- engine.hpp
    |--internal
       |- internal.cpp
       |- internal.hpp
    |--receiver
       |- receiver.cpp
       |- receiver.hpp
    |--sensor
       |- sensor.cpp
       |- sensor.hpp
    |--serial
       |- serial.cpp
       |- serial.hpp
    |--utilitie
       |- utilitie.cpp
       |- utilitie.hpp
 |--src
    |- main.cpp
 ```

## Sobre a Estrutura do Código
 Esse código é uma experiência de criar uma nova organização que tem como objetivo facilitar a manutenção.

# Código
 O código é separado em 4 principais áreas:
 1. `engine`
 2. `internal`
 3. `receiver`
 4. `sensor`
 5. `serial`
 6. `utilitie`
 7. `main`

## Engine
 1. `engine_t`: É uma estrutura que representa um motor (velocidade e direção)
 2. `void setup()`: Inicializa os motores (os pinos em geral do robô)
 3. `bool get_standby()`: Pega o estado (Ligado / Desligado) da corrente elétrica dos motores
 4. `void set_standby(const bool mode)`: Liga / Desliga a corrente elétrica dos motores
 5. `void init()`: Inicializa os motores (valores dos pinos)
 6. `void move(const engine_t engine_left, const engine_t engine_right)`: Altera o movimento dos motores
 7. `void stop()`: Para todos os motores imediatamente
 8. `void debug(char *out_buffer, const size_t out_size, engine_t engine, const char *msg)`: Mostra as informações do motor na string
 9. `void debug(const engine_t engine, const char *msg)`: Mostra as informações do motor

## Internal
 1. `void setup()`: Inicializa os recursos internos
 2. `bool get_led()`: Pega o estado (Ligado / Desligado) do led interno
 3. `void set_led(const bool mode)`: Liga / Desliga o led interno
 4. `void setup_millis()`: Salva o novo tempo inicial
 5. `uint64_t begin_millis()`: Retorna o tempo inicial
 6. `uint64_t end_millis()`: Retorna o tempo final (o tempo atual)
 7. `uint64_t delta_millis()`: Retorna a diferença entre o tempo final e o inicial
 8. `void debug(char *out_buffer, const size_t out_size, const char *msg)`: Mostra as informações dos recursos internos na string
 9. `void debug(const char *msg)`: Mostra as informações dos recursos internos

## Receiver
 1. `receiver_t`: É uma estrutura que representa os valores do infravermelho
 2. `void setup()`: Inicializa o sensor infravermelho
 3. `bool decode()`: Verifica se há algo a ser decodificado
 4. `void resume()`: Atualiza o novo valor do input 'disponível' (se ele existir)
 5. `uint16_t command()`: Pega o valor do input 'disponível'
 6. `bool update()`: Verifica se existe um novo valor e atualiza, retornando se havia algo novo ou não
 7. `receiver_t receiver()`: Pega o valor de `command` e converte para `receiver_t`
 8. `bool signal(receiver_t sig)`: Atualiza o valor e verifica se `receive` é igual a `sig`
 9. `void debug(char *out_buffer, const size_t out_size, sensor_t sensor, const char *msg)`: Mostra as informações do sensor infravermelho na string
 10. `void debug(sensor_t sensor, const char *msg)`: Mostra as informações do sensor infravermelho

## Sensor
 1. `sensor_t`: É uma estrutura que representa os valores dos sensores
 2. `void setup()`: Inicializa os sensores
 3. `sensor_t create_snapshot()`: Captura os valores dos sensores
 4. `void debug(char *out_buffer, const size_t out_size, sensor_t sensor, const char *msg)`: Mostra as informações dos sensores na string
 5. `void debug(sensor_t sensor, const char *msg)`: Mostra as informações dos sensores

## Serial
 1. `void setup(const char *name)`: Inicializa o Bluetooth Serial 
 2. `bool enabled()`: Verifica se o Bluetooth Serial está ativado
 3. `int available()`: Verifica se o Bluetooth Serial está disponivel
 4. `void end()`: Fecha o Bluetooth Serial
 5. `int read()`: Le um caractere do Bluetooth Serial
 6. `void write(uint8_t c)`: Escreve um caractere do Bluetooth Serial
 7. `void printf(const char *fmt, ...)`: `printf` no Bluetooth Serial
 8. `void println<T>(T val)`: `println` no Bluetooth Serial
 9. `void print<T>(T val)`: `print` no Bluetooth Serial

## Utilitie
 1. `void swap<T>(T &a, T &b)`: Inverte o valor das variáveis `a` e `b`
 2. `void swap<T, N>(T (&a)[N], T (&b)[N])`: Inverte o valor das variáveis `a` e `b`

## Main
 1. `setup()`: Primeira parte do código que roda ao ligar o robô, serve para configurar
 2. `init()`: É o código que vai rodar antes do update
 3. `update()`: É o código que vai rodar em um loop
 4. `loop()`: É o loop geral do robô

## Observação
 1. Nenhuma