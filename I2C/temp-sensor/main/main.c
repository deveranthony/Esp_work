#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

// Define I2C settings
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_SDA_GPIO 8
#define I2C_MASTER_SCL_GPIO 10
#define I2C_MASTER_FREQ_HZ 400000
#define ADXL345_SENSOR_ADDR 0x53

// ... ADXL345 register definitions ...
#define ADXL345_REG_DEVID 0x00
#define ADXL345_REG_POWER_CTL 0x2D
#define ADXL345_REG_DATA_FORMAT 0x31
#define ADXL345_REG_DATAX0 0x32 // X-axis data 0

static const char *TAG = "ADXL345";

// Declare the I2C bus handle
static i2c_master_bus_handle_t bus_handle;

// Corrected I2C Init function
static esp_err_t i2c_master_init(void) {
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = I2C_MASTER_SCL_GPIO,
        .sda_io_num = I2C_MASTER_SDA_GPIO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    // Create the bus and store the handle in 'bus_handle'
    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &bus_handle);
    return ret;
}

// Corrected ADXL345 Init function (uses the global bus_handle)
static esp_err_t adxl345_init(i2c_master_dev_handle_t *dev_handle) {
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ADXL345_SENSOR_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    // Add the device to the bus we already created
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, dev_handle));
    // ... rest of the function ...
    uint8_t device_id;
    ESP_ERROR_CHECK(adxl345_register_read(*dev_handle, ADXL345_REG_DEVID, &device_id, 1));
    ESP_LOGI(TAG, "ADXL345 Device ID: 0x%02X", device_id);

    // Put the sensor into measurement mode
    ESP_ERROR_CHECK(adxl345_register_write_byte(*dev_handle, ADXL345_REG_POWER_CTL, 0x08)); // 0x08 = Measurement mode

    // Set the data format to +/-4g, full resolution
    ESP_ERROR_CHECK(adxl345_register_write_byte(*dev_handle, ADXL345_REG_DATA_FORMAT, 0x09));

    return ESP_OK;
}

static esp_err_t adxl345_read_acceleration(i2c_master_dev_handle_t dev_handle, float *x, float *y, float *z) {
    uint8_t data_buffer[6]; // 6 bytes for X, Y, Z (each axis is 2 bytes)

    // Read all 6 data registers in one go
    ESP_ERROR_CHECK(adxl345_register_read(dev_handle, ADXL345_REG_DATAX0, data_buffer, sizeof(data_buffer)));

    // Convert the raw data (two 8-bit bytes) into a 16-bit signed integer
    // The ADXL345 is little-endian: X0 is LSB, X1 is MSB
    int16_t raw_x = (int16_t)((data_buffer[1] << 8) | data_buffer[0]);
    int16_t raw_y = (int16_t)((data_buffer[3] << 8) | data_buffer[2]);
    int16_t raw_z = (int16_t)((data_buffer[5] << 8) | data_buffer[4]);

    // Convert to g-force. The scale factor is 4g / 1024 LSB/g = 0.0039 for +/-4g range.
    // Adjust if using a different range (e.g., +/-2g = 2/512 = 0.0039, +/-8g = 8/256 = 0.03125).
    const float scale_factor = 0.0039;
    *x = raw_x * scale_factor;
    *y = raw_y * scale_factor;
    *z = raw_z * scale_factor;

    return ESP_OK;
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing I2C");
    ESP_ERROR_CHECK(i2c_master_init()); // This now properly sets up the bus

    i2c_master_dev_handle_t adxl345_handle;
    ESP_LOGI(TAG, "Initializing ADXL345 Sensor");
    ESP_ERROR_CHECK(adxl345_init(&adxl345_handle)); // This now uses the valid bus

    // ... rest of your app_main ...
    float accel_x, accel_y, accel_z;

    while (1) {
        // Read the acceleration values
        if (adxl345_read_acceleration(adxl345_handle, &accel_x, &accel_y, &accel_z) == ESP_OK) {
            ESP_LOGI(TAG, "X: %.2fg, Y: %.2fg, Z: %.2fg", accel_x, accel_y, accel_z);
        } else {
            ESP_LOGE(TAG, "Failed to read data from sensor");
        }

        // Wait for 1 second before reading again
        vTaskDelay(pdMS_TO_TICKS(1000));
}