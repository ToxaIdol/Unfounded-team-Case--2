#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include <microLED.h> 
 
#define LEDS_PIN 4     // пин ленты 
#define NUMLEDS 16      // кол-во светодиодов 
#define SCREEN_WIDTH 128 // ширина экрана 
#define SCREEN_HEIGHT 64 // высота экрана 
 
 
// Пины для датчиков 
const int pirPin = 2;       // Пин для датчика движения 
const int lightSensorPin = A0; // Пин для фоторезистора 
 
// Переменная для хранения значения освещенности 
int lightLevel = 0; 
 
// Время работы лампы после обнаружения движения 
unsigned long motionDetectedTime = 0; 
unsigned long motionTimeout = 60000; // Начальное значение таймаута - 60 секунд 
 
// Настройки для OLED дисплея 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 
 
// Инициализация светодиодов 
microLED<NUMLEDS, LEDS_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip; 
 
int timeoutSetting = 60;  // Время работы лампы после обнаружения движения (секунды) 
 
int editState = 0; 
int menuState = 0; 
 
bool lampOn = false; 

void turnOnLamp() {
  strip.setBrightness(255); 
  for (int i = 0; i < NUMLEDS; ++i) {
    strip.set(i, mRGB(255, 255, 255)); // Устанавливаем цвет в белый
  }
  strip.show(); // Обновляем ленту
}

// Функция для выключения лампы
void turnOffLamp() {
  for (int i = 0; i < NUMLEDS; ++i) {
    strip.set(i, mRGB(0, 0, 0)); // Устанавливаем цвет в черный (выключено)
  }
  strip.show(); // Обновляем ленту
}
 
void setup() { 
 
  strip.setBrightness(255); 
  strip.clear(); 
  strip.show(); 
  delay(1);     // между вызовами show должна быть пауза 
 
  for (int i = 0; i <= NUMLEDS; ++i) { 
    strip.set(i, mRGB(255, 255, 255)); 
    strip.show(); // вывод изменений на ленту 
    delay(50); 
  } 
 
  pinMode(pirPin, INPUT);      // Датчик движения как вход 
  pinMode(lightSensorPin, INPUT); // Фоторезистор как вход 
   
 
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
  updateLeds(); 
  delay(50); // Небольшая задержка между итерациями 
} 
 
 
 
 
void checkMotion() { 
  bool motionDetected = digitalRead(pirPin); 
 
  if (motionDetected) { 
    motionDetectedTime = millis(); // Запоминаем время обнаружения движения 
    lampOn = true; 
    turnOnLamp();
  } else { 
    if ((millis() - motionDetectedTime) > (timeoutSetting * 1000UL) && lampOn) { 
      lampOn = false; 
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
  display.println(lampOn); 
  display.display(); 
} 
 
 void updateLeds() { 
  // strip.setBrightness(lightLevel); 
  for (int i = 0; i <= NUMLEDS; ++i) { 
    strip.set(i, mRGB(1*lightLevel, 1*lightLevel, 1*lightLevel)); 
    strip.show(); // вывод изменений на ленту 
    delay(10); 
  } 
}
