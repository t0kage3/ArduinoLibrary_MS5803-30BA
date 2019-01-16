#include "MS580330BA.h"

MS580330BA PressureSensor;

const int PIN_SENSOR_CS = 10;

void setup(){
  Serial.begin(9600);
  PressureSensor.begin(PIN_SENSOR_CS);
}

void loop(){
  PressureSensor.read();
  Serial.print("pressure: ");
  Serial.print(PressureSensor.pressure);
  Serial.print(" temperature: ");
  Serial.print(PressureSensor.temperature);
  Serial.println();
  delay(2000);
}
