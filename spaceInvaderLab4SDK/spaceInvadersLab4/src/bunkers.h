/*
 * bunkers_new.h
 *  Taylor Cowley and Andrew Okazaki
 */

#ifndef BUNKERS_H_
#define BUNKERS_H_

#include <stdint.h>
#include <stdbool.h>
// inits the bunkers
void bunkers_init(uint32_t * framePointer);

// Draws the bunkers
void bunkers_build(uint32_t * framePointer);

// For debugging
void bunkers_debug_print();

// Have I been hit?
bool bunkers_detect_collision(uint32_t row, uint32_t col, bool forceDestroy);

#endif /* BUNKERS_NEW_H_ */
