/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 caseyWebb
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

// #include "common-hal/analogio/AnalogIn.h"
// #include "shared-bindings/util.h"

#include "bindings/espadc/__init__.h"

#include "py/runtime.h"

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

//| """ESP ADC Calibration Module
//|
//| The `espadc` module adds ability to calibrate raw ADC readings to voltage. This is helpful
//| when using the ULP to read sensors and store the raw values in shared memory.
//|
//| .. code-block:: python
//|
//|     import espadc
//|
//|     espadc.raw_to_voltage(1000)
//| """
//| ...
//|

//| def raw_to_voltage(raw: int) -> Optional[int]:
//|     """Return the calibrated voltage for a raw ADC reading."""
//|     ...
//|

STATIC mp_obj_t espadc_raw_to_voltage(const mp_obj_t raw_obj) {
    mp_int_t voltage = common_hal_espadc_raw_to_voltage(raw_obj);
    if (voltage < 0) {
        return mp_const_none;
    }
    return MP_OBJ_NEW_SMALL_INT(voltage);
}
MP_DEFINE_CONST_FUN_OBJ_1(espadc_raw_to_voltage_obj, espadc_raw_to_voltage);

STATIC const mp_rom_map_elem_t espadc_module_globals_table[] = {
    // module name
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_espadc) },

    // module functions
    { MP_ROM_QSTR(MP_QSTR_get_raw_to_voltage), MP_OBJ_FROM_PTR(&espadc_raw_to_voltage_obj) },
};
STATIC MP_DEFINE_CONST_DICT(espadc_module_globals, espadc_module_globals_table);

const mp_obj_module_t espadc_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&espadc_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_espadc, espadc_module, CIRCUITPY_ESPADC);
