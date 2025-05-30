#include <xc.h>  
#include "adc.h"
#include "i2c.h"
#include "i2c_lcd.h"
#include "led_buzzer.h"

// Configuraciones del microcontrolador
#pragma config FOSC = INTIO67
#pragma config WDTEN = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 16000000
#define UMBRAL_TEMP 40.0f

// Frames de animación para temperatura alta
const unsigned char animacion_custom[4][8] = {
    {
        0b00000,
        0b01110,
        0b10101,
        0b11011,
        0b01110,
        0b01110,
        0b00000,
        0b00000
    },
    {
        0b00000,
        0b00000,
        0b01110,
        0b10101,
        0b11011,
        0b01110,
        0b01110,
        0b00000
    },
    {
        0b00000,
        0b01110,
        0b10101,
        0b11011,
        0b01110,
        0b01110,
        0b00000,
        0b00000
    },
    {
        0b01110,
        0b10101,
        0b11011,
        0b01110,
        0b01110,
        0b00000,
        0b00000,
        0b00000
    }
};

// Frames de animación para temperatura estable (llenado)
const unsigned char animacion_estable[4][8] = {
    {
        0b00000,
        0b01010,
        0b01010,
        0b00000,
        0b00000,
        0b00000,
        0b10001,
        0b01110
    },
    {
        0b00000,
        0b01010,
        0b01010,
        0b00000,
        0b00000,
        0b10001,
        0b11111,
        0b01110
    },
    {
        0b00000,
        0b01010,
        0b01010,
        0b00000,
        0b10001,
        0b11111,
        0b10001,
        0b01110
    },
    {
        0b00000,
        0b01010,
        0b01010,
        0b00000,
        0b11111,
        0b10001,
        0b10001,
        0b01110
    }
};

unsigned char anim_index = 0;

void setup(void) {
    ADC_Init();
    I2C_init();
    lcd_init();

    TRIS_DANGER_LED = 0;
    TRIS_BUZZER = 0;
    DANGER_LED = 0;
    BUZZER = 0;
}

void lcd_write_number(unsigned int number) {
    char d1 = (number / 100) % 10;
    char d2 = (number / 10) % 10;
    char d3 = number % 10;

    if (d1 > 0) lcd_write_char(d1 + '0');
    else lcd_write_char(' ');

    lcd_write_char(d2 + '0');
    lcd_write_char('.');
    lcd_write_char(d3 + '0');
    lcd_write_char(' ');
    lcd_write_char('C');
}

void lcd_create_custom_char(unsigned char location, const unsigned char *pattern) {
    location &= 0x07;
    lcd_cmd(0x40 + (location * 8));
    for (unsigned char i = 0; i < 8; i++) {
        lcd_write_char(pattern[i]);
    }
}

void mostrar_temperatura(float temp) {
    unsigned int temp_entera = (unsigned int)(temp * 10);

    lcd_set_cursor(0, 0);
    lcd_write_string("Temp ");
    lcd_write_number(temp_entera);
    lcd_write_string("   ");

    lcd_set_cursor(1, 0);
    if (temp > UMBRAL_TEMP) {
        lcd_write_string("Temp excesiva ");

        // Mostrar animación de temperatura alta
        lcd_create_custom_char(0, animacion_custom[anim_index]);
        lcd_set_cursor(1, 15);
        lcd_write_char(0);
    } else {
        lcd_write_string("Temp estable   ");

        // Mostrar animación de temperatura estable
        lcd_create_custom_char(0, animacion_estable[anim_index]);
        lcd_set_cursor(1, 15);
        lcd_write_char(0);
    }

    // Avanzar frame de animación (compartido)
    anim_index = (anim_index + 1) % 4;
}

void activar_alarma_pwm(void) {
    for (int i = 0; i < 10; i++) {
        DANGER_LED = 1;
        BUZZER = 1;
        __delay_ms(10);
        DANGER_LED = 0;
        BUZZER = 0;
        __delay_ms(10);
    }
}

void desactivar_alarma(void) {
    DANGER_LED = 0;
    BUZZER = 0;
}

void main(void) {
    setup();

    while (1) {
        unsigned int adc_value = ADC_Read(0);
        float temperatura = adc_value * 0.488f;  // Conversión típica para LM35
        
        mostrar_temperatura(temperatura);

        if (temperatura > UMBRAL_TEMP) {
            activar_alarma_pwm();
        } else {
            desactivar_alarma();
        }

        __delay_ms(50); // Tiempo entre cuadros de animación
    }
}