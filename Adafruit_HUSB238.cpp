/**************************************************************************/
/*!
  @file     Adafruit_HUSB238.cpp

  @mainpage Adafruit HUSB238 I2C USB PD sink

  @section intro Introduction

  This is a library for the Adafruit HUSB238 I2C USB PD sink
  ----> http://www.adafruit.com/

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  @section author Author

  Limor Fried (Adafruit Industries)

  @section license License

  BSD (see license.txt)
*/
/**************************************************************************/
#include "Adafruit_HUSB238.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new HUSB238 class.
    @details This constructor initializes a new instance of the HUSB238 class.
*/
/**************************************************************************/
Adafruit_HUSB238::Adafruit_HUSB238() {}

/**************************************************************************/
/*!
    @brief  Sets up the I2C connection and tests that the sensor was found.
    @param addr The 7-bit I2C address of the HUSB238.
    @param theWire Pointer to an I2C device we'll use to communicate; default is
   Wire.
    @return true if sensor was found, otherwise false.
    @details This function initializes the I2C communication with the HUSB238
   device. It uses the provided I2C address and Wire interface. The function
   returns true if the device is successfully initialized.
*/
/**************************************************************************/
bool Adafruit_HUSB238::begin(uint8_t addr, TwoWire *theWire) {
  if (i2c_dev) {
    delete i2c_dev;
  }
  i2c_dev = new Adafruit_I2CDevice(addr, theWire);

  /* Try to instantiate the I2C device. */
  if (!i2c_dev->begin()) {
    return false;
  }

  return true;
}

/**************************************************************************/
/*!
    @brief  Reads the CC direction from the HUSB238 device.
    @return The CC status as a boolean value - false is CC1 connected, true is
   CC2 connected
    @details This function reads the 7th bit of the HUSB238_PD_STATUS1 register
*/
/**************************************************************************/
bool Adafruit_HUSB238::getCCdirection() {
  Adafruit_BusIO_Register ccRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_BusIO_RegisterBits ccBit =
      Adafruit_BusIO_RegisterBits(&ccRegister, 1, 7); // 7th bit
  return ccBit.read();
}

/**************************************************************************/
/*!
    @brief  Reads the attachment status from the HUSB238 device.
    @return The attachment status as a boolean value.
    @details This function reads the 6th bit of the HUSB238_PD_STATUS1 register
   to get the attachment status. It returns true if the attachment status bit is
   set.
*/
/**************************************************************************/
bool Adafruit_HUSB238::isAttached() {
  Adafruit_BusIO_Register attachRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_BusIO_RegisterBits attachBit =
      Adafruit_BusIO_RegisterBits(&attachRegister, 1, 6); // 6th bit
  return attachBit.read();
}

/**************************************************************************/
/*!
    @brief  Reads the PD response from the HUSB238 device.
    @return The PD response as an HUSB238_ResponseCodes enum value.
    @details This function reads bits 3-5 of the HUSB238_PD_STATUS1 register to
   get the PD response. It returns the response as an HUSB238_ResponseCodes enum
   value.
*/
/**************************************************************************/
HUSB238_ResponseCodes Adafruit_HUSB238::getPDResponse() {
  Adafruit_BusIO_Register pdRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_BusIO_RegisterBits pdBits =
      Adafruit_BusIO_RegisterBits(&pdRegister, 3, 3); // Bits 3-5
  return (HUSB238_ResponseCodes)pdBits.read();
}

/**************************************************************************/
/*!
    @brief  Reads the 5V contract voltage status from the HUSB238 device.
    @return The 5V contract voltage status as a boolean value.
    @details This function reads the 2nd bit of the HUSB238_PD_STATUS1 register
   to get the 5V contract voltage status. It returns true if the 5V contract
   voltage bit is set.
*/
/**************************************************************************/
bool Adafruit_HUSB238::get5VContractV() {
  Adafruit_BusIO_Register pdRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_BusIO_RegisterBits contractVBit =
      Adafruit_BusIO_RegisterBits(&pdRegister, 1, 2); // 2nd bit
  return contractVBit.read();
}

/**************************************************************************/
/*!
    @brief  Reads the 5V contract current from the HUSB238 device.
    @return The 5V contract current as an HUSB238_5VCurrentContract enum value.
    @details This function reads the bottom two bits (0-1) of the
   HUSB238_PD_STATUS1 register to get the 5V contract current. It returns the
   current as an HUSB238_5VCurrentContract enum value.
*/
/**************************************************************************/
HUSB238_5VCurrentContract Adafruit_HUSB238::get5VContractA() {
  Adafruit_BusIO_Register pdRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_BusIO_RegisterBits contractABits =
      Adafruit_BusIO_RegisterBits(&pdRegister, 2, 0); // Bottom two bits (0-1)
  return (HUSB238_5VCurrentContract)contractABits.read();
}
/**************************************************************************/
/*!
    @brief  Reads the source voltage from the HUSB238 device.
    @return The source voltage as an HUSB238_VoltageSetting enum value.
    @details This function reads bits 4-7 of the HUSB238_PD_STATUS0 register to
   get the source voltage. It returns the voltage as an HUSB238_VoltageSetting
   enum value.
*/
/**************************************************************************/
HUSB238_VoltageSetting Adafruit_HUSB238::getPDSrcVoltage() {
  Adafruit_BusIO_Register pd0Register =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_PD_STATUS0);
  Adafruit_BusIO_RegisterBits srcVoltageBits =
      Adafruit_BusIO_RegisterBits(&pd0Register, 4, 4); // Bits 4-7
  return (HUSB238_VoltageSetting)srcVoltageBits.read();
}

