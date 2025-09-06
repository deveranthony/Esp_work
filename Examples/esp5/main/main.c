//This is my contorlling RGB LED strip project

#include <stdio.h>
#include "calc.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//connect RGB LED to GPIO 18 pin
#define BLINK_GPIO 18
#define tag "LED:"
void app_main(void)
{
    printf("Hello, esp32\n");
    printf("3 + 5 = %d\n",add(3,5));
    
    led_strip_config_t strip_config = {
    .strip_gpio_num = BLINK_GPIO,  // The GPIO that connected to the LED strip's data line
    .max_leds = 4,                 // The number of LEDs in the strip,
    .led_model = LED_MODEL_WS2812, // LED strip model, it determines the bit timing
    .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // The color component format is G-R-B
    .flags = {
        .invert_out = false, // don't invert the output signal
    }
    
};
    led_strip_config_t strip_config1 = {
    .strip_gpio_num = 48,  // The GPIO that connected to the LED strip's data line
    .max_leds = 1,                 // The number of LEDs in the strip,
    .led_model = LED_MODEL_WS2812, // LED strip model, it determines the bit timing
    .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // The color component format is G-R-B
    .flags = {
        .invert_out = false, // don't invert the output signal
    }
    
};

/// RMT backend specific configuration
led_strip_rmt_config_t rmt_config = {
    .clk_src = RMT_CLK_SRC_DEFAULT,    // different clock source can lead to different power consumption
    .resolution_hz = 10 * 1000 * 1000, // RMT counter clock frequency: 10MHz
    .mem_block_symbols = 64,           // the memory size of each RMT channel, in words (4 bytes)
    .flags = {
        .with_dma = false, // DMA feature is available on chips like ESP32-S3/P4
    }
};

led_strip_rmt_config_t rmt_config1 = {
    .clk_src = RMT_CLK_SRC_DEFAULT,    // different clock source can lead to different power consumption
    .resolution_hz = 10 * 1000 * 1000, // RMT counter clock frequency: 10MHz
    .mem_block_symbols = 64,           // the memory size of each RMT channel, in words (4 bytes)
    .flags = {
        .with_dma = false, // DMA feature is available on chips like ESP32-S3/P4
    }
};

/// Create the LED strip object
led_strip_handle_t led_strip = NULL;
led_strip_handle_t led_strip1 = NULL;

ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config1, &rmt_config1, &led_strip1));
while(true){

    led_strip_clear(led_strip);
    led_strip_clear(led_strip1);

    led_strip_set_pixel(led_strip,0,50,0,0);
    led_strip_set_pixel(led_strip,2,50,0,50);  
    led_strip_set_pixel(led_strip1,0,50,0,0);

    led_strip_refresh(led_strip);
    led_strip_refresh(led_strip1);
    
    ESP_LOGI(tag, "RED");
    vTaskDelay(pdMS_TO_TICKS(200));


    led_strip_clear(led_strip);
    led_strip_set_pixel(led_strip,1,0,50,0);
    led_strip_set_pixel(led_strip,3,50,50,0);
    led_strip_refresh(led_strip);
    
    led_strip_clear(led_strip1);
    led_strip_set_pixel(led_strip1,0,0,50,0);
    led_strip_refresh(led_strip1);
    
    ESP_LOGI(tag, "GREEN");
    vTaskDelay(pdMS_TO_TICKS(300));

    led_strip_clear(led_strip);
    led_strip_set_pixel(led_strip,2,0,0,50);
    led_strip_set_pixel(led_strip,0,0,50,50);
    led_strip_refresh(led_strip);

    led_strip_clear(led_strip1);
    led_strip_set_pixel(led_strip1,0,0,0,50);
    led_strip_refresh(led_strip1);
    
    ESP_LOGI(tag, "BLUE");
    vTaskDelay(pdMS_TO_TICKS(500));

    led_strip_clear(led_strip1);
    int i = 5;
    while(i--){
    led_strip_clear(led_strip);
    led_strip_set_pixel(led_strip,0,0,50,50);
    led_strip_set_pixel(led_strip,1,0,0,50);
    led_strip_set_pixel(led_strip,2,0,50,0);
    led_strip_set_pixel(led_strip,3,50,0,0);
    led_strip_refresh(led_strip);
    vTaskDelay(pdMS_TO_TICKS(200));
    led_strip_clear(led_strip);
    vTaskDelay(pdMS_TO_TICKS(100));
    }
}
}