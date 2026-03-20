#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "hardware/structs/sio.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

// Прототипы функций-колбэков
void version_callback(const char* args);
void led_on_callback(const char* args);
void led_off_callback(const char* args);
void led_blink_callback(const char* args);
void led_blink_set_period_ms_callback(const char* args);
void help_callback(const char* args);
void mem_callback(const char* args);
void wmem_callback(const char* args);

api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "turn LED ON"},
    {"off", led_off_callback, "turn LED OFF"},
    {"blink", led_blink_callback, "make LED blink"},
    {"set_period", led_blink_set_period_ms_callback, "set LED blink period in ms (e.g., 'set_period 1000')"},
    {"help", help_callback, "show this help message"},
    {"mem", mem_callback, "read 32-bit value from memory (hex address)"},
    {"wmem", wmem_callback, "write 32-bit value to memory (hex address hex value)"},
    {NULL, NULL, NULL},
};

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
    printf("LED state: ON\n");
}

void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
    printf("LED state: OFF\n");
}

void led_blink_callback(const char* args)
{
    led_task_state_set(LED_STATE_BLINK);
    printf("LED state: BLINK\n");
}

void led_blink_set_period_ms_callback(const char* args)
{
    if (!args || args[0] == '\0') {
        printf("ERROR: missing period value\n");
        return;
    }

    uint32_t period_ms = 0;
    int result = sscanf(args, "%u", &period_ms);
    
    if (result != 1 || period_ms == 0) {
        printf("ERROR: invalid period value (must be > 0)\n");
        return;
    }

    led_task_set_blink_period_ms(period_ms);
    printf("LED blink period set to %u ms\n", period_ms);
}

void help_callback(const char* args)
{
    printf("Available commands:\n");
    for (int i = 0; device_api[i].command_name != NULL; i++) {
        printf("  %-15s %s\n", device_api[i].command_name, device_api[i].command_help);
    }
}

void mem_callback(const char* args)
{
    if (!args || args[0] == '\0') {
        printf("ERROR: missing address\n");
        return;
    }

    uint32_t addr = 0;
    int result = sscanf(args, "%x", &addr);
    
    if (result != 1) {
        printf("ERROR: invalid address (use hex, e.g., 'mem 20001234')\n");
        return;
    }

    // Чтение значения по адресу
    uint32_t* ptr = (uint32_t*)addr;
    uint32_t value = *ptr;

    printf("Memory at 0x%08X: 0x%08X (%u)\n", addr, value, value);
}

void wmem_callback(const char* args)
{
    if (!args || args[0] == '\0') {
        printf("ERROR: missing address and value\n");
        return;
    }

    uint32_t addr = 0;
    uint32_t value = 0;
    int result = sscanf(args, "%x %x", &addr, &value);
    
    if (result != 2) {
        printf("ERROR: invalid arguments (use hex, e.g., 'wmem 20001234 ABCD1234')\n");
        return;
    }

    // Запись значения по адресу
    uint32_t* ptr = (uint32_t*)addr;
    *ptr = value;

    printf("Written 0x%08X to address 0x%08X\n", value, addr);
}

int main()
{
    stdio_init_all();
    stdio_task_init();
    led_task_init();
    protocol_task_init(device_api);

    while (true)
    {
        char* cmd = stdio_task_handle();
        if (cmd) {
            protocol_task_handle(cmd);
        }
        led_task_handle();
    }
}