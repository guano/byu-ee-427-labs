/*
 * bunkers.h
 * Taylor Cowley and Andrew Okazaki
 */

#ifndef BUNKERS_H_
#define BUNKERS_H_

#include <stdint.h>


// Initializes the bunkers - draws them to the screen
void bunkers_init(uint32_t * framePointer);

// Draws the bunkers to the screen
void bunkers_build(uint32_t * framePointer);

// Hits a random bunker in a random place
void bunkers_hit_rand_bunker(uint32_t * framePointer);

#endif /* BUNKERS_H_ */
