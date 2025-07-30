#include <Arduino.h>
#include <controller.hpp>
#include <engine.hpp>
#include <internal.hpp>
#include "main.hpp"

uint8_t loop_state = LOOP_STATE_INIT;

tt::engine_t engine_left = TT_ENGINE_DEFAULT;
tt::engine_t engine_right = TT_ENGINE_DEFAULT;

tt::controller_t controller;
uint8_t rc_state = RC_STATE_CAREFUL;
bool triangle = false;

/*
comentários provisórios para fins de compreensão inicial
*/

void setup(){

	Serial.begin(115200);
	Serial.printf(STRLN("Serial 115200!"));

	tt::internal::setup();
	tt::internal::set_led(false);
	Serial.printf(STRLN("Setup Internal!"));
	Serial.printf(STRLN("Mac Address: %s"), tt::internal::mac_address());
	vTaskDelay(512);

	tt::controller::setup(tt::internal::mac_address());
	Serial.printf(STRLN("Setup Controller!"));

	tt::engine::setup();
	tt::engine::set_standby(false);
	Serial.printf(STRLN("Setup Engine!"));
}

/*

essa função provavalmente é desnecessária
só é chamada na linha 116

void __init__(){
	tt::engine::init();
}

 */

void __update__(){

_restart_update:
	tt::internal::set_led(true);

	if (tt::controller::disconnected()){
		tt::internal::set_led(false);
		goto _loop_reset_engine;
	}

	controller = tt::controller::create_snapshot();

	switch (rc_state){
		
	case RC_STATE_CAREFUL:
		modifier_careful();
		break;

	case RC_STATE_NORMAL:
		modifier_normal();
		break;

	default:
		modifier_careful();
		break;
	}

	if(controller.triangle){

		Serial.printf(STRLN("(controller.triangle)"));
		if (!triangle){
			switch (rc_state){
			case RC_STATE_CAREFUL:
				rc_state = RC_STATE_NORMAL;
				break;

			case RC_STATE_NORMAL:
				rc_state = RC_STATE_CAREFUL;
				break;

			default:
				rc_state = RC_STATE_CAREFUL;
				break;
			}
		}
		triangle = true;
	}
	else{
		triangle = false;
	}

	if(controller.l1){
		Serial.printf(STRLN("(controller.l1)"));
		macro_ladinho(left);
		goto _restart_update;
	}

	if (controller.r1){
		Serial.printf(STRLN("(controller.r1)"));
		macro_ladinho(right);
		goto _restart_update;
	}

	if(controller.square){
		Serial.printf(STRLN("(controller.square)"));
		macro_curvinha(left);
		goto _restart_update;
	}

	if (controller.circle){
		Serial.printf(STRLN("(controller.circle)"));
		macro_curvinha(right);
		goto _restart_update;
	}

	if (controller.left)
	{
		Serial.printf(STRLN("(controller.left)"));
		macro_curvao(left);
		goto _restart_update;
	}

	if (controller.right){
		Serial.printf(STRLN("(controller.right)"));
		macro_curvao(right);
		goto _restart_update;
	}

	if (controller.cross){
		Serial.printf(STRLN("(controller.cross)"));
		behavior_just_go();
		goto _loop_update_engine;
	}

	if (controller.l2){
		Serial.printf(STRLN("(controller.l2)"));
		behavior_forward(TT_ENGINE_DIRECTION_BACK, controller.l2_value * 0.9);
		goto _loop_update_engine;
	}

	if (controller.r2){
		Serial.printf(STRLN("(controller.r2)"));
		behavior_forward(TT_ENGINE_DIRECTION_FRONT, controller.r2_value);
		goto _loop_update_engine;
	}

	if (controller.l_stick_x <= -STICK_TRIGGER){
		Serial.printf(STRLN("(controller.l_stick_x <= -STICK_TRIGGER)"));
		behavior_curve(TT_ENGINE_DIRECTION_BACK, TT_ENGINE_DIRECTION_FRONT);
		goto _loop_update_engine;
	}

	if (controller.l_stick_x >= STICK_TRIGGER){
		Serial.printf(STRLN("(controller.l_stick_x >= STICK_TRIGGER)"));
		behavior_curve(TT_ENGINE_DIRECTION_FRONT, TT_ENGINE_DIRECTION_BACK);
		goto _loop_update_engine;
	}

_loop_reset_engine:
	reset_engine();

_loop_update_engine:
	update_engine();
}

