#include "encoder.h"
#include "mbed.h"

// UART通信定義
Serial main_mcu(PC_6, PC_7);   // TX, RX

// 関数定義
void main_mcu_rx();
encoder Encoder(PC_4, PA_7, PC_3, PA_6);

// ピン定義
DigitalOut led_1(PC_10);
DigitalOut led_2(PC_11);
DigitalOut led_3(PC_12);

// グローバル変数定義

Timer uart_timer;

int main() {
      main_mcu.baud(38400);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200
      // main_mcu.attach(main_mcu_rx, Serial::RxIrq);   // シリアル割り込み

      uart_timer.start();

      while (1) {
            Encoder.read();
            main_mcu.putc('H');
            main_mcu.putc(Encoder.sensor_1());
            main_mcu.putc(Encoder.sensor_2());
            main_mcu.putc(Encoder.sensor_3());
            main_mcu.putc(Encoder.sensor_4());
      }
}

void main_mcu_rx() {
}