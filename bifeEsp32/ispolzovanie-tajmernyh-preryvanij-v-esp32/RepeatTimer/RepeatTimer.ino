/** Arduino-Esp32-CAM                                   *** RepeatTimer.ino ***
 * 
 *                                        Пример прерывания от таймера повтора.
 *                                  
 *            В скетче запускается таймерное прерывание каждую секунду, которое 
 *    освобождает семафор для основного цикла. В основном цикле при обнаружении 
 *      свободного семафора в последовательный порт выводится значение счётчика 
 *                                    прерываний и время очередного прерывания.  
 *                 Работу таймера можно остановить замкнув контакт IO0 на землю
 *                                       (на контроллере AI-Thinker ESP32-CAM).
 * 
 * v1.0, 02.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 02.11.2024
**/

// Назначаем 0 пин на остановку таймера
#define BTN_STOP_ALARM 0
// Определяем заголовок для объекта таймера
hw_timer_t *timer = NULL;
// Определяем заголовок семафора, который будет указывать на срабатывание таймера
volatile SemaphoreHandle_t timerSemaphore;
// Инициируем спинлок критической секции в обработчике таймерного прерывания
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// Определяем защищенные переменные: счетчик прерываний и время последнего
volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

void ARDUINO_ISR_ATTR onTimer() 
{
   // Инкрементируем счетчик прерываний и фиксируем время текущего прерывания
   portENTER_CRITICAL_ISR(&timerMux);
   isrCounter = isrCounter + 1;
   lastIsrAt = millis();
   portEXIT_CRITICAL_ISR(&timerMux);
   // Освобождаем семафора, который будем проверять в основном цикле
   xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

void setup() 
{
   Serial.begin(115200);
   // Переводим нулевой пин в режим ввода
   pinMode(BTN_STOP_ALARM, INPUT);
   // Создаём бинарный семафор, сообщающий о срабатывании таймера
   timerSemaphore = xSemaphoreCreateBinary();
   // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
   timer = timerBegin(1000000);
   // Подключаем функцию обработчика прерывания от таймера - onTimer
   timerAttachInterrupt(timer, &onTimer);
   // Настраиваем таймер: интервал перезапуска - 1 секунда (1000000 микросекунд),
   // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
   // раз (четвертый параметр = 0) 
   timerAlarm(timer, 1000000, true, 0);
}

void loop() 
{
   // Если семафор свободен, выполняем обработку ситуации
   // (после завершения обработки семафор будет снова занят)
   if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) 
   {
      uint32_t isrCount = 0, isrTime = 0;
      // Выбираем "текущие" значения счетчика прерываний и времени прерывания
      portENTER_CRITICAL(&timerMux);
      isrCount = isrCounter;
      isrTime = lastIsrAt;
      portEXIT_CRITICAL(&timerMux);
      // Распечатываем
      Serial.print("Счетчик прерываний: ");
      Serial.print(isrCount);
      Serial.print(" Время: ");
      Serial.print(isrTime);
      Serial.println(" ms");
   }
   // Если IO0 замкнут GND, то останавливаем таймер
   if (digitalRead(BTN_STOP_ALARM) == LOW) 
   {
      if (timer) 
      {
         timerEnd(timer);
         timer = NULL;
      }
   }
}

// ******************************************************** RepeatTimer.ino ***
