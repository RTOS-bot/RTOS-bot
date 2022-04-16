#include "audio.h"
#include "pwm.h"
#include "core.h"

osSemaphoreId_t musicSem;


const int raiderMelody[] = {
    NOTE_E4, 
    NOTE_F4,
    NOTE_G4,
    NOTE_C5,
    NOTE_D4,
    NOTE_E4,
    NOTE_F4,

    NOTE_G4,
    NOTE_A4,
    NOTE_B4,
    NOTE_F5,

    NOTE_A4,
    NOTE_B4,
    NOTE_C5,
    NOTE_D5,
    NOTE_E5,

    NOTE_E4,  
    NOTE_F4,
    NOTE_G4,
    NOTE_C5,

    NOTE_D5,
    NOTE_E5,
    NOTE_F5,

    NOTE_G4,  
    NOTE_G4,
    NOTE_E5,
    NOTE_D5,

    NOTE_G4,
    NOTE_E5,
    NOTE_D5,

    NOTE_G4,
    NOTE_E5,
    NOTE_D5,

    NOTE_G4,
    NOTE_F5,
    NOTE_E5,

    NOTE_D5,
    NOTE_C5,
};

/* 16 --> semiquaver
 * 8 --> quaver
 * 4 --> crotchet
 * 2 --> minim
 * 1 --> semibreve
 * 
 * numbers in between
 * to have specific rhythm
 */
const float raiderNoteDurations[] = {
    6,16,8,1.6,
    6,16,1.4,

    6,16,8,1.6,
    6,16,4,4,4,

    6,16,8,1.4,
    6,16,1.3,  

    6,16,4,6,16, 

    4,6,16,4, 
    6,16,4,5.5,
    16,
    1
};
 
const int victoryMelody[] = {
	NOTE_G5,
	NOTE_F5,
	NOTE_E5,
	NOTE_F5,
	NOTE_G5,
	NOTE_G5,
	NOTE_G5,
	
	NOTE_A5,
	NOTE_A5,
	NOTE_A5,
	
	NOTE_G5,
	NOTE_G5,
	NOTE_G5,
	
	NOTE_G5,
	NOTE_F5,
	NOTE_E5,
	NOTE_F5,
	NOTE_G5,
	NOTE_G5,
	NOTE_G5,
	
	NOTE_A5,
	NOTE_A5,
	NOTE_A5,
	
	NOTE_G5,

};

const float victoryNoteDurations[] = {
	8,8,8,8,8,8,4,
	8,8,4,
	8,8,4,
	8,8,8,8,8,8,4,
	8,8,4,
	2
};



void initAudioPWM(void) {
	uint8_t pinsAudio[] = {PTE20_PIN};
	uint8_t channelsAudio[] = {TPM_CHANNEL0};

	initPWM(PORT_AUDIO, AUDIO_ALT , pinsAudio, NUM_PINS_AUDIO, 
					TPM_AUDIO, AUDIO_MOD, channelsAudio, NUM_CHANNELS_AUDIO);
}


void tone(int noteFreq) {
	TPM1->MOD = FREQ_TO_MOD(noteFreq);
	TPM1_C0V = FREQ_TO_C0V(noteFreq);
}

void noTone() {
	TPM1->MOD = 0;
	TPM1_C0V = 0;
}
		

/* 
 * to calculate the note duration, take one second 
 * divided by the note type.
 * quarter note = 1000 / 4, eighth note = 1000/8, etc.
 */

void playSongWithSem(const int melody[], const float noteDurations[], int num_notes) {
	
	 for (int note = 0; note < num_notes; note++) {    
		osSemaphoreAcquire(musicSem, osWaitForever);
		float noteDuration = 1000 / noteDurations[note];
		
		tone(melody[note]);
		osSemaphoreRelease(musicSem);

		osDelay(noteDuration);
		noTone();
		osDelay(noteDuration);
    }
}

void playSongNoSem(const int melody[], const float noteDurations[], int num_notes) {
	
	 for (int i = 0; i < 2; i++) {    

	 for (int note = 0; note < num_notes; note++) {    
		float noteDuration = 1000 / noteDurations[note];
		
		tone(melody[note]);
		osDelay(noteDuration);

		noTone();
		osDelay(noteDuration);
    }
	}
}


void raiderMoveAudio(void) {
	playSongWithSem(raiderMelody, raiderNoteDurations, 38);
}

void victoryAudio(void) {
	osSemaphoreAcquire(musicSem, osWaitForever);
	playSongNoSem(victoryMelody, victoryNoteDurations, 24);
	osSemaphoreRelease(musicSem);

}



