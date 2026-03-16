#ifndef ENCODER_H
#define ENCODER_H

#include "driver/pulse_cnt.h"

// Pines del encoder
#define ENCODER_GPIO_A 26
#define ENCODER_GPIO_B 27

// Resolución del encoder
#define ENCODER_PPR 600

// Tiempo de muestreo en ms
#define SAMPLE_TIME_MS 1000

// Límites del contador PCNT
#define PCNT_HIGH_LIMIT 32767
#define PCNT_LOW_LIMIT -32768

// Prototipos de funciones
void encoder_init(void);
int encoder_get_count(void);
float encoder_get_rpm(void);

#endif