//----LIBRARIES----
#include <dht.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Arduino.h>
#include "A4988.h"

//----PINS----
#define OVRD_WINDOW 12
#define OVRD_WATER 5

#define MOTOR_STEPS 200
#define STEP 8
#define DIR 7

#define SOLENOID 6

#define TEMP_SNSR 4

#define WINDOW_UP 3
#define WINDOW_DOWN 2

#define MOISTURE1 A0
#define MOISTURE2 A1
#define MOISTURE3 A2
#define MOISTURE4 A3

//----CONFIG----
dht tempSnsr;
LiquidCrystal lcd(9);
A4988 stepper(MOTOR_STEPS, DIR, STEP);

//milliseconds * seconds
const int delayInterval = 1000 * 10;

const int tempThreshold = 30;
const double soilMoistureThreshold = 30.0;
const int waterDuration = 1000 * 10;

//----STATE----
int startMillis = 0;
bool isWindowOpen = false;
bool isWaterOpen = false;
bool isDisplayingTemperature = true;
bool windowOvrdPressed = false;
bool waterOvrdPressed = false;

void setup() {
  //init serial for debugging
  Serial.begin(9600);

  //16 columns and 2 rows
  lcd.begin(16, 2);

  //init cursor position
  lcd.setCursor(0, 0);
  lcd.print("Initializing");
  delay(3000);

  //setup pins
  pinMode(OVRD_WINDOW, INPUT);
  pinMode(OVRD_WATER, INPUT);
  pinMode(WINDOW_UP, INPUT);
  pinMode(WINDOW_DOWN, INPUT);
  pinMode(SOLENOID, OUTPUT);

  //init water
  CloseWater();

  //init motor
  //30 rpm, full step (no microstepping)
  stepper.begin(30, 1);
  CloseWindow();

  //finish init sequence
  lcd.clear();
  lcd.print("Ready To Start");
  delay(3000);

  //init start time
  startMillis = 0;
}

void loop() {
  //wait for 5 seconds
  int currentMillis = millis();
  if (currentMillis - startMillis >= delayInterval) {
    Serial.println("tick");
    //read input
    if (isDisplayingTemperature) {
      ReadTemperature();
    }
    else {
      ReadSoilMoisture();
    }

    //switch input for next interval
    isDisplayingTemperature = !isDisplayingTemperature;

    //set new start time to now
    startMillis = currentMillis;
  }

  //check override buttons
  //only trigger when button is first pressed down
  if (digitalRead(OVRD_WINDOW) == HIGH && !windowOvrdPressed) {
    //debounce
    //delay(1);
    //if (digitalRead(OVRD_WINDOW) == HIGH) {
    windowOvrdPressed = true;

    if (isWindowOpen) {
      CloseWindow();
    }
    else {
      OpenWindow();
    }
    //}
  }

  //button release
  if (digitalRead(OVRD_WINDOW) == LOW) {
    windowOvrdPressed = false;
  }

  //only trigger when button is first pressed down
  if (digitalRead(OVRD_WATER) == HIGH && !waterOvrdPressed) {
    //debounce
    //delay(1);
    //if (digitalRead(OVRD_WATER) == HIGH) {
    waterOvrdPressed = true;

    if (isWaterOpen) {
      CloseWater();
    }
    else {
      OpenWater();
    }
    //}
  }

  //button release
  if (digitalRead(OVRD_WATER) == LOW) {
    waterOvrdPressed = false;
  }

  //slow down sketch
  delay(50);
}

void ReadTemperature() {
  //read temp sensor
  int result = tempSnsr.read11(TEMP_SNSR);

  //check if result was received ok
  if (result == DHTLIB_OK) {
    //clear lcd
    lcd.clear();

    //display to temp and humidity lcd
    //temperature
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tempSnsr.temperature);
    lcd.print("*C");

    //humidity on next line
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(tempSnsr.humidity);
    lcd.print("%");

    //if temp is over or under threshold - make appropriate action
    if (tempSnsr.temperature >= tempThreshold && !isWindowOpen) {
      OpenWindow();
    }
    if (tempSnsr.temperature < tempThreshold && isWindowOpen) {
      CloseWindow();
    }
  }
}

void ReadSoilMoisture() {
  //flip it (high value should represent high moisture)
  int moisture1 = 1024 - analogRead(MOISTURE1);
  int moisture2 = 1024 - analogRead(MOISTURE2);
  int moisture3 = 1024 - analogRead(MOISTURE3);
  int moisture4 = 1024 - analogRead(MOISTURE4);

  //calculate average across all sensors
  double avgMoisture = moisture1 + moisture2 + moisture3 + moisture4;
  avgMoisture /= 4;
  double moisturePercentage = avgMoisture / 1024.0 * 100.0;

  //clear lcd
  lcd.clear();

  //display moisture to lcd
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture:");
  lcd.setCursor(0, 1);
  lcd.print(moisturePercentage);
  lcd.print("%");

  //if moisture is over or under threshold - make appropriate action
  if (moisturePercentage < soilMoistureThreshold && !isWaterOpen) {
    //open water
    OpenWater();
  }
}

void OpenWindow() {
  //start procedure
  Serial.println("opening window");
  lcd.clear();
  lcd.print("Opening Window..");

  //wait until window up switch is pushed
  int state = LOW;
  while (state == LOW)
  {
    state = digitalRead(WINDOW_UP);

    stepper.rotate(1.8);
    delay(25);
  }

  //inform user that window is now closed
  isWindowOpen = true;
  Serial.println("windows open");
  lcd.clear();
  lcd.print("Windows Open");

  //wait a bit to allow user to process mes
  delay(3000);

  //reset millis so the temp and humidity will immediately be displayed
  startMillis = 0;
}

void CloseWindow() {
  //start procedure
  Serial.println("closing window");
  lcd.clear();
  lcd.print("Closing Window..");

  //wait until window down switch is pushed
  int state = LOW;
  while (state == LOW)
  {
    state = digitalRead(WINDOW_DOWN);

    stepper.rotate(-1.8);
    delay(25);
  }

  //inform user that window is now closed
  isWindowOpen = false;
  Serial.println("windows closed");
  lcd.clear();
  lcd.print("Windows Closed");

  //wait a bit to allow user to process message
  delay(3000);

  //reset millis so the temp and humidity will immediately be displayed
  startMillis = 0;
}

void OpenWater() {
  Serial.println("open water");
  isWaterOpen = true;

  digitalWrite(SOLENOID, HIGH);

  //inform user
  lcd.clear();
  lcd.print("Water On");

  //wait while it waters
  delay(waterDuration);

  //close water when finished
  CloseWater();
}

void CloseWater() {
  Serial.println("close water");
  isWaterOpen = false;

  digitalWrite(SOLENOID, LOW);

  //inform user
  lcd.clear();
  lcd.print("Water Off");

  //wait a bit to allow user to process message
  delay(3000);

  //reset millis so the temp and humidity will immediately be displayed
  startMillis = 0;
}
