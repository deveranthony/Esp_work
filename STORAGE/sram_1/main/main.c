
#include <stdio.h>
#include <string.h>
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define tag "Memory"

void print_memory(){
    ESP_LOGI(tag,"stack %d, total %d\n", uxTaskGetStackHighWaterMark(NULL), heap_caps_get_free_size(MALLOC_CAP_8BIT));
}

void app_main()
{
  //ESP_LOGI(TAG, "xPortGetFreeHeapSize %ld = DRAM", xPortGetFreeHeapSize());

//   int DRam = heap_caps_get_free_size(MALLOC_CAP_8BIT);
//   int IRam = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);

//   ESP_LOGI(TAG, "DRAM \t\t %d", DRam);
//   ESP_LOGI(TAG, "IRam \t\t %d", IRam);
//   int free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
//   ESP_LOGI(TAG,"free = %d", free);
    print_memory();
}