#include "encoder.h"
#include "line.h"
#include "mbed.h"

// UART通信定義
RawSerial mainSerial(PC_6, PC_7);   // TX, RX

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
            encoder.read();

            MainMcu();
      }
}

void MainMcu() {
      // 受信
      const uint8_t send_byte_num = 14;
      uint8_t send_byte[send_byte_num];
      send_byte[0] = 0xFF;
      send_byte[1] = encoder.get(0);
      send_byte[2] = encoder.get(1);
      send_byte[3] = encoder.get(2);
      send_byte[4] = encoder.get(3);
      send_byte[5] = line.Interval();
      send_byte[6] = line.WhiteQTY();
      send_byte[7] = line.IsLeft();
      send_byte[8] = line.IsRight();
      send_byte[9] = line.Vector() > 0 ? line.Vector() : 0;
      send_byte[10] = line.Vector() < 0 ? line.Vector() * -1 : 0;
      send_byte[11] = line.InsideDir() > 0 ? line.InsideDir() : 0;
      send_byte[12] = line.InsideDir() < 0 ? line.InsideDir() * -1 : 0;
      send_byte[13] = 0xAA;

      for (uint8_t i = 0; i < send_byte_num; i++) {
            mainSerial.putc(send_byte[i]);
      }

      // 送信
      if (mainSerial.readable()) do_led_on = mainSerial.getc();
}