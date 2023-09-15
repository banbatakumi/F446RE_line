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
      main_mcu.baud(9600);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200
      // main_mcu.attach(main_mcu_rx, Serial::RxIrq);   // シリアル割り込み

      uart_timer.start();

      while (1) {
            line_left_val = line_left.read_u16() / 256;
            line_right_val = line_right.read_u16() / 256;
            Encoder.read();
            main_mcu.putc('H');
            main_mcu.putc(Encoder.sensor_1());
            main_mcu.putc(Encoder.sensor_2());
            main_mcu.putc(Encoder.sensor_3());
            main_mcu.putc(Encoder.sensor_4());
            main_mcu.putc(line_left_val);
            main_mcu.putc(line_right_val);
      }
}

void main_mcu_rx() {
}