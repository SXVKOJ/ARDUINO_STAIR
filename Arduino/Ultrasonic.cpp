#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(const int &trigger_pin, const int &echo_pin) {
	pinMode(trigger_pin, OUTPUT);
	pinMode(echo_pin, INPUT);

	this->trigger_pin = trigger_pin;
	this->echo_pin = echo_pin;
}

long Ultrasonic::timing() {
	digitalWrite(trigger_pin, LOW);
	delayMicroseconds(30);

	digitalWrite(trigger_pin, HIGH);
	delayMicroseconds(60);
	
	digitalWrite(trigger_pin, LOW);
	
	const long duration = pulseIn(echo_pin, HIGH);

	return duration;
}

long Ultrasonic::ranging(const int& sys) {
	const long duration = timing();

	const long distance_cm = duration / 58;
	const long distance_in = duration / 148;

	if (sys == 1) {
		return distance_cm;
	} else if (sys == 2) {
		return distance_in;
	} else {
		return -1;
	}
}