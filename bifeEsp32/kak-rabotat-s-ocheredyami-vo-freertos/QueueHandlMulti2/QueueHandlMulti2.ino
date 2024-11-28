// QueueHandlMulti2.ino

char buffer[1024];
struct AMessage
{
   int  ucSize;
   char ucData[256];
};  
struct AMessage xMessage, *pxMessage;
struct AMessage xiMessage, *pxiMessage;
struct AMessage xRxedStructure;
QueueHandle_t xQueue;

unsigned long nLoop = 0UL;

#define str "str"
#define chr "chr"
String types(String a) {return str;}
String types(char *a)  {return chr;}

// Определяем заголовок для объекта таймера
hw_timer_t *timer = NULL;
//
void ARDUINO_ISR_ATTR onTimer() 
{
   int i=1958;

   if (xQueue!=0)
   {
      //char cIn;
      BaseType_t xHigherPriorityTaskWoken;
      /* We have not woken a task at the start of the ISR. */
      xHigherPriorityTaskWoken = pdFALSE;
      /* Loop until the buffer is empty. */
      //do
      //{
         /* Obtain a byte from the buffer. */
         // cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
         /* Post the byte. */
      // Отправляем указатель на структуру AMessage 
      sprintf(buffer, "Take:(%d)",21);
      xiMessage.ucSize = 0;
      while (buffer[xiMessage.ucSize]>0) 
      {
         xiMessage.ucData[xiMessage.ucSize]=buffer[xiMessage.ucSize];
         xiMessage.ucSize++;
      }
      pxiMessage = &xiMessage;
      if (xQueueSendFromISR(xQueue, pxiMessage, &xHigherPriorityTaskWoken) != pdPASS)
      //if (xQueueSendFromISR(xQueue,pxiMessage,2) != pdPASS)
      {
         Serial.println("ОПЯТЬ не!");
      }
      else 
      {
         //Serial.print ("tt: "); Serial.println (xiMessage.ucSize);
      }

      
      //} 
      //while( portINPUT_BYTE( BUFFER_COUNT ) );

      /* Now the buffer is empty we can switch context if necessary. */
      if( xHigherPriorityTaskWoken )
      {
         /* Actual macro used here is port specific. */
         //taskYIELD_FROM_ISR ();
      }


    

      /*
      // Отправляем указатель на структуру AMessage 
      sprintf(buffer, "Take:(%d)",21);
      xiMessage.ucSize = 0;
      while (buffer[xiMessage.ucSize]>0) 
      {
         xiMessage.ucData[xiMessage.ucSize]=buffer[xiMessage.ucSize];
         xiMessage.ucSize++;
      }
      pxiMessage = &xiMessage;
      //if (xQueueSend(xQueue,pxiMessage,2) != pdPASS)
      if (xQueueSendFromISR(xQueue,pxiMessage,2) != pdPASS)
 
      BaseType_t xQueueSend       (QueueHandle_t xQueue,const void *pvItemToQueue,TickType_t xTicksToWait);
      BaseType_t xQueueSendFromISR(QueueHandle_t xQueue,const void *pvItemToQueue,BaseType_t *pxHigherPriorityTaskWoken);

      {
         Serial.println("Не удалось отправить структуру даже после 10 тиков!");
      }
      else 
      {
         Serial.print ("tt: "); Serial.println (xiMessage.ucSize);
         //Serial.print ("=>");
         //Serial.println (xMessage.ucData);
      }
      */
      
   }
   else 
   {
      Serial.println("ISR: Очередь для структур не создана!");
   }
   // Инкрементируем счетчик прерываний и фиксируем время текущего прерывания
   //portENTER_CRITICAL_ISR(&timerMux);
   //isrCounter = isrCounter + 1;
   //lastIsrAt = millis();
   //portEXIT_CRITICAL_ISR(&timerMux);
   // Освобождаем семафора, который будем проверять в основном цикле
   //xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

void setup() 
{
   Serial.begin(115200);

   // Определяем дополнительную задачу
   xTaskCreatePinnedToCore (
      vATask,         // название функции, которая будет запускаться, как параллельная задача
      "Сообщение",    // название задачи
      1024,           // размер стека в байтах
      NULL,           // указатель параметра, который будет передан задаче (NULL, если параметр не передаётся)
      0,              // приоритет задачи
      NULL,           // дескриптор или указатель на задачу
      0               // ядро процессора, на котором требуется запустить задачу 
   );

   // Создаём объект таймера, устанавливаем его частоту отсчёта (1Mhz)
   timer = timerBegin(1000000);
   // Подключаем функцию обработчика прерывания от таймера - onTimer
   timerAttachInterrupt(timer, &onTimer);
   // Настраиваем таймер: интервал перезапуска - 1 секунда (1000000 микросекунд),
   // всегда повторяем перезапуск (третий параметр = true), неограниченное число 
   // раз (четвертый параметр = 0) 
   timerAlarm(timer, 1400000, true, 0);

   
   // Создаем очередь из 10 структур
   xQueue = xQueueCreate(10, sizeof(struct AMessage));
   if(xQueue==NULL)
   {
      Serial.println("Очередь не была создана и не может использоваться!");
   }
   Serial.println("Очередь сформирована!");
}

// ****************************************************************************
// *             Выполнять передачу сообщения в бесконечном цикле,            *
// * если задача завершится - не будет циклится, то контроллер перезагрузится *
// ****************************************************************************
void vATask (void* pvParameters) 
{
   while (1) 
   {
      if (xQueue!=0)
      {
         // Отправляем указатель на структуру AMessage 
         sprintf(buffer, "Передан номер цикла: (%d)", 18);
         xMessage.ucSize = 0;
         while (buffer[xMessage.ucSize]>0) 
         {
            xMessage.ucData[xMessage.ucSize]=buffer[xMessage.ucSize];
            xMessage.ucSize++;
         }
         pxMessage = &xMessage;
         if (xQueueSend(xQueue,pxMessage,2) != pdPASS)
         {
            Serial.println(" Task Не удалось отправить структуру даже после 10 тиков!");
         }
            else 
         {
            //Serial.print ("Передано: ");
            //Serial.print (xMessage.ucSize);
            //Serial.print ("=>");
            //Serial.println (xMessage.ucData);
         }
      }
      else 
      {
         Serial.println("TASK: Очередь для структур не создана!");
      }
      delay (1600); 
   }
}
 
void loop() 
{

   /*
   String temp = "cat123456 Всем привет!";
   char tab2[1024];
   strcpy(tab2, temp.c_str());
   String temp1=String(tab2);
  
   Serial.println(temp);
   Serial.println(types(temp));
   
   Serial.println(tab2);
   Serial.println(types(tab2));

   Serial.println(temp1);
   Serial.println(types(temp1));
   
   Serial.println(temp1.length());
   Serial.println("----");
   */
  
  nLoop++;
  Serial.print("Цикл: ");
  Serial.println(nLoop);

  /*
  if (xQueue != 0)
  {
    // Отправляем указатель на структуру AMessage 
    sprintf(buffer, "Передан номер цикла: (%d)", nLoop);
    xMessage.ucSize = 0;
    while (buffer[xMessage.ucSize]>0) 
    {
       xMessage.ucData[xMessage.ucSize]=buffer[xMessage.ucSize];
       xMessage.ucSize++;
    }
    pxMessage = &xMessage;
    if (xQueueSend(xQueue,pxMessage,10) != pdPASS)
    {
      Serial.println("Не удалось отправить структуру даже после 10 тиков!");
    }
    else 
    {
      Serial.print ("Передано: ");
      Serial.print (xMessage.ucSize);
      Serial.print ("=>");
      Serial.println (xMessage.ucData);
    }
  }
  else 
  {
    Serial.println("Очередь для структур не создана!");
  }
  */
  
  if (xQueue != NULL)
  {
    // Получаем сообщение из созданной очереди для хранения сложного
    // структурного сообщения. Блокировка на 10 тиков, если сообщение
    // недоступно немедленно. Значение считывается в структурную
    // переменную AMessage, поэтому после вызова xQueueReceive()  
    // xRxedStructure будет содержать копию сообщения
      
    if (xQueueReceive(xQueue,&xRxedStructure,10) != pdPASS)
    {
      Serial.println("Не удалось принять структуру даже после 10 тиков!");
    }
    else
    {
      Serial.print("Выбрано: ");
      Serial.print(xRxedStructure.ucSize);
      Serial.print("==>");
      Serial.println(xRxedStructure.ucData);
      /*
      //Serial.println(String(xRxedStructure.ucData));
      Serial.print(xRxedStructure.ucData[0]);
      Serial.print(xRxedStructure.ucData[1]);
      Serial.print(xRxedStructure.ucData[2]);
      Serial.print(xRxedStructure.ucData[3]);
      Serial.print(xRxedStructure.ucData[4]);
      Serial.print(xRxedStructure.ucData[5]);
      Serial.println("---------------------");
      */
    }
  }
  else
  {
    Serial.println("Нет очереди!");
  }
  delay(1300);
}
