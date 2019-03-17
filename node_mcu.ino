#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid =  "WIFI NAME"; 
const char *pass =  "wifi password";

WiFiClient client;
#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;

void request_send(String *, String *);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  Serial.println("Connecting to ");
  Serial.println(ssid); 

  WiFi.begin(ssid, pass); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);
  // use this instead: 
  dht.setup(5, DHTesp::DHT22); // Connect DHT sensor to GPIO 17
}

void loop(){
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  String dataSend = "temp="+String(temperature)+"&humdity="+String(humidity);
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
  request_send("http://vayuj-kandoi-vayujk.c9users.io/data/rcv/nodemcu",dataSend);
  delay(3000);
}

void request_send(String url, String postData){  
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(url);  //Specify request destination
    http.addHeader("Host", "vayuj-kandoi-vayujk.c9users.io");
    http.addHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv","66.0) Gecko/20100101 Firefox/66.0");
    http.addHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,/;q=0.8");
    http.addHeader("Accept-Language","en-US,en;q=0.5");
    http.addHeader("Accept-Encoding","gzip, deflate, br");
    http.addHeader("DNT","1");
    http.addHeader("Connection","keep-alive");
    http.addHeader("Cookie","c9.live.user.click-through=ok");
    http.addHeader("Upgrade-Insecure-Requests","1");
    http.addHeader("Cache-Control","max-age=0");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");   
    int httpCode = http.POST(postData); 
    String payload = http.getString();   //Get the request response payload
    Serial.println(payload); 
    http.end();
}
