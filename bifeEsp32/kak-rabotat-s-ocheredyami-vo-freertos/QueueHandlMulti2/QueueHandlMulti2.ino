// QueueHandlMulti2.ino

char buffer[1024];
// Определяем структуру передаваемого сообщения
struct AMessage
{
   int  ucSize;        // Длина сообщения (максимально 256 байт)
   char ucData[256];   // Текст сообщения
};  
struct AMessage xMessage, *pxMessage;
struct AMessage xRxedStructure;
QueueHandle_t xQueue;

unsigned long nLoop = 0UL;

#define str "str"
#define chr "chr"
String types(String a) {return str;}
String types(char *a)  {return chr;}

// ****************************************************************************
// *  Сформировать сообщение о прошедшем времени с начала запуска приложения  *
// ****************************************************************************
// Определяем заголовок для объекта таймера
hw_timer_t *timer = NULL;
// Выделяем и инициируем переменную для прошлого момента времени
int lastMillis = millis(); 

void ARDUINO_ISR_ATTR onTimer() 
{
   // Размещаем структуру для сообщения в статической памяти для того,
   // чтобы уменьшить фрагментацию кучи 
   static DRAM_ATTR struct AMessage xiMessage;
   // Выделяем переменную планировщикe задач FreeRTOS для указания
   // необходимости переключения после прерывания на более приоритетную 
   // задачу, связанную с очередью
   static DRAM_ATTR BaseType_t xHigherPriorityTaskWoken;
   // Выделяем переменную для текущего момента времени
   static DRAM_ATTR int currMillis;
   // Выделяем переменную для прошедшего времени с начала запуска приложения
   static DRAM_ATTR int timeMillis;
    
   // Если в очереди есть место, будем размещать сообщение
   if (xQueue!=0)
   {
      // Сбрасываем признак переключения на более приоритетную задачу
      // после прерывания 
      xHigherPriorityTaskWoken = pdFALSE;
      // Определяем время, прошедшее с начала запуска приложения
      currMillis = millis(); 
      if (currMillis < lastMillis) lastMillis=0;
      timeMillis=currMillis-lastMillis;
      // Формируем сообщение для передачи в очередь
      sprintf(xiMessage.ucData, "Прошло (%d) миллисекунд",timeMillis);
      xiMessage.ucSize = 0;
      while (xiMessage.ucData[xiMessage.ucSize]>0) 
      {
         xiMessage.ucSize++;
      }
      // Отправляем сообщение в структуре AMessage 
      if (xQueueSendFromISR(xQueue, &xiMessage, &xHigherPriorityTaskWoken) != pdPASS)
      {
         Serial.println("ISR: Не удалось отправить структуру!");
      }
   }
   else 
   {
      Serial.println("ISR: Очередь для структур не создана!");
   }
   // Если требуется отдать управление планировщику на переключение 
   // после прерывания на более приоритетную задачу, делаем это 
   if (xHigherPriorityTaskWoken)
   {
      Serial.println("ISR: Управление передаётся планировщику!");
      portYIELD_FROM_ISR();
   }
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
