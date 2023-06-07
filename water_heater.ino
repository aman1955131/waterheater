#define BLYNK_TEMPLATE_ID "TMPLu6STFyfl"
#define BLYNK_DEVICE_NAME "the little mermaids"
#define BLYNK_AUTH_TOKEN "IZ88zyuKnNJ15gW0TdevKrRdThmGn5aS"
#include "max6675.h"
#define BLYNK_PRINT Serial

#include<BlynkSimpleEsp8266.h>
#include<ESP8266WiFi.h>

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
WiFiClient client;

const char* ssid  = "realme 9 Pro 5G";
const char* password  = "sindhu46";

int ktcSO = D3;
int ktcCS = D4;
int ktcCLK = D5;
float val=0;

int relaystat=LOW;
int relay=D6;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}
  
void setup() {
  Serial.begin(9600);
  // give the MAX a little time to settle
  delay(500);

  Blynk.begin(auth, ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.print("connected to ssid");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop() {
  Blynk.run();
  temp();
  upload();
}

void temp() {
  // basic readout test
  
   Serial.print("Deg C = "); 
   Serial.print(ktc.readCelsius());
   Serial.print("\t Deg F = ");
   Serial.println(ktc.readFahrenheit());
  val=ktc.readFahrenheit();
  delay(500);
  if(val>40){
    digitalWrite(relay,HIGH);
    Serial.println("temperature above threshold");
  }
  else{
    digitalWrite(relay,LOW);
    Serial.println("temp. within the limits");
  }
}

void upload() {
  Serial.println("Uploading");
  Blynk.virtualWrite(V0,val);
}

BLYNK_WRITE(V2) {
  Serial.println("TOGGELING THE RELAY");
  if (val>40) {
    relaystat = param.asInt();
    digitalWrite(relay, !relaystat);
    Serial.println("relaytog");
  }
}
