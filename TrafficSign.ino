// نظام إشارات مرور بدون استخدام delay()، باستخدام millis()

#include <Arduino.h>

const int IR1 = 2;
const int IR2 = 3;
const int IR3 = 4;
const int IR4 = 5;

const int red1 = 6;
const int orange1 = 7;
const int green1 = 8;

const int red2 = 12;
const int orange2 = 10;
const int green2 = 11;

int defaultTime = 7000; // 7 ثواني للإشارة الخضراء
int userTime1 = 5000;
int userTime2 = 5000;

int mode = 1; // الوضع الافتراضي زحمة
bool lastServedLane1 = false;
bool priorityGiven = false; // لضمان إعطاء أولوية مرة واحدة فقط في وضع الزحمة

unsigned long lastSwitchTime = 0;
int currentLane = 1;
int currentStep = 0; // 0: Green, 1: Orange
unsigned long stepStartTime = 0;
int currentDuration = defaultTime + 5000; // زيادة 5 ثواني إذا كانت هناك زحمة

void setRed(int lane, bool on = true) {
  if (lane == 1) digitalWrite(red1, on ? HIGH : LOW);
  else digitalWrite(red2, on ? HIGH : LOW);
}

void setOrange(int lane, bool on) {
  if (lane == 1) digitalWrite(orange1, on ? HIGH : LOW);
  else digitalWrite(orange2, on ? HIGH : LOW);
}

void setGreen(int lane, bool on) {
  if (lane == 1) digitalWrite(green1, on ? HIGH : LOW);
  else digitalWrite(green2, on ? HIGH : LOW);
}

void setup() {
  // بدء التشغيل بإشارة خضراء للمسار 1
  setRed(1, false);
  setGreen(1, true);
  stepStartTime = millis();

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  pinMode(red1, OUTPUT);
  pinMode(orange1, OUTPUT);
  pinMode(green1, OUTPUT);

  pinMode(red2, OUTPUT);
  pinMode(orange2, OUTPUT);
  pinMode(green2, OUTPUT);

  Serial.begin(9600);
  setRed(1);
  setRed(2);
}

void loop() {
  static unsigned long lastCountdownPrint = 0;
  // قراءة الحساسات
  bool lane1Sensor = (digitalRead(IR1) == LOW || digitalRead(IR2) == LOW);
  bool lane2Sensor = (digitalRead(IR3) == LOW || digitalRead(IR4) == LOW);

  // تلقي أوامر من المستخدم
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("mode-")) {
      mode = input.substring(5).toInt();
      Serial.println("Mode set to: " + String(mode));
      priorityGiven = false; // إعادة تعيين عند تغيير الوضع
    } else if (input.startsWith("1-")) {
      userTime1 = input.substring(2).toInt() * 1000;
      Serial.println("User time for Lane 1: " + String(userTime1 / 1000));
    } else if (input.startsWith("2-")) {
      userTime2 = input.substring(2).toInt() * 1000;
      Serial.println("User time for Lane 2: " + String(userTime2 / 1000));
    }
  }

  unsigned long now = millis();

  // طباعة عداد تنازلي كل ثانية أثناء الإشارة الخضراء
  if (currentStep == 0 && now - lastCountdownPrint >= 1000) {
    int remaining = (currentDuration - (now - stepStartTime)) / 1000;
    Serial.println("⏳ تبقى " + String(remaining) + " ثانية للإشارة الخضراء في المسار " + String(currentLane));
    lastCountdownPrint = now;
  } else if (currentStep == 1 && now - lastCountdownPrint >= 1000) {
    int remaining = (3000 - (now - stepStartTime)) / 1000;
    Serial.println("⏳ تبقى " + String(remaining) + " ثانية للإشارة البرتقالية في المسار " + String(currentLane));
    lastCountdownPrint = now;
  }

  if (currentStep == 0 && now - stepStartTime >= currentDuration) {
    // انتقل إلى اللون البرتقالي
    setGreen(currentLane, false);
    setOrange(currentLane, true);
    currentStep = 1;
    stepStartTime = now;
  }
  else if (currentStep == 1 && now - stepStartTime >= 3000) {
    // العودة إلى اللون الأحمر
    setOrange(currentLane, false);
    setRed(currentLane);

    // اختيار المسار التالي حسب الوضع
    if (mode == 0) {
      currentLane = (currentLane == 1) ? 2 : 1;
      if ((currentLane == 1 && lane1Sensor) || (currentLane == 2 && lane2Sensor)) {
        currentDuration = defaultTime + 5000;
        Serial.println("🚗 ازدحام في المسار " + String(currentLane) + ": تمت إضافة 3 ثوانٍ للإشارة الخضراء.");
      } else {
        currentDuration = defaultTime;
      }
    }
    else if (mode == 1) {
      if (!priorityGiven) {
        if (lane1Sensor && !lane2Sensor) {
          currentLane = 1;
          priorityGiven = true;
        } else if (lane2Sensor && !lane1Sensor) {
          currentLane = 2;
          priorityGiven = true;
        } else {
          currentLane = lastServedLane1 ? 2 : 1;
        }
      } else {
        currentLane = (lastServedLane1) ? 2 : 1;
        priorityGiven = false;
      }
      if ((currentLane == 1 && lane1Sensor) || (currentLane == 2 && lane2Sensor)) {
        currentDuration = defaultTime + 5000;
        Serial.println("🚗 ازدحام في المسار " + String(currentLane) + ": تمت إضافة 3 ثوانٍ للإشارة الخضراء.");
      } else {
        currentDuration = defaultTime;
      }
    }
    else if (mode == 2) {
      currentLane = (currentLane == 1) ? 2 : 1;
      currentDuration = (currentLane == 1) ? userTime1 : userTime2;
    }

    lastServedLane1 = (currentLane == 1);
    setRed((currentLane == 1) ? 2 : 1); // اجعل الآخر أحمر
    setRed(currentLane, false);
    setGreen(currentLane, true);
    currentStep = 0;
    stepStartTime = now;
  }
}


