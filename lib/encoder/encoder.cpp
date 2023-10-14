#include "encoder.h"

#include "mbed.h"

Encoder::Encoder(PinName pin_1_, PinName pin_2_, PinName pin_3_, PinName pin_4_) : pin_1(pin_1_), pin_2(pin_2_), pin_3(pin_3_), pin_4(pin_4_) {
      sampling_timer.start();
}

void Encoder::read() {
      uint8_t value[SENSOR_QTY];
      static uint8_t pre_value[SENSOR_QTY];
      static uint8_t count[SENSOR_QTY];

      value[0] = pin_1.read_u16() / 256;
      value[1] = pin_2.read_u16() / 256;
      value[2] = pin_3.read_u16() / 256;
      value[3] = pin_4.read_u16() / 256;

      for (uint8_t i = 0; i < SENSOR_QTY; i++) {
            if (value[i] > threshold[i] && pre_value[i] <= threshold[i]) {
                  count[i] += 1;
            }
            if (value[i] < threshold[i] && pre_value[i] >= threshold[i]) {
                  count[i] += 1;
            }
            pre_value[i] = value[i];
      }

      if (sampling_timer.read() > SAMPLE_CYCLE) {
            for (uint8_t i = 0; i < SENSOR_QTY; i++) {
                  speed[i] = count[i];
                  count[i] = 0;
            }
            sampling_timer.reset();
      }
}

uint8_t Encoder::get(uint8_t sensor_num) {
      return speed[sensor_num];
}

uint8_t Encoder::average() {
      uint speed_avg = 0;
      for (int i = 0; i < 4; i++) {
            speed_avg += speed[i] / 3;
      }
      return speed_avg;
}

void Encoder::reset_threshold(uint16_t reset_time) {
      return;
}