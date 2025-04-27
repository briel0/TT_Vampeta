# Serial [v1.0]
 Mini Sumô da Tamandutech - ARRUELA!

## Pastas e Arquivos
 ```
 Serial (AUTO)
 |--src
    |- serial.hpp
    |- serial.hpp
 ```

## Funções
 1. `void setup(const char *name)`: Inicializa o Bluetooth Serial 
 2. `bool enabled()`: Verifica se o Bluetooth Serial está ativado
 3. `int available()`: Verifica se o Bluetooth Serial está disponivel
 4. `void end()`: Fecha o Bluetooth Serial
 5. `int read()`: Le um caractere do Bluetooth Serial
 6. `void write(uint8_t c)`: Escreve um caractere do Bluetooth Serial
 7. `void flush()`: Limpa o Buffer do Serial
 8. `void ansi_cmd(const char *cmd)`: Executa um comando ANSI / ASCII
 9. `void color_cmd(const char *cmd)`: Executa com comando de Cor
 10. `void clear()`: Limpa o Console / Terminal
 11. `void printf(const char *fmt, ...)`: `printf` no Bluetooth Serial
 12. `void println<T>(T val)`: `println` no Bluetooth Serial
 13. `void print<T>(T val)`: `print` no Bluetooth Serial