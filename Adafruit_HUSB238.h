/**************************************************************************/
/**
  @file     Adafruit_HUSB238.h

  Author: Limor Fried (Adafruit Industries)
  License: BSD (see license.txt)

  This is a library for the Adafruit HUSB238 I2C motor driver
  ----> http://www.adafruit.com/

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

*/
/**************************************************************************/
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Arduino.h>
#include <Wire.h>

/** Default HUSB238 I2C address. */
#define HUSB238_I2CADDR_DEFAULT 0x08 ///< I2C address


#define HUSB238_PD_STATUS0 0x00  ///< Register value for PD_STATUS0 in HUSB238
#define HUSB238_PD_STATUS1 0x01  ///< Register value for PD_STATUS1 in HUSB238
#define HUSB238_SRC_PDO_5V 0x02  ///< Register value for SRC_PDO_5V in HUSB238
#define HUSB238_SRC_PDO_9V 0x03  ///< Register value for SRC_PDO_9V in HUSB238
#define HUSB238_SRC_PDO_12V 0x04 ///< Register value for SRC_PDO_12V in HUSB238
#define HUSB238_SRC_PDO_15V 0x05 ///< Register value for SRC_PDO_15V in HUSB238
#define HUSB238_SRC_PDO_18V 0x06 ///< Register value for SRC_PDO_18V in HUSB238
#define HUSB238_SRC_PDO_20V 0x07 ///< Register value for SRC_PDO_20V in HUSB238
#define HUSB238_SRC_PDO 0x08     ///< Register value for SRC_PDO in HUSB238
#define HUSB238_GO_COMMAND 0x09  ///< Register value for GO_COMMAND in HUSB238


typedef enum _husb_currents {
    CURRENT_0_5_A = 0b0000,  ///< 0.5A
    CURRENT_0_7_A = 0b0001,  ///< 0.7A
    CURRENT_1_0_A = 0b0010,  ///< 1.0A
    CURRENT_1_25_A = 0b0011, ///< 1.25A
    CURRENT_1_5_A = 0b0100,  ///< 1.5A
    CURRENT_1_75_A = 0b0101, ///< 1.75A
    CURRENT_2_0_A = 0b0110,  ///< 2.0A
    CURRENT_2_25_A = 0b0111, ///< 2.25A
    CURRENT_2_50_A = 0b1000, ///< 2.50A
    CURRENT_2_75_A = 0b1001, ///< 2.75A
    CURRENT_3_0_A = 0b1010,  ///< 3.0A
    CURRENT_3_25_A = 0b1011, ///< 3.25A
    CURRENT_3_5_A = 0b1100,  ///< 3.5A
    CURRENT_4_0_A = 0b1101,  ///< 4.0A
    CURRENT_4_5_A = 0b1110,  ///< 4.5A
    CURRENT_5_0_A = 0b1111   ///< 5.0A
} HUSB238_CurrentSetting;


typedef enum _husb_voltages {
    UNATTACHED = 0b0000, ///< Unattached
    PD_5V = 0b0001,      ///< 5V
    PD_9V = 0b0010,      ///< 9V
    PD_12V = 0b0011,     ///< 12V
    PD_15V = 0b0100,     ///< 15V
    PD_18V = 0b0101,     ///< 18V
    PD_20V = 0b0110      ///< 20V
} HUSB238_VoltageSetting;

typedef enum _husb_response_codes {
    NO_RESPONSE = 0b000,                  ///< No response
    SUCCESS = 0b001,                      ///< Success
    INVALID_CMD_OR_ARG = 0b011,           ///< Invalid command or argument
    CMD_NOT_SUPPORTED = 0b100,            ///< Command not supported
    TRANSACTION_FAIL_NO_GOOD_CRC = 0b101  ///< Transaction fail. No GoodCRC is received after sending
} HUSB238_ResponseCodes;

typedef enum _husb_5v_current_contract {
    CURRENT5V_DEFAULT = 0b00, ///< Default current
    CURRENT5V_1_5_A = 0b01,   ///< 1.5A
    CURRENT5V_2_4_A = 0b10,   ///< 2.4A
    CURRENT5V_3_A = 0b11      ///< 3A
} HUSB238_5VCurrentContract;

typedef enum _husb_pdo_selection {
    PDO_NOT_SELECTED = 0b0000, ///< Not selected
    PDO_SRC_5V = 0b0001,       ///< SRC_PDO_5V
    PDO_SRC_9V = 0b0010,       ///< SRC_PDO_9V
    PDO_SRC_12V = 0b0011,      ///< SRC_PDO_12V
    PDO_SRC_15V = 0b1000,      ///< SRC_PDO_15V
    PDO_SRC_18V = 0b1001,      ///< SRC_PDO_18V
    PDO_SRC_20V = 0b1010       ///< SRC_PDO_20V
} HUSB238_PDOSelection;



/**************************************************************************/
/*!
    @brief  HUSB238 driver.
*/
/**************************************************************************/
class Adafruit_HUSB238 {
public:
  Adafruit_HUSB238();
  bool begin(uint8_t addr = HUSB238_I2CADDR_DEFAULT, TwoWire *theWire = &Wire);

  public:
    // Getter functions
    bool getAttached();
    bool getCCStatus();
    HUSB238_ResponseCodes getPDResponse();
    bool get5VContractV();
    HUSB238_5VCurrentContract get5VContractA();
    HUSB238_PDOSelection isVoltageDetected(HUSB238_PDOSelection pd);
    HUSB238_CurrentSetting currentDetected(HUSB238_PDOSelection pd);
    HUSB238_VoltageSetting getPDSrcVoltage();
    HUSB238_CurrentSetting getPDSrcCurrent();
    HUSB238_PDOSelection getSelectedPDO();
    
    // Setter functions
    void selectPDO(HUSB238_PDOSelection pd);
    void reset();
    void requestPDO();
    void getSourceCapabilities();

private:
  Adafruit_I2CDevice *i2c_dev = NULL;
};
