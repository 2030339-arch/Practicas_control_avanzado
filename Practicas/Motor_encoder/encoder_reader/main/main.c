#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "encoder.h"

static const char *TAG = "MAIN";

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