#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/timers.h"
//#include "esp_system.h"

/*this code is for binary semaphore
SemaphoreHandle_t binSemaphore;

void listenForHTTP(void *params){
    while(true){
        printf("received http message\n");
        xSemaphoreGive(binSemaphore);
        printf("processeed http message\n");
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

void task1(void *params){
    while(true){
        xSemaphoreTake(binSemaphore, portMAX_DELAY);
        printf("doing something with http\n");
    }
}

void app_main(void)
{
    binSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(&listenForHTTP, "get HTTP", 2048, NULL, 2, NULL);
    xTaskCreate(&task1, "do something with http", 2048, NULL , 1, NULL);
    //xTaskCreatePinnedToCore
}   */

/*this code is for queue
QueueHandle_t queue;
void listenForHTTP(void* params){
    int count = 0;
    while(true){
        count++;
        printf("received http message\n");
        long ok = xQueueSend(queue,&count,5000/portTICK_PERIOD_MS);
        if(ok){
            printf("reading HTTP %ld\n", ok);   
        }
        else{
            
            int rxInt;
            vTaskDelay(1200/portTICK_PERIOD_MS);
            xQueueReceive(queue,&rxInt,1000/portTICK_PERIOD_MS);
            printf("failed reading HTTP %ld, rxInt:%d\n", ok,rxInt);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void task1(void* params){
    while(true){
        int rxInt;
        if(xQueueReceive(queue,&rxInt, 5000/portTICK_PERIOD_MS)){
            printf("reading task1\n");
            vTaskDelay(1000/portTICK_PERIOD_MS);
            int count = 0;
            long ok = xQueueSend(queue,&count,1000/portTICK_PERIOD_MS);
            if(ok){
                printf("count:%d , ok: %ld\n", count, ok);
                vTaskDelay(1000/portTICK_PERIOD_MS);
            }
        }
    }
}

void task2(void* params){
    while(true){
        int rxInt;
        if(xQueueReceive(queue,&rxInt,1000/portTICK_PERIOD_MS)){
            printf("reading task2\n");
            vTaskDelay(1000/portTICK_PERIOD_MS);
        }
    }
}

void app_main(void){
    queue = xQueueCreate(3,sizeof(int));
    xTaskCreate(&listenForHTTP,"get http",2048,NULL,3,NULL);
    //xTaskCreate(&task1,"do sonething with http",2048,NULL,2,NULL);
    //xTaskCreate(&task2,"task2",2048,NULL,1,NULL);
}
    */


    /*this code is for eventGroup*/
// EventGroupHandle_t event;
// const int gotHttp = BIT0; // (1<<1)
// const int gotBLE = BIT1;
// void listenForHTTP(void* params){
//     while(true){
//         xEventGroupSetBits(event,gotHttp);
//         printf("got http\n");
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }

// void listenForBluetooth(void *params){
//     while (true)
//     {
//         xEventGroupSetBits(event,gotBLE);
//         printf("got Buletooth\n");
//         vTaskDelay(pdMS_TO_TICKS(5000));
//     }
// }

// void task1(void* params){
//     while (true)
//     {
//         xEventGroupWaitBits(event, gotHttp | gotBLE, true, true, portMAX_DELAY);
//         printf("receive http and BLE\n");
//     }
// }

// void app_main(void){
//     event = xEventGroupCreate();
//     xTaskCreate(&listenForHTTP, "got http", 2048, NULL, 2, NULL);
//     xTaskCreate(&listenForBluetooth, "got BLE", 2048, NULL, 2, NULL);
//     xTaskCreate(&task1, "do someting", 2048, NULL, 2, NULL);
// }

/*this is timer example*/
void on_timer(TimerHandle_t xTimer){
    int now_us = esp_timer_get_time();
    printf("timer hit %d\n", now_us);
}
void app_main(void){
    
    printf("app started\n");
    TimerHandle_t xTimer = xTimerCreate("my timer", pdMS_TO_TICKS(2000),true,NULL,on_timer);
    xTimerStart(xTimer,0);
    
}