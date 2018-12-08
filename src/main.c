/* ------------------------------------------
Your Heading
  -------------------------------------------- */

#include <MKL25Z4.H>
#include "../include/gpio_defs.h"
#include "../include/SysTick.h"
#include "../include/LCD.h"
#include "../include/adc_defs.h"
#include "../include/pit.h"
#include "../include/tpmPwm.h"

#include <stdbool.h>

#define BUTTONOPEN (0)
#define BUTTONCLOSED (1)
#define BUTTONBOUNCE (2)

/*----------------------------------------------------------------------------
Task 1 Info

 *----------------------------------------------------------------------------*/

int b_state = BUTTONOPEN ;
int pressedEvent = 0 ; // signal
int bounceCounter = 0 ;
float buttonVoltage = 0 ;


void task1PollInput()
{
    if (bounceCounter > 0) bounceCounter -- ;

    switch (b_state) {
        case BUTTONOPEN :
          MeasureVoltage(); // measure voltage using ADC API
          buttonVoltage = (sres * VREF) / ADCRANGE ; // button voltage value
            if (!(buttonVoltage > 3.2 && buttonVoltage <= 3.3)) {
                pressedEvent = 1 ;  // create a 'pressed' event
                b_state = BUTTONCLOSED ;
            }
          break ;
        case BUTTONCLOSED :
          MeasureVoltage();
          buttonVoltage = (sres * VREF) / ADCRANGE ;
          if (buttonVoltage > 3.2 && buttonVoltage <= 3.3) {
            b_state = BUTTONBOUNCE ;
            bounceCounter = 20 ;
          }
          break ;
        case BUTTONBOUNCE :
          MeasureVoltage();
          buttonVoltage = (sres * VREF) / ADCRANGE ;
          if (!(buttonVoltage > 3.2 && buttonVoltage <= 3.3)) {
            b_state = BUTTONCLOSED ;
          }
          if (bounceCounter == 0) {
            b_state = BUTTONOPEN ;
          }
          break ;
    }
}

/*----------------------------------------------------------------------------
Task 2 Info

 *----------------------------------------------------------------------------*/
int digitString[] = {48,48,48} ;
int buttonState = NOT_PRESSED;
int pos = 0 ;

int digit1 = 0;

int digit2 = 0;

int digit3 = 0;

int value = 0;

void task2LCD(){
  switch(buttonState){

    case NOT_PRESSED:
      MeasureVoltage();
      buttonVoltage = (sres * VREF) / ADCRANGE ;
			if (pressedEvent == 1) {
				if (buttonVoltage >= 0.40 && buttonVoltage <= 0.65) {
					buttonState = UP;
				} else if (buttonVoltage >= 1.20 && buttonVoltage <= 1.60) {
					buttonState = DOWN;
				} else if (buttonVoltage >= 1.9 && buttonVoltage <= 2.0) {
					buttonState = LEFT;
				} else if (buttonVoltage >= 0 && buttonVoltage <= 0.39) {
					buttonState = RIGHT;
				}
			}
    break;

  case LEFT:

    if(pos > 0) {
			pos--;
			setLCDAddress(1,pos);
		}
		pressedEvent = 0;
    buttonState = NOT_PRESSED;
    break;

  case RIGHT:

	  if(pos < 2) {
			pos++;
			setLCDAddress(1,pos);
		}

    else if (pos == 2) {

			digit1 = digitString[0]-48;
			digit2 = digitString[1]-48;
			digit3 = digitString[2]-48;

			value = (digit1*100)+(digit2*10)+digit3;
			value = (result * 10) + 7316;

			if (value < 7617) {
				stopTimer(0);
				setLCDAddress(1,0) ;
				writeLCDString("Error") ;
				setLCDAddress(1,0) ;
			}
      else {
				setTimer(0, value;
			}
		}
		pessedEvent = 0;
    buttonState = NOT_PRESSED;
    break;

  case DOWN:

    if(digitString[pos] >48) {
      digitString[pos]--;
      setLCDAddress(1,pos) ;
      writeLCDChar(digitString[pos]) ;
      setLCDAddress(1,pos);
		}
		pressedEvent = 0;
		buttonState = NOT_PRESSED;
		break;

  case UP:

    if(digitString[pos] < 57) {
			digitString[pos]++;
      setLCDAddress(1,pos) ;
      writeLCDChar(digitString[pos]) ;
      setLCDAddress(1,pos) ;
		}
		pressedEvent = 0;
    buttonState = NOT_PRESSED;
    break;

  }
}

/*----------------------------------------------------------------------------
Your Reset info
 *----------------------------------------------------------------------------*/
char resetString[] = "000";

void defaultText() {
  lcdClear(true) ;  // clear memory
	lcdHome(true) ;
  lcdMode(M_Inc) ;
  lcdCntrl(C_BLINK) ;
  setLCDAddress(0,0) ;
  writeLCDString("Enter 3 digits:") ;
  setLCDAddress(1,pos) ;
	writeLCDString(resetString) ;
	setLCDAddress(1,pos) ;
}
/*----------------------------------------------------------------------------
  MAIN function

Your info

 *----------------------------------------------------------------------------*/

int main (void) {
    uint8_t calibrationFailed ; // zero expected
    configureGPIOinput() ;       // Initialise button
    configureGPIOoutput() ;      // Initialise output
    configurePIT(0) ;            // Configure PIT channel 0
    setTimer(0, 20040) ; // Frequency for MIDI 60 - middle C
    configureTPM0forPWM() ;
    setPWMDuty(64) ;     // 50% volume
                         // Max is 128; off is 0
    SystemCoreClockUpdate() ;
		startTimer(0); // starts tone
    Init_SysTick(1000) ;
    initLCD() ;
    Init_ADC() ;
    calibrationFailed = ADC_Cal(ADC0) ; // calibrate the ADC
    while (calibrationFailed) ; // block progress if calibration failed
    Init_ADC() ;
    lcdClear(true) ;
		defaultText() ;
    waitSysTickCounter(10) ; // initialise counter

    while (1) {
      task1PollInput() ;       // task1
      task2LCDfunction() ;     // task 2
      waitSysTickCounter(10) ; // cycle every 10 ms
    }
}
