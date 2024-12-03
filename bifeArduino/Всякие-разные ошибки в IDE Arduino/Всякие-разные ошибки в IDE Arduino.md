## [Всякие-разные ошибки в IDE Arduino](#) 

```
c:/users/??????????/appdata/local/arduino15/packages/esp32/tools/esp-x32/2302/
  bin/../lib/gcc/xtensa-esp32-elf/12.2.0/../../../../xtensa-esp32-elf/bin/ld.exe: 
  C:\Users\615A~1\AppData\Local\Temp\arduino_build_749877\
  sketch\QueueHandlMulti.ino.cpp.o: in function `messQueueHandlMulti(int, String, String)':
  
C:\ittve-ESP32-CAM\04-Работаем с сообщениями через очередь\QueueHandlMulti/QueueHandlMulti.hpp:26: 
                         multiple definition of `messQueueHandlMulti(int, String, String)'; 
                         C:\Users\615A~1\AppData\Local\Temp\arduino_build_749877\sketch\QueMessage.cpp.o:
                         C:\ittve-ESP32-CAM\04-Работаем с сообщениями через очередь\QueueHandlMulti
                         /QueueHandlMulti.hpp:26: first defined here
c:/users/??????????/appdata/local/arduino15/packages/esp32/tools/esp-x32/2302/bin/../lib/gcc/xtensa-esp32-elf/12.2.0/../../../../
xtensa-esp32-elf/bin/ld.exe: C:\Users\615A~1\AppData\Local\Temp\arduino_build_749877\sketch\QueueHandlMulti.ino.cpp.o:C:\ittve-ESP32-CAM\04-
Работаем с сообщениями через очередь\QueueHandlMulti/QueMessage.h:69: multiple definition of `EmptyMessage'; 
C:\Users\615A~1\AppData\Local\Temp\arduino_build_749877\sketch\QueMessage.cpp.o:C:\ittve-ESP32-CAM\04-Работаем с сообщениями через очередь\QueueHandlMulti/QueMessage.h:69: first defined here
collect2.exe: error: ld returned 1 exit status
exit status 1
Ошибка компиляции для платы AI Thinker ESP32-CAM.

```
Ответ
```
https://stackoverflow.com/questions/17107786/arduino-library-multiple-definitions-of-a-function


13

Что ж, вы определили функцию в двух местах: один раз в Foo.cpp, где она включает в себя заголовок, и ещё раз в вашем эскизе, где она включает в себя заголовок. Заголовки C и C++ не предоставляют модульную систему, они просто вставляются вместо оператора include.

Либо объявите AAA его в заголовке, но определите в Foo.cpp (чтобы было только одно определение), либо помечайте его inline.

Поделиться
Улучшите этот ответ
Подписаться
ответил 14 июня 2013 года в 11:50
Аватар бесполезного пользователя
Бесполезный
67,3 тыс.66 золотых значков9797 серебряных значков139139 бронзовых значков
1
Да, это работает, спасибо! Я думал, что #ifndef Foo_H предотвращает многократное определение элементов. И разве inline не должен использоваться только для повышения производительности? 
–
Сюнь Ян
 Опубликовано14 июня 2013 года в 11:58
@XunYang декларация != определение 
–
хуанчопанца
 Опубликовано14 июня 2013 года в 12:01
@XunYang inline изменяет привязку для работы с функциями, которые определяются встроено. С точки зрения производительности, это всего лишь подсказка для современных компиляторов, так что я бы не стал считать это оптимизацией. 
–
Бесполезно
 Опубликовано14 июня 2013 года в 12:34
Этот совет — не вся история. Ключевое слово inline не имеет ничего общего с тем, определена ли функция в заголовочном файле. Ключевое слово inline указывает компилятору, что вы хотите, чтобы он вырезал тело функции и вставил его в то место, где эта функция вызывается. Ключевое слово inline приведёт к устранению вызова функции и возврата. Это сэкономит некоторое время выполнения за счёт передачи параметров и возврата значений, но может увеличить размер кода. Каждое место вызова функции будет заменено копией всего кода функции. 
–
jdr5ca
 Опубликовано15 июня 2013 года в 20:37 
@jdr5ca, это определённо больше не так. Так было раньше, но компиляторы не обязаны встраивать вызовы функций только потому, что вы используете ключевое слово inline, и они вполне могут встраивать вызовы даже без него. 
–
Бесполезно
 Опубликовано16 июня 2013 года в 19:50
```


### Библиография

#### [----Queue Management](https://www.freertos.org/Documentation/02-Kernel/04-API-references/06-Queues/00-QueueManagement)

###### [к содержанию](#%D0%B2%D0%B2%D0%B5%D0%B4%D0%B5%D0%BD%D0%B8%D0%B5---%D0%B1%D1%8B%D1%81%D1%82%D1%80%D0%BE%D0%B5-%D0%B7%D0%BD%D0%B0%D0%BA%D0%BE%D0%BC%D1%81%D1%82%D0%B2%D0%BE)
