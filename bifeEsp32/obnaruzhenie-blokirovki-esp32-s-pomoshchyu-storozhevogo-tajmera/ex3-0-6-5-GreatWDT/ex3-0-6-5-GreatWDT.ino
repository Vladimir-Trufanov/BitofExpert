/** Arduino-Esp32-CAM                            *** ex3-0-6-5-GreatWDT.ino ***
 * 
 *                                                 Всеобъемлющий сторожевой пес
 * 
 *   В данном скетче сторожевой таймер, реализованный как обработчик прерывания 
 *   по времени, обслуживает четыре задачи и вызывает перезагрузку контроллера, 
 *                                          когда зацикливаются любая из задач.
 *  
 *  Две задачи загружены на нулевое ядро контроллера, две другие на первое. Все 
 *     контролируемые задачи после успешного завершения своего цикла выставляют 
 *   свой флаг в 1. Обработчик сторожевого таймера содержит критическую секцию, 
 *   в которой проверяет состояния флагов всех задач. Если все флаги говорят об 
 *   успешном выполнении задач, то обработчик сбрасывает их в 0 и перезапускает 
 *   сторожевой счётчик.
 *   
 *        Искусственно любую задачу можно зациклить. Для этого в основном цикле 
 *   считываются целые числа из последовательного порта, что позволяет ввести и 
 * числа 1, 2, 3, 4. Введенные числа проверяются задачами на соответствие своим 
 * номерам и, следовательно, запуску бесконечного цикла.
 * 
 *          При зацикливании, флаг задачи сброшенный сторожевым обработчиком не 
 *            устанавливается в 1, что затем вызывает перезагрузку контроллера. 
 *             
 * v1.1, 04.11.2024                                   Автор:      Труфанов В.Е.
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
// Определяем число, которое будет считываться в основном цикле
// с последовательного порта
volatile int inumber;
// Определяем задачи и их флаги
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask4(void *pvParameters);
int flag[] = {0, 0, 0, 0};

// Обработка прерывания от таймера
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

// Начальная настройка: выделяем четыре задачи (две на 0 процессоре, две на 1)
// и обеспечиваем запуск прерывания от таймера периодически через 3 секунды
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

// Основной цикл: считываем с последовательного порта целое число
// (так как в зависимости от окружения за целым числом может следовать нулевое значение,
// то отсекаем 0)
void loop() 
{
   if (Serial.available() > 0) 
   {
      int ii=Serial.parseInt();
      if (ii>0) inumber=ii;
      delay(100);
   }
}

// Имитируем событие зависания процессора
void MimicMCUhangEvent(String NameTask)
{
   while (true)
   {
      Serial.print(NameTask);
      Serial.println(": зависание процессора!!!");
   }
}

void vTask1(void* pvParameters) 
{
   for (;;)
   {
      Serial.print("Task1 ");
      // В обычном режиме делаем паузу 500 мсек и выставляем бит задачи в 1
      vTaskDelay(500/portTICK_PERIOD_MS);
      flag[0] = 1;
      // Имитируем зависание микроконтроллера с помощью опознанного числа,
      // принятого в последовательном порту
      if (inumber == 1) MimicMCUhangEvent("Task1");   
   }
}
void vTask2(void* pvParameters) 
{
   for ( ;; )
   {
      Serial.print("Task2 ");
      vTaskDelay(500/portTICK_PERIOD_MS);
      flag[1] = 1;
      if (inumber == 2) MimicMCUhangEvent("Task2");   
   }
}
void vTask3(void* pvParameters) 
{
   for ( ;; )
   {
      Serial.print("Task3 ");
      vTaskDelay(500/portTICK_PERIOD_MS);
      flag[2] = 1;
      if (inumber == 3) MimicMCUhangEvent("Task3");   
   }
}
void vTask4(void* pvParameters) 
{
   for ( ;; )
   {
      Serial.print("Task4 ");
      vTaskDelay(500/portTICK_PERIOD_MS);
      flag[3] = 1;
      if (inumber == 4) MimicMCUhangEvent("Task4");   
   }
}

// ************************************************* ex3-0-6-5-GreatWDT.ino ***
