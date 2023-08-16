#include "mbed.h"

// UART通信定義
Serial main_mcu(PC_6, PC_7);   // TX, RX

// 関数定義
void main_mcu_rx();

// ピン定義
DigitalOut led_1(PC_10);
DigitalOut led_2(PC_11);
DigitalOut led_3(PC_12);

// グローバル変数定義

int main() {
      main_mcu.baud(38400);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200
      main_mcu.attach(main_mcu_rx, Serial::RxIrq);   // シリアル割り込み

      while (1) {
      }
}

void main_mcu_rx() {
}