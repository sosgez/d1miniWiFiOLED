/* PAR 13 Jul 20
D1mini WifFi scan with output to OLED screen
 */
#include "ESP8266WiFi.h"
#define WIFI_OPEN  7
#define PAUSE 7000

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example

void setup()
{
    Serial.begin(115200);
    while(!Serial);
    delay(100);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
      Serial.println("SSD1306 allocation failed");
      for(;;); // Don't proceed, loop forever
    }
    Serial.println("display begin OK\n");  
    display.clearDisplay();
    display.display();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA); 
    WiFi.disconnect();
    delay(100);
}

void loop()
{
    char  buf[40];
    int j;
    Serial.println("scan start");
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
        display.print("No networks");
        display.display();
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        display.clearDisplay();
        display.setCursor(0,0);
        display.print("NW=");
        display.print(n);
        j = 1;
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            display.setCursor(0,j*16);
            display.print(-WiFi.RSSI(i));     
            display.print(WiFi.SSID(i).substring(0,8));
            display.display();
            if(++j>=4){
               j = 0;
               delay(PAUSE);
               display.clearDisplay();        
            }
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_OPEN)?" ":"*");
            delay(10);       
        }
        display.display();
    }
    Serial.println("");

    // Wait a bit before scanning again
    delay(PAUSE);
}
