#include "encoder.h"
#include "line.h"
#include "mbed.h"

// UART通信定義
UnbufferedSerial mainSerial(PC_6, PC_7);   // TX, RX

// 関数定義
void MainMcu();

Encoder encoder(PC_4, PA_7, PC_3, PA_6);

Line line(PC_2, PB_1,
          PB_7, PB_9, PC_0, PC_1,
          PA_4, PA_5, PA_2, PA_3,
          PB_12, PB_13, PC_5, PB_0);

// ピン定義
DigitalOut led[3] = {PC_10, PC_11, PC_12};
DigitalOut lineLed(PC_8);

// グローバル変数定義
uint8_t do_led_on = 0;

void setup() {
      mainSerial.baud(230400);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200

      lineLed = 0;
      wait_us(10000);
      line.SetTh();
      lineLed = 1;
      wait_us(10000);
}

int main() {
      setup();
      while (1) {
            if (do_led_on != 0) {
                  lineLed = 0;
            } else {
                  lineLed = 1;
            }

            line.Read();
            encoder.Read();

            line.Compute();
            MainMcu();
      }
}

void MainMcu() {
      // 送信
      const uint8_t send_byte_num = 8;
      uint8_t send_byte[send_byte_num];
      send_byte[0] = 0xFF;
      send_byte[1] = encoder.GetSpeed(0) << 4 | encoder.GetSpeed(1);
      send_byte[2] = encoder.GetSpeed(2) << 4 | encoder.GetSpeed(3);
      send_byte[3] = line.max_interval;
      send_byte[4] = line.IsOnLine() << 2 | line.IsLeft() << 1 | line.IsRight();
      send_byte[5] = line.dir / 2 + 90;
      send_byte[6] = line.inside_dir / 2 + 90;
      send_byte[7] = 0xAA;
      mainSerial.write(&send_byte, send_byte_num);

      // 受信
      mainSerial.read(&do_led_on, 1);
}