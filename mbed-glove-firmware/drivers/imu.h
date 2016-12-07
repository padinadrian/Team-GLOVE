/*
 * Filename:  imu.h
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
 *  Top-level interface for a task to get data from the IMU
 *
 *  Uses the BNO055_fusion mbed library by Kenji Arai
 */

#ifndef IMU_H_
#define IMU_H_

#include "BNO055.h"
/*
 * PinName definitions for the BNO055 on the board
 */
//const PinName IMU_I2C_SCL = I2C_SCL0;  // = p7
//const PinName IMU_I2C_SDA = I2C_SDA0;  // = p30
const PinName IMU_RST = p20;

/*
 * Update Period (in milliseconds)
 */
const uint32_t IMU_UPDATE_PERIOD = 10;

/*
 * Section 3.4 Axis Remap, p.25
 * On prototype in P1
 * On PCB in P?
 */
const BNO055_MOUNT_POSITIONS_TypeDef IMU_MOUNT_POSITION = MT_P1;

/* bno_imu_t
 *
 * Euler orientation in degrees (pitch, roll, yaw)
 * Linear acceleration in m/s^2 (gravity not included!)
 *
 * This structure can be directly serialized
 * and sent over BLE
 */
typedef struct {
    float orient_pitch;
    float orient_roll;
    float orient_yaw;

    float accel_x;
    float accel_y;
    float accel_z;
} bno_imu_t;

class IMU_BNO055 {
public:
    /*
     * Constructor initializes the BNO055 and takes care
     * of the default configuration.
     */
    IMU_BNO055(I2C& i2c);

    /*
     * Update the orientation and acceleration information
     * Callback for the periodic mode
     */
    void update();

    /*
     * Calls the start() method on the periodic update task,
     * an internal timer is set up in the constructor
     */
    //void startUpdateTask(uint32_t ms=IMU_UPDATE_PERIOD);

    /*
     * Calls the stop() method on the periodic update timer,
     */
    //void stopUpdateTask();

    /*
     * Write the imu orientation values to the given struct
     * This assumes no ownership or locking of the given container
     */
    void writeSensors(bno_imu_t*);

    /*
     * Single function call for manual polling
     * Calls update then writes to the provided
     */
    void updateAndWrite(bno_imu_t*);

    /*
     * something something do manual calibration
     */
    // .manualCalibration()

    /*
     * For debuggs
     */
    void print(Serial& debug_out);

    uint8_t hwDebugCheckVal () {
        return imu.read_calib_status();
    }

private:
    BNO055 imu;
    bno_imu_t imu_data;

    BNO055_ID_INF_TypeDef bno055_id_inf;
    BNO055_EULER_TypeDef euler_angles;
    BNO055_LIN_ACC_TypeDef linear_acc;

    //RtosTimer* update_task_timer;
};

/*
 * Need some way to save the calibration for the IMU
 * and then reload it on setup
 */
void saveIMUCalibration();
void loadIMUCalibration();

#endif /* IMU_H_ */
