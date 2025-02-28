#include <LiquidCrystal_I2C.h>
#include <OneWire.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);      //0x27 being the scanned address
OneWire ds(2); // Use pin 2 for OneWire
const char blank_line_20[] = "                    ";
void setup(void) {
  Serial.begin(9600);
  lcd.init();
  pinMode(3, OUTPUT);
}

void loop(void) {
  byte addr[8];
  byte data[9];
  int16_t raw;
  float celsius;

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

  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(celsius);
  lcd.print((char)223);
  lcd.print("C");
  delay(2000);
  if (celsius >= 30) {
    digitalWrite(3, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(2000);                      // wait for half a second
    digitalWrite(3, LOW);   // turn the LED off by making the voltage LOW
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("TOO HOT!");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print(blank_line_20);   //create the blinking effect. if temp goes back down, the text disappears
  }

  Serial.print("Temp = ");
  Serial.print(celsius);
  Serial.println(" °C");
}
