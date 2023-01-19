/*
 * @brief: IMU ICM20948 Driver
 * @file: IMU_ICM20948.h
 * @details: Driver for IMU ICM20948 
 * @author: Hussien Saleh
 * @date: 2021
 * @copyright: Institut f�r Mechatronik (IMEK) - Technische Universit�t Hamburg Harburg (TUHH)
 *
*/

#ifndef IMU_ICM20948_H_
#define IMU_ICM20948_H_

#include <atmel_start.h>
#include "config.h"
#include "scheduler/scheduler.h"
#include "data_processing/fifo_ring_buffer.h"
#include "ECAL/on_board/SD/SD.h"

scheduled_task_description_ IMU_sampling_update_task;
scheduled_task_description_ IMU_update_task;

#define ICM20948_OK 0x0000 //no errors
#define ICM20948_ERROR_INVALID_DEVICE_ID 0x0001 //invalid device id

/* Bank Select register*/
#define ICM20948_REG_BANK_SEL            0x7F                 
/* ICM20948 register banks */
#define ICM20948_BANK_0                  ( 0 << 7 )   /**< Register bank 0 */
#define ICM20948_BANK_1                  ( 1 << 7 )   /**< Register bank 1 */
#define ICM20948_BANK_2                  ( 2 << 7 )   /**< Register bank 2 */
#define ICM20948_BANK_3                  ( 3 << 7 )   /**< Register bank 3 */

#define ICM20948_REG_ACCEL_XOUT_H_SH     ( ICM20948_BANK_0 | 0x2D )  /**< Accelerometer X-axis data high byte                    */
#define ICM20948_REG_ACCEL_XOUT_L_SH     ( ICM20948_BANK_0 | 0x2E )  /**< Accelerometer X-axis data low byte                     */
#define ICM20948_REG_ACCEL_YOUT_H_SH     ( ICM20948_BANK_0 | 0x2F )  /**< Accelerometer Y-axis data high byte                    */
#define ICM20948_REG_ACCEL_YOUT_L_SH     ( ICM20948_BANK_0 | 0x30 )  /**< Accelerometer Y-axis data low byte                     */
#define ICM20948_REG_ACCEL_ZOUT_H_SH     ( ICM20948_BANK_0 | 0x31 )  /**< Accelerometer Z-axis data high byte                    */
#define ICM20948_REG_ACCEL_ZOUT_L_SH     ( ICM20948_BANK_0 | 0x32 )  /**< Accelerometer Z-axis data low byte                     */

#define ICM20948_REG_GYRO_XOUT_H_SH      ( ICM20948_BANK_0 | 0x33 )  /**< Gyroscope X-axis data high byte                        */
#define ICM20948_REG_GYRO_XOUT_L_SH      ( ICM20948_BANK_0 | 0x34 )  /**< Gyroscope X-axis data low byte                         */
#define ICM20948_REG_GYRO_YOUT_H_SH      ( ICM20948_BANK_0 | 0x35 )  /**< Gyroscope Y-axis data high byte                        */
#define ICM20948_REG_GYRO_YOUT_L_SH      ( ICM20948_BANK_0 | 0x36 )  /**< Gyroscope Y-axis data low byte                         */
#define ICM20948_REG_GYRO_ZOUT_H_SH      ( ICM20948_BANK_0 | 0x37 )  /**< Gyroscope Z-axis data high byte                        */
#define ICM20948_REG_GYRO_ZOUT_L_SH      ( ICM20948_BANK_0 | 0x38 )  /**< Gyroscope Z-axis data low byte                         */

#define ICM20948_REG_TEMPERATURE_H       ( ICM20948_BANK_0 | 0x39 )  /**< Temperature data high byte                             */
#define ICM20948_REG_TEMPERATURE_L       ( ICM20948_BANK_0 | 0x3A )  /**< Temperature data low byte                              */
#define ICM20948_REG_TEMP_CONFIG         ( ICM20948_BANK_0 | 0x53 )  /**< Temperature Configuration register                     */

