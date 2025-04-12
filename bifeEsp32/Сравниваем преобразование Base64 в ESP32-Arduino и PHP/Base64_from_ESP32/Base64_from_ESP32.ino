#include <Arduino.h>

// для проверки фотогр ------------------------------------------------------

// Инициируем объект преобразования изображения base64
#include <base64.h>
base64 b;


#include "OV2640.h"
#include "SD_MMC.h"            
#include <EEPROM.h>        

// Определяем число байт для хранения в постоянной памяти
#define EEPROM_SIZE 1
// Определяем пин вспышки
#define BUILTIN_LED 4
// Инициализируем переменную, обеспечивающую генерацию названий фотографий:
// picture1.jpg, picture2.jpg ...
int pictureNumber = 0;
// Инициируем объект для фотографирования
OV2640 cam;
// Определяем режим записи на SD
#define SD_MODE_NORMAL 4  // обычный по 4 контактам со вспышкой
#define SD_MODE_1BIT   1  // однобитовый, медленный, без вспышки

#define SD_MODE SD_MODE_1BIT  
// для проверки фотогр ------------------------------------------------------



#include <WiFi.h>
#include <HTTPClient.h>

// Определяем объект для синхронизации времени 
#include "AttachSNTP.h"
TAttachSNTP oSNTP;

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";




String readFile(String path) 
{
  char *str;
  uint8_t bufi[20000];
  //encode(const uint8_t *data, size_t length)


  Serial.printf("Reading file: %s\n", path);
  // ----Сохраняем фотографию на карту microSD
  fs::FS & fs = SD_MMC; 

  File file = fs.open(path);
  if (!file) 
  {
    Serial.println("Failed to open file for reading");
    return "Failed to open file for reading";
  }

  int rlen = file.available();
  Serial.print("Read from file, rlen = "); Serial.println(rlen);
  file.read(bufi, rlen); 
  String stringOne = b.encode(bufi, rlen);


  //bufi[rlen]=0;

  //char *str;
  //str = (char*)bufi;

  //String stringOne = String(str);
  //Serial.println(stringOne);

  /*
  while (file.available()) 
  {
    char ch = file.read();    // read the first character
    Serial.print(ch);
    //Serial.write(file.read());
  }
  */
  //Serial.println(" ");
  Serial.print("stringOne, len = "); Serial.println(stringOne.length());
  Serial.print("***"); Serial.print(stringOne); Serial.println("***"); 
  return stringOne;
}




void setup() 
{
  // put your setup code here, to run once:
  // Включаем последовательный порт
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("Последовательный порт работает!");

  // Подключаемся к Wi-Fi сети
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  Serial.print("Соединяемся с Wi-Fi.");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("К Wi-Fi сети подключились!");
  // Проверяем системное время, если время еще не установлено, производим его 
  // синхронизацию по протоколу SNTP с серверами точного времени,
  oSNTP.Create();

  
// для проверки фотогр ------------------------------------------------------

// Инициируем постоянную память заданного размера
  EEPROM.begin(EEPROM_SIZE);
  // Назначаем номер первой сохраняемой фотографии
  EEPROM.write(0, 254);
  EEPROM.commit();
  Serial.println("Инициирована постоянная память\n");

  Serial.println("Подключаем SD-карту");
  // Настраиваем обычный или однобитовый режим работы
  #if (SD_MODE==SD_MODE_NORMAL)
    if(!SD_MMC.begin())
  #else
    if (!SD_MMC.begin("/sdcard", true))
  #endif
  {
    Serial.println("SD-карта не смонтирована");
    return;
  }
  else Serial.println("SD карта смонтирована");
  // Гасим вспышку в однобитовом режиме
  #if (SD_MODE==SD_MODE_1BIT)
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
  #endif 
  // Подключаем устройство
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE)
  {
    Serial.println("SD карта не подключена");
    return;
  }
  else Serial.println("SD карта подключена");
  
  // Инициируем камеру 
  cam.init(esp32cam_aithinker_config);
  // Определяем дополнительную задачу
  xTaskCreatePinnedToCore (
    instream,       // название функции, которая будет запускаться, как параллельная задача
    "instream",     // название задачи
    48480,          // размер стека в байтах. Задача будет использовать этот объем памяти, когда 
                    // ей потребуется сохранить временные переменные и результаты. Для задач с 
                    // большим объемом памяти потребуется больший размер стека.
    NULL,           // указатель на параметр, который будет передан новой задаче. 
                    // NULL, если параметр не передаётся.
    9,              // приоритет задачи
    NULL,           // дескриптор или указатель на задачу. Его можно использовать для вызова задачи.
                    // Если это не требуется, то NULL.
    1               // идентификатор ядра процессора, на котором требуется запустить задачу. 
                    // У ESP32 есть два ядра, обозначенные как 0 и 1.
  );

}

void loop() 
{
  // put your main code here, to run repeatedly:

}


// ****************************************************************************
// *              Выполнять фотографирование в некоторых циклах               *
// * !!! Если задача завершится (не будет циклится),контроллер перезагрузится *
// ****************************************************************************
void instream (void* pvParameters) 
{

  time_t nTime;      // время отправки фрэйма (секунда с начала эпохи)
  time_t nTimeOld;   // время отправки предыдущего фрэйма
  int nFrame=0;      // номер кадра в секунде

  // Инициируем метки времени
  time(&nTime);                    // время отправки фрэйма (секунда с начала эпохи)
  time(&nTimeOld);                 // время отправки предыдущего фрэйма

  while (1) 
  {
    // Пересчитываем время и номер кадра
    time(&nTime); 
    if (nTime==nTimeOld) nFrame=nFrame+1;
    else {nTimeOld=nTime; nFrame=0;}

    cam.run();
    size_t SizeFR = cam.getSize();
    Serial.println("Фото сделано!");

  //Serial.print("***");
  String fil1 = readFile("/fil1.jpg"); 
  //String fil1 = readFile("/Arduino.txt"); 
  //String fil2 = b.encode(fil1);
  //Serial.print(fil2);
  //Serial.println("***");

    //readFile("/fil1.jpg"); 


/*
  String fil1 = readFile("/fil1.jpg"); 
  
  Serial.print("***");
  Serial.print(fil1);
  */

//Serial.println("***");


    /*

    //const char * bb = callphoto(cam.getfb(),SizeFR);
    String imgname = callphoto(cam.getfb(),SizeFR);
    Serial.print("imgname = "); Serial.println(imgname);

    //sendhttp(nTime, nFrame, *bb);
    //sendhttp(nTime, nFrame, "/vga640x480.jpg");
    sendhttp(nTime, nFrame, imgname);
    //sendhttp(nTime, nFrame, "/fil1.jpg");
    //sendhttp(nTime, nFrame, callphoto(cam.getfb(),cam.getSize()));
    */
    vTaskDelay(1200/portTICK_PERIOD_MS);
  }
}
