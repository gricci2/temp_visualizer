#include <stdio.h>
#include "temp.h"
#include "esp_log.h"

#define TMP102_ADDR 0x48

static i2c_master_dev_handle_t tmp102_dev;

static float temp;

void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = TMP102_ADDR,
        .scl_speed_hz = 10000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));

    tmp102_dev = *dev_handle;
}

float get_temp(void)
{
    return temp;
}

void update_temp(void)
{
    uint8_t write_buf = 0x00;

    uint8_t read_buf[2];
    float temp_c;

    esp_err_t err = i2c_master_transmit_receive(tmp102_dev, &write_buf, 1, read_buf, 2, 1000);

    ESP_LOGI("TEMP", "err=%d raw=%02X %02X",
         err, read_buf[0], read_buf[1]);

    uint16_t temp_bits = ((uint16_t)read_buf[0] << 4) | (read_buf[1] >> 4);
        if(temp_bits > 0x7FF)
        {
            temp_bits |= 0xF000;
        }

        ESP_LOGI("TEMP", "temp_bits = 0x%04X", temp_bits);

        temp_c = temp_bits * 0.0625;

        ESP_LOGI("TEMP", "temp_c = %.2f", temp_c);

        temp = temp_c;
}