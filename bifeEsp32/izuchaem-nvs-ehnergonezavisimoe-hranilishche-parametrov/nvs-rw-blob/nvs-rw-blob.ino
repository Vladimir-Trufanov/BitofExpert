/** Arduino-Esp32-CAM                                   *** nvs-rw-blob.ino ***
 * 
 *       Показать, как считывать и записывать целое значение и большой двоичный 
 *          объект (binary large object) с использованием NVS для их сохранения 
 *                                   между перезапусками контроллеров ESP, где:
 *  целое значение - отслеживает количество программных и жестких перезапусков,
 *  большой двоичный объект - содержит таблицу(в смыле - массив) с указанием 
 *  времени выполнения модуля между перезапусками. 
 *  
 *  Таблица считывается из NVS в динамически выделяемую оперативную память. При 
 *  каждом программном перезапуске, запускаемом вручную, в таблицу добавляется 
 *  новое время выполнения и записывается обратно в NVS. Перезагрузки контроллера 
 *  запускаются замыканием пинов для перезагрузки на GND (IO0 на ESP32 и ESP32S2, 
 *  IO9 на ESP32C3).
 * 
 * v1.1, 24.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 23.10.2024
 * 
**/

#include <Arduino.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

// Переопределяем пин для запуска перезагрузок контроллера
#if CONFIG_IDF_TARGET_ESP32C3
   #define BOOT_MODE_PIN GPIO_NUM_9
#else
   #define BOOT_MODE_PIN GPIO_NUM_0
#endif

// ****************************************************************************
// *    Сохранить количество перезапусков модуля в NVS, сначала прочитав, а   *
// *           затем увеличив число, которое было сохранено ранее.            *
// *         Вернуть сообщение об ошибке, если что-то пойдет не так           *
// *                         во время этого процесса.                         *
// ****************************************************************************
esp_err_t save_restart_counter(void)
{
   nvs_handle_t my_handle;
   esp_err_t err;
   // Открываем хранилище
   err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
   if (err != ESP_OK) return err;
   // Читаем, меняем значение и записываем счетчик перезагрузок
   int32_t restart_counter = 0; 
   err = nvs_get_i32(my_handle, "restart_conter", &restart_counter);
   if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
   restart_counter++;
   err = nvs_set_i32(my_handle, "restart_conter", restart_counter);
   if (err != ESP_OK) return err;
   // Фиксируем записанные значения (после установки любых значений необходимо 
   // вызывать функцию nvs_commit(), чтобы обеспечить запись изменений во 
   // флэш-память.
   err = nvs_commit(my_handle);
   if (err != ESP_OK) return err;
   // Закрываем хранилище
   nvs_close(my_handle);
   return ESP_OK;
}
// ****************************************************************************
// *  Сохранить новое значение времени выполнения между перезапусками в NVS,  *
// *     сначала прочитав таблицу с ранее сохраненными значениями, а затем    *
// *                 добавив новое значение в конец таблицы.                  *
// *         Вернуть сообщение об ошибке, если что-то пойдет не так           *
// *                        во время этого процесса.                          *
// ****************************************************************************
esp_err_t save_run_time(void)
{
   nvs_handle_t my_handle;
   esp_err_t err;
   // Открываем хранилище
   err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
   if (err != ESP_OK) return err;
   // Определяем размер памяти, которая требуется для двоичного объекта (таблицы)
   size_t required_size = 0;  
   err = nvs_get_blob(my_handle, "run_time", NULL, &required_size);
   if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
   // Если это возможно, то считываем ранее сохранённую таблицу
   uint32_t* run_time = malloc(required_size + sizeof(uint32_t));
   if (required_size > 0) 
   {
      err = nvs_get_blob(my_handle, "run_time", run_time, &required_size);
      if (err != ESP_OK) 
      {
         free(run_time);
         return err;
      }
   }
   // Записываем новое значение, включая ранее сохраненные значения 
   required_size += sizeof(uint32_t);
   run_time[required_size / sizeof(uint32_t) - 1] = xTaskGetTickCount() * portTICK_PERIOD_MS;
   err = nvs_set_blob(my_handle, "run_time", run_time, required_size);
   free(run_time);
   if (err != ESP_OK) return err;
   // Фиксируем новые занесённые значения
   err = nvs_commit(my_handle);
   if (err != ESP_OK) return err;
   // Закрываем хранилище
   nvs_close(my_handle);
   return ESP_OK;
}
// ****************************************************************************
// *    Выбрать из NVS и показать счетчик перезапусков и таблицу с указанием  *
// * времён выполнения между перезапусками. Вернуть сообщение об ошибке, если *
// *               что-то пойдет не так во время этого процесса.
// ****************************************************************************
esp_err_t print_what_saved(void)
{
   // Открываем хранилище параметров
   nvs_handle_t my_handle;
   esp_err_t err;
   err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
   if (err != ESP_OK) return err;
   printf("Раздел NVS открыт!\n");
   // Считываем значение счетчика перезапусков из NVS
   int32_t restart_counter = 0;
   err = nvs_get_i32(my_handle, "restart_conter", &restart_counter);
   if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
   printf("Счетчик перезапусков = %" PRIu32 "\n", restart_counter);
   // Считываем таблицу времен выполнения между перезапусками
   size_t required_size = 0;  
   // Инициируем выборку размера blob-элемента
   err = nvs_get_blob(my_handle, "run_time", NULL, &required_size);
   if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
   printf("Таблица времён между перезапусками:\n");
   // В соответствии с определенным размером таблицы, выделяем память
   // и считываем таблицу
   if (required_size == 0) 
   {
      printf("еще данные не сохранялись!\n");
   } 
   else 
   {
      uint32_t* run_time = malloc(required_size);
      err = nvs_get_blob(my_handle, "run_time", run_time, &required_size);
      if (err != ESP_OK) 
      {
         free(run_time);
         return err;
      }
      for (int i = 0; i < required_size / sizeof(uint32_t); i++) 
      {
         printf("%d: %" PRIu32 "\n", i + 1, run_time[i]);
      }
      free(run_time);
   }
   // Закрываем хранилище
   nvs_close(my_handle);
   printf("Раздел NVS закрыт!\n");
   return ESP_OK;
}

