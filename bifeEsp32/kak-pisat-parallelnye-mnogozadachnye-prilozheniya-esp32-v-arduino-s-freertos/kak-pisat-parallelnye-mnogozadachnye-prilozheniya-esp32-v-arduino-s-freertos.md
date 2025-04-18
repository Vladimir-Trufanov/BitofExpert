### [Как писать параллельные многозадачные приложения ESP32 в Arduino с FreeRTOS](https://github.com/Vladimir-Trufanov/BitofExpert/blob/main/bifeEsp32/kak-pisat-parallelnye-mnogozadachnye-prilozheniya-esp32-v-arduino-s-freertos/kak-pisat-parallelnye-mnogozadachnye-prilozheniya-esp32-v-arduino-s-freertos.md) [2024-10-16]

ESP32 - это автономная электронная плата на двух процессорах от Espressif, выполняющая функции целого устройства (контроллера - SoC) и размещённая на одной интегральной схеме с установленной многозадачной операционной системой FreeRTOS. 

ESP32 — это двухъядерный SoC с высокой тактовой частотой и большим объёмом памяти. Чтобы в полной мере воспользоваться этими возможностями, обычного метода последовательного программирования недостаточно. 

> Последовательное программирование или выполнение — это когда одна задача выполняется, а все остальные задачи ожидают завершения выполняемой задачи.

Последовательное программирование может обеспечить  выполнение нескольких задач одновременно, как если бы каждая задача выполнялась параллельно и независимо друг от друга, если все происходит достаточно быстро, но есть реальные преимущества в использовании «истинного параллелизма» с несколькими ядрами. 

Для ESP32 в среде Arduino можно писать настоящие параллельные приложения благодаря отличной поддержке инженеров Espressif. 

