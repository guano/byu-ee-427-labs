/*
 * sound.h
 * Taylor Cowley and Andrew Okazaki
 */

void sound_run();					// Sends data to the sound card
void sound_init_AC_97();			// Inits the sound card
void sound_init_tank();				// Play the tank fire sound
void sound_init_explosion();		// Play the tank explode sound
void sound_init_alienIn();			// Play the alien sound 1
void sound_init_alienOut();			// Play the alien sound 2
void sound_init_alienKill();		// Play the alien death sound
void sound_init_motherShipHigh();	// Not currently used
void sound_init_motherShipLow();	// Play the mother ship sound
void sound_vol_up();				// Makes the volume go up
void sound_vol_down();				// Makes the volume go down
void sound_shoot();					// Play the sound when the tank shoots
void sound_motherShipStop();		// Stop playing the mothership sound