void loop(){
	switch (loop_state){
	case LOOP_STATE_INIT:

		tt::engine::init();

		loop_state = LOOP_STATE_UPDATE;
		break;
	case LOOP_STATE_UPDATE:
		__update__();
		break;
	default:
		break;
	}
}

void update_engine()
{
	Serial.printf("################################################################################################################################\n");
	tt::engine::debug(engine_left, "engine_left");
	tt::engine::debug(engine_right, "engine_right");
	tt::controller::debug(controller, "controller");
	tt::internal::debug("internal");
	tt::engine::move(engine_left, engine_right);
}

void reset_engine()
{
	tt::engine::stop();
	engine_left = TT_ENGINE_FRONT_STOP;
	engine_right = TT_ENGINE_FRONT_STOP;
	tt::internal::setup_millis();
}

void modifier_normal()
{
	const uint8_t min_v = TT_ENGINE_SPEED_SLOW(2);
	const uint8_t max_v = TT_ENGINE_SPEED_FULL;
	const uint8_t base_speed = static_cast<uint8_t>(TT_INTERNAL_BETWEEN((tt::internal::delta_millis() / 3) * 2 + min_v, min_v, max_v));
	engine_left.speed = base_speed;
	engine_right.speed = base_speed;
}

void modifier_careful()
{
	const uint8_t min_v = TT_ENGINE_SPEED_SLOW(3) + TT_ENGINE_SPEED_SLOW(4);
	const uint8_t max_v = TT_ENGINE_SPEED_SLOW(2) + TT_ENGINE_SPEED_SLOW(3);
	const uint8_t base_speed = static_cast<uint8_t>(TT_INTERNAL_BETWEEN((tt::internal::delta_millis() / 3) * 2 + min_v, min_v, max_v));
	engine_left.speed = base_speed;
	engine_right.speed = base_speed;
}

void behavior_forward(const uint8_t direction, const uint8_t speed_modifier)
{
	const uint8_t left_speed = map(speed_modifier, TT_ENGINE_SPEED_STOP, TT_ENGINE_SPEED_FULL,
								   TT_ENGINE_SPEED_STOP, static_cast<uint8_t>(engine_left.speed));
	const uint8_t right_speed = map(speed_modifier, TT_ENGINE_SPEED_STOP, TT_ENGINE_SPEED_FULL,
									TT_ENGINE_SPEED_STOP, static_cast<uint8_t>(engine_right.speed));
	engine_left = {direction, left_speed};
	engine_right = {direction, right_speed};
	if (controller.l_stick_x <= -STICK_TRIGGER)
	{
		engine_left.speed >>= 3;
	}
	else if (controller.l_stick_x >= STICK_TRIGGER)
	{
		engine_right.speed >>= 3;
	}
}

void behavior_curve(const uint8_t left_direction, const uint8_t righ_direction)
{
	engine_left.speed = TT_INTERNAL_BETWEEN(engine_left.speed >> 1, TT_ENGINE_SPEED_SLOW(2), TT_ENGINE_SPEED_SLOW(1));
	engine_left.speed = TT_INTERNAL_BETWEEN(engine_left.speed >> 1, TT_ENGINE_SPEED_SLOW(2), TT_ENGINE_SPEED_SLOW(1));
	engine_left.direction = left_direction;
	engine_right.direction = righ_direction;
}

void behavior_just_go()
{
	engine_left = TT_ENGINE_FRONT_FULL;
	engine_right = TT_ENGINE_FRONT_FULL;
}

void macro_curvinha(const direction_t direction) {
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(64), TT_ENGINE_FRONT(255));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(64));
		vTaskDelay(196);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}

void macro_curvao(const direction_t direction) {
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(64), TT_ENGINE_FRONT(255));
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(64));
		vTaskDelay(392);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}

void macro_ladinho(const direction_t direction) {
	if (direction == right)
	{
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(24), TT_ENGINE_FRONT(255));
		vTaskDelay(224);
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
	}
	else
	{
		tt::engine::move(TT_ENGINE_BACK_FULL, TT_ENGINE_FRONT_FULL);
		vTaskDelay(70);
		tt::engine::move(TT_ENGINE_FRONT(255), TT_ENGINE_FRONT(24));
		vTaskDelay(224);
		tt::engine::move(TT_ENGINE_FRONT_FULL, TT_ENGINE_BACK_FULL);
		vTaskDelay(70);
	}
}
