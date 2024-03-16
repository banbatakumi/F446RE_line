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
}

void Line::Read() {
      // アナログ値の取得
      left_val = left.read_u16() / 256;
      right_val = right.read_u16() / 256;

      mux1a = 0;
      mux1b = 0;
      mux2a = 0;
      mux2b = 0;
      mux3a = 0;
      mux3b = 0;
      wait_us(2);
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
      wait_us(2);
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
      wait_us(2);
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
      wait_us(2);
      val[23] = mux1x.read_u16() / 256;
      val[17] = mux1y.read_u16() / 256;
      val[12] = mux2x.read_u16() / 256;
      val[9] = mux2y.read_u16() / 256;
      val[7] = mux3x.read_u16() / 256;
      val[1] = mux3y.read_u16() / 256;

      // それぞれのセンサが反応しているかの判定
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

void Line::Compute() {
      // 反応してるライン
      white_qty = 0;
      for (uint8_t i = 0; i < LINE_QTY; i++) {
            white_qty += is_white[i];
      }

      if (white_qty != 0) {
            // ラインの間隔
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

            // ラインの最大の間隔
            max_interval = 0;
            uint8_t pos_max_interval = 0;
            for (uint8_t i = 0; i < white_qty; i++) {
                  if (max_interval < interval[i]) {
                        max_interval = interval[i];
                        pos_max_interval = i;
                  }
            }

            // ラインの方向
            float line_dir_num = pos_white_num[pos_max_interval] + max_interval * 0.5;
            dir = SimplifyDeg((line_dir_num / LINE_QTY * 360) + 180);

            if (max_interval > 12) max_interval = 24 - max_interval;  // 間隔を１２までにする

            // ラインから戻る方向
            if (pre_white_qty != 0) {
                  int16_t dir_difference = abs(dir - pre_dir);
                  if (dir_difference > 180) dir_difference = 360 - dir_difference;
                  if (dir_difference > 120) is_half_out = 1 - is_half_out;
            }

            pre_dir = dir;
            pre_white_qty = white_qty;

            if (is_half_out) {
                  inside_dir = dir;
            } else {
                  inside_dir = SimplifyDeg(dir + 180);
            }
      } else {
            is_half_out = 0;
            max_interval = 0;
            dir = 0;
            inside_dir = 180;

            pre_dir = 0;
            pre_white_qty = 0;
      }
}

bool Line::IsLeft() {
      return is_left_white;
}

bool Line::IsRight() {
      return is_right_white;
}

bool Line::IsOnLine() {
      bool is_on_line = 0;
      if (white_qty > 0) is_on_line = 1;
      return is_on_line;
}