/*
 * interface.h
 * Taylor Cowley and Andrew Okazaki
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <stdbool.h>

// adds a value to the score
void interface_increment_score(uint32_t incrementor);

//Initialize entire board
void interface_init_board(uint32_t * framePointer);

// The tank has been hit
void interface_kill_tank();

// Our game over screen :)
void interface_game_over();

// Our success screen
void interface_success();

// Draws the mother ship points that you scored.
void interface_alien_ship_points(uint32_t mother_ship_points, uint32_t col_loc, bool erase);

#endif /* INTERFACE_H_ */