В представленном материале показаны примеры многозадачных скетчей для контроллера AI-Thinker ESP32-CAM на основании оригинальной статьи [***How to Write Parallel Multitasking Applications for ESP32 using FreeRTOS & Arduino***](https://www.circuitstate.com/tutorials/how-to-write-parallel-multitasking-applications-for-esp32-using-freertos-arduino/).

---

#### [Нaписание параллельных задач](#%D0%BD%D0%B0%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

#### [Какoе ядро использовать?](#%D0%BA%D0%B0%D0%BA%D0%BE%D0%B5-%D1%8F%D0%B4%D1%80%D0%BE-%D0%B8%D1%81%D0%BF%D0%BE%D0%BB%D1%8C%D0%B7%D0%BE%D0%B2%D0%B0%D1%82%D1%8C)

#### [Совместное использoвание ресурсов](#%D1%81%D0%BE%D0%B2%D0%BC%D0%B5%D1%81%D1%82%D0%BD%D0%BE%D0%B5-%D0%B8%D1%81%D0%BF%D0%BE%D0%BB%D1%8C%D0%B7%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5-%D1%80%D0%B5%D1%81%D1%83%D1%80%D1%81%D0%BE%D0%B2)

#### [Синхрoнизация задач мьютексом](#%D1%81%D0%B8%D0%BD%D1%85%D1%80%D0%BE%D0%BD%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D1%8F-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87-%D0%BC%D1%8C%D1%8E%D1%82%D0%B5%D0%BA%D1%81%D0%BE%D0%BC)

#### [Синхронизaция задач семафором](#%D1%81%D0%B8%D0%BD%D1%85%D1%80%D0%BE%D0%BD%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D1%8F-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87-%D1%81%D0%B5%D0%BC%D0%B0%D1%84%D0%BE%D1%80%D0%BE%D0%BC)

#### [Критические сeкции FreeRTOS](#%D0%BA%D1%80%D0%B8%D1%82%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B8%D0%B5-%D1%81%D0%B5%D0%BA%D1%86%D0%B8%D0%B8-freertos)

---
#### Написание параллельных задач

В базовой программе Arduino есть как минимум две функции или задачи: setup() и loop(). Эти функции представляют собой две логические конструкции, которые помогают нам реализовать логику нашей прикладной программы. Функция setup() запускается только один раз после включения питания и инициализирует все необходимые нам контакты и интерфейсы. Функция loop() с другой стороны может содержать последовательность инструкций, которая будет перезапускаться после завершения итерации/цикла. 

Добавить новую задачу, которая будет выполняться параллельно с другими задачами во FreeRTOS, очень просто. 

В следующем примере запускаются две задачи, которые работают одновременно во внутренней операционной системе FreeRTOS для ESP32. Первая задача мигает контрольным светодиодом, а вторая выводит в последовательный порт традиционный текст "Всем привет!".

Первая задача - это обычный цикл loop() в Ардуино. Вторая задача - это функция loop2(), имеющая единственный аргумент, ***void*** * ***pvParameters***, который мы нигде внутри функции не используем. Эта часть аргумента обязательна для любой функции, установленной для выполнения, как задача. Параметр представляет собой тип void указателя с именем pvParameters. Это название не обязательно должно быть таким, оно может быть любым. Но для наглядности оставьте название как есть. Тип возвращаемого значения функции задачи всегда должен быть void. В противном случае это приведет к ошибке компиляции. У задачи  loop() по умолчанию нет никаких аргументов, так как она автоматически инкапсулируется препроцессором (приложением, которое сканирует и перестраивает ваш код) перед его компиляцией.

***Следует также отметить наличие цикла while внутри loop2(). Когда мы преобразуем функцию в задачу, мы просим операционную систему только запустить её, а не выполнять многократно. Поэтому нам нужен бесконечный цикл внутри функции loop2(). Без цикла while задача loop2() будет выполнена только один раз, а затем выполнит перезагрузку контроллера. 
***

Причина, по которой loop() по умолчанию не содержит такого цикла, заключается в том, что среда Arduino уже управляет им.

#### [Базовый пример параллельной работы двух задач в ESP32 board 5.1.33  (на контроллере AI-Thinker ESP32-CAM)](ex5-1-33-twousoneBlink/ex5-1-33-twousoneBlink.ino)

###### [к содержанию](#%D0%BDa%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

#### Какое ядро использовать

Поскольку у Esp32 есть два ядра, то возникает вопрос, какое ядро, как используется и для каких задач.  По умолчанию - код Arduino работает на ядре 1, а функции Wi-Fi и радиочастот (обычно скрытые в среде Arduino) используют ядро 0. Чтобы узнать, на каком ядре выполняется ваша текущая задача, вы можете использовать функцию ***xPortGetCoreID()***. При этом будет выведен идентификатор ядра, на котором выполняются текущая функция. 

```
#define LED_BUILTIN 33
// the setup function runs once when you press reset 
// or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode (LED_BUILTIN, OUTPUT);

  Serial.begin (115200);

  xTaskCreatePinnedToCore (
    loop2,     // Function to implement the task
    "loop2",   // Name of the task
    1000,      // Stack size in words
    NULL,      // Task input parameter
    0,         // Priority of the task
    NULL,      // Task handle.
    0          // Core where the task should run
  );
}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite (LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay (1000);                     // wait for a second
  digitalWrite (LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  delay (1000);                     // wait for a second
  Serial.print ("loop() running in core ");
  Serial.println (xPortGetCoreID());
}

// the loop2 function also runs forver but as a parallel task
void loop2 (void* pvParameters) 
{
  while (1) {
    Serial.print ("Hello");
    delay (500); // wait for half a second
    Serial.print (" World from loop2() at ");
    Serial.println (xPortGetCoreID());
    delay (500); // wait for half a second
  }
}
```
Решение о том, какое ядро использовать, зависит от того, какая часть рабочей нагрузки выполняется на этом ядре. Для работы с радиочастотами на ESP32 SoC требуется программное обеспечение, чувствительное ко времени и основанное на прерываниях, которое может быть сложным. Поскольку ядро 0 уже используется для этих задач, всегда лучше использовать второе ядро 1. Но это применимо только в том случае, если вы используете некоторые функции радиочастот, такие как Wi-Fi или BLE. Если вам не нужны какие-либо функции, выберите те ядра, которые вам нравятся.

В приведенном примере можно попробовать изменить ядро с loop2() на 1. Это позволит loop() и loop2() работать на одном ядре. 

###### [к содержанию](#%D0%BDa%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

#### Совместное использование ресурсов

![Внутренняя блок-схема ESP32. Источник: Espressif](CIRCUITSTATE-Electronics-01.png)

Внутренняя блок-схема SoC ESP32 показывает, что имеются два (или 1 для некоторых вариантов) 32-разрядных микропроцессорных ядра Xtensa LX6 внутри SoC. Но все остальные ресурсы являются общими. Например, оба ядра используют одно и то же пространство ROM и SRAM. Это также верно для других периферийных устройств. Оба ядра могут обращаться к любым периферийным устройствам, к которым захотят. Но если это так, то не возникнут ли конфликты, если одно ядро попытается выполнить какую-то операцию с одним периферийным устройством, а другое ядро попытается сделать что-то еще с тем же самым? 

Да, такая ситуация может возникнуть в многоядерной среде. По этой причине разработчик кода несёт ответственность за написание бесконфликтного кода при разработке приложений, требующих параллельных задач, выполняемых на нескольких ядрах. Существует достаточно инструментов и методик, которые помогут вам написать чистый и эффективный код для ваших многозадачных приложений.

Одним из примеров конфликта при совместном использовании ресурсов в многоядерной среде является состояние гонки, возникающее при доступе к памяти. Предположим, у нас есть ячейка памяти с адресом 0xF00. Мы можем написать две параллельные задачи; обе они будут пытаться записать случайное число в это место и прочитать его позже. Что-же происходит, когда одна задача записывает значение, а в то же время другая задача пытается сделать то же самое? Какая задача получает возможность записи в ячейку памяти? Ответ не определен. Это означает, что мы не можем предсказать, какая из задач сделает запись в ячейку памяти.

Условия гонки приводят к появлению странных ошибок, которые могут не обнаруживаться при обычном тестировании. Поэтому всякий раз, когда вам нужно разделить переменную, объект, функцию или периферийное устройство между задачами, убедитесь, что только одна задача обращается к ресурсу одновременно. 

Кроме этого задача доступа должна оставить ресурс в определенном состоянии после выполнения операции. 

###### [к содержанию](#%D0%BDa%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

#### Синхронизация задач мьютексом

Всякий раз, когда нам нужно поделиться ресурсом или заставить две задачи взаимодействовать друг с другом, нам нужен механизм, предотвращающий одновременный доступ обеих задач к ресурсу и его изменение. 

Когда задача обращается к ресурсу, нам нужно сообщить всем другим задачам, что ресурс используется в данный момент. Это называется синхронизацией задач. Для достижения этой цели есть два метода: мьютекс и семафор. 

Мьютекс расшифровывается как “взаимное исключение”, это просто общая переменная, доступ к которой всегда осуществляется с помощью атомарных операций. 

> ***Атомарная операция - это способ выполнения последовательности инструкций без каких-либо прерываний до завершения всей последовательности ***

Переменная мьютекса может оставаться в двух состояниях: заблокирована и разблокирована (0 или 1). Можно рассматривать её, как логическую переменную. Она используется для защиты общего ресурса от одновременного доступа к нему нескольких задач. 

Задача может получить блокировку мьютекса, вызвав функцию с именем acquire(). После получения блокировки задача может продолжить работу с объектом, защищенным мьютексом, не беспокоясь о том, что другие задачи изменят ресурс. Если какие-либо другие задачи попытаются получить блокировку мьютекса, когда он уже заблокирован, то они перейдут в состояние ожидания, называемое занятым ожиданием. Процессы, пытающиеся получить блокировку, будут многократно пытаться получить ее, пока не добьются успеха. Занятое ожидание также называется спин-блокировкой.

![Иллюстрация функциональности мьютекса](CIRCUITSTATE-Electronics-01-1280x751.png)

Задача, которая изначально получила блокировку, освободит её, когда завершит все свои операции. Освободить блокировку мьютекса можно с помощью release(). Теперь другие задачи, ожидающие получения блокировки, могут получить её с помощью acquire(). Acquire() и release() - это общие названия функций, используемые для объяснения концепции, а фактические названия могут зависеть от платформы.

#### [Скетч "МЬЮТЕКС: основной цикл мигает контрольным светодиодом, а две другие задачи делят между собой с помощью мьютекса счетчик и последовательный порт"](ex5-1-33-twousoneMutex/ex5-1-33-twousoneMutex.ino)

###### [к содержанию](#%D0%BDa%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

#### Синхронизация задач семафором

Другой тип синхронизации задач называется семафором и представляет собой метод передачи сигналов. Разница между семафором и мьютексом заключается в том, что семафор действует как сигнал, например, световая индикация, а мьютекс — это блокировка, аналогичная навесному замку с ключом. 

Семафор также является переменной, совместно используемой задачами. Но в отличие от мьютексов, он может находиться в более чем двух состояниях. Семафоры реализуются двумя способами: счётные семафоры и двоичные семафоры. 

Семафор-счётчик — это целочисленная переменная, значение которой инициализируется количеством экземпляров определённого ресурса, доступ к которому осуществляется совместно. Например, если у вас есть 10 экземпляров структуры данных, то семафор инициализируется значением 10. Это означает, что все 10 структур в начале свободны. 

Затем задача может запросить один экземпляр этих данных и уменьшить значение переменной семафора. Значение семафора станет равным 9. Когда все экземпляры заняты задачами, семафор становится равным 0. Когда задача завершает использование ресурса, она может освободить ресурс и увеличить семафор на 1. Если задача запрашивает доступ, когда семафор равен 0, она может либо подождать, пока не освободится хотя бы один ресурс, либо выполнить некоторые другие операции, не тратя время на ожидание.

![Пример семафора. Несколько задач пытаются получить доступ к трём экземплярам ресурса. Доступ получают только первые три задачи, а остальные находятся в состоянии ожидания](CIRCUITSTATE-Electronics-01-1280x556.png)

Двоичный семафор, как следует из названия, может принимать только одно значение или находиться в одном из двух состояний (0 и 1). Разница между двоичным семафором и мьютексом заключается в том, что двоичный семафор служит только для передачи сигнала о том, используется ресурс или нет, а мьютекс блокирует ресурс для других задач. Оба варианта имеют разные области применения. 

Концепции мьютекса и семафора:

```
Мьютекс — это блокировка взаимного исключения, которая используется, когда несколько задач конкурируют за один ресурс.

Мьютекс может находиться только в двух состояниях: заблокирован и разблокирован.

Мьютекс заставляет запрашивающие задачи “вращаться” - опрашивать мьютекс и ждать до тех пор, пока не будет доступен ресурс.

Семафор — это метод передачи сигналов. Задачи могут свободно заниматься чем-то другим, не тратя время на ожидание.

Семафоры могут находиться в N состояниях, максимальное значение указывает на общее количество доступных ресурсов.

Семафоры отслеживают использование ресурсов и хорошо подходят для данных, которые можно разделить или использовать в нескольких экземплярах.

Семафор не предоставляет исключительный доступ к ресурсу, а вместо этого предоставляет доступ к одному из множества экземпляров ресурса.

Двоичный семафор указывает на доступность только одного экземпляра ресурса.
```

#### [Скетч "Двоичный семафор: основной цикл мигает контрольным светодиодом, а две другие задачи делят между собой с помощью мьютекса счетчик и последовательный порт"](ex5-1-33-twousoneSemaphore/ex5-1-33-twousoneSemaphore.ino)

###### [к содержанию](#%D0%BDa%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

#### Критические секции FreeRTOS

Ход исполнения любых последовательных или параллельных задач, которые крутятся в контроллере всегда может быть остановлен для обработки прерывания. После обработки прерывания ситуация внутри прерванной задачи может измениться, что повлечет за собой неправильное исполнение дальнейшего кода в задаче. 

Для решения этой проблемы, FreeRTOS позволяет нам определять критически важные фрагменты кода с помощью макросов taskENTER_CRITICAL() и taskEXIT_CRITICAL(), которые все-таки не прерываются при возникновении прерывания и выполняются полностью. 

> Для критических фрагментов кода действует тоже правило, что и для программ обработки прерывания - фрагмент кода должен быть максимально коротким, вся аналитическая обработка должна быть вынесена во вне. 

Вызов ***taskENTER_CRITICAL()*** должен осуществляться непосредственно перед критически важным фрагментом кода, а ***taskEXIT_CRITICAL()*** после его выполнения. Ещё раз: следует всегда следить за тем, чтобы код критического раздела был как можно короче. В противном случае это негативно скажется на времени отклика на прерывание.

#### [Пример использования критических секций в ESP32 board 5.1.33 (на контроллере AI-Thinker ESP32-CAM)](ex5-1-33-twousonePortCritical/ex5-1-33-twousonePortCritical.ino)

###### [к содержанию](#%D0%BDa%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

### Библиография

#### [Использование семафоров и мьютексов в FreeRTOS на Arduino Uno](https://microkontroller.ru/arduino-projects/ispolzovanie-semaforov-i-myuteksov-v-freertos-na-arduino-uno/)

В данной статье показан пример управления семафором из прерывания.

###### [к содержанию](#%D0%BDa%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5-%D0%BF%D0%B0%D1%80%D0%B0%D0%BB%D0%BB%D0%B5%D0%BB%D1%8C%D0%BD%D1%8B%D1%85-%D0%B7%D0%B0%D0%B4%D0%B0%D1%87)

