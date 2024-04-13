
/*   Blink-ESP8266-IKD.ino
 
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/


/** Arduino UNO                         *** proverka-komand-upravleniya.ino ***
 * 
 * ���������� ������ ������ �� ��� ��� ���������� ������� ����������
 * https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/
 * 
 * v1.1, 29.03.2024                                   �����:      �������� �.�.
 * Copyright � 2024 tve                               ���� ��������: 28.03.2024
**/

#include <IRremote.hpp>
// #define LED_BUILTIN 2
#define IR_RECEIVE_PIN 2

void setup() 
{
   pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output
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
