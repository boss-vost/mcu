#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    printf("Введите символ:\n");

    while (1) {
        char symbol = getchar();
        printf("ASCII код: %d\n", symbol);
    }
}