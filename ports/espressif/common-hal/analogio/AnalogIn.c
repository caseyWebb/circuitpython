/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Lucian Copeland for Adafruit Industries
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
#include "shared-bindings/analogio/AnalogIn.h"
#include "py/mperrno.h"
#include "py/runtime.h"
#include "supervisor/shared/translate/translate.h"

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#include "shared-bindings/microcontroller/Pin.h"

#include <string.h>

#define DEFAULT_VREF 1100
#define NO_OF_SAMPLES 2

void common_hal_analogio_analogin_construct(analogio_analogin_obj_t *self,
    const mcu_pin_obj_t *pin) {
    if (pin->adc_index == NO_ADC || pin->adc_channel == NO_ADC_CHANNEL) {
        raise_ValueError_invalid_pin();
    }
    common_hal_mcu_pin_claim(pin);
    self->pin = pin;
    // Pull-ups are enabled by default for power-saving reasons on quiescent pins.
    // Turn off the pull-up as soon as we know the pin will be used for analog reads,
    // since it may take a while for the voltage to stabilize if the input is high-impedance.
    gpio_pullup_dis(pin->number);
}

bool common_hal_analogio_analogin_deinited(analogio_analogin_obj_t *self) {
    return self->pin == NULL;
}

void common_hal_analogio_analogin_deinit(analogio_analogin_obj_t *self) {
    if (common_hal_analogio_analogin_deinited(self)) {
        return;
    }
    reset_pin_number(self->pin->number);
    self->pin = NULL;
}

uint16_t common_hal_analogio_analogin_get_value(analogio_analogin_obj_t *self) {
    adc_unit_t adc_unit = self->pin->adc_index;
    adc_channel_t adc_channel = self->pin->adc_channel;

    switch (adc_unit)
    {
        case ADC_UNIT_1:
        case ADC_UNIT_2:
            break;

        default:
            raise_ValueError_invalid_pin();
    }

    adc_oneshot_unit_handle_t adc_handle;
    adc_cali_scheme_ver_t calibration_scheme;
    adc_cali_handle_t calibration_handle;

    const adc_oneshot_unit_init_cfg_t adc_init_cfg = {
        .unit_id = adc_unit,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    const adc_oneshot_chan_cfg_t adc_channel_config = {
        .bitwidth = DATA_WIDTH,
        .atten = ATTENUATION,
    };
    const adc_cali_line_fitting_config_t calibration_config = {
        .unit_id = adc_unit,
        .atten = ATTENUATION,
        .bitwidth = DATA_WIDTH,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_init_cfg, &adc_handle));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, adc_channel, &adc_channel_config));
    ESP_ERROR_CHECK(adc_cali_check_scheme(&calibration_scheme));

    switch (calibration_scheme)
    {
        case ADC_CALI_SCHEME_VER_LINE_FITTING:
            ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&calibration_config, &calibration_handle));
            break;

        case ADC_CALI_SCHEME_VER_CURVE_FITTING:
            mp_raise_NotImplementedError(NULL);
    }

    int out_raw;
    int out_voltage;

    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, adc_channel, &out_raw));
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));

    // This corrects non-linear regions of the ADC range with a LUT, so it's a better reading than raw
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(calibration_handle, out_raw, &out_voltage));
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(calibration_handle));

    return out_voltage * ((1 << 16) - 1) / 3300;
}

float common_hal_analogio_analogin_get_reference_voltage(analogio_analogin_obj_t *self) {
    return 3.3f;
}
