/*
 * Filename:  flex_sensor.h
 * Project:   EECS 473 - Team GLOVE
 * Date:      Fall 2016
 * Authors:
 *     Nick Bertoldi
 *     Ben Heckathorn
 *     Ryan O’Keefe
 *     Adrian Padin
 *     Tim Schumacher
 *
 * Purpose:
 *  Top-level interface to the flex sensors
 *
 * Copyright (c) 2016 by Nick Bertoldi, Ben Heckathorn, Ryan O'Keefe,
 *                       Adrian Padin, Timothy Schumacher
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef FLEX_SENSOR_H_
#define FLEX_SENSOR_H_

#include <inttypes.h>

#include "mbed.h"

#define FLEX_SENSORS_COUNT 4

#if defined(TARGET_NRF51_DK)
const PinName FLEX_0 = A0;
const PinName FLEX_1 = A1;
const PinName FLEX_2 = A2;
const PinName FLEX_3 = A3;
#elif defined(TARGET_MCU_NRF51822)
const PinName FLEX_0 = p1;
const PinName FLEX_1 = p2;
const PinName FLEX_2 = p3;
const PinName FLEX_3 = p4;
#endif

/* flex_sensor_t
 *
 * deflection: scaled analog-to-digital value read from the GPIO
 */
typedef uint16_t flex_sensor_t;


/* FlexSensors
 *
 * Single class to handle the flex sensor analog read objects,
 * with methods to update internally, and write into the
 * marshelled all sensors data structure
 */
class FlexSensors {
public:
    /*
     * Constructor initializes the AnalogIn objects
     */
    FlexSensors();

    /*
     * Update the deflection for all flex sensors
     */
    void update();

    /*
     * Write the flex sensor values to the given array.
     * This assumes no ownership or locking of the given container
     */
    void writeSensors(flex_sensor_t* buf);

    /*
     * Alternative interface to both update each pin
     * And write it to the destination buffer
     */
    void updateAndWrite(flex_sensor_t* buf);

    /*
     * Print the value of all the flex sensors for debugging
     */
    void print(Serial& debug_out);

    /*
     * Print the value of one sensor
     */
    void printSingle(Serial& pc, uint8_t index);

private:
    flex_sensor_t values[FLEX_SENSORS_COUNT];
    AnalogIn* pins[FLEX_SENSORS_COUNT];
};
#endif /* FLEX_SENSOR_H_ */
