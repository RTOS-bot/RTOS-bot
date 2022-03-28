#include "audio.h"
#include "pwm.h"

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
	uint8_t pinsAudio[] = {PTE29_PIN};
	uint8_t channelsAudio[] = {TPM_CHANNEL2};

	initPWM(PORT_AUDIO, AUDIO_ALT , pinsAudio, NUM_PINS_AUDIO, 
					TPM_AUDIO, AUDIO_MOD, channelsAudio, NUM_CHANNELS_AUDIO);
}


void tone(int noteFreq) {
	TPM0->MOD = FREQ_TO_MOD(noteFreq);
	TPM0_C2V = FREQ_TO_C0V(noteFreq);
}

void noTone() {
	TPM0->MOD = 0;
	TPM0_C2V = 0;
}
		

/* 
 * to calculate the note duration, take one second 
 * divided by the note type.
 * quarter note = 1000 / 4, eighth note = 1000/8, etc.
 */

void playSong(const int melody[], const float noteDurations[], int num_notes) {
	
	 for (int note = 0; note < num_notes; note++) {    
 
		float noteDuration = 1000 / noteDurations[note];

		tone(melody[note]);
		osDelay(noteDuration);

		noTone();
		osDelay(noteDuration);
 
    }

}

void moveAudioRaider(void) {
	playSong(raiderMelody, raiderNoteDurations, 38);
	//	playSong(victoryMelody, victoryNoteDurations, 24);


}


