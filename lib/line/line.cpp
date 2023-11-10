#include "line.h"

#include "mbed.h"
Line::Line(PinName left_, PinName right_, PinName mux1a_, PinName mux1b_, PinName mux1x_, PinName mux1y_, PinName mux2a_, PinName mux2b_, PinName mux2x_, PinName mux2y_, PinName mux3a_, PinName mux3b_, PinName mux3x_, PinName mux3y_)
    : left(left_), right(right_), mux1a(mux1a_), mux1b(mux1b_), mux1x(mux1x_), mux1y(mux1y_), mux2a(mux2a_), mux2b(mux2b_), mux2x(mux2x_), mux2y(mux2y_), mux3a(mux3a_), mux3b(mux3b_), mux3x(mux3x_), mux3y(mux3y_) {
      mux1a = 0;
      mux1b = 0;
      mux2a = 0;
      mux2b = 0;
      mux3a = 0;
      mux3b = 0;

      for (uint8_t i = 0; i < LINE_QTY; i++) {   // それぞれのセンサにベクトルを与える
            unit_vector_x[i] = MyCos(i * 360.00000 / LINE_QTY);
            unit_vector_y[i] = MySin(i * 360.00000 / LINE_QTY);
      }

      dirDifferenceTimer.start();
}

void Line::Read() {
      left_val = left.read_u16() / 256;
      right_val = right.read_u16() / 256;

      mux1a = 0;
      mux1b = 0;
      mux2a = 0;
      mux2b = 0;
      mux3a = 0;
      mux3b = 0;
      wait_us(5);
      val[22] = mux1x.read_u16() / 256;
      val[19] = mux1y.read_u16() / 256;
      val[15] = mux2x.read_u16() / 256;
      val[11] = mux2y.read_u16() / 256;
      val[2] = mux3x.read_u16() / 256;
      val[5] = mux3y.read_u16() / 256;

      mux1a = 1;
      mux1b = 0;
      mux2a = 1;
      mux2b = 0;
      mux3a = 1;
      mux3b = 0;
      wait_us(5);
      val[21] = mux1x.read_u16() / 256;
      val[16] = mux1y.read_u16() / 256;
      val[14] = mux2x.read_u16() / 256;
      val[8] = mux2y.read_u16() / 256;
      val[3] = mux3x.read_u16() / 256;
      val[0] = mux3y.read_u16() / 256;

      mux1a = 0;
      mux1b = 1;
      mux2a = 0;
      mux2b = 1;
      mux3a = 0;
      mux3b = 1;
      wait_us(5);
      val[20] = mux1x.read_u16() / 256;
      val[18] = mux1y.read_u16() / 256;
      val[13] = mux2x.read_u16() / 256;
      val[10] = mux2y.read_u16() / 256;
      val[6] = mux3x.read_u16() / 256;
      val[4] = mux3y.read_u16() / 256;

      mux1a = 1;
      mux1b = 1;
      mux2a = 1;
      mux2b = 1;
      mux3a = 1;
      mux3b = 1;
      wait_us(5);
      val[23] = mux1x.read_u16() / 256;
      val[17] = mux1y.read_u16() / 256;
      val[12] = mux2x.read_u16() / 256;
      val[9] = mux2y.read_u16() / 256;
      val[7] = mux3x.read_u16() / 256;
      val[1] = mux3y.read_u16() / 256;

      if (left_val > left_th_val) {
            is_left_white = 1;
      } else {
            is_left_white = 0;
      }
      if (right_val > right_th_val) {
            is_right_white = 1;
      } else {
            is_right_white = 0;
      }
      for (uint16_t i = 0; i < LINE_QTY; i++) {
            if (val[i] > th_val[i]) {
                  is_white[i] = 1;
            } else {
                  is_white[i] = 0;
            }
      }
}

