# Engine [v1.0]
 Mini Sumô da Tamandutech - ARRUELA!

## Pastas e Arquivos
 ```
 Engine (AUTO / RC)
 |--src
    |- engine.hpp
    |- engine.hpp
 ```

## Funções
 1. `engine_t`: É uma estrutura que representa um motor (velocidade e direção)
 2. `void setup()`: Inicializa os motores (os pinos em geral do robô)
 3. `bool get_standby()`: Pega o estado (Ligado / Desligado) da corrente elétrica dos motores
 4. `void set_standby(const bool mode)`: Liga / Desliga a corrente elétrica dos motores
 5. `void init()`: Inicializa os motores (valores dos pinos)
 6. `void move(const engine_t engine_left, const engine_t engine_right)`: Altera o movimento dos motores
 7. `void stop()`: Para todos os motores imediatamente
 8. `void debug(char *out_buffer, const size_t out_size, engine_t engine, const char *msg)`: Mostra as informações do motor na string
 9. `void debug(const engine_t engine, const char *msg)`: Mostra as informações do motor
