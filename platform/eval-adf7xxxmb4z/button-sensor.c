#include "lib/sensors.h"
#include "dev/button-sensor.h"

#include <signal.h>

#define JOYSTICK_PUSH  (!(P5 & BIT(5)))
#define JOYSTICK_RIGHT (!(P5 & BIT(4)))
#define JOYSTICK_DOWN  (!(P5 & BIT(3)))
#define JOYSTICK_LEFT  (!(P5 & BIT(2)))
#define JOYSTICK_UP    (!(P5 & BIT(1)))

const struct sensors_sensor button_sensor;

static int
value(int type)
{
  return JOYSTICK_PUSH;
}
static int
configure(int type, int c)
{
  switch(type) {
  case SENSORS_ACTIVE:
    /* TODO */
    return 1;
  }
  return 0;
}
static int
status(int type)
{
  switch(type) {
  case SENSORS_ACTIVE:
  case SENSORS_READY:
    return 0; /* TODO */
  }
  return 0;
}
SENSORS_SENSOR(button_sensor, BUTTON_SENSOR,
               value, configure, status);
