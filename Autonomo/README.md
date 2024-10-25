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
 2. `void engine_begin()`: Inicializa os motores (os pinos em geral do robô)
 3. `void engine_standby(const bool mode)`: Liga / Desliga a corrente elétrica dos motores
 4. `void engine_move(const engine_t engine_left, const engine_t engine_right)`: Altera o movimento dos motores
 5. `void engine_stop()`: Para todos os motores imediatamente
 6. `void engine_debug(char *out_buffer, const size_t out_size, engine_t engine, const char *msg)`: Mostra as informações do motor na string
 7. `void engine_debug(const engine_t engine, const char *msg)`: Mostra as informações do motor

## Internal
 1. `void internal_begin()`: Inicializa os recursos internos
 2. `void internal_led(const bool mode)`: Liga / Desliga o led internos
 3. `void internal_setup_millis()`: Salva o novo tempo inicial
 4. `uint64_t internal_begin_millis()`: Retorna o tempo inicial
 5. `uint64_t internal_end_millis()`: Retorna o tempo final (o tempo atual)
 6. `uint64_t internal_delta_millis()`: Retorna a diferença entre o tempo final e o inicial
 7. `void internal_debug(char *out_buffer, const size_t out_size, const char *msg)`: Mostra as informações dos recursos internos na string
 8. `void internal_debug(const char *msg)`: Mostra as informações dos recursos internos

## Main
 1. `setup()`: Primeira parte do código que roda ao ligar o robô, serve para configurar
 2. `init()`: É o código que vai rodar antes do update
 3. `update()`: É o código que vai rodar em um loop
 4. `loop()`: É o loop geral do robô