/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 microDev
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

#pragma once

#include "py/obj.h"
#include "common-hal/espulp/ULP.h"

#include "shared-bindings/microcontroller/Pin.h"

extern const mp_obj_type_t espulp_ulp_type;

void common_hal_espulp_ulp_construct(espulp_ulp_obj_t *self, espulp_architecture_t arch);
bool common_hal_espulp_ulp_deinited(espulp_ulp_obj_t *self);
void common_hal_espulp_ulp_deinit(espulp_ulp_obj_t *self);

void common_hal_espulp_ulp_run(espulp_ulp_obj_t *self, uint32_t *program, size_t length, uint32_t pin_mask, uint16_t adc_channel_mask);
void common_hal_espulp_ulp_halt(espulp_ulp_obj_t *self);
