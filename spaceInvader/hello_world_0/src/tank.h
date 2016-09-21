/*
 * tank.h
 *
 *  Created on: Sep 21, 2016
 *      Author: Taylor
 *  This header file contains everything you'll ever need to know or do about the tank. So yeah
 */

#ifndef TANK_H_
#define TANK_H_

#include <stdint.h>

// moves our tank left by a certain number of pixels
void move_left(uint16_t pixels);
// moves our tank right by a certain number of pixels
void move_right(uint16_t pixels);

#endif /* TANK_H_ */