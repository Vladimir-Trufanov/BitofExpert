
/** Arduino UNO ******************************************* *** simple2.ino ***
 * 
 * Обеспечить мигание светодиода с интервалом в 1 сек (1Гц) на плате через 
 * прерывания по переполнению 8-разрядного второго таймера, с целью 
 * минимизировать время обработки прерывания и всю управляющую деятельность 
 * вести в основной программе.
 * 
 * v1.0, 24.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.03.2024
**/

// Timer2 устанавливается на прерывание по переполнению. Поскольку таймер
// 8-битный, он может считать до максимального значения (2^8 – 1), или 255, 
// таким образом на 256 тике произойдет вызов ISR.
// При 16 МГц цикл выполняется 1/16 000 000 (16*10^6) или 0,0000000625 сек.
// Это означает что 256 отсчетов произойдут за 256*0,0000000625 = 0,000016 сек.

// Отсюда следует, что на одну секунду потребуется 1 / 0,000016 = 62500 
// таких прерываний от таймера.

#define LEDPIN 13

// Определяем счетчик прерываний от таймера и общее их количество до
// события переключения светодиода  
volatile unsigned int cntr;
unsigned int BtnToggle = 62499;

void setup()
{
   pinMode(LEDPIN, OUTPUT);
   cntr=0;
   cli(); // отключили прерывания

   // Инициализируем регистры Timer2
   TCCR2A = 0; 
   TCCR2B = 0;
   TIMSK2 = 0;
   // Включаем прерывание Timer2 по переполнению
   TIMSK2 = (1 << TOIE2);
   // Устанавливаем CS10 бит так, чтобы таймер работал при тактовой частоте:
   TCCR2B |= (1 << CS20);

   sei(); // включили прерывания
}

void loop() 
{
   if (cntr>BtnToggle)
   {
      cli();
      digitalWrite(LEDPIN, !digitalRead(LEDPIN));
      cntr=0;
      sei();
   }
}

ISR(TIMER2_OVF_vect)
{
   cntr=cntr+1;
}

// ************************************************************ simple2.ino ***