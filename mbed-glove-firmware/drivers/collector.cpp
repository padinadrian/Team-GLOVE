/*
 * Filename:  collector.cpp
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
 *  Implementation of collector.h
 */

#include "collector.h"
#include "string.h"

const PinName COLLECTOR_DEBUG_PIN = p15;
static DigitalOut l4(p12);

Collector::Collector(FlexSensors* _flex, IMU_BNO055* _imu,
                     TouchSensor* _touch, AdvertBLE& _adble)
    : flex(_flex), imu(_imu), touch(_touch), adble(_adble),
      working(COLLECTOR_DEBUG_PIN) {

    flex_data = glove_data.flex_sensors; // ptr to the first flex_sensor_t
    touch_data = &(glove_data.touch_sensor); // ptr to the key_states_t struct
    imu_data = &(glove_data.imu); // ptr to the bno_imu_t struct in glove data

    update_task_timer =
          new RtosTimer(this, &Collector::updateAndAdvertise, osTimerPeriodic);
}

void Collector::updateAndAdvertise() {
    working = 1;
    l4 = 0;

    imu->updateAndWrite(imu_data);
    flex->updateAndWrite(flex_data);

    touch->spawnUpdateThread();
    l4 = 1;
    Thread::wait(10);
    l4 = 0;
    touch->writeKeys(touch_data);

    compressGloveSensors(&glove_data, &glove_data_compressed);
    adble.update((uint8_t*)&glove_data_compressed, glove_sensors_compressed_size);

    touch->terminateUpdateThreadIfBlocking();

    working = 0;
    l4 = 1;
    adble.waitForEvent();
}

void Collector::startUpdateTask(uint32_t ms) {
    update_task_timer->start(ms);
}

void Collector::stopUpdateTask() {
    update_task_timer->stop();
}
