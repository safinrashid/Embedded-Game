// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// 11/15/2021 
#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>



void Sound_Init(void);

void Sound_Start(const uint8_t *pt, uint32_t count);

void Sound_eat(void);
void Sound_powerup(void);
void Sound_killed(void);

#endif
