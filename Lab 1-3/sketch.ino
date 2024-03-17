#include <WiFi.h>
#include <esp_now.h>

uint8_t brcAdr[] = {0x8C,0xAA,0xB5,0x84,0xFB,0x90};
esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t*mac,const uint8_t*data, int len){
  Serial.println("Msg Received: ");
  char rcvstring[len];
  memcpy(rcvstring,data,len);
  Serial.println(String(rcvstring));
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S2!");
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
  memcpy(peerInfo.peer_addr,brcAdr,6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

}

void loop() {
  // put your main code here, to run repeatedly:
  String message = "HI There";
  esp_now_send(brcAdr,(uint8_t*)message.c_str(),message.length()+1);
  delay(10); // this speeds up the simulation
}
