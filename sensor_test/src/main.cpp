/*
 * To use these examples you need to connect the VL53L8CX satellite sensor directly to the Nucleo board with wires as explained below:
 * pin 1 (SPI_I2C_N) of the VL53L8CX satellite connected to pin GND of the Nucleo board
 * pin 2 (LPn) of the VL53L8CX satellite connected to pin A3 of the Nucleo board
 * pin 3 (NCS) not connected
 * pin 4 (MISO) not connected
 * pin 5 (MOSI_SDA) of the VL53L8CX satellite connected to pin D14 (SDA) of the Nucleo board
 * pin 6 (MCLK_SCL) of the VL53L8CX satellite connected to pin D15 (SCL) of the Nucleo board
 * pin 7 (PWREN) of the VL53L8CX satellite connected to pin D11 of the Nucleo board
 * pin 8 (I0VDD) of the VL53L8CX satellite not connected
 * pin 9 (3V3) of the VL53L8CX satellite not connected
 * pin 10 (1V8) of the VL53L8CX satellite not connected
 * pin 11 (5V) of the VL53L8CX satellite connected to 5V of the Nucleo board
 * GPIO1 of VL53L8CX satellite connected to A2 pin of the Nucleo board (not used)
 * GND of the VL53L8CX satellite connected to GND of the Nucleo board
 */

/* Includes ------------------------------------------------------------------*/

#include <vl53l8cx.h>

#define SerialPort Serial

#define LPN_PIN 4
#define PWREN_PIN 5
#define SDA_PIN 21
#define SCL_PIN 22

void print_result(VL53L8CX_ResultsData *Result);
void clear_screen(void);
void handle_cmd(uint8_t cmd);
void display_commands_banner(void);

// Components.
VL53L8CX tofsensor(&Wire, LPN_PIN);

bool EnableAmbient = false;
bool EnableSignal = false;
uint8_t res = VL53L8CX_RESOLUTION_4X4; //é o valor da resolução multiplicada, 16 nesse caso
char report[256];
uint8_t status;

/* Setup ---------------------------------------------------------------------*/
void setup(){

    // Enable PWREN pin if present
    if (PWREN_PIN >= 0){
        pinMode(PWREN_PIN, OUTPUT);
        digitalWrite(PWREN_PIN, HIGH);
        delay(10);
    }

    // Initialize serial for output.
    SerialPort.begin(460800);

    // Initialize I2C bus.
    Wire.begin(SDA_PIN, SCL_PIN);

    // Configure VL53L8CX component.
    tofsensor.begin(); //seta os pinos
    status = tofsensor.init(); //bagulhos loucos que fazem o sensor funfar

    // Start Measurementstofsensor0
    status = tofsensor.start_ranging();

    if(status == 0){
        Serial.println("O sensor iniciou corretamente.");
    }
    else{
        Serial.println("O sensor não iniciou corretamente.");
    }

}

void loop(){

    VL53L8CX_ResultsData Results;
    uint8_t NewDataReady = 0;

    do{
        status = tofsensor.check_data_ready(&NewDataReady);

    }while(!NewDataReady);

    Serial.println("Dados prontos");

    if ((!status) && (NewDataReady != 0)){
        status = tofsensor.get_ranging_data(&Results);
        print_result(&Results);
    }

    if (Serial.available() > 0){
        handle_cmd(Serial.read());
    }
    delay(1000);
}

