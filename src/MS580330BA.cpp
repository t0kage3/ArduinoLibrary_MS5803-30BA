#include "MS580330BA.h"

#include <SPI.h>

void MS580330BA::begin(int pin){
    pin_ss = pin;
    digitalWrite(pin_ss, HIGH);
    pinMode(pin_ss, OUTPUT);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.setDataMode(SPI_MODE0);

    // PROMから校正データ読み込み
    for (int i = 0; i < 6; i++){
        digitalWrite(pin_ss, LOW);
        SPI.transfer(0xA0 | (i * 2 + 2));
        uint8_t h1,h2;
        h1 = SPI.transfer(0x00);
        h2 = SPI.transfer(0x00);
        c[i] = h1 << 8 | h2;
        digitalWrite(pin_ss, HIGH);
    }
}

float MS580330BA::read(){
    uint8_t h1, h2, h3;

    // 圧力のADC要求と読み込み
    digitalWrite(pin_ss, LOW);
    SPI.transfer(0x48);
    delay(10);
    digitalWrite(pin_ss, HIGH);
    delay(10);
    digitalWrite(pin_ss, LOW);
    SPI.transfer(0x00);
    h1 = SPI.transfer(0x00);
    h2 = SPI.transfer(0x00);
    h3 = SPI.transfer(0x00);
    d1 = (uint32_t)h1 << 16 | (uint32_t)h2 << 8 | (uint32_t)h3;
    digitalWrite(pin_ss, HIGH);

    // 温度のADC要求と読み込み
    digitalWrite(pin_ss, LOW);
    SPI.transfer(0x58);
    delay(10);
    digitalWrite(pin_ss, HIGH);
    delay(10);
    digitalWrite(pin_ss, LOW);
    SPI.transfer(0x00);
    h1 = SPI.transfer(0x00);
    h2 = SPI.transfer(0x00);
    h3 = SPI.transfer(0x00);
    d2 = (uint32_t)h1 << 16 | (uint32_t)h2 << 8 | (uint32_t)h3;
    digitalWrite(pin_ss, HIGH);

    // 温度の補正と結果出力
    int32_t dt;
    int32_t temperature_int32;
    dt = (int32_t)d2 - (int32_t)((uint32_t)c[4] << 8);
    temperature_int32 = 2000 + (int32_t)(((int64_t)dt * (int64_t)c[5]) >> 23);
    temperature = (float)temperature_int32 / 100.0;

    // 圧力の補正と結果出力
    int32_t pressure_int32;
    uint64_t off, sens;
    off = ((int64_t)c[1] << 16) + (((int64_t)c[3] * dt) >> 7);
    sens = ((int64_t)c[0] << 15) + (((int64_t)c[2] * dt) >> 8);
    pressure_int32 = (((d1 * sens) >> 21) - off) >> 13;
    pressure = (float)pressure_int32 / 10.0;

    return pressure;
}
