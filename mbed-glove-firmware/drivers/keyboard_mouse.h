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
#include "KeyboardMouseService.h"
#include "ble/services/BatteryService.h"
#include "ble/services/DeviceInformationService.h"

/* IO capabilities of the device. */
#ifndef HID_SECURITY_IOCAPS
#define HID_SECURITY_IOCAPS (SecurityManager::IO_CAPS_NONE)
#endif

/* Security level. */
#ifndef HID_SECURITY_REQUIRE_MITM
#define HID_SECURITY_REQUIRE_MITM false
#endif

static const char DEVICE_NAME[] = "TeamGLOVE";
static const char SHORT_NAME[] = "glove1"; 

/* Wrapper class for Keyboard Mouse BLE Service */
class KeyboardMouse {

public:

    /* Constructor and Destructor*/
    KeyboardMouse();
    ~KeyboardMouse();
    
    /******************** MOUSE INTERFACE ********************/

    /* Set a button to be pressed or released
     * Parameters:
     *   button - button selection (can be LEFT, RIGHT, or MIDDLE) 
     *   state - button status (can be UP or DOWN)
     */
    void setMouseButton(MouseButton button, ButtonState state);
    
    /* Set the speed of the mouse cursor in the x direction
     * Parameters:
     *   speed - speed of the cursor (range -127 to 127)
     */
    void setMouseSpeedX(int8_t speed);
    
    /* Set the speed of the mouse cursor in the y direction
     * Parameters:
     *   speed - speed of the cursor (range -127 to 127)
     */
    void setMouseSpeedY(int8_t speed);
    
    /* Set the scroll value of the mouse scroll wheel 
     * Parameters:
     *   speed - speed of the scroll wheel (range -127 to 127)
     */
    void setMouseScroll(int8_t speed);
    
    /* Set the x, y, and scroll speed of the mouse
     * Parameters:
     *   x - speed of the cursor in x (range -127 to 127)
     *   y - speed of the cursor in y (range -127 to 127)
     *   scroll - speed of the scroll wheel (range -127 to 127)
     */
    void setMouseSpeedAll(int8_t x, int8_t y, int8_t scroll);
    
    /******************** KEYBOARD INTERFACE ********************/
    
    /* Send a given character to the keyboard
     * Parameters:
     *   c - character to send
     */
    void sendChar(char c);
    
    /* Set a keyboard button to be 'pressed' 
     * Parameters:
     *
     *   key - the key which is being pressed. This can either be
     *      an ASCII character (must be one which is represented
     *      on a standard keyboard) or a function key, such as 
     *      RIGHT_ARROW or KEY_HOME.
     *
     *   modifier - a command which modifies the key being pressed.
     *      Modifiers which are currently supported are:
     *      - KEY_CTRL
     *      - KEY_SHIFT
     *      - KEY_ALT
     *
     *      See keyboard_types.h for more information on function
     *      keys and modifiers.
     */
    void keyPress(uint8_t key, uint8_t modifier);
    
    /* Set the keyboard to be all buttons released */
    void keyRelease();
    
    
    /******************** BLE INTERFACE ********************/    

    /* Check if the device is paired to a computer */
    bool isConnected() { return service_ptr->isConnected(); }
    
    /* Wait to be interrupted */
    void waitForEvent() { ble.waitForEvent(); }
    

private:

    /******************** PRIVATE VARIABLES ********************/
    KeyboardMouseService * service_ptr;
    BLE ble;
};

