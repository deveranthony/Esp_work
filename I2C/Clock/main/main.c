#include <stdio.h>
#include <time.h>
#include "esp_log.h"
#include "driver/i2c_master.h"

#define SDA_PIN 8
#define SCL_PIN 9

#define Add 0x68


uint8_t int_to_bdc(int val);
int bcd_to_int(uint8_t val);

void app_main(void)
{
    i2c_master_bus_config_t i2c_master_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
        .scl_io_num = SCL_PIN,
        .sda_io_num = SDA_PIN
    };
    i2c_master_bus_handle_t i2c_master_bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_bus_config, &i2c_master_bus_handle));

    i2c_device_config_t i2c_device_config = {
        .scl_speed_hz = 100000,
        .device_address = Add,
    };
    i2c_master_dev_handle_t dev_handle;
    i2c_master_bus_add_device(i2c_master_bus_handle, &i2c_device_config, &dev_handle);

    uint8_t write_buffer_for_read[]= {0X00};
    uint8_t read_buffer[7];
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle,write_buffer_for_read,sizeof(write_buffer_for_read),read_buffer,sizeof(read_buffer),-1));

    struct tm dateTime = {
        .tm_sec = bcd_to_int(read_buffer[0]),
        .tm_min = bcd_to_int(read_buffer[1]),
        .tm_hour = bcd_to_int(read_buffer[2]),
        .tm_wday = bcd_to_int(read_buffer[3]),
        .tm_mday = bcd_to_int(read_buffer[4]),
        .tm_mon = bcd_to_int(read_buffer[5]) -1,
        .tm_year = bcd_to_int(read_buffer[6]) + 2000 - 1900,
    };
    
    char time_buffer[50];
    strftime(time_buffer,sizeof(time_buffer),"%c",&dateTime);
        
    printf("date now: %s\n",time_buffer);

}

uint8_t int_to_bdc(int val)
{
    return ((val / 10) << 4) |  (val % 10);
}

int bcd_to_int(uint8_t val)
{
    return (((val & 0xf0) >> 4) * 10) + (val & 0x0f);
}
