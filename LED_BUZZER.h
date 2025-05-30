#ifndef LED_BUZZER_H
#define LED_BUZZER_H

#include <xc.h>
#define _XTAL_FREQ 16000000  // Frecuencia del oscilador interno


// Pines de salida
#define DANGER_LED LATCbits.LATC6
#define TRIS_DANGER_LED TRISCbits.RC6
#define BUZZER LATCbits.LATC7
#define TRIS_BUZZER TRISCbits.RC7

#endif	/* XC_HEADER_TEMPLATE_H */
