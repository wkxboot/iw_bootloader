/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO023_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO023_FUNC_ALT0 0x00u
/*!
 * @brief Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-up. Pull-up resistor enabled. */
#define PIO023_MODE_PULL_UP 0x02u
/*!
 * @brief Controls open-drain mode.: Normal. Normal push-pull output */
#define PIO023_OD_NORMAL 0x00u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO04_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO04_FUNC_ALT0 0x00u
/*!
 * @brief Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-up. Pull-up resistor enabled. */
#define PIO04_MODE_PULL_UP 0x02u
/*!
 * @brief Controls open-drain mode.: Normal. Normal push-pull output */
#define PIO04_OD_NORMAL 0x00u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO119_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO119_FUNC_ALT0 0x00u
/*!
 * @brief Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-up. Pull-up resistor enabled. */
#define PIO119_MODE_PULL_UP 0x02u
/*!
 * @brief Controls open-drain mode.: Normal. Normal push-pull output */
#define PIO119_OD_NORMAL 0x00u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO11_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO11_FUNC_ALT0 0x00u
/*!
 * @brief Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-up. Pull-up resistor enabled. */
#define PIO11_MODE_PULL_UP 0x02u
/*!
 * @brief Controls open-drain mode.: Normal. Normal push-pull output */
#define PIO11_OD_NORMAL 0x00u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO120_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO120_FUNC_ALT0 0x00u
/*!
 * @brief Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-up. Pull-up resistor enabled. */
#define PIO120_MODE_PULL_UP 0x02u
/*!
 * @brief Controls open-drain mode.: Normal. Normal push-pull output */
#define PIO120_OD_NORMAL 0x00u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO17_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO17_FUNC_ALT0 0x00u
/*!
 * @brief Selects function mode (on-chip pull-up/pull-down resistor control).: Pull-up. Pull-up resistor enabled. */
#define PIO17_MODE_PULL_UP 0x02u
/*!
 * @brief Controls open-drain mode.: Normal. Normal push-pull output */
#define PIO17_OD_NORMAL 0x00u

/*! @name PIO1_19 (number 16), LOCK_CTRL
  @{ */
#define BOARD_INITPINS_LOCK_CTRL_GPIO GPIO /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_LOCK_CTRL_PORT 1U   /*!<@brief PORT device name: 1U */
#define BOARD_INITPINS_LOCK_CTRL_PIN 19U   /*!<@brief 1U pin index: 19 */
                                           /* @} */

/*! @name PIO0_23 (number 35), COMPRESSOR_CTRL
  @{ */
#define BOARD_INITPINS_COMPRESSOR_CTRL_GPIO GPIO /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_COMPRESSOR_CTRL_PORT 0U   /*!<@brief PORT device name: 0U */
#define BOARD_INITPINS_COMPRESSOR_CTRL_PIN 23U   /*!<@brief 0U pin index: 23 */
                                                 /* @} */

/*! @name PIO0_4 (number 87), UNLOCK_SW
  @{ */
#define BOARD_INITPINS_UNLOCK_SW_GPIO GPIO /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_UNLOCK_SW_PORT 0U   /*!<@brief PORT device name: 0U */
#define BOARD_INITPINS_UNLOCK_SW_PIN 4U    /*!<@brief 0U pin index: 4 */
                                           /* @} */

/*! @name PIO1_7 (number 18), LOCK_SENSOR
  @{ */
#define BOARD_INITPINS_LOCK_SENSOR_GPIO GPIO /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_LOCK_SENSOR_PORT 1U   /*!<@brief PORT device name: 1U */
#define BOARD_INITPINS_LOCK_SENSOR_PIN 7U    /*!<@brief 1U pin index: 7 */
                                             /* @} */

/*! @name PIO1_1 (number 55), HOLE_SENSOR
  @{ */
#define BOARD_INITPINS_HOLE_SENSOR_GPIO GPIO /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_HOLE_SENSOR_PORT 1U   /*!<@brief PORT device name: 1U */
#define BOARD_INITPINS_HOLE_SENSOR_PIN 1U    /*!<@brief 1U pin index: 1 */
                                             /* @} */

/*! @name PIO1_20 (number 17), DOOR_SENSOR
  @{ */
#define BOARD_INITPINS_DOOR_SENSOR_GPIO GPIO /*!<@brief GPIO device name: GPIO */
#define BOARD_INITPINS_DOOR_SENSOR_PORT 1U   /*!<@brief PORT device name: 1U */
#define BOARD_INITPINS_DOOR_SENSOR_PIN 20U   /*!<@brief 1U pin index: 20 */
                                             /* @} */

/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO010_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 4. */
#define PIO010_FUNC_ALT4 0x04u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO011_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 6. */
#define PIO011_FUNC_ALT6 0x06u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO012_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 6. */
#define PIO012_FUNC_ALT6 0x06u
/*!
 * @brief Select Analog/Digital mode.: Analog mode. */
#define PIO015_DIGIMODE_ANALOG 0x00u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO015_FUNC_ALT0 0x00u
/*!
 * @brief
 * Selects function mode (on-chip pull-up/pull-down resistor control).
 * : Pull-down.
 * Pull-down resistor enabled.
 */
#define PIO015_MODE_PULL_DOWN 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO016_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO016_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO022_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO022_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO02_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO02_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Analog mode. */
#define PIO031_DIGIMODE_ANALOG 0x00u
/*!
 * @brief Selects pin function.: Alternative connection 0. */
#define PIO031_FUNC_ALT0 0x00u
/*!
 * @brief
 * Selects function mode (on-chip pull-up/pull-down resistor control).
 * : Pull-down.
 * Pull-down resistor enabled.
 */
#define PIO031_MODE_PULL_DOWN 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO03_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO03_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO08_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 3. */
#define PIO08_FUNC_ALT3 0x03u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO09_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 3. */
#define PIO09_FUNC_ALT3 0x03u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO110_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 2. */
#define PIO110_FUNC_ALT2 0x02u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO113_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 2. */
#define PIO113_FUNC_ALT2 0x02u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO117_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 2. */
#define PIO117_FUNC_ALT2 0x02u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO118_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 2. */
#define PIO118_FUNC_ALT2 0x02u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO121_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 5. */
#define PIO121_FUNC_ALT5 0x05u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO124_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO124_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO125_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO125_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO129_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO129_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO130_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO130_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO15_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO15_FUNC_ALT1 0x01u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO16_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO16_FUNC_ALT1 0x01u

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void); /* Function assigned for the Cortex-M4F */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/