/** Arduino-Esp32-CAM                                  *** Rus32CAMExt6.ino ***
 *                          
 *                  Пример расщепления uint32_t на составляющие с помощью union
 * 
 * v1.0.1, 24.02.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 20.02.2025
**/

typedef union
{
  // 32-разрядное целочисленное беззнаковое представление переменной
  uint32_t v;         
  // Разбиение значения на тетрады
  struct 
  {
    unsigned v00:4;
    unsigned v01:4;
    unsigned v10:4;
    unsigned v11:4;
    unsigned v20:4;
    unsigned v21:4;
    unsigned v30:4;
    unsigned v31:4;
  }
  as_nibbles;
  // Представление значение через массив байт
  uint8_t as_bytes[4];
}
uint32_split_t, *uint32_split_p;  // через указатель

// Выделяем переменную для расщепления
static uint32_split_t product;

void setup() 
{
  Serial.begin(115200);
  // Инициализируем переменную начальным значением
  product.v = 0x01010101; 
}

void loop()
{
  // Меняем значение и расщепляем
  product.v++;
  uint8_t va = product.as_bytes[0];
  uint8_t vc = product.as_nibbles.v21;
  // Выводим значения после расщепления
  Serial.print("PORTA = "); Serial.println(va);
  Serial.print("PORTС = "); Serial.println(vc);
  Serial.print("product.as_nibbles.v31 = "); Serial.println(product.as_nibbles.v31);
  Serial.print("product.as_nibbles.v00 = "); Serial.println(product.as_nibbles.v00);
  delay(2000); 
}
// ******************************************************* Rus32CAMExt6.ino ***

