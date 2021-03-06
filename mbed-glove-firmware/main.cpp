/*
 * Filename:  main.cpp
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
 *  Defines the function of the glove firmware and
 *  its idle lights pattern
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

#include <inttypes.h>
#include <utility>

#include "mbed.h"

#include "drivers/collector.h"
#include "drivers/ble_advert.h"
#include "drivers/dot_star_leds.h"

extern void blink(void);
extern void boot_delay(uint8_t);
extern void sensors_to_lights(void);
extern void ble_scan_test(void);
extern void uart_test(void);
extern void advert_test(void);
extern void touch_to_lights(void);
extern void imu_to_lights(void);

typedef pair<DotStarColor, DotStarColor> color_pair_t;
#if defined RIGHT_GLOVE
const uint8_t num_led_patterns_c = 2;
const color_pair_t led_patterns_c[num_led_patterns_c] = {
    color_pair_t(Blue, Maize),
    color_pair_t(Maize, Blue),
};
const uint8_t portal_intensity_c = 12;
const DotStarColor portal_color_c = Blue;
#elif defined LEFT_GLOVE
const uint8_t num_led_patterns_c = 3;
const color_pair_t led_patterns_c[num_led_patterns_c] = {
    color_pair_t(Blue, Cyan),
    color_pair_t(Red, Magenta),
    color_pair_t(Green, Yellow),
};
const uint8_t portal_intensity_c = 11;
const DotStarColor portal_color_c = Orange;
#else
#error "Must define either LEFT_GLOVE or RIGHT_GLOVE"
#endif

/*
 * Use this to signal or very the lights based on
 * the glove data or return true to let the set
 * pattern continue.
 */
bool check_signal_conditions(const glove_sensors_raw_t& glove_data, DotStarLEDs& leds) {
    /*
     * Indicate IMU flatlining
     */
    float ubound = 0.001;
    float lbound = ubound - (2*ubound);
    if ((lbound < glove_data.imu.orient_pitch && glove_data.imu.orient_pitch < ubound)
         && (lbound < glove_data.imu.orient_roll && glove_data.imu.orient_roll < ubound)) {

        leds.set_color_all(Pink, 10);
        return false;
    }

    /*
     * Indicate the first flex sensor is pulled
     */
    if (glove_data.flex_sensors[0] < 500) {
        leds.set_color_all(portal_color_c, portal_intensity_c);
        return false;
    }

    /*
     * Indicate a touch has occured
     */
    if (glove_data.touch_sensor.a || glove_data.touch_sensor.b || glove_data.touch_sensor.c || glove_data.touch_sensor.d) {
        leds.set_color_all(Magenta, 3);
        return false;
    }

    return true;
}

void launch() {
    DigitalOut l1(LED1);
    DigitalOut l2(LED2);
    DigitalOut l3(LED3);
    DigitalOut l4(LED4);
    l1 = 1; l2 = 1; l3 = 1; l4 = 1;

    DotStarLEDs ds_leds(2);
    ds_leds.set_color_all(White);

    // used by glove sensors compressGloveData
    crcInit();

    I2C i2c(I2C_SDA0, I2C_SCL0); // Initialize i2c bus for imu and touch_sensor
    IMU_BNO055 imu(i2c);
    TouchSensor touch_sensor(i2c, TOUCH_INTERRUPT);
    FlexSensors flex_sensors;

    // This encapsulates the BLE stack
    AdvertBLE adble;

    Collector collector(flex_sensors, imu, touch_sensor, adble);
    collector.startUpdateTask(20);
    const glove_sensors_raw_t& glove_data = collector.getGloveSensorData();

    //Blink blk(adble); blk.startUpdateTask();
    l1 = 0;

    //glove_sensors_raw_t glove_data;

    for (;;) {
        l2 = !l2;
        adble.waitForEvent();
        Thread::wait(50);
    }
    l1 = 0;
    DigitalOut d1(p20);
    /* The following main loop sets the light pattern on the
     * gloves after all the sensors have been initialized,
     * the BLE advertising set up, and the collector is running
     */
    for (;;) {
        //printf("f: %d, t: 0x%x\r\n", collector.flex_data[0], collector.touch_data->pack());
        //printf("comp: %d, fp: %f\r\n", compress_double(110.565), extract_double(11056));

        for (uint8_t i = 0; i < num_led_patterns_c; ++i) {
            l1 = !l1;
            for (int j=0; j < 50; ++j) {
                if (check_signal_conditions(glove_data, ds_leds)) {
                    ds_leds.set_color(0, led_patterns_c[i].first);
                    ds_leds.set_color(1, led_patterns_c[i].second);
                }
                Thread::wait(10);
            }
        }
    }
}

int main() {

    /*
     * The current setup with main.cpp is to have single calls to
     * test functions so we can each have test code without having
     * to comment out/have multiple versions.
     * Just change your local one to call the test loop you need.
     */
    launch();
}
