#include <Arduino.h>
#include <functional>
#include <array>
#include "macro.hpp"

#include "engine.hpp"
#include "controller.hpp"

#pragma region "Macro Source"
void macro_just_go(controller_t controller)
{
	while (controller.cross)
	{
		engine_move(ENGINE_FRONT_FULL, ENGINE_FRONT_FULL);
		controller = controller_create_snapshot();
	}
}
#pragma endregion "Macro Source"

#pragma region "Macro Engine"
struct macro_t
{
	std::function<void(controller_t)> func;
};

std::array<macro_t, 1> macros = {
	{macro_just_go},
};

void macro_load()
{
}

void macro_call()
{
	controller_t controller = controller_create_snapshot();
	for (macro_t macro : macros)
	{
		macro.func(controller);
	}
}
#pragma endregion "Macro Engine"
