/*
 * https://github.com/Andy4495/GPS-Breakout-with-LCD
 *
 * GPS Receiver setup using Tiva TM4C LaunchPad, SparkFun ZOE-M8Q GPS breakout, and CrystalFonz 8x2 LCD.
 * Sketch is based on Example12_UseUart.ino from SparkFun Ublox Arduino Library:
 *     https://github.com/sparkfun/SparkFun_Ublox_Arduino_Library
 *
 * The ZOE-M8Q I2C interface is unreliable in my setup. I am not sure if this is a problem with the
 * hardware or the SparkFun library. This sketch uses UART to communicate between the LaunchPad and
 * the GPS module, without issue.
 * The Tiva TM4C LaunchPad has 8 available hardware serial ports. This sketch uses UART module 1
 * (accessed using the "Serial1" object). Debug information is sent through the usual "Serial"
 * USB backchannel.
 *
 * The LCD display cycles between several GPS data values (lat/long, altitude, # satellites, ground speed, date/time).
 * Hold down "SW1" on the LaunchPad to freeze the display at the current reading.
 *
 * 10/17/2020  1.0  A.T.  Initial
 *
 * Original code by:
  By: Nathan Seidle, Adapted from Example3_GetPosition by Thorsten von Eicken
  SparkFun Electronics
  Date: January 28rd, 2019
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.


*/

#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
SFE_UBLOX_GPS myGPS;

#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 12, 13, 14, 15, 18);

long lastTime = 0;

void setup()
{
  pinMode(PUSH1, INPUT_PULLUP);
  Serial.begin(38400);
  while (!Serial); //Wait for user to open terminal
  Serial.println("SparkFun Ublox Example");

  // set up the LCD's number of columns and rows:
  lcd.begin(8, 2);
  lcd.clear();
  lcd.print("SparkFun");
  lcd.setCursor(0, 1);
  lcd.print("ZOE GPS");
  delay(1000);

  //Assume that the U-Blox GPS is running at 9600 baud (the default) or at 38400 baud.
  //Loop until we're in sync and then ensure it's at 38400 baud.
  do {
    Serial.println("GPS: trying 38400 baud");
    Serial1.begin(38400);
    if (myGPS.begin(Serial1) == true) break;

    delay(100);
    Serial.println("GPS: trying 9600 baud");
    Serial1.begin(9600);
    if (myGPS.begin(Serial1) == true) {
      Serial.println("GPS: connected at 9600 baud, switching to 38400");
      myGPS.setSerialRate(38400);
      delay(100);
    } else {
      //myGPS.factoryReset();
      delay(2000); //Wait a bit before trying again to limit the Serial output
    }
  } while (1);
  Serial.println("GPS serial connected");

  myGPS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
  myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGPS.saveConfiguration(); //Save the current settings to flash and BBR
}

void loop()
{
  long waitStart;
  char displayBuffer[17];

  //Query module only every second.
  if (millis() - lastTime > 1000)
  {
    lastTime = millis(); //Update the timer
    lcd.clear();
    lcd.print("Lat:");
    lcd.setCursor(0, 1);
    lcd.print("Long:");

    long latitude = myGPS.getLatitude();
    long latitudeAbs;
    Serial.print(F("Lat: "));
    Serial.print(latitude);
    if (latitude < 0) latitudeAbs = -latitude;
    else latitudeAbs = latitude;
    snprintf(displayBuffer, 9, "%+3d.%4.4d", latitude / 10000000, latitudeAbs % 10000000);
    lcd.clear();
    lcd.print(displayBuffer);

    long longitude = myGPS.getLongitude();
    long longitudeAbs;
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));
    if (longitude < 0) longitudeAbs = -longitude;
    else longitudeAbs = longitude;
    lcd.setCursor(0, 1);
    snprintf(displayBuffer, 9, "%+3d.%4.4d", longitude / 10000000, longitudeAbs % 10000000);
    lcd.print(displayBuffer);
    waitStart = millis();
    while (millis() - waitStart < 1000)
      while (digitalRead(PUSH1) == LOW); // empty loop

    long altitude = myGPS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));
    lcd.clear();
    lcd.print("Alt (m):");
    snprintf(displayBuffer, 9, "%5d.%2.2d", altitude / 1000, ((altitude + 5) / 10) % 100);
    lcd.setCursor(0, 1);
    lcd.print(displayBuffer);
    waitStart = millis();
    while (millis() - waitStart < 1000)
      while (digitalRead(PUSH1) == LOW); // empty loop

    int SIV = myGPS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);
    lcd.clear();
    lcd.print("Sats:");
    lcd.setCursor(0, 1);
    lcd.print(SIV);
    waitStart = millis();
    while (millis() - waitStart < 1000)
      while (digitalRead(PUSH1) == LOW); // empty loop

    int32_t groundSpeed = myGPS.getGroundSpeed();
    Serial.print(F(" Ground Speed: "));
    Serial.print(groundSpeed);
    Serial.print(F(" (mm/s)"));
    lcd.clear();
    lcd.print("Spd m/s:");
    snprintf(displayBuffer, 9, "%4d.%3.3d", groundSpeed / 1000, groundSpeed % 1000);
    lcd.setCursor(0, 1);
    lcd.print(displayBuffer);
    waitStart = millis();
    while (millis() - waitStart < 1000)
      while (digitalRead(PUSH1) == LOW); // empty loop

    uint16_t gpsYear = myGPS.getYear();
    uint8_t  gpsMonth = myGPS.getMonth();
    uint8_t  gpsDay = myGPS.getDay();
    uint8_t  gpsHour = myGPS.getHour();
    uint8_t  gpsMinute = myGPS.getMinute();
    uint8_t  gpsSecond = myGPS.getSecond();
    Serial.print(F(" DateTime: "));
    Serial.print(gpsYear);
    Serial.print("-");
    Serial.print(gpsMonth);
    Serial.print("-");
    Serial.print(gpsDay);
    Serial.print("Z");
    Serial.print(gpsHour);
    Serial.print(":");
    Serial.print(gpsMinute);
    Serial.print(":");
    Serial.print(gpsSecond);
    lcd.clear();
    snprintf(displayBuffer, 9, "%2d-%02d-%02d", gpsYear - 2000, gpsMonth, gpsDay);
    lcd.print(displayBuffer);
    lcd.setCursor(0, 1);
    snprintf(displayBuffer, 9, "%02d:%02d:02d", gpsHour, gpsMinute, gpsSecond);
    lcd.print(displayBuffer);
    waitStart = millis();
    while (millis() - waitStart < 1000)
      while (digitalRead(PUSH1) == LOW); // empty loop

    int dateValid = myGPS.getDateValid();
    int timeValid = myGPS.getTimeValid();
    Serial.print(" Date, Time Valid: ");
    Serial.print(dateValid);
    Serial.print(", ");
    Serial.print(timeValid);

    Serial.println();
  }
}
