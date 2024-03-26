#include <esp_now.h> // Include ESP-NOW library for peer-to-peer communication
#include <WiFi.h> // Include WiFi library to set up WiFi mode

#define TRIGGER_PIN 12 // GPIO pin for the HC-SR04 trigger pin
#define ECHO_PIN 13    // GPIO pin for the HC-SR04 echo pin
#define DEEP_SLEEP_TIME 5e6 // Time ESP32 will be in deep sleep, in microseconds (5 seconds)

// MAC address of the central ESP32 sink node
uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x84, 0xFB, 0x90};

// Callback function definition for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Data Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) { // Check if send was successful
    Serial.println("Success");
  } else {
    Serial.println("Failure");
  }
}

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  pinMode(TRIGGER_PIN, OUTPUT); // Set trigger pin as output
  pinMode(ECHO_PIN, INPUT); // Set echo pin as input
  WiFi.mode(WIFI_STA); // Set WiFi mode to STA (station)

  if (esp_now_init() != ESP_OK) { // Initialize ESP-NOW
    Serial.println("Error initializing ESP-NOW"); // Print error if initialization fails
    return;
  }

  esp_now_register_send_cb(OnDataSent); // Register callback for when data is sent
  
  esp_now_peer_info_t peerInfo; // Create a peer info struct
  memset(&peerInfo, 0, sizeof(peerInfo)); // Initialize the structure with zeros
  memcpy(peerInfo.peer_addr, broadcastAddress, 6); // Copy the sink node's MAC address
  peerInfo.channel = 0;  // Use the default channel
  peerInfo.encrypt = false; // Disable encryption

  if (esp_now_add_peer(&peerInfo) != ESP_OK){ // Add the peer
    Serial.println("Failed to add peer"); // Print error if adding peer fails
    return;
  }
}

void loop() {
  digitalWrite(TRIGGER_PIN, LOW); // Ensure trigger pin is low
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(TRIGGER_PIN, HIGH); // Send a 10-microsecond pulse
  delayMicroseconds(10); // Wait for 10 microseconds
  digitalWrite(TRIGGER_PIN, LOW); // Return trigger pin to low
  long duration = pulseIn(ECHO_PIN, HIGH); // Measure duration of echo pin high state
  long distance = duration * 0.034 / 2; // Calculate distance based on duration of echo

  Serial.print("Distance: "); // Print distance
  Serial.println(distance);

  char message[8]; // Prepare message array
  if (distance < 50 && distance > 0) { // Check if object is within 50 cm
    Serial.println("Occupied"); // Print "Occupied" if true
  } else {
    Serial.println("Free"); // Print "Free" otherwise
  }

  esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message)); // Send message via ESP-NOW
  delay(100); // Delay to allow time for message to be sent

  Serial.println("Going to deep sleep now"); // Indicate going to sleep
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME); // Set deep sleep wakeup timer
  esp_deep_sleep_start(); // Start deep sleep
}
