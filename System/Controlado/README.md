# Controlado (RC) [v1.0]
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
 Controlado (RC)
 |--include
    |- main.hpp
 |--lib
    |--controller
       |- controller.cpp
       |- controller.hpp
    |--engine
       |- engine.cpp
       |- engine.hpp
    |--internal
       |- internal.cpp
       |- internal.hpp
    |--utilitie
       |- utilitie.cpp
       |- utilitie.hpp
 |--src
    |- main.cpp
 ```

## Sobre a Estrutura do Código
 Esse código é uma experiência de criar uma nova organização que tem como objetivo facilitar a manutenção.

# Emparelhando o controlador PS4 com o ESP32:
[Controller Pairing Guide](https://github.com/darthcloud/BlueRetro/wiki/Controller-pairing-guide)

## Alterar/Pegar o MAC Address do Controle PS4
 Quando um controlador PS4 é 'emparelhado' com um console PS4, significa apenas que ele armazenou o endereço MAC Bluetooth do console, que é o único dispositivo ao qual o controlador se conectará. Normalmente, esse emparelhamento acontece quando você conecta o controlador ao console PS4 usando um cabo USB e pressiona o botão PS. Isso inicia a gravação do endereço MAC do console no controlador.

 Portanto, se você deseja conectar seu controlador PS4 ao ESP32, você precisa descobrir qual é o endereço MAC Bluetooth do seu console PS4 e definir o endereço do ESP32 para ele ou alterar o endereço MAC armazenado no controlador PS4.

 Seja qual for o caminho escolhido, você pode querer uma ferramenta para ler e/ou gravar o endereço MAC atualmente emparelhado do controlador PS4. Você pode tentar usar [sixaxispairer](https://github.com/user-none/sixaxispairer) ou [SixaxisPairTool](https://sixaxispairtool.en.lo4d.com/windows) para essa finalidade.

## Pegar o MAC Address do ESP32
 [ESP32: Erase Flash Memory (Factory Reset)](https://randomnerdtutorials.com/esp32-erase-flash-memory/)
 [esptool.py](https://github.com/espressif/esptool)

 Existem alguns bugs no ESP32 que se resolvem limpando a memória flash com `erase_flash`, e algumas informações aparecem ao fazer isso, como o MAC Address. Porém, existe a opção para pegar as informações de segurança do ESP com `get_security_info`, que mostra o MAC Address do ESP32 sem limpar a memória flash.

 Instalar esptool
 ```
 pip install setuptools
 pip install esptool
 ```

 Limpar a Memória Flash
 ```
 python -m esptool --chip esp32 erase_flash
 ```

 Pegar o MAC Address
 ```
 python -m esptool --chip esp32 get_security_info
 ```

# Código
 O código é separado em três principais áreas:
 1. `source`
 2. `include`
 3. `lib`

## Main
 1. `setup()`: Primeira parte do código que roda ao ligar o robô, serve para configurar
 2. `__init__()`: É o código que vai rodar antes do update
 3. `__update__()`: É o código que vai rodar em um loop
 4. `loop()`: É o loop geral do robô