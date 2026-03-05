#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"
#define LED_PIN 25

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        printf("Hello World!\n");
        printf("Device name: '%s'\n", DEVICE_NAME);
        printf("Device version: %s\n", DEVICE_VRSN);
        printf("------------------------\n");

        sleep_ms(200);
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
}