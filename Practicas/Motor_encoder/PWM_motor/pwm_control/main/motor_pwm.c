#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define PWM_GPIO 26
#define IN1 27
#define IN2 14

#define PWM_FREQ_HZ 20000
#define PWM_RESOLUTION LEDC_TIMER_10_BIT
#define PWM_TIMER LEDC_TIMER_0
#define PWM_CHANNEL LEDC_CHANNEL_0

static const char *TAG = "MOTOR_PWM";

void app_main(void)
{
    // Configurar pines de dirección
    gpio_set_direction(IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN2, GPIO_MODE_OUTPUT);

    // Dirección del motor
    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 0);

    // Configurar temporizador PWM
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = PWM_TIMER,
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);

    // Configurar canal PWM
    ledc_channel_config_t channel = {
        .gpio_num = PWM_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = PWM_CHANNEL,
        .timer_sel = PWM_TIMER,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&channel);

    ESP_LOGI(TAG, "Control de velocidad del motor con PWM");

    while (1)
    {
        for (int duty = 0; duty <= 1023; duty += 150)
        {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL, duty);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL);

            ESP_LOGI(TAG, "PWM Duty: %d", duty);

            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}