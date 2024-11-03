/** Arduino-Esp32-CAM                             *** ex5-1-33_GreatWDT.ino ***
 * 
 *              -------Пример прерывания и управления программным сторожевым таймером.
 *                          
 *    В скетче задается время сторожевому таймеру для перезагрузки контроллера, 
 *   равное 3 секундам. Но пока контакт IO0 не замкнут на GND, скетч работает 1
 *            секунду и сбрасывает сторожевой таймер, предотвращая перезагрузку 
 *          контроллера. После замыкания контакта скетч переходит в бесконечный 
 *    цикл, что вызывает прерывание через 3 секунды и перезагрузку контроллера.
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.1, 03.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 03.11.2024
**/

#if CONFIG_FREERTOS_UNICORE
   #define ARDUINO_RUNNING_CORE 0
#else
   #define ARDUINO_RUNNING_CORE 1
#endif

// Определяем заголовок для объекта таймера
hw_timer_t *timer = NULL;
// Инициируем спинлок критической секции в обработчике таймерного прерывания
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile int inumber;


void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask4(void *pvParameters);

int flag[] = {0, 0, 0, 0};

void IRAM_ATTR onTimer() 
{
   portENTER_CRITICAL_ISR(&timerMux);
   // Если флаги всех задач установлены в 1, 
   // то сбрасываем флаги задач и счетчик сторожевого таймера
   if (flag[0] == 1 && flag[1] == 1 && flag[2] == 1 && flag[3] == 1) 
   {
      // Сбрасываем флаги задач
      flag[0] = 0;
      flag[1] = 0;
      flag[2] = 0;
      flag[3] = 0;
      // "Пинаем собаку" - сбрасываем счетчик сторожевого таймера
      timerWrite(timer, 0);
   }
   // Иначе перезагружаем контроллер
   else 
   {
      ESP.restart();
   }
   portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() 
{
   Serial.begin(115200);

   xTaskCreatePinnedToCore(
      vTask1,                 // Task function
      "Task1",                // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      1,                      // Priority
      NULL,                   // Task handle
      0); //ARDUINO_RUNNING_CORE);

   xTaskCreatePinnedToCore(
      vTask2,                 // Task function
      "Task2",                // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      1,                      // Priority
      NULL,                   // Task handle
      0); //ARDUINO_RUNNING_CORE);

   xTaskCreatePinnedToCore(
      vTask3,                 // Task function
      "Task3",                // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      2,                      // Priority
      NULL,                   // Task handle
      1); //ARDUINO_RUNNING_CORE);

   xTaskCreatePinnedToCore(
      vTask4,                 // Task function
      "Task4",                // Task name
      1024,                   // Stack size
      NULL,                   // Parameters passed to the task function
      3,                      // Priority
      NULL,                   // Task handle
      1); //ARDUINO_RUNNING_CORE);

   // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
   timer = timerBegin(1000000);
   // Подключаем функцию обработчика прерывания от таймера - onTimer
   timerAttachInterrupt(timer, &onTimer);
   // Настраиваем таймер: интервал перезапуска - 3 секунды (3000000 микросекунд),
   // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
   // раз (четвертый параметр = 0) 
   timerAlarm(timer, 3000000, true, 0);
}

void loop() 
{
   if (Serial.available() > 0) 
   {
      int ii=Serial.parseInt();
      if (ii>0) inumber=ii;
      //inumber=Serial.parseInt();
      delay(100);
   }
}

void vTask1(void* pvParameters) 
{
   for (;;)
   {
      Serial.println("Task1");
      // В обычном режиме делаем паузу 500 мсек и выставляем бит задачи в 1
      vTaskDelay(500/portTICK_PERIOD_MS);
      flag[0] = 1;
      // Имитируем зависание микроконтроллера с помощью последовательного порта
      if (inumber == 1) 
      {
         while (true) 
         {
            Serial.println("MCU-Task1 hang event!!!");
         }
      }

      /*
      if (Serial.available() > 0) 
      {
         if (Serial.parseInt() == 1) 
         {
            while (true) 
            {
               Serial.println("MCU-Task1 hang event!!!");
            }
         }
      }
      */
  }
}

void vTask2(void* pvParameters) 
{
   for ( ;; )
   {
      Serial.println("Task2");
      /*
      // Mimic the MCU hang event using Serial port
      if (Serial.available() > 0) 
      {
         if (Serial.parseInt() == 2) 
         {
            while (true) 
            {
               Serial.println("MCU-Task2 hang event!!!");
            }
         }
      }
      */
      flag[1] = 1;
      vTaskDelay(500/portTICK_PERIOD_MS);
   }
}

void vTask3(void* pvParameters) 
{
   for ( ;; )
   {
      Serial.println("Task3");
      /*
      // Mimic the MCU hang event using Serial port
      if (Serial.available() > 0) 
      {
         if (Serial.parseInt() == 1) 
         {
            while (true) 
            {
               Serial.println("MCU hang event!!!");
           }
         }
      }
      */
      flag[2] = 1;
      vTaskDelay(500/portTICK_PERIOD_MS);
   }
}

void vTask4(void* pvParameters) 
{
   for ( ;; )
   {
      Serial.println("Task4");
      /*
      // Mimic the MCU hang event using Serial port
      if (Serial.available() > 0) 
      {
         if (Serial.parseInt() == 1) 
         {
            while (true) 
            {
               Serial.println("MCU hang event!!!");
           }
         }
      }
      */
      flag[3] = 1;
      vTaskDelay(500/portTICK_PERIOD_MS);
   }
}

// ************************************************** ex5-1-33_GreatWDT.ino ***
