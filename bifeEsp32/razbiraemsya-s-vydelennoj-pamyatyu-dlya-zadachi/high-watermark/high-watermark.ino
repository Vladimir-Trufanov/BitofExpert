/** Arduino-Esp32-CAM ********************************** high-watermark.ino ***

                        Вычислить объем памяти (стека), требующийся для задачи. 
   Исследуемые задачи просто мигают контрольным светодиодом и вспышкой на своих 
   штатных контактах 4 и 33 (для ESP32-CAM). При объявлении задач резервируется 
             достаточный объем памяти (но различный для каждой задачи для пущей 
    убедительности), а затем используется функция uxTaskGetStackHighWaterMark() 
           для запроса оставшейся свободной памяти в зарезервированной области.    

     Первоисточник: 
     https://wolles-elektronikkiste.de/en/using-freertos-with-esp32-and-arduino

   v1.0, 13.10.2024                                   Автор:      Труфанов В.Е.
   Copyright © 2024 tve                               Дата создания: 13.10.2024
   
   1. UBaseType_t uxTaskGetStackHighWaterMark(TaskHandle_t xTask) - возвращает 
   максимально допустимый размер стека, связанный с xTask. То есть минимальное 
   свободное пространство стека (в байтах, с момента запуска задачи). 
   
   Чем меньше возвращаемое число, тем ближе задача к переполнению стека (если 
   возвращаемое значение равно нулю, то задача, скорее всего, переполнила 
   свой стек). 

   Иначе говоря, функция возвращает минимальное количество оставшегося места в 
   стеке, доступного задаче с момента начала её выполнения, то есть количество 
   стека, которое оставалось неиспользуемым в тот момент, когда стек задач был 
   заполнен максимально (глубоко). Это так называемая «высшая точка» стека.

   Для доступа к этой функции в FreeRTOSConfig.h необходимо установить значение
   INCLUDE_uxTaskGetStackHighWaterMark равным 1.
   
   Функции uxTaskGetStackHighWaterMark() и uxTaskGetStackHighWaterMark2() идентичны, 
   за исключением типа возвращаемого значения. Использование configSTACK_DEPTH_TYPE 
   позволяет пользователю определить тип возвращаемого значения. Это позволяет 
   избежать проблемы переполнения 8-битных типов без нарушения обратной 
   совместимости для приложений, которые ожидают 8-битный тип возвращаемого 
   значения.
   
   Параметры: xTask — дескриптор задачи, связанной с проверяемым стеком. 
   Следует задать для xTask значение NULL, чтобы проверить стек вызывающей задачи.
   
   2. uint8_t *pxTaskGetStackStart(TaskHandle_t xTask) - возвращает начало стека, 
   связанного с xTask. Для доступа к этой функции в FreeRTOSConfig.h необходимо 
   установить значение INCLUDE_pxTaskGetStackStart равным 1.
   
   Возвращает самый нижний адрес памяти в стеке (указатель на начало стека), 
   независимо от того, увеличивается ли стек или уменьшается.
   
   Параметры: xTask — дескриптор задачи, связанной с проверяемым стеком. 
   Следует задать для xTask значение NULL, чтобы проверить стек вызывающей задачи.
   
   3. eTaskState eTaskGetState(TaskHandle_t xTask) - возвращает состояние любой 
   задачи (xTask – дескриптор запрашиваемой задачи.). Состояния кодируются 
   перечисляемым типом eTaskState. Для доступности этой функции необходимо, 
   чтобы параметр INCLUDE_eTaskGetState был равен 1. 
  
   ВНИМАНИЕ: Состояние задачи может измениться между вызовом функции и передачей 
   возвращаемого функцией значения вызывающей задаче.
   
   enum eTaskState - состояния задачи, возвращаемые eTaskGetState:
      enumerator eRunning, enumerator eReady, enumerator eBlocked,
      enumerator eSuspended, enumerator eDeleted, enumerator eInvalid
**/

#define LED1 4  // определили пин вспышки
#define LED2 33 // определили пин контрольного светодиода

// Определяем дескрипторы задач
TaskHandle_t taskBlink1Handle; 
TaskHandle_t taskBlink2Handle;

// Определяем счетчик рекурсий для понижения стека
int Frecurs=0;
int Fbycicl=0;
int x=0;
//byte *by;

void setup()
{
   Serial.begin(115200);
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);

   xTaskCreate(
      blink1,             // Function name of the task
      "Blink 1",          // Name of the task (e.g. for debugging)

      1900,               // Достаточный размер стека (bytes)
      //2000,               // Достаточный размер стека (bytes)
      //1900,               // Достаточный размер стека (bytes)
      // 1860,            // Недостаточный размер стека, который вызывает переполнение стека и перезапуск контроллера:
                          //    Guru Meditation Error: Core  0 panic'ed (Unhandled debug exception). 
                          //    Debug exception reason: Stack canary watchpoint triggered (Blink 1) 

      NULL,               // Parameter to pass
      1,                  // Task priority
      &taskBlink1Handle   // Assign task handle
   );

   xTaskCreate(
      blink2,             // Function name of the task
      "Blink 2",          // Name of the task (e.g. for debugging)
      2048,               // Stack size (bytes)
      NULL,               // Parameter to pass
      1,                  // Task priority
      &taskBlink2Handle   // Assign task handle
   );

   xTaskCreate(
      printWatermark,     // Function name of the task
      "Print Watermark",  // Name of the task (e.g. for debugging)
      2048,               // Рекомендуемый размер стека от Espressif Systems (в байтах)
      NULL,               // Parameter to pass
      1,                  // Task priority
      NULL                // Task handle
   );
}

