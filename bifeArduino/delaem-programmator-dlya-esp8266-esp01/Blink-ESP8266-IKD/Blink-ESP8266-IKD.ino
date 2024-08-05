
/*   Blink-ESP8266-IKD.ino
 
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/


/** Arduino UNO ************************************* *** MakeProntoHex.ino ***
 *  (this file is based on ReceiveDump.cpp by MIT License, Armin Joachimsmeyer)
 * 
 * Вывести коды полученного инфракрасного сигнала в формате ProntoHEX
 * 
 * v2.1, 05.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2023 tve                               Дата создания: 30.12.2023
**/

#include <IRremote.hpp>
#define IR_RECEIVE_PIN 2
#define buzPin 0

void setup() 
{
   pinMode(buzPin, OUTPUT);  
   digitalWrite(buzPin, LOW);
   Serial.begin(115200);
   IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
}

// the loop function runs over and over again forever
void loop() 
{

   if (IrReceiver.decode()) 
   {
      // Извлекаем код, отправленный пультом дистанционного управления 
      // в зависимости от того, какая клавиша была нажата из структуры IRData
      uint16_t command = IrReceiver.decodedIRData.command;
      if (command==50)
      {
         digitalWrite(buzPin, HIGH);
         Serial.println("до");
         delay(100);  
         Serial.println("после");
         digitalWrite(buzPin, LOW);
      }
      Serial.println(command);
      // Делаем задержку в 100 мс, чтобы замедлить печать при непрерывном 
      // нажатии клавиши, и устранить повторную распечатку
      delay(100);  
      // Готовим прием следующего нажатия клавиши
      IrReceiver.resume();
   }

   /*
   digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level
   Serial.println("LED_BUILTIN --> LOW");
   // but actually the LED is on; this is because
   // it is active low on the ESP-01)
   delay(500);                      // Wait for a second
   digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
   Serial.println("LED_BUILTIN --> HIGH");
   delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
   */
}
