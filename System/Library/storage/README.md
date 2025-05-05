# Storage [v1.0]
 Mini Sumô da Tamandutech - ARRUELA!

## Pastas e Arquivos
 ```
 Storage (AUTO)
 |--src
    |- storage.hpp
    |- storage.hpp
 ```

## Funções
 1. `void setup()`: Inicializa o Storage 
 2. `void close()`: Finaliza o Storage
 3. `void write_buffer(const char *path, const uint8_t *buffer, const size_t len)`: Escrever buffer no arquivo do Storage
 4. `void append_buffer(const char *path, const uint8_t *buffer, const size_t len)`: Acrescentar buffer no arquivo do Storage
 5. `void read_buffer(const char *path, uint8_t *buffer, const size_t len)`: Ler buffer no arquivo do Storage
 6. `void write(const char *path, const T data)`: Escrever data no arquivo do Storage
 7. `void append(const char *path, const T data)`: Acrescentar data no arquivo do Storage
 8. `T read(const char *path)`: Ler data no arquivo do Storage