void Line::SetTh() {
      left_th_val = 0;
      right_th_val = 0;
      for (uint16_t i = 0; i < LINE_QTY; i++) {
            th_val[i] = 0;
      }

      for (uint16_t i = 0; i < SET_TH_NUM; i++) {
            Read();
            left_th_val += left_val;
            right_th_val += right_val;

            for (uint16_t j = 0; j < LINE_QTY; j++) {
                  th_val[j] += val[j];
            }
      }

      left_th_val /= SET_TH_NUM;
      right_th_val /= SET_TH_NUM;
      left_th_val += TH_VALUE;
      right_th_val += TH_VALUE;
      for (uint16_t i = 0; i < LINE_QTY; i++) {
            th_val[i] /= SET_TH_NUM;
            th_val[i] += TH_VALUE;
      }
}

int16_t Line::Vector() {
      float result_vector_x = 0;
      float result_vector_y = 0;
      int16_t degree;

      for (uint8_t i = 0; i < LINE_QTY; i++) {   // 全てのセンサのベクトルを合成
            result_vector_x += is_white[i] * unit_vector_x[i];
            result_vector_y += is_white[i] * unit_vector_y[i];
      }

      degree = atan2(result_vector_y, result_vector_x) * 180.00 / PI;
      return degree;
}

uint8_t Line::Interval() {
      uint8_t white_qty = WhiteQTY();
      uint8_t pos_white_num[white_qty];
      uint8_t white_num = 0;

      for (uint8_t i = 0; i < LINE_QTY; i++) {
            if (is_white[i]) {
                  pos_white_num[white_num] = i;
                  white_num++;
            }
      }

      uint8_t interval[white_qty];
      for (uint8_t i = 0; i < white_qty - 1; i++) {
            interval[i] = pos_white_num[i + 1] - pos_white_num[i];
      }
      interval[white_qty - 1] = pos_white_num[0] - pos_white_num[white_qty - 1] + LINE_QTY;

      uint8_t max_interval = 0;
      for (uint8_t i = 0; i < white_qty; i++) {
            if (interval[i] > 12) interval[i] = 24 - interval[i];
            if (max_interval < interval[i]) {
                  max_interval = interval[i];
            }
      }

      return max_interval;
}

int16_t Line::InsideDir() {
      int16_t line_dir = Vector();
      bool is_on_white = IsOnWhite();
      int16_t inside_dir;
      static int16_t pre_line_dir;
      static bool pre_is_on_white;
      static bool is_half_out;

      if (dirDifferenceTimer.read_ms() > 50) {
            if (pre_is_on_white == 1) {
                  int16_t dir_difference = abs(line_dir - pre_line_dir);
                  if (dir_difference > 180) dir_difference = 360 - dir_difference;
                  if (dir_difference > 100) is_half_out = 1 - is_half_out;
            }

            if (is_on_white == 0) is_half_out = 0;

            pre_line_dir = line_dir;
            pre_is_on_white = is_on_white;
            dirDifferenceTimer.reset();
      }


      if (is_half_out) {
            inside_dir = line_dir;
      } else {
            inside_dir = SimplifyDeg(line_dir + 180);
      }


      return SimplifyDeg(inside_dir);
}

uint8_t Line::WhiteQTY() {
      uint8_t white_qty = 0;
      for (uint8_t i = 0; i < LINE_QTY; i++) {   // 全てのセンサのベクトルを合成
            white_qty += is_white[i];
      }
      return white_qty;
}

bool Line::IsOnWhite() {
      static bool is_on_white;

      if (WhiteQTY() > 0) {
            whiteOnTimer.start();
            whiteOnTimer.reset();
            is_on_white = 1;
      }
      if (whiteOnTimer.read_ms() > 100) {
            whiteOnTimer.stop();
            whiteOnTimer.reset();
            is_on_white = 0;
      }
      return is_on_white;
}

uint8_t Line::IsLeft() {
      return is_left_white;
}

uint8_t Line::IsRight() {
      return is_right_white;
}