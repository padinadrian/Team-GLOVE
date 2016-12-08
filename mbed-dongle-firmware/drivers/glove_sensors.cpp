/*
 * Filename:  glove_sensors.h
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
 *  This is the definition of the struct used on either end
 *  of the bluetooth connection, and it contains synchronization things.
 *
 *  Implements the compression/decompression for glove sensors
 */

#include "glove_sensors.h"

void compressGloveSensors(glove_sensors_raw_t* raw, glove_sensors_compressed_t* compressed) {
    compressed->f[0] = ((raw->flex_sensors[0] & 0x0FFF) << 4) | ((raw->flex_sensors[1] & 0x0FFF) >> 8);
    compressed->f[1] = ((raw->flex_sensors[1] & 0x00FF) << 8) | ((raw->flex_sensors[2] & 0x0FFF) >> 4);
    compressed->f[2] = ((raw->flex_sensors[2] & 0x000F) << 12) | (raw->flex_sensors[3] & 0x0FFF);

    compressed->t = 0x00FF & raw->touch_sensor.pack();

    compressed->roll = (raw->imu.orient_roll);
    compressed->pitch = (raw->imu.orient_pitch);
    compressed->yaw = (raw->imu.orient_yaw);

    compressed->checksum = crcFast((uint8_t*)compressed, glove_sensors_compressed_size_no_crc);
}

int extractGloveSensors(glove_sensors_raw_t& raw, glove_sensors_compressed_t* compressed) {

    /*
    uint16_t crc_result = crcFast((uint8_t*)compressed, glove_sensors_compressed_size_no_crc);
    if (crc_result != compressed->checksum) {
        return -1;
    }
    */

    raw.flex_sensors[0] = (compressed->f[0] >> 4);
    raw.flex_sensors[1] = ((compressed->f[0] & 0x000F) << 8) | (compressed->f[1] >> 8);
    raw.flex_sensors[2] = ((compressed->f[1] & 0x00FF) << 4) | (compressed->f[2] >> 12);
    raw.flex_sensors[3] = (compressed->f[2] & 0x0FFF);

    raw.touch_sensor.a = (compressed->t & 0x8) >> 3;
    raw.touch_sensor.b = (compressed->t & 0x4) >> 2;
    raw.touch_sensor.c = (compressed->t & 0x2) >> 1;
    raw.touch_sensor.d = (compressed->t & 0x1);

    raw.imu.orient_roll = compressed->pitch;
    raw.imu.orient_pitch = compressed->roll;
    raw.imu.orient_yaw = compressed->yaw;

    return 0;
}
