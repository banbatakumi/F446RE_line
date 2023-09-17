#include "encoder.h"
#include "mbed.h"

// UART通信定義
RawSerial main_mcu(PC_6, PC_7);   // TX, RX

// 関数定義
void main_mcu_rx();
encoder Encoder(PC_4, PA_7, PC_3, PA_6);

// ピン定義
DigitalOut led_1(PC_10);
DigitalOut led_2(PC_11);
DigitalOut led_3(PC_12);
AnalogIn line_left(PC_2);
AnalogIn line_right(PB_1);

// グローバル変数定義
uint8_t line_left_val;
uint8_t line_right_val;

Timer uart_timer;

int main() {
      main_mcu.baud(57600);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200
      // main_mcu.attach(main_mcu_rx, Serial::RxIrq);   // シリアル割り込み

      uart_timer.start();

      while (1) {
            line_left_val = line_left.read_u16() / 256;
            line_right_val = line_right.read_u16() / 256;
            Encoder.read();

            uint8_t send_byte_num = 5;
            uint8_t send_byte[send_byte_num];
            send_byte[0] = 0xFF;
            send_byte[1] = Encoder.average();
            send_byte[2] = line_left_val;
            send_byte[3] = line_right_val;
            send_byte[4] = 0xAA;

            for (uint8_t i = 0; i < send_byte_num; i++) {
                  main_mcu.putc(send_byte[i]);
            }
      }
}

void main_mcu_rx() {
}