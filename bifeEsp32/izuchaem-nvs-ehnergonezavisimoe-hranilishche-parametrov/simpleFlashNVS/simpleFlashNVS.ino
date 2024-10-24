/** Arduino-Esp32-CAM                                *** simpleFlashNVS.ino ***
 * 
 *               Пример работы с библиотекой ArduinoNvs:
 *               
                 * Authors:
                 * dRKr, Sinai RnD (info@sinai.io)
                 * (original version) TridentTD (https://github.com/TridentTD/)
 * 
 * v1.0, 24.10.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.10.2024
 * 
**/

#include <Arduino.h>
#include "ArduinoNvs.h"

bool res;

void setup() 
{   
   Serial.begin(115200);
   // Инициируем NVS
   NVS.begin();
   // Записываем в NVS (flash) и читаем целое число
   const uint32_t ui32_set = 4294967295;
   res = NVS.setInt("unsigned-long", ui32_set);
   uint32_t uint32_max = NVS.getInt("unsigned-long"); 
   printf("МАХ целое без знака: в десятичном виде = %u, в шестнадцатеричном = %#x"  "\n", uint32_max, uint32_max);
   // Записываем в NVS и читаем строку
   const String st_set = "Это простая незамысловатая строка для записи в NVS";
   res = NVS.setString("str", st_set);
   String str = NVS.getString("str");
   Serial.println(str);
   // Записываем и читаем двоичные данные переменной длины (blob)
   uint8_t blolb_set[8] = {1,2,3,99,100,0xEE,0xFE,0xEE};
   res = NVS.setBlob("blob", blolb_set, sizeof(blolb_set));
   size_t blobLength = NVS.getBlobSize("blob"); 
   uint8_t blob[blobLength];
   res = NVS.getBlob("blob", blob, sizeof(blob));
   if (res) 
   {
      for (uint8_t i = 0; i < blobLength; i++) 
      {
         Serial.printf("blob[%u] = %u; ", i, blob[i]);
      }
   }
   else
   {
      Serial.println("Не получилось извлечь BLOB из NVS");
   }
}

void loop(){}

// ***************************************************** simpleFlashNVS.ino ***
