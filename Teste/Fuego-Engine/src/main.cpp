#pragma region includes
#include <defines.hpp>
#include <move.hpp>
#include <parallel.hpp>
#include <functions.hpp>
#pragma endregion includes


void setup()
{
    setPins();
    digitalWrite(led, 0);
    digitalWrite(stby, 1);
    move('F',255,'T', 255);
}

void loop()
{
    vTaskDelay(1);
}