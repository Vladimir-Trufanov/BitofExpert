/** Arduino-Esp32-CAM                                  *** nvs-rw-value.ino ***
 * 
 *                Сохранить в NVS количество перезапусков модуля ESP32, которое
 *     увеличивается при каждом запуске (поскольку значение записывается в NVS, 
 *                                        оно сохраняется между перезапусками).
 *                 Также проверить, была ли операция чтения/записи успешной или 
 *            определённое значение не было инициализировано в NVS. Диагностику 
 *         представить в виде обычного текста, чтобы можно было отслеживать ход 
 *                               выполнения программы и выявлять любые проблемы
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.1, 21.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 21.10.2024
 * 
**/

#include <Arduino.h>
#include "nvs_flash.h"
#include "nvs.h"

char buffer[60];

void setup() 
{
   Serial.begin(115200);
   // --------------------------------------------------- Инициализация NVS ---
   esp_err_t err = nvs_flash_init();
   // Если раздел NVS не содержит пустых страниц или он содержит данные в 
   // незнакомом формате, который не распознаётся текущей версией кода,
   // то стираем весь раздел и снова вызываем инициализацию
   if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
   {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
   }
   ESP_ERROR_CHECK(err);
   // Открываем хранилище параметров
   Serial.print("\n");
   Serial.print("Открываем энергонезависимое хранилище (NVS) ... ");
   nvs_handle_t my_handle;
   err = nvs_open("storage", NVS_READWRITE, &my_handle);
   if (err != ESP_OK) 
   {
      sprintf(buffer,"Ошибка (%s) открытия хранилища!\n", esp_err_to_name(err));
      Serial.print(buffer);
   } 
   else 
   {
      Serial.println("Сделано!");
      // ----------------------------------------------------------- Чтение ---
      Serial.print("Считываем значение счетчика перезапусков из NVS ... ");
      // Присваиваем начальное значение счетчику = 0, 
      // на случай, если значение счетчика еще не было записано в NVS
      int32_t restart_counter = 0; 
      err = nvs_get_i32(my_handle, "restart_counter", &restart_counter);
      switch (err) 
      {
         case ESP_OK:
            Serial.println("Сделано!");
            sprintf(buffer,"Значение счётчика перезапусков = %" PRIu32 "\n", restart_counter);
            Serial.print(buffer);
         break;
         case ESP_ERR_NVS_NOT_FOUND:
            Serial.print("Значение счётчика еще не инициализировано!\n");
            break;
         default :
            sprintf(buffer,"Ошибка чтения (%s)!\n", esp_err_to_name(err));
            Serial.print(buffer);
      }
      // ----------------------------------------------------------- Запись ---
      Serial.print("Обновляем счётчик перезапусков в NVS ... ");
      restart_counter++;
      err = nvs_set_i32(my_handle, "restart_counter", restart_counter);
      Serial.print((err != ESP_OK) ? "Не получилось!\n" : "Сделано!\n");
      // Фиксируем записанные значения (после установки любых значений необходимо
      // вызывать функцию nvs_commit(), чтобы обеспечить запись изменений во 
      // флэш-память)
      Serial.print("Фиксируем обновления в NVS ... ");
      err = nvs_commit(my_handle);
      Serial.print((err != ESP_OK) ? "Не получилось!\n" : "Сделано!\n");
      // ----------------------------------------------------- Закрытие NVS ---
      nvs_close(my_handle);
   }
   // Предупреждаем о перезапусе контроллера и перезапускаем его
   for (int i = 10; i >= 0; i--) 
   {
      sprintf(buffer,"Перезапуск через %d секунд ...\n", i);
      Serial.print(buffer);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
   }
   Serial.print("Перезапускаем контроллер.\n\n");
   esp_restart();
}

void loop() {}

// ******************************************************* nvs-rw-value.ino ***
