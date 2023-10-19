#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);
void lightOn();
void lightOff();

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // turn the LED on (HIGH is the voltage level)
  lightOn();
  // wait for a second
  delay(5000);
  // turn the LED off by making the voltage LOW
  lightOff();
   // wait for a second
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void lightOn(){
  digitalWrite(LED_BUILTIN, HIGH);
}

void lightOff(){
  digitalWrite(LED_BUILTIN, LOW);
}