#ifndef TPM_PWM_H
#define TPM_PWM_H
#include <MKL25Z4.H>

#define TPM_CHAN (1)
#define PWM_PORT (PORTD)
#define PWM_PIN (7)
#define ALT_TPM (3)
#define PWM_DUTY_MAX (128)

// prototypes
void configureTPM0forPWM(void) ;
void setPWMDuty(unsigned int duty) ;

#endif

