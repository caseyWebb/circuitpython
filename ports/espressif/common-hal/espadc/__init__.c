/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Casey Webb for Adafruit Industries LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "common-hal/analogio/AnalogIn.h"
#include "bindings/espadc/__init__.h"
#include "py/runtime.h"

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

mp_int_t common_hal_espadc_raw_to_voltage(const mp_obj_t raw_obj) {
    mp_int_t raw_value = mp_obj_get_int(raw_obj);
    adc_cali_scheme_ver_t cali_scheme;
    adc_cali_handle_t cali_handle;
    const adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ATTENUATION,
        .bitwidth = DATA_WIDTH,
    };

    ESP_ERROR_CHECK(adc_cali_check_scheme(&cali_scheme));

    switch (cali_scheme)
    {
        case ADC_CALI_SCHEME_VER_LINE_FITTING:
            ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle));
            break;

        case ADC_CALI_SCHEME_VER_CURVE_FITTING:
            mp_raise_NotImplementedError(NULL);
    }

    int voltage;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_value, &voltage));

    switch (cali_scheme)
    {
        case ADC_CALI_SCHEME_VER_LINE_FITTING:
            ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(cali_handle));
            break;

        case ADC_CALI_SCHEME_VER_CURVE_FITTING:
            mp_raise_NotImplementedError(NULL);
    }

    return voltage;
}
