#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

void app_main(void)
{
    printf("hello esp32\n");
    printf(CONFIG_MY_STRING);

    bool my_bool = false;
#ifdef CONFIG_MY_BOOL
    my_bool = true;
#else
    my_bool = false;
#endif

int option = 0;
#ifdef CONFIG_OPTION_1
    option = 1;
#elif CONFIG_OPTION_2
    option = 2;
#else
    option = 3;
#endif

}