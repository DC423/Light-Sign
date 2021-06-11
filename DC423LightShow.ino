#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

// WIFI SSID AND PASS
const char *WIFI_SSID = "SSID";
const char *WIFI_PASS = "PASSWORD";

// What PIN the NeoPixel is on
// This is D1 on the D1 Mini, so Pin 5
#define PIN 5
// Setup variable to use to turn the color white for the first run
int first_run; 
// Create Webserver on port 1337
ESP8266WebServer webserver(1337);

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
// Go to http://192.168.4.1:1337 in a web browser
//connected to this access point to see it. this will change the LED's Red warning someone is on server
//
void rootPage() {
  webserver.send(200, "text/html", "<h1>ZnNrIG9mZiB5b3Ugd2FubmEgYmUgaGFja2Vy</h1>");
  colorWipe(strip.Color(255, 0, 0), 50); // Turn Strip Red
   
}
// 404 Page to be desplayed 
// having issueschanging the color, every page I go to it loads the 404 so will blink RED 
void notfoundPage() {
  webserver.send(200, "text/plain", "DC423: YOU'VE MADE THE HACKER UNHAPPY");
  if (first_run == 0){
    theaterChase(strip.Color(127, 0, 0), 50); // Chase Strip Red 
  }
}
// This will be the page where the control of the lightshow happens. This is the main control page 
void lightshowPage(){
  webserver.send(200, "text/html", webpageBuilder()); 
}
// Change color to blue, and show the changing options
void bluePage(){
  webserver.send(200, "text/html", webpageBuilder());
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
}
// Change color to green and show the changing options
void greenPage(){
  webserver.send(200, "text/html", webpageBuilder());
  colorWipe(strip.Color(0,255,0),50); // Green
}
// Change color to green and show the changing options
void purplePage(){
  webserver.send(200, "text/html", webpageBuilder());
  colorWipe(strip.Color(139,0,139),50); // Purple
}
// Change color to pink and show the changing options
void pinkPage(){
  webserver.send(200, "text/html", webpageBuilder());
  colorWipe(strip.Color(139,0,139),50); //Pink
}
// Change color to yellow and show the changing options
void yellowPage(){
  webserver.send(200, "text/html", webpageBuilder());
  colorWipe(strip.Color(255,255,0),50); // Yellow
}
// Change to rainbow and show the changing options
void rainbowPage(){
  webserver.send(200, "text/html", webpageBuilder());
  rainbow(20);
}
// Will build the webpage, called by other page options to keep the code down
String webpageBuilder(){
  return "<b>Change The Color Here</b><br>Change To: <a href=\"blue\">Blue</a><br>Change To: <a href=\"green\">Green</a><br>Change To: <a href=\"purple\">Purple</a><br>Change To: <a href=\"pink\">Pink</a><br>Chagne To: <a href=\"yellow\">Yellow</a><br>Change To: <a href=\"rainbow\">Rainbow</a>";
}
// SETUP
void setup() {
  Serial.begin(115200);
  Serial.println();
  // begin wifi 
  WiFi.softAP(WIFI_SSID, WIFI_PASS);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: "); 
  Serial.print(myIP); 
  // Start Webserver
  webserver.on("/", rootPage); 
  webserver.on("/LightShow", lightshowPage);
  webserver.on("/blue", bluePage);
  webserver.on("/green", greenPage);
  webserver.on("/purple",purplePage);
  webserver.on("/pink",pinkPage);
  webserver.on("/yellow",yellowPage);
  webserver.on("/rainbow",rainbowPage);
  webserver.onNotFound(notfoundPage);
  webserver.begin();
  // Start the LED Strip
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'

  int first_run = 0; 
}
// LOOP
void loop() {
  
  webserver.handleClient();
  if (first_run == 0){
    //colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(255,255,255), 0); // White
    first_run = 1; 
  }

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
// RAINBOW
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
      Serial.println(Wheel((i+j))); 
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
