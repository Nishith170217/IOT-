#include<DHTesp.h>

#define DHT_PIN 17
#define SLIDE 16
DHTesp dhtSensor;
void setup() {
  // put your setup code here, to run once:
  pinMode(SLIDE, INPUT);
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S2!");
  dhtSensor.setup(DHT_PIN,DHTesp::DHT22);
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = dhtSensor.getTemperature();
  float hum = dhtSensor.getHumidity();
  Serial.println("Temp :" + String(temp,2)+"°C");
  Serial.println("Hum :"+ String(hum,2)+ "%");
  Serial.println("Slide value : "+ String(analogRead(SLIDE)));
  delay(500); // this speeds up the simulation
}
