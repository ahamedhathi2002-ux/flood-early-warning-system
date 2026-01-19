#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>   // 🔥 REQUIRED for esp_wifi_set_channel()

#define TRIG_PIN 16
#define ECHO_PIN 17

typedef struct struct_message {
  float distance;
} struct_message;

struct_message myData;

// 🔁 Receiver MAC (ESP32-C3 Super Mini)
uint8_t receiverAddress[] = {0xD8, 0x3B, 0xDA, 0x34, 0xF1, 0x8C};

esp_now_peer_info_t peerInfo;

// Measure distance
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout added
  if (duration == 0) return -1;

  return duration * 0.0343 / 2;
}

// Send callback
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✅ Success" : "❌ Fail");
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // ✅ MUST SET SAME CHANNEL AS RECEIVER
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);  // 🔥 IMPORTANT

  Serial.print("Transmitter MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Add peer ONLY ONCE
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 1;      // 🔥 MUST MATCH
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP-NOW Transmitter Ready");
}

void loop() {
  myData.distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(myData.distance);

  esp_now_send(receiverAddress, (uint8_t *)&myData, sizeof(myData));
  delay(1000);
}
