/** Arduino-Esp32-CAM                                 *** WatchdogTimer.ino ***
 * 
 *              Пример прерывания и управления программным сторожевым таймером.
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

// #include "esp_system.h"
// #include "rom/ets_sys.h"

// Назначаем 0 пин на остановку таймера
const int button = 0;
// Назначаем время срабатывания прерывания сторожевого таймера 3 секунды         
const int wdtTimeout = 3000;  
// Формируем заголовок таймера
hw_timer_t *timer = NULL;
// Определяем счетчик перезапусков главного цикла
int icounter;

void ARDUINO_ISR_ATTR resetModule() 
{
   ets_printf("\nПерезагрузка!\n");
   esp_restart();
}

void setup() 
{
   Serial.begin(115200);
   Serial.println();
   icounter = 1;
   Serial.println("Запускаются настройки таймера в setup");
   pinMode(button, INPUT_PULLUP);                   // init control pin
   timer = timerBegin(1000000);                     // timer 1Mhz resolution
   timerAttachInterrupt(timer, &resetModule);       // attach callback
   timerAlarm(timer, wdtTimeout * 1000, false, 0);  // set time in us
}

void loop() 
{
   ets_printf("Запускается главный цикл %d раз\n",icounter++);
   // Фиксируем время начала цикла
   long loopTime = millis();
   // Выполняем основную работу цикла
   delay(1000);  
   loopTime = millis() - loopTime;
   ets_printf("Отработано в цикле %d миллисекунд.\n",loopTime);
   // Сбрасываем WatchdogTimer - "Кормим сторожевого пса"
   timerWrite(timer, 0); 
   Serial.println(" ");

   // Пока IO0 замкнут на GND, с интервалом в полсекунды выводим сообщение
   // до тех пор, пока не произойдет перезагрузка контроллера
  while (!digitalRead(button)) 
  {
    Serial.println("IO0 замкнут на GND");
    delay(500);
  }
}

// ****************************************************** WatchdogTimer.ino ***