/**************************************************************************/
/*!
    @brief  Reads the source current from the HUSB238 device.
    @return The source current as an HUSB238_CurrentSetting enum value.
    @details This function reads the bottom four bits (0-3) of the
   HUSB238_PD_STATUS0 register to get the source current. It returns the current
   as an HUSB238_CurrentSetting enum value.
*/
/**************************************************************************/
HUSB238_CurrentSetting Adafruit_HUSB238::getPDSrcCurrent() {
  Adafruit_BusIO_Register pd0Register =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_PD_STATUS0);
  Adafruit_BusIO_RegisterBits srcCurrentBits =
      Adafruit_BusIO_RegisterBits(&pd0Register, 4, 0); // Bits 0-3
  return (HUSB238_CurrentSetting)srcCurrentBits.read();
}

/**************************************************************************/
/*!
    @brief  Checks if a specific voltage is detected.
    @param pd The PD output selection as an HUSB238_PDSelection enum value.
    @return true if the voltage is detected, otherwise false.
    @details This function checks if a specific voltage is detected based on the
   PD selection. It reads the 7th bit of the corresponding register to determine
   the status.
*/
/**************************************************************************/
bool Adafruit_HUSB238::isVoltageDetected(HUSB238_PDSelection pd) {
  uint8_t registerAddress;

  // Determine the register address based on the PD selection
  switch (pd) {
  case PD_SRC_5V:
    registerAddress = HUSB238_SRC_PDO_5V;
    break;
  case PD_SRC_9V:
    registerAddress = HUSB238_SRC_PDO_9V;
    break;
  case PD_SRC_12V:
    registerAddress = HUSB238_SRC_PDO_12V;
    break;
  case PD_SRC_15V:
    registerAddress = HUSB238_SRC_PDO_15V;
    break;
  case PD_SRC_18V:
    registerAddress = HUSB238_SRC_PDO_18V;
    break;
  case PD_SRC_20V:
    registerAddress = HUSB238_SRC_PDO_20V;
    break;
  default:
    return false; // Unknown PD
  }

  // Create an Adafruit_BusIO_Register object for the selected register
  Adafruit_BusIO_Register pdoRegister =
      Adafruit_BusIO_Register(i2c_dev, registerAddress);

  // Create an Adafruit_BusIO_RegisterBits object for the 7th bit
  Adafruit_BusIO_RegisterBits pdoBit =
      Adafruit_BusIO_RegisterBits(&pdoRegister, 1, 7); // 7th bit

  // Read and return the bit
  return pdoBit.read();
}

/**************************************************************************/
/*!
    @brief  Reads the detected current based on the PD output selection.
    @param pd The PD selection as an HUSB238_PDSelection enum value.
    @return The detected current as an HUSB238_CurrentSetting enum value.
    @details This function reads the bottom four bits (0-3) of the corresponding
   register based on the PD selection to get the detected current. It returns
   the current as an HUSB238_CurrentSetting enum value.
*/
/**************************************************************************/
HUSB238_CurrentSetting
Adafruit_HUSB238::currentDetected(HUSB238_PDSelection pd) {
  uint8_t registerAddress;

  // Determine the register address based on the PD selection
  switch (pd) {
  case PD_SRC_5V:
    registerAddress = HUSB238_SRC_PDO_5V;
    break;
  case PD_SRC_9V:
    registerAddress = HUSB238_SRC_PDO_9V;
    break;
  case PD_SRC_12V:
    registerAddress = HUSB238_SRC_PDO_12V;
    break;
  case PD_SRC_15V:
    registerAddress = HUSB238_SRC_PDO_15V;
    break;
  case PD_SRC_18V:
    registerAddress = HUSB238_SRC_PDO_18V;
    break;
  case PD_SRC_20V:
    registerAddress = HUSB238_SRC_PDO_20V;
    break;
  default:
    return CURRENT_0_5_A; // we'll have to just return 0
  }

  // Create an Adafruit_BusIO_Register object for the selected register
  Adafruit_BusIO_Register pdoRegister =
      Adafruit_BusIO_Register(i2c_dev, registerAddress);

  // Create an Adafruit_BusIO_RegisterBits object for the bottom four bits (0-3)
  Adafruit_BusIO_RegisterBits currentBits =
      Adafruit_BusIO_RegisterBits(&pdoRegister, 4, 0); // Bits 0-3

  // Read and return the bits, cast to HUSB238_CurrentSetting
  return (HUSB238_CurrentSetting)currentBits.read();
}

