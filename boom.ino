// Multi sensor based real time data monitoring and security system
#define BLYNK_TEMPLATE_ID "TMPL3axb_KWcH"
#define BLYNK_TEMPLATE_NAME "Home Security and Real time Monitoring System"
#define BLYNK_AUTH_TOKEN "YbbonXdPGFejMUOeZc0mJM-emONe6Egr"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Block-G";  // type your wifi name
char pass[] = "Rise@2023";  // type your wifi password

#define PIR_SENSOR  12
#define VIBRATION_SENSOR  13
#define TOUCH_SENSOR  14
#define GAS_SENSOR  A0

#define VIRTUAL_PIN_PIR V0
#define VIRTUAL_PIN_VIBRATION V1
#define VIRTUAL_PIN_TOUCH V2
#define VIRTUAL_PIN_GAS V3

BlynkTimer timer;

void notifyOnSensor(const char* sensor_name, int sensor_value, int virtual_pin) {
  if (sensor_value == 1) {
    Serial.printf("%s Alert\n", sensor_name);
    Blynk.logEvent(sensor_name, String(sensor_name) + " Alert");
    Blynk.virtualWrite(virtual_pin, 1);  // Send alert status to Blynk
  } else {
    Blynk.virtualWrite(virtual_pin, 0);  // Send no alert status to Blynk
  }
}

void checkSensors() {
  int pirState = digitalRead(PIR_SENSOR);
  int vibrationState = digitalRead(VIBRATION_SENSOR);
  int touchState = digitalRead(TOUCH_SENSOR);
  int gasState = digitalRead(GAS_SENSOR);

  notifyOnSensor("PIR Sensor", pirState, VIRTUAL_PIN_PIR);
  notifyOnSensor("Vibration Sensor", vibrationState, VIRTUAL_PIN_VIBRATION);
  notifyOnSensor("Touch Sensor", touchState, VIRTUAL_PIN_TOUCH);
  notifyOnSensor("Gas Sensor", gasState, VIRTUAL_PIN_GAS);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.printf("PIR: %s\n", pirState ? "Alert" : "Safe");
  display.printf("Vibration: %s\n", vibrationState ? "Alert" : "Safe");
  display.printf("Touch: %s\n", touchState ? "Alert" : "Safe");
  display.printf("Gas: %s\n", gasState ? "Alert" : "Safe");
  display.display();
}

void setup(){
  pinMode(PIR_SENSOR, INPUT);
  pinMode(VIBRATION_SENSOR, INPUT);
  pinMode(TOUCH_SENSOR, INPUT);
  pinMode(GAS_SENSOR, INPUT);

  Serial.begin(115200);

  if(!display.begin(SSD1306_PAGEADDR, SCREEN_WIDTH, SCREEN_HEIGHT)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkSensors);
}

void loop(){
  Blynk.run();
  timer.run();
}
