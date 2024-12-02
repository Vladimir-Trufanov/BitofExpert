## [gde-nahoditsya-fpermissive](#) 


```
в н

class TQueMessage
{
  public:
  
  TQueMessage(int _QueueSize=4);
  bool Create();
  //bool Send();

  private:
  
  QueueHandle_t tQueue;  // очередь (дескриптор) будущих сообщений из структур tStruMessage   
  int QueueSize;         // размер очереди 
  String cNULL="";       // отсутствие текста - заполнитель массивов char
  
 в cpp
 
 
#include "Arduino.h"
#include "QueMessage.h"

TQueMessage::TQueMessage(int _QueueSize=4)
{
   QueueSize=_QueueSize;
}

ошибка

QueMessage.cpp:13:1: error: default argument given for parameter 1 of 'TQueMessage::TQueMessage(int)' [-fpermissive]
In file included from C:\ittve-ESP32-CAM\04-Работаем с сообщениями через очередь\QueueHandlMulti\QueMessage.cpp:11:
C:\ittve-ESP32-CAM\04-Работаем с сообщениями через очередь\QueueHandlMulti\QueMessage.h:33:3: note: previous specification in 'TQueMessage::TQueMessage(int)' here
exit status 1
default argument given for parameter 1 of 'TQueMessage::TQueMessage(int)' [-fpermissive]

просле обновления каждый раз .5.6.7 !!!

как искал

Ошибка «default argument given for parameter 1 of '(int)' [-fpermissive]» может возникать, если параметр по умолчанию задан более одного раза. 3

Параметр по умолчанию нужно указывать только в объявлении функции. Можно и в определении, но только если определение находится выше вызова функции. 3



```


### Библиография

#### [---Queue Management](https://www.freertos.org/Documentation/02-Kernel/04-API-references/06-Queues/00-QueueManagement)

#### [---FreeRTOS queues](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/01-Queues)

#### [---Очереди FreeRTOS](https://kotyara12.ru/iot/esp32_queue/)

#### [---ESP32 Arduino: FreeRTOS Queues](https://techtutorialsx.com/2017/08/20/esp32-arduino-freertos-queues/)

###### [к содержанию](#%D0%B2%D0%B2%D0%B5%D0%B4%D0%B5%D0%BD%D0%B8%D0%B5---%D0%B1%D1%8B%D1%81%D1%82%D1%80%D0%BE%D0%B5-%D0%B7%D0%BD%D0%B0%D0%BA%D0%BE%D0%BC%D1%81%D1%82%D0%B2%D0%BE)
