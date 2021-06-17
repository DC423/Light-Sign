//
// Author: Stephen Hilt 
// Purpose: Control a D1 Mini that is connected to a NeoPixel LED STrip for the DC423 Sign
//          That was cut from Acrlyic at Chatt*Lab with the help of Jeff. 
//          
//  Version 0.0.0.0.0.0.0.0.1
//  Published: 6.12.2021
//
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

// WIFI SSID AND PASS
const char *WIFI_SSID = "SSID";
const char *WIFI_PASS = "PASSWORD";

// What PIN the NeoPixel is on
// Our Case is D1 on the D1 Mini, so PIN 5 
#define PIN 5
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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);
/* Just a little test message.  Go to http://192.168.4.1:1337 in a web browser
 * connected to this access point to see it.
 */
void rootPage() {
  webserver.send(200, "text/html", "<h1>BASE64MESSAGETOTROLL</h1>");
  dc423Strip(strip.Color(255, 255, 255),strip.Color(255, 0, 0), 50); // Turn Strip Red
   
}
// 404 Page to be desplayed 
void notfoundPage() {
  webserver.send(200, "text/plain", "DC423: YOU'VE MADE THE HACKER UNHAPPY");
  // having issues so this really doesn't run
  if (first_run == 0){
    dc423Strip(strip.Color(255, 255, 255),strip.Color(127, 0, 0), 50); // Chase Strip Red 
  }
}
// This will be the page where the control of the lightshow happens. 
void lightshowPage(){
  webserver.send(200, "text/html", webpageBuilder()); 
}
// Show Page and Turn LED's Blue
void bluePage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(0, 0, 255),strip.Color(0, 0, 255), 50); // Blue
}
// Show Page and turn LED's Green
void greenPage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(0,255,0),strip.Color(0,255,0),50); // Green
}
// Show Page and Turn LED's Purple
void purplePage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(139,0,139),strip.Color(139,0,139),50); // Purple
}
// Show Page and Turn LED's Pink
void pinkPage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(255,20,147),strip.Color(255,20,147),50); //Pink
}
// Show Page and Turn LED's Yellow
void yellowPage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(255,255,0),strip.Color(255,255,0),50); // Yellow
}
// Show Page and turn LED's Orange
void orangePage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(255,140,0),strip.Color(255,140,0),50); // Orange
}
// Show Page and turn LED's White
void whitePage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(255,255,255),strip.Color(255,255,255),50); // "White"
}
// Show Page and turn LED's Off (Black)
void offPage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(0,0,0),strip.Color(0,0,0),50); // Off
}
// Inside Lights change White, Outside Turns Purple
void wpPage(){
  webserver.send(200, "text/html", webpageBuilder());
  dc423Strip(strip.Color(255,255,255),strip.Color(139,0,139),50); // white-purple
}
// Show Page and change LED's to random colors (each LED is seperate) 
void ranPage(){
  webserver.send(200, "text/html", webpageBuilder());
  randomStrip(); // random colors
}

// Build the webpage so each page looks the same
String webpageBuilder(){
  return "<b>Change The Color Here</b>\
  <br>Change To: <form method=\"get\" action=\"/blue\"> \
    <button type=\"submit\">Blue</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/green\"> \
    <button type=\"submit\">Green</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/purple\"> \
    <button type=\"submit\">Purple</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/pink\"> \
    <button type=\"submit\">Pink</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/yellow\"> \
    <button type=\"submit\">Yellow</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/orange\"> \
    <button type=\"submit\">Orange</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/white-purple\"> \
    <button type=\"submit\">white-purple</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/white\"> \
    <button type=\"submit\">White</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/random\"> \
    <button type=\"submit\">random</button>\
  </form>\
  <br>Change To: <form method=\"get\" action=\"/dc423/off\"> \
    <button type=\"submit\">Off</button>\
  </form>";
  
}

// Function Written to contorol the light strip for our use
// Under the sign is 0-11 
// Outside the sign is 12 - strip.numPixels()
// Arguments are two colors Inside, and outside and then a number for how long to wait 
// Before changing the next pixel. 
void dc423Strip(uint32_t c1, uint32_t c2, uint8_t wait){
  // Loop through each pixel 
  for (int i = 0; i < strip.numPixels(); i++){
    // This is for under the sign
    if (i < 11){
      strip.setPixelColor(i, c1);
      strip.show();
    }
    // This is for outside of the sign 
    else 
    {
      strip.setPixelColor(i, c2);
      strip.show();
    }
    delay(wait);
  }
  
}
// Change each Pixel on the strip to a different random color 
// 
void randomStrip(){
  // Setup Values before the loop
  int ran1 = 0;
  int ran2 = 0;
  int ran3 = 0;
  // Loop through each pixel in the strip
  for (int i = 0; i < strip.numPixels(); i++){
    ran1 = random(255);
    ran2 = random(255);
    ran3 = random(255);
    strip.setPixelColor(i, strip.Color(ran1,ran2,ran3));
    strip.show();
    delay(50);
  }
}

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
  webserver.on("/orange",orangePage);
  webserver.on("/white-purple",wpPage);
  webserver.on("/dc423/off",offPage);
  webserver.on("/white", whitePage);
  webserver.on("/random", ranPage);
  webserver.onNotFound(notfoundPage);
  webserver.begin();
  // Start the LED Strip
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
  int first_run = 0; 

}

void loop() {
  
  webserver.handleClient();
  // Only want to set up the strip once, otherwise it will always be changing it back to this
  if (first_run == 0){
    dc423Strip(strip.Color(255,255,255),strip.Color(139,0,139),50);
    first_run = 1; 
  }

}

