#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi Setup- Replace with your network credentials
const char *ssid     = "SSID";
const char *password = "PASSWORD";

// NTP Client Setup, to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Screen Setup, to display info
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1 // -1 sharing Arduino reset pin, 4 connections on display
#define SCREEN_ADDRESS 0x3C // 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//BME280 Setup, to get Temp, RH and Press
#define BME280_I2C_ADDRESS  0x76
Adafruit_BME280  bme280;
 
void setup() {
  
Serial.begin(115200);//activate serial monitor, in case display fails
//init display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.display();//display confidence check (logo display)
  delay(1000); 
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);
  display.print("Connecting to ");
  display.print(ssid);
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
timeClient.begin();
timeClient.setTimeOffset(0);
}

  // initialize the BME280 sensor
  if( bme280.begin(BME280_I2C_ADDRESS) == 0 )
  {  // connection error or device address wrong!
    display.setCursor(34, 03);
    display.print("Connection");
    display.setCursor(49, 13);
    display.print("Error");
    display.display();        // update the display
    while(1)  // stay here
      delay(1000);
}
  display.clearDisplay(); //well yeah, just put it out there
  display.setCursor(29, 0);
  display.print("Weather");
  display.setCursor(29, 16);
  display.print("Station");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {

  // read temperature, humidity and pressure from the BME280 sensor
  float temp = bme280.readTemperature();  // get temperature in degree Celsius
  float humi = bme280.readHumidity();     // get humidity in rH%
  float pres = bme280.readPressure();     // get pressure in Pa
 
  delay(1000);  // wait a second
  // print data on the OLED
  
  // 1: print temperature
  timeClient.update();//update the TOD
  String formattedTime = timeClient.getFormattedTime();
  display.setCursor(37, 11);
  if(temp < 0)
    display.printf("-%02u.%02u C", (int)abs(temp) % 100, (int)(abs(temp) * 100) % 100 );
  else
    display.printf(" %02u.%02u C", (int)temp % 100, (int)(temp * 100) % 100 );
 
  // print degree symbols ( ° )
  display.drawRect(75, 10, 3, 3, WHITE);
  display.setCursor(29, 0);
  display.print("TEMPERATURE:");
  display.setCursor(38, 23);
  display.print(formattedTime);
  display.display();
  delay(2000);
  display.clearDisplay();
  
  // 2: print humidity
  timeClient.update();//update the TOD
  String formattedTimea = timeClient.getFormattedTime();
  display.setCursor(43, 11);
  display.printf("%02u.%02u %%", (int)humi, (int)(humi * 100) % 100 );
   display.setCursor(38, 0);
  display.print("HUMIDITY:");
  display.setCursor(38, 23);
  display.print(formattedTimea);
  display.display();
  delay(2000);
  display.clearDisplay();
  
  // 3: print pressure
  timeClient.update();//update the TOD
  String formattedTimeb = timeClient.getFormattedTime();
  display.setCursor(31, 11);
  display.printf("%04u.%02u hPa", (int)(pres/100), (int)((uint32_t)pres % 100) );
  display.setCursor(38, 0);
  display.print("PRESSURE:");
  display.setCursor(38, 23);
  display.print(formattedTimeb);
  display.display();
  delay(2000);
  display.clearDisplay();
  
 }
