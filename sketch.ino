#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define led 8
#define SCREEN_WIDTH 128 // ширина экрана
#define SCREEN_HEIGHT 64 // высота экрана

// Пины для датчиков
const int pirPin = 2;       // Пин для датчика движения
const int lightSensorPin = A0; // Пин для фоторезистора
const int relayPin = 8;     // Пин для реле

// Переменная для хранения значения освещенности
int lightLevel = 0;

// Время работы лампы после обнаружения движения
unsigned long motionDetectedTime = 0;
unsigned long motionTimeout = 60000; // Начальное значение таймаута - 60 секунд

// Настройки для OLED дисплея
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Глобальные переменные для настроек
int lightThreshold = 500; // Порог срабатывания датчика освещенности
int timeoutSetting = 60;  // Время работы лампы после обнаружения движения (секунды)

int editState = 0;
int menuState = 0;
void setup() {
  pinMode(pirPin, INPUT);      // Датчик движения как вход
  pinMode(lightSensorPin, INPUT); // Фоторезистор как вход
  pinMode(relayPin, OUTPUT);    // Реле как выход
  
  digitalWrite(relayPin, LOW);  // Выключение лампы при старте

  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Адрес I2C дисплея может отличаться
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Если экран не инициализировался, зацикливаемся
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("System started");
  display.display();
  delay(1000);
}

void loop() {
  checkMotion(); // Проверка наличия движения
  adjustLightLevel(); // Регулировка яркости лампы
  updateDisplay(); // Обновление информации на дисплее
  delay(50); // Небольшая задержка между итерациями
}




void checkMotion() {
  bool motionDetected = digitalRead(pirPin);

  if (motionDetected) {
    motionDetectedTime = millis(); // Запоминаем время обнаружения движения
    digitalWrite(relayPin, HIGH);  // Включаем лампу
    digitalWrite(led, HIGH);
  } else {
    if ((millis() - motionDetectedTime) > (timeoutSetting * 1000UL) && digitalRead(relayPin) == HIGH) {
      digitalWrite(relayPin, LOW); // Выключаем лампу, если прошло больше заданного времени
    }
  }
}

void adjustLightLevel() {
  lightLevel = analogRead(lightSensorPin); // Читаем значение с фоторезистора
  lightLevel = map(lightLevel, 0, 1023, 0, 255); // Преобразуем значение в диапазон от 0 до 255
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Motion detected: ");
  display.println(digitalRead(pirPin));
  display.print("Light level: ");
  display.println(lightLevel);
  display.print("Lamp status: ");
  display.println(digitalRead(relayPin));
  display.display();
}