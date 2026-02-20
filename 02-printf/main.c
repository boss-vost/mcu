#include <stdio.h>
#include "pico/stdlib.h"
#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

int main() {
    stdio_init_all();

    while (true) {
        printf("Hello World!\n");
        printf("Device name: '%s'\n", DEVICE_NAME);
        printf("Device version: %s\n", DEVICE_VRSN);
        printf("------------------------\n");
        sleep_ms(1000);
    }
}