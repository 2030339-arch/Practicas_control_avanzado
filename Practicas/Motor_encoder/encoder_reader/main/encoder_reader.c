#include <stdio.h>
#include "encoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "ENCODER";

static pcnt_unit_handle_t pcnt_unit = NULL;

static int last_count = 0;

void encoder_init(void)
{
    ESP_LOGI(TAG, "Inicializando PCNT");

    pcnt_unit_config_t unit_config = {
        .high_limit = PCNT_HIGH_LIMIT,
        .low_limit = PCNT_LOW_LIMIT,
    };

    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    // filtro para ruido
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };

    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    // Canal A
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_GPIO_A,
        .level_gpio_num = ENCODER_GPIO_B,
    };

    pcnt_channel_handle_t chan_a = NULL;

    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &chan_a));

    // Canal B
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = ENCODER_GPIO_B,
        .level_gpio_num = ENCODER_GPIO_A,
    };

    pcnt_channel_handle_t chan_b = NULL;

    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &chan_b));

    // Configuración cuadratura x4
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(
        chan_a,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE));

    ESP_ERROR_CHECK(pcnt_channel_set_level_action(
        chan_a,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(
        chan_b,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE));

    ESP_ERROR_CHECK(pcnt_channel_set_level_action(
        chan_b,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

    ESP_LOGI(TAG, "Encoder listo");
}

int encoder_get_count(void)
{
    int count = 0;

    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &count));

    return count;
}

float encoder_get_rpm(void)
{
    int current_count = encoder_get_count();

    int delta = current_count - last_count;

    last_count = current_count;

    float revolutions = (float)delta / (ENCODER_PPR * 4);

    float rpm = (revolutions * 60000.0) / SAMPLE_TIME_MS;

    return rpm;
}

void app_main(void)
{
    encoder_init();

    while (1)
    {
        int count = encoder_get_count();

        float rpm = encoder_get_rpm();

        ESP_LOGI(TAG, "Conteo de pulsos: %d", count);
        ESP_LOGI(TAG, "Velocidad: %.2f RPM", rpm);

        if (rpm > 0)
        {
            ESP_LOGI(TAG, "Sentido: Horario");
        }
        else if (rpm < 0)
        {
            ESP_LOGI(TAG, "Sentido: Antihorario");
        }
        else
        {
            ESP_LOGI(TAG, "Motor detenido");
        }

        vTaskDelay(pdMS_TO_TICKS(SAMPLE_TIME_MS));
    }
}