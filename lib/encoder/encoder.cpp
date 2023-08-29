#include "encoder.h"

#include "mbed.h"

encoder::encoder(PinName pin_1_, PinName pin_2_, PinName pin_3_, PinName pin_4_) : pin_1(pin_1_), pin_2(pin_2_), pin_3(pin_3_), pin_4(pin_4_) {
      sampling_timer.start();
}

void encoder::read() {
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
                  count[i] = 0;
            }
            sampling_timer.reset();
      }
}

uint8_t encoder::sensor_1() {
      return count[0];
}

uint8_t encoder::sensor_2() {
      return count[1];
}

uint8_t encoder::sensor_3() {
      return count[2];
}

uint8_t encoder::sensor_4() {
      return count[3];
}