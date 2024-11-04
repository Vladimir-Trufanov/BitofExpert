/** Arduino-Esp32-CAM               *** ex3-0-6_3_SanityCheckSingleLoop.ino ***
 * 
 *                                     Проверка работоспособности в одном цикле
 *         для плат ESP32 в версии библиотеки от Espressif Systems версии 3.0.6
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 *                                        
 *  В скетче в основном цикле выполняются три деятельности. Каждая деятельность 
 *  после своей работы устанавливает соответствующий флаг. В к
 * 
 * v1.1, 04.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 04.11.2024
 * 
**/

#include <esp_task_wdt.h>
#define WDT_TIMEOUT 4    // тайм-аут в 4 секунды
esp_err_t ESP32_ERROR;   // возвращенное значение при инициализации TWDT

int flag[] = {0, 0, 0};

void setup() 
{
   Serial.begin(115200);
   // Отменяем подписку незанятых задач и деинициализируем сторожевой таймер
   esp_task_wdt_deinit();
   // Конфигурируем структуру таймера
   esp_task_wdt_config_t wdt_config = 
   {
      .timeout_ms = WDT_TIMEOUT * 1000,                 // длительность тайм-аута в мс
      .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,  // битовая маска для всех ядер
      .trigger_panic = true                             // включить перезагрузку ESP32
   };
   // Инициализируем сторожевой таймер контроля задач 
   ESP32_ERROR = esp_task_wdt_init(&wdt_config);
   // Подписываем текущую задачу под наблюдение TWDT
   esp_task_wdt_add(NULL);  
}

void loop() 
{
   Serial.println("Task 1");
   flag[0] = 1;
   delay(100);

   Serial.println("Task 2");
   flag[1] = 1;
   delay(100);

   Serial.println("Task 3");
   flag[2] = 1;
   delay(100);

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
   //else 
   //{
      // Check all flags if True then kick the dog
      if (flag[0] == 1 && flag[1] == 1 && flag[2] == 1) 
      {
         // Reset all flags
         flag[0] = 0;
         flag[1] = 0;
         flag[2] = 0;
         // Kick the dog
         esp_task_wdt_reset();
      }
   //}
}

// ************************************ ex3-0-6_3_SanityCheckSingleLoop.ino ***
