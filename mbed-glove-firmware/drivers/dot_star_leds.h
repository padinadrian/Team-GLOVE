/*
 * Filename:  dot_star_leds.h
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
 *   Interface to the DotStar LEDs
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

#ifndef DOT_STAR_LEDS_H_
#define DOT_STAR_LEDS_H_

#include "mbed.h"

const PinName DOT_STAR_MOSI = p17;
const PinName DOT_STAR_MISO = p19; // not used, must be declared
const PinName DOT_STAR_SCLK = p18;

const uint8_t DOT_STAR_DEFAULT_BRIGHTNESS = 3; // 5-bit value 0-31

enum DotStarColor {Red, Green, Blue, Orange, Yellow, Maize, Purple, Cyan, Magenta, Pink, White, Off};

class DotStarLEDs {
public:

    /*
     * Initialize a string of DotStar LEDs,
     * given the number of the leds in the strand,
     * and the pins for the SPI output
     *
     * NOTE: MISO must be provided but is not used by the LEDs
     */
    DotStarLEDs(uint8_t num_leds=2, PinName mosi=DOT_STAR_MOSI,
            PinName miso=DOT_STAR_MISO, PinName sclk=DOT_STAR_SCLK);

    /*
     * Select LED in the strand (zero indexed, do your own bounds check, nerd)
     * Sets the RGB values to make the specified color
     * birghtness is 0-31
     */
    void set_color(uint8_t led, DotStarColor color,
            uint8_t brightness=DOT_STAR_DEFAULT_BRIGHTNESS);

    /*
     * Set all LEDs in the strand
     * Sets the RGB values to make the specified color
     * birghtness is 0-31
     */
    void set_color_all(DotStarColor color,
            uint8_t brightness=DOT_STAR_DEFAULT_BRIGHTNESS);

    /*
     * Select LED in the strand (zero indexed, do your own bounds check)
     * red, green, blue are 0-255
     * birghtness is 0-31
     */
    void set_RGB(uint8_t led, uint8_t red, uint8_t green, uint8_t blue,
            uint8_t brightness=DOT_STAR_DEFAULT_BRIGHTNESS);

    /*
     * Set all LEDs in the strand
     * red, green, blue are 0-255
     * birghtness is 0-31
     */
    void set_RGB_all(uint8_t red, uint8_t green, uint8_t blue,
            uint8_t brightness=DOT_STAR_DEFAULT_BRIGHTNESS);


private:
    uint8_t num_leds;
    SPI spi;
    // buffer with the packed led's values in it (plus start and stop codes)
    uint32_t* leds;
    // dirty bit, skips flush_to_spi to avoid waver
    bool clean;

    void flush_to_spi();
    void set_led_rgb(uint8_t led, uint8_t red, uint8_t green,
            uint8_t blue, uint8_t brightness);
};

/*
 * Given the upper and lower bound on unsigned analog values,
 * map the value into the range and return as a percent
 */
float map_unsigned_analog_to_percent(uint16_t min_, uint16_t max_, uint16_t val);
float map_float_analog_to_percent(float min_, float max_, float val);

#endif /* DOT_STAR_LEDS_H_ */