void setup() 
{
   Serial.begin(115200);
   
   // Инициализируем NVS
   esp_err_t err = nvs_flash_init();
   printf("Проинициализировали NVS сразу ...\n");
   // Если раздел NVS не содержит пустых страниц или он содержит данные в 
   // незнакомом формате, который не распознаётся текущей версией кода,
   // то стираем весь раздел и снова вызываем инициализацию
   if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
   {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
      printf("Затерли весь раздел NVS и проинициализировали ...\n");
   }
   ESP_ERROR_CHECK(err);
   // Выбираем из NVS и показываем счетчик перезапусков и таблицу с указанием
   // времён выполнения между перезапусками
   err = print_what_saved();
   if (err != ESP_OK) printf("Ошибка (%s) открытия хранилища!\n", esp_err_to_name(err));
   // Сохраняем количество перезапусков модуля в NVS
   err = save_restart_counter();
   if (err != ESP_OK) printf("Ошибка (%s) сохранения счетчика перезапусков в NVS!\n", esp_err_to_name(err));

   printf("\nЗамкните GPIO0 на GND для того, чтобы продолжить работу скетча! ...\n");
   
   // По обязательному правилу после сброса контроллера заданный контакт 
   // устанавливаем в режим ввода-вывода
   gpio_reset_pin(BOOT_MODE_PIN);
   // Определяем пин как GPIO_MODE_INPUT - работающий только на вход
   gpio_set_direction(BOOT_MODE_PIN, GPIO_MODE_INPUT);
   
   // Считываем состояние GPIO0. Если GPIO0 на нижнем уровне, то есть
   // замкнут на GND, то через 1000 ms (1000 тактов),
   // сохраняем время выполнения и перезапускаем контроллер
   while (1) 
   {
      // Если IO0 замкнут на GND, запускаем фрагмент кода
      if (gpio_get_level(BOOT_MODE_PIN) == 0) 
      {
         vTaskDelay(1000 / portTICK_PERIOD_MS);
         if(gpio_get_level(BOOT_MODE_PIN) == 0) 
         {
            err = save_run_time();
            if (err != ESP_OK) printf("Ошибка (%s) сохранения таблицы времен выполнения в NVS!\n", esp_err_to_name(err));
            printf("Перезапуск...\n");
            // Выводим остаток последней незавершённой строки (без "\n")
            fflush(stdout);
            esp_restart();
         }
      }
      vTaskDelay(2200 / portTICK_PERIOD_MS);
   }
}

void loop() {}

// ******************************************************** nvs-rw-blob.ino ***
