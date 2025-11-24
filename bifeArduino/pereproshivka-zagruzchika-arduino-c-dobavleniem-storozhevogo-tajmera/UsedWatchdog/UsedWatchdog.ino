/** Arduino UNO, SIM900 ********************************** UsedWatchdog.ino ***
 * 
 * Познакомиться с вариантом использования Watchdog на Arduino Uno
 * (по материалам https://alexgyver.ru/lessons/arduino-watchdog/)
 * 
 * v1.0.0, 24.11.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 24.11.2025
 *
**/

#include "GyverWDT.h"

/*
   Пример использования watchdog в комбинированном режиме
   Может использоваться для оповещения в случае зависания
   Первый тайм-аут вызывает прерывание, второе - сброс
   Перенастройте watchdog чтобы избежать сброса
   Зависимость таймаутов от делителей см. в GyverWDT.h
*/

void setup() 
{
  Serial.begin(9600);
  Serial.println("Program started");
  Watchdog.enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_1024);   // запустили watchdog с таймаутом ~8c
  Serial.println("watchdog enabled");
  // while (1);                                                // создали причину зависания в Setup
  Serial.println("loop started");                              // при зависании не дойдём
}

void loop() 
{
  // Этого кода программа не достигнет при зависании в setup 
  Serial.println("hello");
  delay(500);
}

// Первый тайм-аут вызовет прерывание и если Watchdog не будет перезапущен,
// то на втором прерывании произойдет жёсткая перезагрузка контроллера
ISR(WATCHDOG) 
{
  // Если причина зависания программная - тут можно попытаться исправить ее 
  Serial.println("warning!");
  // Если исправить причину не вышло - следующий таймаут вызывает сброс
  // Если перенастроить watchdog здесь - сброса не будет
  // Watchdog.enable(INTERRUPT_RESET_MODE, WDT_PRESCALER_1024); 
}

// ******************************************************* UsedWatchdog.ino ***

