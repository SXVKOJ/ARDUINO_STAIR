#include "Ultrasonic.h"
#include "Adafruit_NeoPixel.h"

// === настройки, которые можно поменять === //

#define LESENKA 17    // количество ступеней на лесенке
#define LED 50        // количество светодиодов на одной ступени
#define PIN_LED 3     // пин для подключения адресной ленты
#define PIN_VKL 2     // пин для подключения переключателя

#define TIMER 5       // пауза для плавного включения/выключения
#define PAUSA 100     // пауза между включениями ступенек
#define BRIGHT 200    // максимальная яркость ленты
#define LIGHT 10      // минимальная яркость ленты
#define DIST1 50      // расстояние срабатывания 1 датчика
#define DIST2 50      // расстояние срабатывания 2 датчика

#define CM  1 // Ultrasonic сантиметры
#define INC 2 // Ultrasonic дюймы

// === настройки, которые можно поменять === // 

Ultrasonic ultrasonic1(0, 1);   //Trig - 0, Echo - 1
Ultrasonic ultrasonic2(4, 5);   //Trig - 4, Echo - 5

int NUM_LED = LESENKA * LED;
Adafruit_NeoPixel strip(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

int cm1, cm2, t, i, j, g, J;
byte w;

void setup() {
    strip.begin();                             // инициализируем объект NeoPixel
    strip.show();                              // отключаем все пиксели на ленте
    strip.setBrightness(BRIGHT);               // указываем яркость (максимум 255)
    Serial.begin(9600);
    pinMode(PIN_VKL, INPUT);  // инициализируем пин для включателя
}

// узнаем расстояние с 1 датчика
void u1() {
    for (byte i = 0; i <= 3; i++) {
        cm1 = ultrasonic1.ranging(CM);
        cm1 = cm1 + cm1;
        delay(10);
    }
    cm1 = cm1 / 3;
    Serial.print("DIST1 - ");
    Serial.println(cm1);
}

// узнаем расстояние со 2 датчика
void u2() {
    for (byte i = 0; i <= 3; i++) {
        cm2 = ultrasonic2.ranging(CM);
        cm2 = cm2 + cm2;
        delay(10);
    }
    cm2 = cm2 / 3;
    Serial.print("DIST2 - ");
    Serial.println(cm2);
}

void loop() {

    // если переключатель включен устанавливаем минимальную яркость ленты
    if (digitalRead(PIN_VKL) == HIGH) { J = 0; }
    if (digitalRead(PIN_VKL) == LOW) { J = LIGHT; }

    // включаем все ступени на лестнице с заданной яркостью
    for (i = 0; i <= NUM_LED; i++) {
        strip.setPixelColor(i, strip.Color(J, J, J * 2));
        strip.show();
    }

    // поочередно опрашиваем ультразвуковые датчики
    u1();
    delay(100);
    u2();
    delay(100);

    if (cm1 < DIST1) { w = 1; }  // если сработал первый датчик  -  заходим в 1 цикл
    if (cm2 < DIST2) { w = 2; }  // если сработал второй датчик  -  заходим во 2 цикл

    while (w == 1) {
        // плавное включение адресной ленты на лестнице снизу-вверх
        for (i = 0; i <= NUM_LED + 1; i = i + LED) {
            for (j = J; j <= 125; j++) {
                strip.setPixelColor(i, strip.Color(j, j, j * 2));
                strip.setPixelColor(i + 1, strip.Color(j, j, j * 2));
                strip.show();
                delay(TIMER);
            }
            delay(PAUSA);
        }
        w = 5; // переход в пятый цикл для ожидания
    }

    while (w == 2) {
        // плавное включение адресной ленты на лестнице сверху-вниз
        for (i = NUM_LED + 1; i >= 0; i = i - LED) {
            for (j = J; j <= 160; j++) {
                strip.setPixelColor(i, strip.Color(j, j, j * 2));
                strip.setPixelColor(i - 1, strip.Color(j, j, j * 2));
                strip.show();
                delay(TIMER);
            }
            delay(PAUSA);
        }
        w = 5; // переход в пятый цикл для ожидания
    }

    while (w == 5) {
        // плавное затухание подсветки лестницы на адресной ленте
        for (j = 124; j >= J; j = j - 2) {
            for (i = 0; i <= NUM_LED; i++) {
                strip.setPixelColor(i, strip.Color(j, j, j * 2));
                strip.show();
            }

            // поочередно опрашиваем ультразвуковые датчики
            u1();
            delay(j / 4);
            u2();
            delay(j / 4);

            // если сработал один из датчиков - переходим в соответствующий цикл
            if (cm1 < DIST1) { w = 3; goto w3; }
            if (cm2 < DIST2) { w = 4; goto w4; }
        }
        w = 0; // если ни один из датчиков не сработал выходим из цикла
    }

    while (w == 3) {
    w3:
        t = map(j, 10, 125, 20, 0);
        // плавное выключение подсветки лестницы снизу-вверх
        for (i = 0; i <= NUM_LED + 1; i = i + LED) {
            for (g = j; g >= J; g--) {
                strip.setPixelColor(i, strip.Color(g, g, g * 2));
                strip.setPixelColor(i + 1, strip.Color(g, g, g * 2));
                strip.show();
                delay(TIMER + t / 2);
            }
            delay(PAUSA);
        }
        w = 0; // выходим из цикла в начало программы
    }

    while (w == 4) {
    w4:
        t = map(j, 10, 125, 20, 0);
        // плавное выключение подсветки лестницы сверху-вниз
        for (i = NUM_LED + 1; i >= 0; i = i - LED) {
            for (g = j; g >= J; g--) {
                strip.setPixelColor(i, strip.Color(g, g, g * 2));
                strip.setPixelColor(i - 1, strip.Color(g, g, g * 2));
                strip.show();
                delay(TIMER + t / 2);
            }
            delay(PAUSA);
        }
        w = 0; // выходим из цикла в начало программы
    }

}