void blink1(void *pvParameters)
{
   while (1)
   {
      digitalWrite(LED1, HIGH);
      delay(10);
      digitalWrite(LED1, LOW);
      delay(2990);
      //byte *by = malloc(f++);
      //Serial.print("f=");
      //Serial.println(f);

      //for (int i = 0; i < f; i++) 
      //{
      //   by[i] = 25;
      //}

      x=x+1;
      //Frecurs=recursFactorial(x);
      //Fbycicl=byciclFactorial(x);
      //int Frecurs1=recursFactorial(5);
      //int Fbycicl1=byciclFactorial(5);
      Frecurs=recursFactorial(x);
      Fbycicl=byciclFactorial(x);


      int Fbycicl1_1=byciclFactorial(1);
      Serial.print("Fbycicl1_1!=");      
      Serial.println(Fbycicl1_1);      

      int Fbycicl1_2=byciclFactorial(2);
      Serial.print("Fbycicl1_2!=");      
      Serial.println(Fbycicl1_2);      

      int Fbycicl1_3=byciclFactorial(3);
      Serial.print("Fbycicl1_3!=");      
      Serial.println(Fbycicl1_3);      

      int Fbycicl1_4=byciclFactorial(4);
      Serial.print("Fbycicl1_4!=");      
      Serial.println(Fbycicl1_4);      

      int Fbycicl1_5=byciclFactorial(5);
      Serial.print("Fbycicl1_5!=");      
      Serial.println(Fbycicl1_5);      

      int Fbycicl1_6=byciclFactorial(6);
      Serial.print("Fbycicl1_6!=");      
      Serial.println(Fbycicl1_6);      

      int Fbycicl1_7=byciclFactorial(7);
      Serial.print("Fbycicl1_7!=");      
      Serial.println(Fbycicl1_7);      

      int Fbycicl1_8=byciclFactorial(8);
      Serial.print("Fbycicl1_8!=");      
      Serial.println(Fbycicl1_8);      

      int Fbycicl1_9=byciclFactorial(9);
      Serial.print("Fbycicl1_9!=");      
      Serial.println(Fbycicl1_9);      

      int Fbycicl1_10=byciclFactorial(10);
      Serial.print("Fbycicl1_10!=");      
      Serial.println(Fbycicl1_10);      

      int Fbycicl1_11=byciclFactorial(11);
      Serial.print("Fbycicl1_11!=");      
      Serial.println(Fbycicl1_11);      

      int Fbycicl1_12=byciclFactorial(12);
      Serial.print("Fbycicl1_12!=");      
      Serial.println(Fbycicl1_12);      


      
      HighWatermark("У себя",NULL);
      //free(by);
   }
}

void blink2(void *pvParameters)
{
   while (1)
   {
      digitalWrite(LED2, HIGH);
      delay(333);
      digitalWrite(LED2, LOW);
      delay(333);
      //factorial(f++);
   }
}

void printWatermark(void *pvParameters)
{
   while (1)
   {
      delay(3000);
      HighWatermark("Задача",taskBlink1Handle);
      HighWatermark("Задача",taskBlink2Handle);
      Serial.println();
   }
}

void loop() {}

unsigned long long factorial(unsigned n)
{
   //if (n > 1) return n * factorial(n-1);
   //return 1;

   //size_t required_size = 10;  
   //uint32_t* run_time = malloc(required_size + sizeof(uint32_t));

   //byte *by = malloc(10);

}

int recursFactorial(int currNum)
{
   if (currNum==1) return 1;
   return currNum * recursFactorial(currNum-1);
}

int byciclFactorial(int currNum)
{
   int p = 1;
   while (currNum > 1) 
   {
      p=p*currNum;
      currNum--;
   }
   return p;
}

void HighWatermark(String Task, TaskHandle_t taskHandle)
{
   // Определяем минимально свободное пространство в стеке задачи
   // после очередного цикла задачи
   Serial.print(Task);
   Serial.print(": ");
   Serial.print(pcTaskGetName(taskHandle)); // Get task name with handler
   Serial.print(", высшая точка = ");
   Serial.print(uxTaskGetStackHighWaterMark(taskHandle));
   Serial.println();
   // Определяем начало стека
   uint8_t Point = *pxTaskGetStackStart(taskHandle);
   printf("Начало стека: %u = %#x\n", Point, Point);
   //
   //Serial.println(f);
   //Serial.println(f);
   printf("F!(%d): Frecurs=%d %d=Fbycicl \n",x,Frecurs,Fbycicl);
   //f=f+200000000;
}

// ***************************************************** high-watermark.ino ***
