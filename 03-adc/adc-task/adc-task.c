#include "adc-task.h"
#include "hardware/adc.h"
#include "stdio.h"
#include "pico/stdlib.h"

#define ADC_GP 26
#define ADC_CHANNEL 0
#define ADC_TEMP_CHANNEL 4
#define ADC_TASK_MEAS_PERIOD_US 100000

adc_task_state_t adc_task_state = ADC_TASK_STATE_IDLE;
uint64_t timestamp;

void adc_task_init()
{
    adc_init();
    timestamp = time_us_64();
    adc_gpio_init(ADC_GP);
    adc_set_temp_sensor_enabled(true);
}

float adc_task_read_voltage()
{
    adc_select_input(ADC_CHANNEL);
    const uint16_t adc_raw = adc_read();
    return (float)adc_raw / 4095.0f * 3.3f;
}

float adc_task_read_temperature() {
    adc_select_input(ADC_TEMP_CHANNEL);
    const uint16_t adc_raw = adc_read();
    const float voltage = (float)adc_raw / 4095.0f * 3.3f;
    return 27.0f - (voltage - 0.706f) / 0.001721f; //27.0f - (temp_V - 0.706f) / 0.001721f;

}

void adc_task_set_state(const adc_task_state_t state) {
    adc_task_state = state;
}

void adc_task_handle() {
    if (adc_task_state == ADC_TASK_STATE_IDLE) {
        return;
    }
    if (time_us_64() - timestamp > ADC_TASK_MEAS_PERIOD_US) {
        timestamp = time_us_64();
        const float voltage_V = adc_task_read_voltage();
        const float temp_C = adc_task_read_temperature();
        printf("%f %f\n", voltage_V, temp_C);
    }
}