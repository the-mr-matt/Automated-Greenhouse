//----LIBRARIES----
#include <dht.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Arduino.h>
#include "A4988.h"
#include <Wire.h>
#include "RTClib.h"

//----PINS----
#define OVRD_WINDOW 4
#define OVRD_WATER 5

#define MOTOR_STEPS 200
#define STEP 7
#define DIR 8

#define SOLENOID 6

#define TEMP_SNSR 12

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
RTC_DS1307 rtc;

//milliseconds * seconds
const int delayInterval = 1000 * 10;

const int tempThreshold = 30;
const double soilMoistureThreshold = 30.0;
const int waterDuration = 1000 * 10;

int debounceDuration = 50;

//----STATE----
int startMillis = 0;

bool isWindowOpen = false;
bool isDisplayingTemperature = true;
bool windowOvrdPressed = false;
bool waterOvrdPressed = false;
bool waterScheduled;

//----BOOK KEEPING----
int b_OvrdWindowDebounceTime = 0;
int b_OvrdWaterDebounceTime = 0;
int b_OvrdWindowState = LOW;
int b_OvrdWaterState = LOW;

bool b_IsDay;

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

  //init clock
  Wire.begin();
  rtc.begin();

  //set clock time - only done once
  //once the time has been set, this is commented out and reuploaded
  //rtc.adjust(DateTime(2018, 7, 23, 7, 59, 30));
  //rtc.adjust(DateTime(__DATE__, __TIME__));

  //init water
  StopWater();

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
  //check if the day time status has changed
  Serial.print("Is Day: ");
  Serial.print(IsDay());
  Serial.print(" Hour: ");
  Serial.print(rtc.now().hour());
  Serial.print(" Minute: ");
  Serial.print(rtc.now().minute());
  Serial.print(" Second: ");
  Serial.println(rtc.now().second());
  if(IsDay() != b_IsDay) {
    //check if it is now day time and water was schedualed from last night
    if(IsDay() && waterScheduled) {
      Serial.println("Starting Schedual");
      delay(100);
      StartWater();
      waterScheduled = false;
    }
  }

  //wait for 5 seconds before switching to the next input to display on the lcd
  int currentMillis = millis();
  if (currentMillis - startMillis >= delayInterval) {
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
  int windowState = digitalRead(OVRD_WINDOW);
  int waterState = digitalRead(OVRD_WATER);

  //check when the button state has changed
  if (windowState != b_OvrdWindowState) {
    //set last debounce to now
    b_OvrdWindowDebounceTime = millis();
  }

  if (waterState != b_OvrdWaterState) {
    b_OvrdWaterDebounceTime = millis();
  }

  //wait until enough time has passed to consider the next input
  if ((millis() - b_OvrdWindowDebounceTime) > debounceDuration) {
    if (windowState == HIGH) {
      //store new state
      b_OvrdWindowState = windowState;

      //toggle windows
      if (isWindowOpen) {
        CloseWindow();
      }
      else {
        OpenWindow();
      }
    }
  }

  if ((millis() - b_OvrdWaterDebounceTime) > debounceDuration) {
    if (waterState == HIGH) {
      b_OvrdWaterState = waterState;

      StartWater();
    }
  }

  //get the current day time status
  b_IsDay = IsDay();

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
  //dont bother if water is already schedualed for next morning
  if (!waterScheduled && moisturePercentage < soilMoistureThreshold) {
    //check to make sure its ok to turn on the water now
    if (IsDay()) {
      //open water
      StartWater();
    }
    else {
      //schedual watering for the next morning
      waterScheduled = true;
      Serial.println("Schedualed Water");
      delay(100);
    }
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

void StartWater() {
  Serial.println("water on");

  digitalWrite(SOLENOID, HIGH);

  //inform user
  lcd.clear();
  lcd.print("Water On");

  //wait while it waters
  delay(waterDuration);

  //close water when finished
  StopWater();
}

void StopWater() {
  Serial.println("water off");

  digitalWrite(SOLENOID, LOW);

  //inform user
  lcd.clear();
  lcd.print("Water Off");

  //wait a bit to allow user to process message
  delay(3000);

  //reset millis so the temp and humidity will immediately be displayed
  startMillis = 0;
}

bool IsDay() {
  //get current time
  DateTime now = rtc.now();

  //day time is considered to be between 8AM and 5PM for this project
  return (now.hour() >= 8 && now.hour() < 17);
}
