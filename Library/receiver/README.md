# Receiver (v1.0)
 Mini Sumô da Tamandutech - ARRUELA!

## Pastas e Arquivos
 ```
 Receiver (AUTO)
 |--src
    |- receiver.hpp
    |- receiver.hpp
 ```

## Funções
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