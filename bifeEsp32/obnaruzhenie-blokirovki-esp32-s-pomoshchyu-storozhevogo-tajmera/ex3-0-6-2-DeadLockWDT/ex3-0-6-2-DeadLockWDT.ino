/** Arduino-Esp32-CAM                         *** ex3-0-6-2-DeadLockWDT.ino ***
 * 
 *                                               Потенциальная взаимоблокировка
 *         для плат ESP32 в версии библиотеки от Espressif Systems версии 3.0.6
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * 
 * v1.1, 04.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 13.10.2024
 * 
 * В скетче показано возможное зависание -  явление, при котором две задачи 
 * находятся в заблокированном состоянии, одновременно ожидая доступа к 
 * ресурсам, которые удерживаются одной из них. 
 * 
 * Замечание от 2024-11-04: на сегодня, в версии библиотеки от Espressif 
 * Systems версии 3.0.6 по итогам проверки на контроллере AI-Thinker ESP32-CAM 
 * проблема взаимоблокировки в FreeRTOS разрешается (задачам дается возможность
 * выполнить свою работу), а сторожевой таймер, при необходимомти,
 * перезагружает контроллер!
**/

#include <esp_task_wdt.h>
#define WDT_TIMEOUT 10   // тайм-аут в 10 секунд
esp_err_t ESP32_ERROR;   // возвращенное значение при инициализации TWDT

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

// Task A (высокий приоритет = 2)
void doTaskA(void *parameters) 
{
   while (1) 
   {
      // Берем мьютекс1 на 1 тик
      // (начинаем ожидание для принудительной взаимоблокировки)
      xSemaphoreTake(mutex_1, portMAX_DELAY);
      Serial.println("Task A взяла mutex_1 и делает паузу на 1 тик");
      vTaskDelay(1/portTICK_PERIOD_MS);
      // Берем мьютекс2
      xSemaphoreTake(mutex_2, portMAX_DELAY);
      Serial.println("Task A взяла mutex_2");
      // Держим критическую секцию, защищённую двумя мьютексами
      Serial.println("          == А == ВЫПОЛНЯЕТ ПРОСТУЮ РАБОТУ в 500 ТИКОВ");
      vTaskDelay(500 / portTICK_PERIOD_MS);
      // Возвращаем мьютексы
      Serial.println("Task A возвращает мьютексы");
      xSemaphoreGive(mutex_2);
      xSemaphoreGive(mutex_1);
      // Ждем и даем возможность запуститься другой задаче
      Serial.println("Task A ждет еще 500 тиков и дает запуститься второй задаче");
      vTaskDelay(500 / portTICK_PERIOD_MS);
   }
}

// Task B (низкий приоритет = 1)
void doTaskB(void *parameters) 
{
   while (1) 
   {
      xSemaphoreTake(mutex_2, portMAX_DELAY);
      vTaskDelay(1 / portTICK_PERIOD_MS);
      xSemaphoreTake(mutex_1, portMAX_DELAY);
      Serial.println("          == В == ВЫПОЛНЯЕТ ПРОСТУЮ РАБОТУ в 500 ТИКОВ");
      vTaskDelay(500 / portTICK_PERIOD_MS);
      xSemaphoreGive(mutex_1);
      xSemaphoreGive(mutex_2);
      vTaskDelay(500 / portTICK_PERIOD_MS);
   }
}

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

   // Создаем мьютексы перед запуском задач
   mutex_1 = xSemaphoreCreateMutex();
   mutex_2 = xSemaphoreCreateMutex();

   // Создаем Task A (с высоким приоритетом) и Task B (с низким приоритетом)
   xTaskCreate(doTaskA, "Task A", 1024, NULL, 2, NULL);
   xTaskCreate(doTaskB, "Task B", 1024, NULL, 1, NULL);
}

void loop(){}

// ********************************************** ex3-0-6-2-DeadLockWDT.ino ***
