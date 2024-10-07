#include <stdint.h>

struct sensor_t
{
	int left;
	int front;
	int right;
};

void sensor_begin();
sensor_t sensor_create_snapshot();
// void sensor_task(void *pvParameters);