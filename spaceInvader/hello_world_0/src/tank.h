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
#include <stdbool.h>

void init_tank();
// moves our tank left by a certain number of pixels
void move_left(unsigned int * framePointer);
// moves our tank right by a certain number of pixels
void move_right(unsigned int * framePointer);

// This simply draws the tank on the screen, where it is at now.
void draw_tank(unsigned int * framePointer, bool erase);

void fire_tank(unsigned int * framePointer);

void update_shell(unsigned int * framePointer);

#endif /* TANK_H_ */
