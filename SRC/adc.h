#ifndef ADC_H
#define ADC_H

#include <xc.h>  // Incluir la librería de configuraciones específicas del PIC

// Definición de la frecuencia del oscilador
#define _XTAL_FREQ 16000000  // 16 MHz

// Prototipo de las funciones
void ADC_Init(void);            // Inicializa el ADC
unsigned int ADC_Read(unsigned char canal);    // Lee el valor del ADC

#endif  // ADC_H