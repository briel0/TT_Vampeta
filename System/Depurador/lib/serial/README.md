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
 4. `void wait()`: Espera até novos dados serem adicionados no Stream de Input
 5. `void erase()`: Apaga o restante da Stream de Input
 6. `void end()`: Fecha o Bluetooth Serial
 7. `int read()`: Le um caractere do Bluetooth Serial
 8. `void write(uint8_t c)`: Escreve um caractere do Bluetooth Serial
 9. `void flush()`: Limpa o Buffer do Serial
 10. `void ansi_cmd(const char *cmd)`: Executa um comando ANSI / ASCII
 11. `void color_cmd(const char *cmd)`: Executa com comando de Cor
 12. `void clear()`: Limpa o Console / Terminal
 13. `void printf(const char *fmt, ...)`: `printf` no Bluetooth Serial
 14. `void scanf(const char *fmt, ...)`: `scanf` no Bluetooth Serial
 15. `String input()`: `input` no Bluetooth Serial
 16. `void println<T>(T val)`: `println` no Bluetooth Serial
 17. `void print<T>(T val)`: `print` no Bluetooth Serial
