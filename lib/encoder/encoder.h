#ifndef MBED_ENCODER_H
#define MBED_ENCODER_H

#include "mbed.h"

#define SENSOR_QTY 4
#define SAMPLE_CYCLE 0.1

class encoder {
     public:
      encoder(PinName pin_1_, PinName pin_2_, PinName pin_3_, PinName pin_4_);
      void read();
      uint8_t sensor_1();
      uint8_t sensor_2();
      uint8_t sensor_3();
      uint8_t sensor_4();
      void reset_threshold(uint16_t reset_time = 1000);

     private:
      AnalogIn pin_1;
      AnalogIn pin_2;
      AnalogIn pin_3;
      AnalogIn pin_4;

      uint8_t count[SENSOR_QTY];
      uint8_t value[SENSOR_QTY];
      uint8_t pre_value[SENSOR_QTY];
      uint8_t threshold[SENSOR_QTY] = {40, 40, 40, 40};

      Timer sampling_timer;
};

#endif