#ifndef MBED_ENCODER_H
#define MBED_ENCODER_H

#include "mbed.h"

#define SENSOR_QTY 4
#define SAMPLE_CYCLE 0.05
#define MAX_SPEED 15
class Encoder {
     public:
      Encoder(PinName pin_1_, PinName pin_2_, PinName pin_3_, PinName pin_4_);
      void Read();
      uint8_t GetSpeed(uint8_t sensor_num);
      void GetVal();

     private:
      AnalogIn pin_1;
      AnalogIn pin_2;
      AnalogIn pin_3;
      AnalogIn pin_4;

      uint8_t speed[SENSOR_QTY];
      uint8_t threshold[SENSOR_QTY];
      uint8_t max_val[SENSOR_QTY];
      uint8_t count[SENSOR_QTY];

      Ticker readTicker;
};

#endif