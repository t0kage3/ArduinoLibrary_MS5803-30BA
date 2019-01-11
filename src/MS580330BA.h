#ifndef MS580330BA_H
#define MS580330BA_H

#include <Arduino.h>

class MS580330BA{
    public:
      float temperature;
      float pressure;

      void begin(int pin);
      float read();

    private:
      uint16_t c[6];
      int pin_ss;
};

#endif
