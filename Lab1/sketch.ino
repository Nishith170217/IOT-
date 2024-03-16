
#define LED 12
#define BUTTON 13
#define C_TIME 500000

void IRAM_ATTR trigger_LED(){
  digitalWrite(LED,!digitalRead(LED));

}

hw_timer_t *My_timer = NULL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S2!");
  pinMode(LED,OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON, trigger_LED, RISING);
  My_timer = timerBegin(0,80,true);
  timerAttachInterrupt(My_timer, &trigger_LED,true);
  timerAlarmWrite(My_timer,C_TIME,true);
  timerAlarmEnable(My_timer);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED, HIGH);
  //delay(500); // this speeds up the simulation
  //digitalWrite(LED, LOW);
  //delay(500);
  //Serial.println(digitalRead(BUTTON));
  //delay(10);
  //if(!digitalRead(BUTTON)){
    
  //}
  delay(100);
}
