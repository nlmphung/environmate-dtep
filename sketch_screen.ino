#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);      //0x27 being the scanned address
OneWire ds(2); // Use pin 2 for OneWire

void setup(void) {
  Serial.begin(9600);
  lcd.init();
  pinMode(6,OUTPUT);
}

void loop(void) {
  byte addr[8];
  byte data[9];
  int16_t raw;
  float celsius;
  float thresholdHigh = 40.0;
  float thresholdLow = 20.0;
  int pinLED = 6;

  if (!ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // Start temperature conversion
  
  delay(750); // Wait for conversion (750ms for 12-bit resolution)

  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad

  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  raw = (data[1] << 8) | data[0];
  celsius = (float)raw / 16.0;

  //In this version the LED lights up whenever the value exceeds any threshold Low or High
  if (celcius > thresholdHigh) {
    digitalWrite(pinLED, HIGH);
    lcd.print("----TOO HOT!----"); //It should have 16 chars
  } else if (celcius < thresholdLow) {
    digitalWrite(pinLed,LOW);
    lcd.print("---TOO COLD!---"); // It has less than 16 chars
  } else {
    digitalWrite(pinLed,Low);
    lcd.setCursor(0,0);
    lcd.print("Temp : ");
    lcd.print(celsius);
    lcd.print((char)223);
    lcd.print("C");
    delay(2000);
  }

  // Serial.print("Temp = ");
  // Serial.print(celsius);
  // Serial.println(" °C");
}
