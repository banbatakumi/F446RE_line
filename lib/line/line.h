#ifndef MBED_LINE_H
#define MBED_LINE_H

#include "mbed.h"
#include "simplify_deg.h"

#define PI 3.1415926535  // 円周率

#define LINE_QTY 24
#define SET_TH_NUM 5000
#define TH_VALUE 500

#define readms(timer_name_) chrono::duration_cast<chrono::milliseconds>((timer_name_).elapsed_time()).count()

class Line {
     public:
      Line(PinName left_, PinName right_,
           PinName mux1a_, PinName mux1b_, PinName mux1x_, PinName mux1y_,
           PinName mux2a_, PinName mux2b_, PinName mux2x_, PinName mux2y_,
           PinName mux3a_, PinName mux3b_, PinName mux3x_, PinName mux3y_);

      void Read();
      void Compute();
      void SetTh();
      bool IsLeft();
      bool IsRight();
      bool IsOnLine();

      bool is_white[LINE_QTY];

      uint8_t white_qty;
      uint8_t max_interval;
      int16_t dir;
      int16_t inside_dir;

     private:
      AnalogIn left;
      AnalogIn right;
      DigitalOut mux1a;
      DigitalOut mux1b;
      AnalogIn mux1x;
      AnalogIn mux1y;
      DigitalOut mux2a;
      DigitalOut mux2b;
      AnalogIn mux2x;
      AnalogIn mux2y;
      DigitalOut mux3a;
      DigitalOut mux3b;
      AnalogIn mux3x;
      AnalogIn mux3y;

      uint8_t read_sensors_group = 0;

      uint16_t val[LINE_QTY];
      uint32_t th_val[LINE_QTY];

      uint16_t left_val;
      uint16_t right_val;
      uint32_t left_th_val;
      uint32_t right_th_val;
      bool is_left_white;
      bool is_right_white;

      bool is_half_out;
      int16_t pre_dir;
      bool pre_white_qty;
};

#endif