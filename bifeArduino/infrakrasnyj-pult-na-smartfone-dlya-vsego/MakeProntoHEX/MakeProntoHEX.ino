
/** Arduino UNO ************************************* *** MakeProntoHex.ino ***
 *  (this file is based on ReceiveDump.cpp by MIT License, Armin Joachimsmeyer)
 * 
 * Вывести коды полученного инфракрасного сигнала в формате ProntoHEX
 * 
 * v2.1, 05.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2023 tve                               Дата создания: 30.12.2023
**/

#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

void setup() 
{
   Serial.begin(115200);   
   pinMode(LED_BUILTIN, OUTPUT);
   // Инициируем приёмник (объект IrReceiver). Параметрами, передаваемыми в метод 
   // begin, являются номер порта (пин) на плате Arduino, к которому подключён 
   // выход датчика и константа ENABLE_LED_FEEDBACK, которая включает ретрансляции 
   // кода, полученного от датчика на светодиод. 
   // Используйте константy DISABLE_LED_FEEDBACK для отмены ретрансляции сигнала. 
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 
   Serial.println();
}

void loop() 
{
   // Если пришел IR-код
   if (IrReceiver.decode()) 
   {
      // Отмечаем, что пришел сигнал на контакт
      Serial.print(F("Принят IR-сигнал на контакте ")); Serial.println(IR_RECEIVE_PIN);
      // Если буфер переполнен, то выводим предупреждающее сообщение
      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) 
      {
         Serial.println(F("Обнаружено переполнение буфера"));
         Serial.print(F("Сделайте значение RAW_BUFFER_LENGTH более "));
         Serial.println(F(STR(RAW_BUFFER_LENGTH) " в " __FILE__));
      } 
      else 
      // Выводим информацию по IR-сигналу
      {
         // Выводим общую информацию по сигналу от нажатой кнопки
         IrReceiver.printIRResultShort(&Serial);
         // Выводим последовательность Pronto HEX функцией compensateAndStorePronto()
         String ProntoHEX = F(""); String ProntoHEXi = F("");            
         if (int size = IrReceiver.compensateAndStorePronto(&ProntoHEX)) 
         {
            ProntoHEXi += F("ProntoHEX:");             
            Serial.println(ProntoHEXi.c_str());                
          }
          else ProntoHEX += F("ProntoHEX не сформировался!");
          Serial.println(ProntoHEX.c_str());                
          Serial.println();
      }
      // Готовим прием следующего значения
      IrReceiver.resume();                          
   }
}

// ****************************************************** MakeProntoHex.ino ***
