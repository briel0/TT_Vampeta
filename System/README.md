# System (SYS) [v1.1]
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
    |--storage
       |- storage.cpp
       |- storage.hpp
    |--utilitie
       |- utilitie.cpp
       |- utilitie.hpp
 |--src
    |- main.cpp
 ```

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

## Sobre a Estrutura do Codigo
 Esse código é uma experiência de criar uma nova organização que tem como principio facilitar a manutenção.

# Configuração de Ambiente
 Para o __Arruela__ __v1.0__ usamos apenas o __PlatformIO__ com o __VS CODE__, migramos o __Controlado (RC)__ do __Arduino IDE__ para o __PlatformIO__ na verção __v1.0__

## Git & GitHub
 1. Site do [GitHub Desktop](https://github.com/apps/desktop)
 2. Site do [GitHub CLI](https://cli.github.com/)
 3. Site do [Git](https://git-scm.com)

 ```cmd
 winget install GitHub.GitHubDesktop
 winget install GitHub.cli
 winget install Git.Git
 ``` 

## Visual Studio Code
 1. Site do [VS Code](https://code.visualstudio.com/)
 2. [Documentação do VS Code](https://code.visualstudio.com/docs)

 ```cmd
 winget install Microsoft.VisualStudioCode
 ``` 

## Arduino IDE 2
 1. Site do [Arduino IDE 2](https://www.arduino.cc/en/software/)
 2. [Documentação do Arduino IDE 2](https://docs.arduino.cc/software/ide/#ide-v2)

 ```cmd
 winget install ArduinoSA.IDE.stable
 ``` 

## CoreUtils
 1. Site do [CoreUtils](https://github.com/uutils/coreutils)
 2. Baixe o [CoreUtils](https://github.com/uutils/coreutils/releases/)

 ```
 winget install uutils.coreutils
 ```

## BinUtils
 1. Site do [BinUtils](https://winlibs.com/)
 2. Baixe o [BinUtils](https://winlibs.com/#download-release)

 ```
 winget install BrechtSanders.WinLibs.POSIX.UCRT
 ```

## Zip Tools
 1. Site do [WinRAR](https://www.win-rar.com/start.html?&L=0)
 2. Site do [7-zip](https://www.7-zip.org/)

 ```
 winget install RARLab.WinRAR
 winget install 7zip.7zip
 ```

## Python Tools
 1. Site do [Python](https://www.python.org/)
 2. Baixe o [Python](https://www.python.org/downloads/)

 ```
 winget install Python.Python.3.13
 pip install setuptoola
 pip install esptool
 ```

## Pair Tools
 1. Site do [SixaxisPairTool](https://sixaxispairtool.en.lo4d.com/windows)
 ```cmd
 ```

## PlatformIO
 1. Site do [PlatformIO](https://platformio.org)
 2. Baixe o [PlatformIO IDE for VSCode](https://platformio.org/install/ide?install=vscode)

 ```cmd
 wget https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
 python get-platformio.py
 rm get-platformio.py
 ```

## Tamandutech Core Collections (TTCC)
 ```cmd
 ```

## ESP32 SDK
 1. [Multiprocessamento no ESP32](https://embarcados.com.br/serie/multiprocessamento-no-esp32/)
 2. [ESP32 API Reference](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/)
 
## Observação
 1. Usar o `tt::serial::end()` causa um BUG que faz com que o robô não ande (provávelmente erro com relação à memória ou task)
 2. Quando `direction1 != direction2`, as direções de ambos os robôs invertem, sla o pq...
