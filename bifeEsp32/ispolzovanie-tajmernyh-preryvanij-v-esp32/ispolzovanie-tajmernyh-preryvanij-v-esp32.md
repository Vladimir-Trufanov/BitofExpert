## [Использование таймерных прерываний в Esp32](#)

В материале представлен перевод документации ESPRESSIF по таймерам [https://docs.espressif.com/projects/arduino-esp32/en/latest/api/timer.html](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/timer.html).

В контроллерах ESP32 содержится от 2 до 4 аппаратных таймеров. Все они представляют собой 64-битные (54-битные для ESP32-C3) универсальные таймеры на основе 16-битных предварительных масштабирующих устройств и 64-битных (54-битных для ESP32-C3) счётчиков с возможностью автоматической перезагрузки.

```
Контроллер ESP32       Количество таймеров
------------------------------------------
ESP32                  4
ESP32-S2               4
ESP32-S3               4
ESP32-C3               2
ESP32-C6               2
ESP32-H2               2
```

#### [Пример таймера повтора](RepeatTimer/RepeatTimer.ino)



### Библиография

#### [----Использование семафоров и мьютексов в FreeRTOS на Arduino Uno](https://microkontroller.ru/arduino-projects/ispolzovanie-semaforov-i-myuteksov-v-freertos-na-arduino-uno/)



###### [в начало](#kvizzy)

0

1

2

3

4

5

6

7

8

9

0

1

2

3

4

5




