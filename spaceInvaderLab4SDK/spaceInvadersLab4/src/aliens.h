/*
 * aliens.h
 *	Taylor Cowley and Andrew Okazaki
 */

#include <stdbool.h>
#include <stdint.h>
#ifndef ALIENS_H_
#define ALIENS_H_


#endif /* ALIENS_H_ */

void aliens_init(uint32_t * framePointer);	// Initializes the aliens
void aliens_move(uint32_t * framePointer);	// Moves the aliens
void aliens_left(uint32_t * framePointer);	// Moves aliens left
void aliens_right(uint32_t * framePointer);	// Move aliens right
void aliens_kill(uint32_t * framePointer);	// Kills a random alien
void alien_missle(uint32_t * framePointer);	// Shoots an alien bullet
void aliens_update_bullets(uint32_t * framePointer);	// Updates the bullets
bool aliens_detect_collision(uint32_t row,uint32_t col);// Detect collision w me
