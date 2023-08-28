#include "mbed.h"

// UART通信定義
Serial main_mcu(PC_6, PC_7);   // TX, RX

// 関数定義
void main_mcu_rx();

// ピン定義
DigitalOut led_1(PC_10);
DigitalOut led_2(PC_11);
DigitalOut led_3(PC_12);

AnalogIn encoder_1(PC_3);

uint8_t value, pre_value;
uint16_t m_count, pre_count;
uint8_t speed;

Timer test;

// グローバル変数定義

int main() {
      main_mcu.baud(9600);   // 通信速度: 9600, 14400, 19200, 28800, 38400, 57600, 115200
      // main_mcu.attach(main_mcu_rx, Serial::RxIrq);   // シリアル割り込み

      test.start();

      while (1) {
            pre_value = value;
            value = encoder_1.read_u16() / 256;
            if (value > 40 && pre_value <= 40) {
                  m_count += 1;
            }
            if (value < 40 && pre_value >= 40) {
                  m_count += 1;
            }
            if (m_count > 250) {
                  m_count = 0;
            }
            if (test.read() > 0.1) {
                  speed = m_count - pre_count;
                  pre_count = m_count;
                  test.reset();
            }
            main_mcu.putc(speed);
      }
}

void main_mcu_rx() {
}