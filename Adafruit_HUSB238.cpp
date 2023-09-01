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
    @brief  Instantiates a new HUSB238 class
*/
/**************************************************************************/
Adafruit_HUSB238::Adafruit_HUSB238() {}

/**************************************************************************/
/*!
    @brief  Sets up the I2C connection and tests that the sensor was found.
    @param addr The 7-bit I2C address of the HUSB238
    @param theWire Pointer to an I2C device we'll use to communicate
    default is Wire
    @return true if sensor was found, otherwise false.
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

bool Adafruit_HUSB238::getCCStatus() {
  Adafruit_I2CRegister ccRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_I2CRegisterBit ccBit = Adafruit_I2CRegisterBit(&ccRegister, 7);  // 7th bit
  return ccBit.read();
}


bool Adafruit_HUSB238::getAttached() {
  Adafruit_I2CRegister attachRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_I2CRegisterBit attachBit = Adafruit_I2CRegisterBit(&attachRegister, 6);  // 6th bit
  return attachBit.read();
}

HUSB238_ResponseCodes Adafruit_HUSB238::getPDResponse() {
  Adafruit_I2CRegister pdRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_I2CRegisterBits pdBits = Adafruit_I2CRegisterBits(&pdRegister, 3, 3);  // Bits 3-5
  return (HUSB238_ResponseCodes) pdBits.read();
}

bool Adafruit_HUSB238::get5VContractV() {
  Adafruit_I2CRegister pdRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_I2CRegisterBit contractVBit = Adafruit_I2CRegisterBit(&pdRegister, 2);  // 2nd bit
  return contractVBit.read();
}

HUSB238_5VCurrentContract Adafruit_HUSB238::get5VContractA() {
  Adafruit_I2CRegister pdRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_PD_STATUS1);
  Adafruit_I2CRegisterBits contractABits = Adafruit_I2CRegisterBits(&pdRegister, 2, 0);  // Bottom two bits (0-1)
  return (HUSB238_5VCurrentContract) contractABits.read();
}

HUSB238_VoltageSetting Adafruit_HUSB238::getPDSrcVoltage() {
  // Create an Adafruit_I2CRegister object for the HUSB238_PD_STATUS0 register
  Adafruit_I2CRegister pd0Register = Adafruit_I2CRegister(i2c_dev, HUSB238_PD_STATUS0);

  // Create an Adafruit_I2CRegisterBits object for bits 4-7 of HUSB238_PD_STATUS0
  Adafruit_I2CRegisterBits srcVoltageBits = Adafruit_I2CRegisterBits(&pd0Register, 4, 4);  // Bits 4-7

  // Read and return the bits, cast to HUSB238_VoltageSetting
  return (HUSB238_VoltageSetting) srcVoltageBits.read();
}

HUSB238_CurrentSetting Adafruit_HUSB238::getPDSrcCurrent() {
  // Create an Adafruit_I2CRegister object for the HUSB238_PD_STATUS0 register
  Adafruit_I2CRegister pd0Register = Adafruit_I2CRegister(i2c_dev, HUSB238_PD_STATUS0);

  // Create an Adafruit_I2CRegisterBits object for the bottom four bits (0-3) of HUSB238_PD_STATUS0
  Adafruit_I2CRegisterBits srcCurrentBits = Adafruit_I2CRegisterBits(&pd0Register, 4, 0);  // Bits 0-3

  // Read and return the bits, cast to HUSB238_CurrentSetting
  return (HUSB238_CurrentSetting) srcCurrentBits.read();
}


bool Adafruit_HUSB238::isVoltageDetected(HUSB238_PDOSelection pd) {
  uint8_t registerAddress;
  
  // Determine the register address based on the PDO selection
  switch(pd) {
    case PDO_5V:
      registerAddress = HUSB238_SRC_PDO_5V;
      break;
    case PDO_9V:
      registerAddress = HUSB238_SRC_PDO_9V;
      break;
    case PDO_12V:
      registerAddress = HUSB238_SRC_PDO_12V;
      break;
    case PDO_15V:
      registerAddress = HUSB238_SRC_PDO_15V;
      break;
    case PDO_18V:
      registerAddress = HUSB238_SRC_PDO_18V;
      break;
    case PDO_20V:
      registerAddress = HUSB238_SRC_PDO_20V;
      break;
    default:
      return false;  // Unknown PDO
  }
  
  // Create an Adafruit_I2CRegister object for the selected register
  Adafruit_I2CRegister pdoRegister = Adafruit_I2CRegister(i2c_dev, registerAddress);
  
  // Create an Adafruit_I2CRegisterBit object for the 7th bit
  Adafruit_I2CRegisterBit pdoBit = Adafruit_I2CRegisterBit(&pdoRegister, 7);  // 7th bit
  
  // Read and return the bit
  return pdoBit.read();
}


HUSB238_CurrentSetting Adafruit_HUSB238::currentDetected(HUSB238_PDOSelection pd) {
  uint8_t registerAddress;
  
  // Determine the register address based on the PDO selection
  switch(pd) {
    case PDO_5V:
      registerAddress = HUSB238_SRC_PDO_5V;
      break;
    case PDO_9V:
      registerAddress = HUSB238_SRC_PDO_9V;
      break;
    case PDO_12V:
      registerAddress = HUSB238_SRC_PDO_12V;
      break;
    case PDO_15V:
      registerAddress = HUSB238_SRC_PDO_15V;
      break;
    case PDO_18V:
      registerAddress = HUSB238_SRC_PDO_18V;
      break;
    case PDO_20V:
      registerAddress = HUSB238_SRC_PDO_20V;
      break;
    default:
      return CURRENT_UNKNOWN;  // Unknown PDO, you can define this value in the enum if needed
  }
  
  // Create an Adafruit_I2CRegister object for the selected register
  Adafruit_I2CRegister pdoRegister = Adafruit_I2CRegister(i2c_dev, registerAddress);
  
  // Create an Adafruit_I2CRegisterBits object for the bottom four bits (0-3)
  Adafruit_I2CRegisterBits currentBits = Adafruit_I2CRegisterBits(&pdoRegister, 4, 0);  // Bits 0-3
  
  // Read and return the bits, cast to HUSB238_CurrentSetting
  return (HUSB238_CurrentSetting) currentBits.read();
}


void Adafruit_HUSB238::selectPDO(HUSB238_PDOSelection pd) {
  // Create an Adafruit_I2CRegister object for the PDO_SELECT register
  Adafruit_I2CRegister pdoRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_PDO_SELECT);

  // Create an Adafruit_I2CRegisterBits object for bits 4-7 of PDO_SELECT
  Adafruit_I2CRegisterBits pdoBits = Adafruit_I2CRegisterBits(&pdoRegister, 4, 4);  // Bits 4-7

  // Set the bits based on the provided HUSB238_PDOSelection value
  pdoBits.write(static_cast<uint8_t>(pd));
}


HUSB238_PDOSelection Adafruit_HUSB238::getSelectedPDO() {
  // Create an Adafruit_I2CRegister object for the PDO_SELECT register
  Adafruit_I2CRegister pdoRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_PDO_SELECT);

  // Create an Adafruit_I2CRegisterBits object for bits 4-7 of PDO_SELECT
  Adafruit_I2CRegisterBits pdoBits = Adafruit_I2CRegisterBits(&pdoRegister, 4, 4);  // Bits 4-7

  // Read and return the bits, cast to HUSB238_PDOSelection
  return (HUSB238_PDOSelection) pdoBits.read();
}

void Adafruit_HUSB238::requestPDO() {
  // Create an Adafruit_I2CRegister object for the GO_COMMAND register
  Adafruit_I2CRegister goCommandRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_GO_COMMAND);

  // Create an Adafruit_I2CRegisterBits object for bits 0-1 of GO_COMMAND
  Adafruit_I2CRegisterBits requestBits = Adafruit_I2CRegisterBits(&goCommandRegister, 5, 0);  // Bits 0-4

  // Write 0b00001 to the bits
  requestBits.write(0b00001);
}



void Adafruit_HUSB238::getSourceCapabilities() {
  // Create an Adafruit_I2CRegister object for the GO_COMMAND register
  Adafruit_I2CRegister goCommandRegister = Adafruit_I2CRegister(i2c_dev, HUSB238_GO_COMMAND);

  // Create an Adafruit_I2CRegisterBits object for the bottom 5 bits (0-4) of GO_COMMAND
  Adafruit_I2CRegisterBits capabilitiesBits = Adafruit_I2CRegisterBits(&goCommandRegister, 5, 0);  // Bits 0-4

  // Write 0b00100 to the bits
  capabilitiesBits.write(0b00100);
}
