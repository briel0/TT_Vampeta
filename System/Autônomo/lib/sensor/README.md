# Sensor [v1.0]
 Mini Sumô da Tamandutech - ARRUELA!

## Pastas e Arquivos
 ```
 Sensor (AUTO)
 |--src
    |- sensor.hpp
    |- sensor.hpp
 ```

## Funções
 1. `sensor_t`: É uma estrutura que representa os valores dos sensores
 2. `void setup()`: Inicializa os sensores
 3. `sensor_t create_snapshot()`: Captura os valores dos sensores
 4. `void debug(char *out_buffer, const size_t out_size, sensor_t sensor, const char *msg)`: Mostra as informações dos sensores na string
 5. `void debug(sensor_t sensor, const char *msg)`: Mostra as informações dos sensores
