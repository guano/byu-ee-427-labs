/*
 * tank.h
 * Taylor Cowley and Andrew Okazaki
 */

#ifndef TANK_H_
#define TANK_H_

#include <stdint.h>
#include <stdbool.h>

void tank_init();
// moves our tank left by a certain number of pixels
void tank_move_left(uint32_t * framePointer);
// moves our tank right by a certain number of pixels
void tank_move_right(uint32_t * framePointer);

// This simply draws the tank on the screen, where it is at now.
void tank_draw(uint32_t * framePointer, bool erase);

// Alives a shell and draws it to the screen
void tank_fire(uint32_t * framePointer);

// Moves the shell up on the screen
void tank_update_bullet(uint32_t * framePointer);

#endif /* TANK_H_ */
