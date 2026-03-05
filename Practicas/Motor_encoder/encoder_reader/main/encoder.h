#ifndef ENCODER_H
#define ENCODER_H

#include "driver/pulse_cnt.h"

// Pines del encoder
#define ENCODER_GPIO_A 33
#define ENCODER_GPIO_B 25

// Límites del contador PCNT
#define PCNT_HIGH_LIMIT 32767
#define PCNT_LOW_LIMIT  -32768

// Pulsos por revolución del encoder (eje de salida)
#define ENCODER_PPR 494

// Tiempo de muestreo (ms)
#define SAMPLE_TIME_MS 1000

void encoder_init(void);
int encoder_get_count(void);
float encoder_get_rpm(void);

#endif