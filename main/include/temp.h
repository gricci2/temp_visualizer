/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#ifndef TEMP_H
#define TEMP_H

/* Includes */
/* ESP APIs */
#include <stdint.h>
#include "driver/i2c_master.h"

/* Defines */
#define TEMP_TASK_PERIOD (2000 / portTICK_PERIOD_MS)

/* Public function declarations */
float get_temp(void);
void update_temp(void);
void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle);
void update_temp(void);

#endif 
