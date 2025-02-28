#include <DFRobot_ENS160.h>

DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);

void setup(void)
{
  Serial.begin(115200);
  Wire.begin();
  
  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with device failed, please check connection"); //The part was copied from teh documentation
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * Set power mode
   * mode Configurable power mode:
   *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
   *   ENS160_IDLE_MODE: IDLE mode (low-power)
   *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
   */
  ENS160.setPWRMode(ENS160_STANDARD_MODE);

  //Default values of temperature and humidity, in the case of integration with the humidity sensor this data woudl be updated base on the humidity and temp readings, but this was for testing

  ENS160.setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);

}

void loop()
{
  /**
   * Get the sensor operating status
   * Return value: 0-Normal operation, 
   *         1-Warm-Up phase, first 3 minutes after power-on.
   *         2-Initial Start-Up phase, first full hour of operation after initial power-on. Only once in the sensor’s lifetime.
   * note: Note that the status will only be stored in the non-volatile memory after an initial 24h of continuous
   *       operation. If unpowered before conclusion of said period, the ENS160 will resume "Initial Start-up" mode
   *       after re-powering.
   */
  uint8_t Status = ENS160.getENS160Status();
  Serial.print("Sensor operating status : ");
  Serial.println(Status);

  /**
   * Get the air quality index
   * Return value: 1-Excellent, 2-Good, 3-Moderate, 4-Poor, 5-Unhealthy
   */
  uint8_t AQI = ENS160.getAQI();
  Serial.print("Air quality index : ");
  Serial.println(AQI);

  /**
   * Get TVOC concentration
   * Return value range: 0–65000, unit: ppb
   */
  uint16_t TVOC = ENS160.getTVOC();
  Serial.print("Concentration of total volatile organic compounds : ");
  Serial.print(TVOC);
  Serial.println(" ppb");

  /**
   * Get CO2 equivalent concentration calculated according to the detected data of VOCs and hydrogen (eCO2 – Equivalent CO2)
   * Return value range: 400–65000, unit: ppm
   * Five levels: Excellent(400 - 600), Good(600 - 800), Moderate(800 - 1000), 
   *               Poor(1000 - 1500), Unhealthy(> 1500)
   */
  uint16_t ECO2 = ENS160.getECO2();
  Serial.print("Carbon dioxide equivalent concentration : ");
  Serial.print(ECO2);
  Serial.println(" ppm");

  Serial.println();
  delay(1000);
}
