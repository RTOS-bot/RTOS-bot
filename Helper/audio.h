#ifndef AUDIO_H
#define AUDIO_H

#include "MKL25Z4.h"     
#include "core.h"
               

#define PORT_AUDIO 		PORTE
#define NUM_PINS_AUDIO	1
#define PTE20_PIN 20

#define TPM_AUDIO	TPM1
#define TPM_CHANNEL0 0

#define NUM_CHANNELS_AUDIO 1

#define AUDIO_MOD 0
#define AUDIO_ALT 3


#define FREQ_TO_MOD(x)   (375000 / x)
#define FREQ_TO_C0V(x)   (375000 / x / 4)


#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988


void initAudioPWM(void);

void tone(int noteFreq);
void noTone(void);
	
void playSongWithSem(const int melody[], const float noteDurations[], int num_notes);
void playSongNoSem(const int melody[], const float noteDurations[], int num_notes);


void raiderMoveAudio(void);
void victoryAudio(void);





#endif


