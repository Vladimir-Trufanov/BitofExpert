## [Все про SPI Flash - файловую систему флеш-памяти Esp32](#)

> ***!!! про Fat, Spiffs, LittleFS***

SPI Flash в ESP32 — это файловая система флеш-памяти, подключаемой по последовательному периферийному интерфейсу (SPI).

Она позволяет получать доступ к флеш-памяти, как к обычной файловой системе на компьютере, но с упрощёнными возможностями и ограничениями. В SPIFFS можно читать, писать, закрывать и удалять файлы. 

Использование SPIFFS с ESP32 полезно для создания файлов конфигураций с настройками, постоянного сохранения данных, сохранения небольших объёмов данных вместо использования карты microSD, а также для хранения файлов HTML и CSS для создания веб-сервера. 

SPI Flash также предоставляет прямой доступ к флеш-памяти для операций чтения и записи по любому адресу в пространстве памяти. 

```
// https://arduino.stackexchange.com/questions/74668/checking-size-in-flash-memory-esp32


#include <SPIFFS.h>

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  // Initialize the SPIFFS file system
  if (!SPIFFS.begin(true)) {
    Serial.println("Initialization failed!");
    return;
  }
  
  // size of the compiled program
  uint32_t program_size = ESP.getSketchSize();
  
  // size of the file system
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  uint32_t file_system_size = fs_info.totalBytes;
  
  // used size of the file system
  uint32_t file_system_used = fs_info.usedBytes;
  
  // free size in the flash memory
  uint32_t free_size = ESP.getFlashChipSize() - program_size - file_system_size + file_system_used;
  
  Serial.print("Program size: ");
  Serial.print(program_size);
  Serial.println(" bytes");
  
  Serial.print("File system size: ");
  Serial.print(file_system_size);
  Serial.println(" bytes");
  
  Serial.print("File system used: ");
  Serial.print(file_system_used);
  Serial.println(" bytes");
  
  Serial.print("Free space: ");
  Serial.print(free_size);
  Serial.println(" bytes");
}

void loop() {
  // Do Nothing Here
}
```

---


### [Настройка таблицы разделов FLASH-памяти для ESP32](https://kotyara12.ru/iot/esp32_flash/)

> Что делать после перезагрузки или выключения и включения устройства? Где хранить последние установленные значения на ESP?
> 
> Нужно воспользоваться свободным пространством на встроенном в модуль ESP разделе flash-памяти - NVS.



---


### Библиография

#### [---Типы памяти ESP-IDF ESPRESSIF 5.3.1](https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32/api-guides/memory-types.html#memory-layout)

#### [---ESP32 Programmers’ Memory Model](https://developer.espressif.com/blog/esp32-programmers-memory-model/)

#### [---Распределение памяти в ESP32](https://kotyara12.ru/iot/esp32_memory/?ysclid=m3li0vnd5z57164113)

###### [в начало](#%D0%B2%D1%81%D0%B5-%D0%BF%D1%80%D0%BE-%D0%BF%D0%B0%D0%BC%D1%8F%D1%82%D1%8C-esp32)
