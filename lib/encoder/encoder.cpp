#include "encoder.h"

#include "mbed.h"

Encoder::Encoder(PinName pin_1_, PinName pin_2_, PinName pin_3_, PinName pin_4_) : pin_1(pin_1_), pin_2(pin_2_), pin_3(pin_3_), pin_4(pin_4_) {
      readTicker.attach(mbed::callback(this, &Encoder::GetVal), SAMPLE_CYCLE);
}

void Encoder::GetVal() {
      for (uint8_t i = 0; i < SENSOR_QTY; i++) {
            speed[i] = count[i];
            if (speed[i] > MAX_SPEED) speed[i] = MAX_SPEED;
            count[i] = 0;
      }
}

void Encoder::Read() {
      uint8_t val[SENSOR_QTY];
      static uint8_t pre_val[SENSOR_QTY];

      val[0] = pin_1.read_u16() / 256;
      val[1] = pin_2.read_u16() / 256;
      val[2] = pin_3.read_u16() / 256;
      val[3] = pin_4.read_u16() / 256;

      for (uint8_t i = 0; i < SENSOR_QTY; i++) {
            if (max_val[i] < val[i]) max_val[i] = val[i];
            if (min_val[i] > val[i]) min_val[i] = val[i];
            threshold[i] = (max_val[i] + min_val[i]) / 2;
      }

      for (uint8_t i = 0; i < SENSOR_QTY; i++) {
            if (val[i] > threshold[i] && pre_val[i] <= threshold[i]) {
                  count[i] += 1;
            }
            if (val[i] < threshold[i] && pre_val[i] >= threshold[i]) {
                  count[i] += 1;
            }
            pre_val[i] = val[i];
      }
}

uint8_t Encoder::GetSpeed(uint8_t sensor_num) {
      return speed[sensor_num];
}