#define ICM20948_REG_ACCEL_CONFIG        ( ICM20948_BANK_2 | 0x14 )  /**< Accelerometer Configuration register                   */
#define ICM20948_BIT_ACCEL_FCHOICE       0x01                        /**< Accelerometer Digital Low-Pass Filter enable bit               */
#define ICM20948_SHIFT_ACCEL_FS          1                           /**< Accelerometer Full Scale Select bit shift                      */
#define ICM20948_SHIFT_ACCEL_DLPCFG      3                           /**< Accelerometer DLPF Config bit shift                            */
#define ICM20948_MASK_ACCEL_FULLSCALE    0x06                        /**< Accelerometer Full Scale Select bit mask                        */
#define ICM20948_MASK_ACCEL_BW           0x39                        /**< Accelerometer Bandwidth Select bit mask                         */
#define ICM20948_ACCEL_FULLSCALE_2G      ( 0x00 << ICM20948_SHIFT_ACCEL_FS )  /**< Accelerometer Full Scale = 2 g  */
#define ICM20948_ACCEL_FULLSCALE_4G      ( 0x01 << ICM20948_SHIFT_ACCEL_FS )  /**< Accelerometer Full Scale = 4 g  */
#define ICM20948_ACCEL_FULLSCALE_8G      ( 0x02 << ICM20948_SHIFT_ACCEL_FS )  /**< Accelerometer Full Scale = 8 g  */
#define ICM20948_ACCEL_FULLSCALE_16G     ( 0x03 << ICM20948_SHIFT_ACCEL_FS )  /**< Accelerometer Full Scale = 16 g */

#define ICM20948_REG_GYRO_CONFIG_1       ( ICM20948_BANK_2 | 0x01 )  /**< Gyroscope Configuration 1 register         */
#define ICM20948_BIT_GYRO_FCHOICE        0x01                        /**< Gyro Digital Low-Pass Filter enable bit    */
#define ICM20948_SHIFT_GYRO_FS_SEL       1                           /**< Gyro Full Scale Select bit shift           */
#define ICM20948_SHIFT_GYRO_DLPCFG       3                           /**< Gyro DLPF Config bit shift                 */
#define ICM20948_MASK_GYRO_FULLSCALE     0x06                        /**< Gyro Full Scale Select bit mask             */
#define ICM20948_MASK_GYRO_BW            0x39                        /**< Gyro Bandwidth Select bit mask              */
#define ICM20948_GYRO_FULLSCALE_250DPS   ( 0x00 << ICM20948_SHIFT_GYRO_FS_SEL )  /**< Gyro Full Scale = 250 deg/sec  */
#define ICM20948_GYRO_FULLSCALE_500DPS   ( 0x01 << ICM20948_SHIFT_GYRO_FS_SEL )  /**< Gyro Full Scale = 500 deg/sec  */
#define ICM20948_GYRO_FULLSCALE_1000DPS  ( 0x02 << ICM20948_SHIFT_GYRO_FS_SEL )  /**< Gyro Full Scale = 1000 deg/sec */
#define ICM20948_GYRO_FULLSCALE_2000DPS  ( 0x03 << ICM20948_SHIFT_GYRO_FS_SEL )  /**< Gyro Full Scale = 2000 deg/sec */

#define ICM20948_REG_PWR_MGMT_1 ( ICM20948_BANK_0 | 0x06 )  /**< Power Management 1 register */
#define ICM20948_BIT_H_RESET             0x80                        /**< Device reset bit                                       */
//#define ICM20948_BIT_SLEEP               0x40                        /**< Sleep mode enable bit   */
#define ICM20948_BIT_SLEEP				6							// bit 6, enable and disable sleep mode 
#define ICM20948_BIT_LP_EN               0x20                        /**< Low Power feature enable bit                           */
#define ICM20948_BIT_TEMP_DIS            0x08                        /**< Temperature sensor disable bit                         */
#define ICM20948_BIT_CLK_PLL             0x01                        /**< Auto clock source selection setting					 */

#define ICM20948_REG_WHO_AM_I            ( ICM20948_BANK_0 | 0x00 )  /**< Device ID register                                     */
//ICM20948 Device ID
#define ICM20948_DEVICE_ID				0xEAU