void print_result(VL53L8CX_ResultsData *Result)
{
    int8_t i, j, k;
    uint8_t l, zones_per_line;
    uint8_t number_of_zones = res;

    zones_per_line = (number_of_zones == 16) ? 4 : 8;

    display_commands_banner();

    SerialPort.print("Cell Format :\n\n");

    for (l = 0; l < VL53L8CX_NB_TARGET_PER_ZONE; l++)
    {
        snprintf(report, sizeof(report), " \033[38;5;10m%20s\033[0m : %20s\n", "Distance [mm]", "Status");
        SerialPort.print(report);

        if (EnableAmbient || EnableSignal)
        {
            snprintf(report, sizeof(report), " %20s : %20s\n", "Signal [kcps/spad]", "Ambient [kcps/spad]");
            SerialPort.print(report);
        }
    }

    SerialPort.print("\n\n");

    for (j = 0; j < number_of_zones; j += zones_per_line)
    {
        for (i = 0; i < zones_per_line; i++)
        {
            SerialPort.print(" -----------------");
        }
        SerialPort.print("\n");

        for (i = 0; i < zones_per_line; i++)
        {
            SerialPort.print("|                 ");
        }
        SerialPort.print("|\n");

        for (l = 0; l < VL53L8CX_NB_TARGET_PER_ZONE; l++)
        {
            // Print distance and status
            for (k = (zones_per_line - 1); k >= 0; k--)
            {
                if (Result->nb_target_detected[j + k] > 0)
                {
                    snprintf(report, sizeof(report), "| \033[38;5;10m%5ld\033[0m  :  %5ld ",
                             (long)Result->distance_mm[(VL53L8CX_NB_TARGET_PER_ZONE * (j + k)) + l],
                             (long)Result->target_status[(VL53L8CX_NB_TARGET_PER_ZONE * (j + k)) + l]);
                    SerialPort.print(report);
                }
                else
                {
                    snprintf(report, sizeof(report), "| %5s  :  %5s ", "X", "X");
                    SerialPort.print(report);
                }
            }
            SerialPort.print("|\n");

            if (EnableAmbient || EnableSignal)
            {
                // Print Signal and Ambient
                for (k = (zones_per_line - 1); k >= 0; k--)
                {
                    if (Result->nb_target_detected[j + k] > 0)
                    {
                        if (EnableSignal)
                        {
                            snprintf(report, sizeof(report), "| %5ld  :  ", (long)Result->signal_per_spad[(VL53L8CX_NB_TARGET_PER_ZONE * (j + k)) + l]);
                            SerialPort.print(report);
                        }
                        else
                        {
                            snprintf(report, sizeof(report), "| %5s  :  ", "X");
                            SerialPort.print(report);
                        }
                        if (EnableAmbient)
                        {
                            snprintf(report, sizeof(report), "%5ld ", (long)Result->ambient_per_spad[j + k]);
                            SerialPort.print(report);
                        }
                        else
                        {
                            snprintf(report, sizeof(report), "%5s ", "X");
                            SerialPort.print(report);
                        }
                    }
                    else
                    {
                        snprintf(report, sizeof(report), "| %5s  :  %5s ", "X", "X");
                        SerialPort.print(report);
                    }
                }
                SerialPort.print("|\n");
            }
        }
    }
    for (i = 0; i < zones_per_line; i++)
    {
        SerialPort.print(" -----------------");
    }
    SerialPort.print("\n");
}

void toggle_resolution(void)
{
  status = tofsensor.stop_ranging();

  switch (res) {
    case VL53L8CX_RESOLUTION_4X4:
      res = VL53L8CX_RESOLUTION_8X8;
      break;

    case VL53L8CX_RESOLUTION_8X8:
      res = VL53L8CX_RESOLUTION_4X4;
      break;

    default:
      break;
  }
  status = tofsensor.set_resolution(res);
  status = tofsensor.start_ranging();
}

void toggle_signal_and_ambient(void)
{
  EnableAmbient = (EnableAmbient) ? false : true;
  EnableSignal = (EnableSignal) ? false : true;
}

void clear_screen(void)
{
  snprintf(report, sizeof(report), "%c[2J", 27); /* 27 is ESC command */
  SerialPort.print(report);
}

void display_commands_banner(void)
{
  snprintf(report, sizeof(report), "%c[2H", 27); /* 27 is ESC command */
  SerialPort.print(report);

  Serial.print("53L8A1 Simple Ranging demo application\n");
  Serial.print("--------------------------------------\n\n");

  Serial.print("Use the following keys to control application\n");
  Serial.print(" 'r' : change resolution\n");
  Serial.print(" 's' : enable signal and ambient\n");
  Serial.print(" 'c' : clear screen\n");
  Serial.print("\n");
}

void handle_cmd(uint8_t cmd)
{
  switch (cmd) {
    case 'r':
      toggle_resolution();
      clear_screen();
      break;

    case 's':
      toggle_signal_and_ambient();
      clear_screen();
      break;

    case 'c':
      clear_screen();
      break;

    default:
      break;
  }
}