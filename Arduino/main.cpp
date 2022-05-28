#include "Ultrasonic.h"
#include "Adafruit_NeoPixel.h"

// === ���������, ������� ����� �������� === //

#define LESENKA 17    // ���������� �������� �� �������
#define LED 50        // ���������� ����������� �� ����� �������
#define PIN_LED 3     // ��� ��� ����������� �������� �����
#define PIN_VKL 2     // ��� ��� ����������� �������������

#define TIMER 5       // ����� ��� �������� ���������/����������
#define PAUSA 100     // ����� ����� ����������� ��������
#define BRIGHT 200    // ������������ ������� �����
#define LIGHT 10      // ����������� ������� �����
#define DIST1 50      // ���������� ������������ 1 �������
#define DIST2 50      // ���������� ������������ 2 �������

#define CM  1 // Ultrasonic ����������
#define INC 2 // Ultrasonic �����

// === ���������, ������� ����� �������� === // 

Ultrasonic ultrasonic1(0, 1);   //Trig - 0, Echo - 1
Ultrasonic ultrasonic2(4, 5);   //Trig - 4, Echo - 5

int NUM_LED = LESENKA * LED;
Adafruit_NeoPixel strip(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

int cm1, cm2, t, i, j, g, J;
byte w;

void setup() {
    strip.begin();                             // �������������� ������ NeoPixel
    strip.show();                              // ��������� ��� ������� �� �����
    strip.setBrightness(BRIGHT);               // ��������� ������� (�������� 255)
    Serial.begin(9600);
    pinMode(PIN_VKL, INPUT);  // �������������� ��� ��� ����������
}

// ������ ���������� � 1 �������
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

// ������ ���������� �� 2 �������
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

    // ���� ������������� ������� ������������� ����������� ������� �����
    if (digitalRead(PIN_VKL) == HIGH) { J = 0; }
    if (digitalRead(PIN_VKL) == LOW) { J = LIGHT; }

    // �������� ��� ������� �� �������� � �������� ��������
    for (i = 0; i <= NUM_LED; i++) {
        strip.setPixelColor(i, strip.Color(J, J, J * 2));
        strip.show();
    }

    // ���������� ���������� �������������� �������
    u1();
    delay(100);
    u2();
    delay(100);

    if (cm1 < DIST1) { w = 1; }  // ���� �������� ������ ������  -  ������� � 1 ����
    if (cm2 < DIST2) { w = 2; }  // ���� �������� ������ ������  -  ������� �� 2 ����

    while (w == 1) {
        // ������� ��������� �������� ����� �� �������� �����-�����
        for (i = 0; i <= NUM_LED + 1; i = i + LED) {
            for (j = J; j <= 125; j++) {
                strip.setPixelColor(i, strip.Color(j, j, j * 2));
                strip.setPixelColor(i + 1, strip.Color(j, j, j * 2));
                strip.show();
                delay(TIMER);
            }
            delay(PAUSA);
        }
        w = 5; // ������� � ����� ���� ��� ��������
    }

    while (w == 2) {
        // ������� ��������� �������� ����� �� �������� ������-����
        for (i = NUM_LED + 1; i >= 0; i = i - LED) {
            for (j = J; j <= 160; j++) {
                strip.setPixelColor(i, strip.Color(j, j, j * 2));
                strip.setPixelColor(i - 1, strip.Color(j, j, j * 2));
                strip.show();
                delay(TIMER);
            }
            delay(PAUSA);
        }
        w = 5; // ������� � ����� ���� ��� ��������
    }

    while (w == 5) {
        // ������� ��������� ��������� �������� �� �������� �����
        for (j = 124; j >= J; j = j - 2) {
            for (i = 0; i <= NUM_LED; i++) {
                strip.setPixelColor(i, strip.Color(j, j, j * 2));
                strip.show();
            }

            // ���������� ���������� �������������� �������
            u1();
            delay(j / 4);
            u2();
            delay(j / 4);

            // ���� �������� ���� �� �������� - ��������� � ��������������� ����
            if (cm1 < DIST1) { w = 3; goto w3; }
            if (cm2 < DIST2) { w = 4; goto w4; }
        }
        w = 0; // ���� �� ���� �� �������� �� �������� ������� �� �����
    }

    while (w == 3) {
    w3:
        t = map(j, 10, 125, 20, 0);
        // ������� ���������� ��������� �������� �����-�����
        for (i = 0; i <= NUM_LED + 1; i = i + LED) {
            for (g = j; g >= J; g--) {
                strip.setPixelColor(i, strip.Color(g, g, g * 2));
                strip.setPixelColor(i + 1, strip.Color(g, g, g * 2));
                strip.show();
                delay(TIMER + t / 2);
            }
            delay(PAUSA);
        }
        w = 0; // ������� �� ����� � ������ ���������
    }

    while (w == 4) {
    w4:
        t = map(j, 10, 125, 20, 0);
        // ������� ���������� ��������� �������� ������-����
        for (i = NUM_LED + 1; i >= 0; i = i - LED) {
            for (g = j; g >= J; g--) {
                strip.setPixelColor(i, strip.Color(g, g, g * 2));
                strip.setPixelColor(i - 1, strip.Color(g, g, g * 2));
                strip.show();
                delay(TIMER + t / 2);
            }
            delay(PAUSA);
        }
        w = 0; // ������� �� ����� � ������ ���������
    }

}