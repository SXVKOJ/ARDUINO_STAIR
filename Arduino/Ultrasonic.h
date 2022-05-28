#pragma once

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

class Ultrasonic {
public:
	Ultrasonic(const int& trigger_pin, const int& echo_pin);
public:
	long timing();
	long ranging(const int& sys);
private:
	int trigger_pin;
	int echo_pin;
};

#endif // !ULTRASONIC_H_