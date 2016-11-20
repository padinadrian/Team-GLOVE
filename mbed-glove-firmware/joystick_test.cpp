/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "ble/BLE.h"
#include "drivers/BLE_HID/JoystickService.h"
#include "examples_common.h"

#define LED_ON 0
#define LED_OFF 1


/******************** GLOBALS ********************/

static DigitalOut led1(LED1);
static DigitalOut led2(LED2);
static DigitalOut led3(LED3);
static DigitalOut led4(LED4);

static InterruptIn b1(BUTTON1);
static InterruptIn b2(BUTTON2);
static InterruptIn b3(BUTTON3);
static InterruptIn b4(BUTTON4);

static BLE ble;
static JoystickService* jsServicePtr;

static const char DEVICE_NAME[] = "AdrianJS";
static const char SHORT_DEVICE_NAME[] = "js0";


/******************** CALLBACKS ********************/

static void onDisconnect(const Gap::DisconnectionCallbackParams_t *params) {
    HID_DEBUG("disconnected\r\n");
    led1 = LED_OFF;
    jsServicePtr->setConnected(false);
    ble.gap().startAdvertising(); // restart advertising
}

static void onConnect(const Gap::ConnectionCallbackParams_t *params) {
    HID_DEBUG("connected\r\n");
    led1 = LED_ON;
    jsServicePtr->setConnected(true);
}

static void waiting() {
    if (!jsServicePtr->isConnected())
        led1 = !led1;
    else
        led1 = LED_ON;
}

void b1_rise() {
    led2 = LED_OFF;
}

void b1_fall() {
    led2 = LED_ON;
}


void joystick_test() {
    Ticker connect_ticker;

    b1.fall(b1_fall);
    b1.rise(b1_rise);

    HID_DEBUG("initialising ticker\r\n");

    heartbeat.attach(waiting, 1);

    HID_DEBUG("initialising ble\r\n");
    ble.init();

    ble.gap().onDisconnection(onDisconnect);
    ble.gap().onConnection(onConnect);

    initializeSecurity(ble);

    HID_DEBUG("adding hid service\r\n");

    jsServicePtr = new JoystickService(ble);

    HID_DEBUG("adding device info and battery service\r\n");
    initializeHOGP(ble);

    HID_DEBUG("setting up gap\r\n");
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::JOYSTICK);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME,
                                           (const uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                           (const uint8_t *)SHORT_DEVICE_NAME, sizeof(SHORT_DEVICE_NAME));

    ble.gap().setDeviceName((const uint8_t *)DEVICE_NAME);

    HID_DEBUG("advertising\r\n");
    ble.gap().startAdvertising();

    while (true) {
        ble.waitForEvent();
    }
}
