/*
 * Servo.h
 *
 *  Created on: Mar 4, 2013
 *      Author: Rasmus Raag
 *      Tallinn University of Technology
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <dc1394/dc1394.h>

class Servo
{
	dc1394camera_t *camera;
	uint16_t lower_limit_us, center_us, upper_limit_us, position_us;
	uint8_t gpio_pin_no;
public:
	Servo(dc1394camera_t *camera, uint8_t gpio_pin_no,  uint16_t position_us=1500,
			uint16_t lower_limit_us=1000, uint16_t center_us=1500, uint16_t upper_limit_us=2000);
	uint16_t getPosition(); // in microseconds
	bool setPosition(uint16_t position_us); // in microseconds
	bool center();
};


#endif /* SERVO_H_ */
