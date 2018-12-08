#include <stdbool.h>
#ifndef GPIO_DEFS_H
#define GPIO_DEFS_H

#define MASK(x) (1UL << (x))


// Switches is on port D, pins 6, 7
#define BUTTON1_POS (6)
#define BUTTON2_POS (7)

// button states
#define NOT_PRESSED (0)
#define UP (1)
#define DOWN (2)
#define LEFT (3)
#define RIGHT (4)

// GPIO output used for the frequency, port A pin 16
#define AUDIO_POS (16)


// Function prototypes
void configureGPIOinput(void) ;       // Initialise button
void configureGPIOoutput(void) ;      // Initialise output
void audioToggle(void) ;              // Toggle the output GPIO

#endif