/* Function prototypes */

/************************************************************************************************************/
void config_IMU_tasks_();
/************************************************************************************************************/
void task_config_IMU_sampling_update_();
void task_config_IMU_update_();
/************************************************************************************************************/
void IMU_sampling_update_cb_(void *param, void* param_2);
void IMU_update_cb_(void* param, void* param_2);
/************************************************************************************************************/

 /* @brief
 *    Reads register from the ICM20948 device
 *
 * @param[in] addr
 *    The register address to read from in the sensor
 *    Bit[8:7] - bank address
 *    Bit[6:0] - register address
 *
 * @param[in] numBytes
 *    The number of bytes to read
 *
 * @param[out] data
 *    The data read from the register
 *
 * @return
 *    None
 */
void ICM20948_registerRead_( uint16_t addr, int numBytes, uint8_t *data );

/* @brief
 *    selects the bank
 *
 * @return
 *    None
 */
void ICM20948_bankSelect_( uint8_t bank );
/*
 * @brief
 *    Writes a register in the ICM20948 device
 *
 * @param[in] addr
 *    The register address to write
 *    Bit[8:7] - bank address
 *    Bit[6:0] - register address
 *
 * @param[in] data
 *    The data to write to the register
 *
 * @return
 *    None
 */
void ICM20948_registerWrite_( uint16_t addr, uint8_t data );

/*
 * @brief
 *    Reads the raw accelerometer value and converts to deg/sec value based on
 *    the actual resolution
 *
 * @param[out] acc
 *    A 3-element array of float numbers containing the gyroscope values
 *    for the x, y and z axes in deg/sec units.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 */
uint32_t ICM20948_accelDataRead_(int32_t *acc);
/*
 * @brief
 *    Reads the raw gyroscope value and converts to deg/sec value based on
 *    the actual resolution
 *
 * @param[out] gyro
 *    A 3-element array of float numbers containing the gyroscope values
 *    for the x, y and z axes in deg/sec units.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 */
uint32_t ICM20948_gyroDataRead_(int32_t *gyro);
/*
 * @brief
 *    Gets the Full Scale of the accelerometer
 *
 * @param[out] accelFS
 *    The full scale in g units
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 *
 */
uint32_t ICM20948_accelFullScaleGet_(uint8_t *accelFS);
/*
 * @brief
 *    Gets the full scale of the gyroscope
 *
 * @param[out] gyroFS
 *    The actual full scale in (deg/sec) units
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 *
 */

uint32_t ICM20948_gyroFullScaleGet_( uint16_t *gyroFS );
/*
 * @brief
 *    Sets the full scale value of the accelerometer
 *
 * @param[in] accelFs
 *    The desired full scale value. Use the ICM20948_ACCEL_FULLSCALE_xG
 *    macros, which are defined in the ICM20948.h file. The value of x can be
 *    2, 4, 8 or 16.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 */
uint32_t ICM20948_accelFullscaleSet_(uint8_t accelFs);
/*
 * @brief
 *    Sets the full scale value of the gyroscope
 *
 * @param[in] gyroFs
 *    The desired full scale value. Use the ICM20948_GYRO_FULLSCALE_yDPS
 *    macros, which are defined in the ICM20948.h file. The value of y can be
 *    250, 500, 1000 or 2000.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 *
 */
uint32_t ICM20948_gyroFullscaleSet_( uint8_t gyroFs );

/*
 * @brief
 *    Setup and initialize the IMU
 *
 */
uint8_t IMU_setup_();
/*
* @brief
*    Update the IMU readings 
*
*/
void IMU_read_();

// bool ret_imuSensor_status();

//void platform_read_TDK_imu(uint8_t reg, uint8_t *bufp, uint16_t len);
//void platform_write_TDK_imu(uint8_t reg, uint8_t *bufp, uint16_t len);
//void platform_TDK_imu_readAccelData(int16_t * destination);
//void platform_TDK_imu_readGyroData(int16_t * destination);

#endif /* IMU_ICM20948_H_ */