/*
 * mother_ship.h
 *
 *  Created on: Oct 12, 2016
 *      Author: superman
 */

#ifndef MOTHER_SHIP_H_
#define MOTHER_SHIP_H_

#include <stdbool.h>
#include <stdint.h>


// Initializes the mother ship
void mother_ship_init();

// Spawns a mother ship
void mother_ship_spawn();

// Moves the mother ship right
void mother_ship_move();

// Detects a bullet collision on the mother ship
bool mother_ship_detect_collision(uint32_t row, uint32_t col);


void mother_ship_draw(uint32_t color);

void mother_ship_points_blink();





#endif /* MOTHER_SHIP_H_ */
