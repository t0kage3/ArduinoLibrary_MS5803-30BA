#ifndef MS580330BA_H
#define MS580330BA_H

#include <Arduino.h>

class MS580330BA{
  public:
    float temperature;
    float pressure;
    uint16_t c[6];
    uint32_t d1, d2;

    void begin(int pin);
    float read();

  private:
    int pin_ss;
};

#endif
