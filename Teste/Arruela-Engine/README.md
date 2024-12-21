# TT_MiniSumo_Arruela (v1.0)
 Mini Sumo da Tamandutech - ARRUELA!

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
 6. `main`

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

## Main
 1. `setup()`: Primeira parte do código que roda ao ligar o robô, serve para configurar
 2. `init()`: É o código que vai rodar antes do update
 3. `update()`: É o código que vai rodar em um loop
 4. `loop()`: É o loop geral do robô
