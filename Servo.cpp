/*
 * Servo.cpp
 *
 *  Created on: Mar 4, 2013
 *      Author: Rasmus Raag
 *      Tallinn University of Technology
 */

#include "Servo.h"

#define GPIO_CTRL_PIN_0	0x1110
#define GPIO_CTRL_PIN_1	0x1120
#define GPIO_CTRL_PIN_2	0x1130
#define GPIO_CTRL_PIN_3	0x1140

#define GPIO_XTRA_PIN_0 0x1114
#define GPIO_XTRA_PIN_1 0x1124
#define GPIO_XTRA_PIN_2 0x1134
#define GPIO_XTRA_PIN_3 0x1144


Servo::Servo(dc1394camera_t _camera, uint8_t _gpio_pin_no,  uint16_t _position_us=1500, uint16_t _lower_limit_us=1000,
		uint16_t _center_us=1500, uint16_t _upper_limit_us=2000)
{
	camera = _camera;
	gpio_pin_no = _gpio_pin_no;
	position_us = _position_us;
	lower_limit_us = _lower_limit_us;
	center_us = _center_us;
	upper_limit_us = _upper_limit_us;

	if(camera == 0)
		throw "No camera attached to servo";
	if(gpio_pin_no > 3)
			throw "Servo pin number out of range";
	if(position_us > upper_limit_us || position_us < lower_limit_us)
				throw "Servo position out of range";

	uint16_t address = GPIO_CTRL_PIN_0 + gpio_pin_no*10;
	uint32_t data = 0x80000001;
	dc1394error_t err = dc1394_set_register(camera, address, data);
	if(err)
		throw "Cannot set camera registers";

	if(!setPosition(position))
		throw "Cannot set servo position";

	// enable pwm
	address = GPIO_CTRL_PIN_0 + gpio_pin_no*10;
	data = 0x8004ff01;
	dc1394error_t err = dc1394_set_register(camera, address, data);
	if(err)
		throw "Cannot enable PWM";
}
uint16_t Servo::getPosition() // in microseconds
{
	return position_us;
}
bool Servo::setPosition(uint16_t _position_us) // in microseconds
{
	if(_position_us<lower_limit_us || _position_us > upper_limit_us)
		return false;

	uint16_t address = GPIO_XTRA_PIN_0 + gpio_pin_no*10;
	uint32_t data = (uint32_t)position_us*1024/1000; // 1.024 MHz clock
	const uint32_t period = 20000*1024/1000; // hard-coded 20 ms period
	data = data<<16 | (period - data);
	dc1394error_t err = dc1394_set_register(camera, address, data);
	if(err)
		return false;

	position_us = _position_us;
	return true;
}
bool Servo::center()
{
	return setPosition(center_us);
}
