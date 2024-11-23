/** Arduino-Esp32-CAM                                   *** StackFiller.ino ***
 * 
 *          Показать заполнение свободной памяти, как внутренней, так и внешней
 * 
 * v1.0, 23.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 23.11.2024
**/

int n=1; // счетчик циклов
String cstr = "Строка становится всё длиннее! ";

void setup() 
{
   Serial.begin(115200);
}

void Viewloop() 
{
   // Полный размер кучи в памяти
   printf("Общий размер ВСТРОЕННОЙ памяти:     %u\n", ESP.getHeapSize());
   // Количество доступной кучи в памяти
   printf("Оставшаяся доступная память в куче: %u\n", ESP.getFreeHeap());
   // Размер общей кучи SPI PSRAM
   printf("Общий размер SPI PSRAM:             %u\n", ESP.getPsramSize());
   // Количество свободной PSRAM
   printf("Количество свободной PSRAM:         %d\n", ESP.getFreePsram());
}

void loop() 
{
   // Сообщаем текущую информацию по памяти
   Serial.println("");
   Serial.print("Цикл №");
   Serial.println(n);
   Viewloop();
   uint32_t lens = cstr.length();
   Serial.println(cstr);
   Serial.print("Длина строки: ");
   Serial.println(lens);
   delay(10000);
   // Увеличиваем размер строки и меняем счетчик циклов
   cstr=cstr+cstr;
   n++;
}
// ******************************************************** StackFiller.ino ***
