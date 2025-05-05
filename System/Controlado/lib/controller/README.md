# Controller [v1.1]
 Mini Sumô da Tamandutech - ARRUELA!

## Pastas e Arquivos
 ```
 Controller (RC)
 |--src
    |- controller.hpp
    |- controller.hpp
 ```

## Funções
 1. `controller_t`: É uma estrutura que representa o controle de PS4
 2. `void setup(const char *mac)`: Inicializa o pareamento com o controle
 3. `bool is_connected()`: Verifica se o controle está conectado
 4. `bool disconnected()`: Verifica se o controle foi desconectado após ter sido inicializado com sucesso
 5. `controller_t create_snapshot()`: Captura todos os inputs do controle
 6. `void debug(char *out_buffer, const size_t out_size, const controller_t controller, const char *msg)`:  Mostra as informações do controle na string
 7. `void debug(const controller_t controller, const char *msg)`:  Mostra as informações do controle