/**************************************************************************/
/*!
    @brief  Selects a PD output.
    @param pd The PD selection as an HUSB238_PDOelection enum value.
    @details This function writes to bits 4-7 of the SRC_PDO register to select
   a PD.
*/
/**************************************************************************/
void Adafruit_HUSB238::selectPD(HUSB238_PDSelection pd) {
  // Create an Adafruit_BusIO_Register object for the SRC_PD register
  Adafruit_BusIO_Register pdoRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_SRC_PDO);

  // Create an Adafruit_BusIO_RegisterBits object for bits 4-7 of SRC_PDO
  Adafruit_BusIO_RegisterBits pdoBits =
      Adafruit_BusIO_RegisterBits(&pdoRegister, 4, 4); // Bits 4-7

  // Set the bits based on the provided HUSB238_PDSelection value
  pdoBits.write(static_cast<uint8_t>(pd));
}

/**************************************************************************/
/*!
    @brief  Gets the selected PDO.
    @return The selected PD as an HUSB238_PDSelection enum value.
    @details This function reads bits 4-7 of the SRC_PDO register to get the
   selected PDO. It returns the PDO as an HUSB238_PDSelection enum value.
*/
/**************************************************************************/
HUSB238_PDSelection Adafruit_HUSB238::getSelectedPD() {
  // Create an Adafruit_BusIO_Register object for the SRC_PDO register
  Adafruit_BusIO_Register pdoRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_SRC_PDO);

  // Create an Adafruit_BusIO_RegisterBits object for bits 4-7 of SRC_PDO
  Adafruit_BusIO_RegisterBits pdoBits =
      Adafruit_BusIO_RegisterBits(&pdoRegister, 4, 4); // Bits 4-7

  // Read and return the bits, cast to HUSB238_PDSelection
  return (HUSB238_PDSelection)pdoBits.read();
}

/**
 * @brief Resets the HUSB238 device.
 *
 * This function writes to the GO_COMMAND register to initiate a hard reset.
 * Specifically, it writes 0b10000 to the bottom 5 bits of the GO_COMMAND
 * register.
 *
 * @see GO_COMMAND register in HUSB238 Register Information (Page 7)
 */
void Adafruit_HUSB238::reset() {
  // Create an Adafruit_I2CRegister object for the GO_COMMAND register
  Adafruit_I2CRegister goCommandRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_GO_COMMAND);

  // Create an Adafruit_I2CRegisterBits object for the bottom 5 bits (0-4) of
  // GO_COMMAND
  Adafruit_I2CRegisterBits resetBits =
      Adafruit_BusIO_RegisterBits(&goCommandRegister, 5, 0); // Bits 0-4

  // Write 0b10000 to the bottom 5 bits
  resetBits.write(0b10000);
}

/**
 * @brief Requests Power Delivery (PD) from the HUSB238 device.
 *
 * This function writes to the GO_COMMAND register to request a PD contract.
 * Specifically, it writes 0b00001 to bits 0-1 of the GO_COMMAND register.
 *
 * @see GO_COMMAND register in HUSB238 Register Information (Page 7)
 */
void Adafruit_HUSB238::requestPD() {
  // Create an Adafruit_I2CRegister object for the GO_COMMAND register
  Adafruit_I2CRegister goCommandRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_GO_COMMAND);

  // Create an Adafruit_I2CRegisterBits object for bits 0-1 of GO_COMMAND
  Adafruit_I2CRegisterBits requestBits =
      Adafruit_BusIO_RegisterBits(&goCommandRegister, 1, 0); // Bits 0-1

  // Write 0b00001 to the bits
  requestBits.write(0b00001);
}

/**
 * @brief Retrieves the source capabilities of the HUSB238 device.
 *
 * This function writes to the GO_COMMAND register to send out a Get_SRC_Cap
 * command. Specifically, it writes 0b00100 to the bottom 5 bits of the
 * GO_COMMAND register.
 *
 * @see GO_COMMAND register in HUSB238 Register Information (Page 7)
 */
void Adafruit_HUSB238::getSourceCapabilities() {
  // Create an Adafruit_I2CRegister object for the GO_COMMAND register
  Adafruit_I2CRegister goCommandRegister =
      Adafruit_BusIO_Register(i2c_dev, HUSB238_GO_COMMAND);

  // Create an Adafruit_I2CRegisterBits object for the bottom 5 bits (0-4) of
  // GO_COMMAND
  Adafruit_I2CRegisterBits capabilitiesBits =
      Adafruit_BusIO_RegisterBits(&goCommandRegister, 5, 0); // Bits 0-4

  // Write 0b00100 to the bits
  capabilitiesBits.write(0b00100);
}
