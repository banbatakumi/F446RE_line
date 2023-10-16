#ifndef MBED_ENCODER_H
#define MBED_ENCODER_H

#include "mbed.h"

#define SENSOR_QTY 4
#define SAMPLE_CYCLE 0.1

class Encoder {
     public:
      Encoder(PinName pin_1_, PinName pin_2_, PinName pin_3_, PinName pin_4_);
      void read();
      uint8_t get(uint8_t sensor_num);
      uint8_t average();
      void reset_threshold(uint16_t reset_time = 1000);

     private:
      AnalogIn pin_1;
      AnalogIn pin_2;
      AnalogIn pin_3;
      AnalogIn pin_4;

      uint8_t speed[SENSOR_QTY];
      uint8_t threshold[SENSOR_QTY] = {15, 25, 15, 15};

      Timer sampling_timer;
};

#endif