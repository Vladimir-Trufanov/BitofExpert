### [Восстановление программатора на CP2102 для Arduino Pro Mini](https://github.com/Vladimir-Trufanov/BitofExpert/blob/main/bifeArduino/vosstanovlenie-programmatora-na-cp2102-dlya-arduino-pro-mini.md)  [2024-04-22]

Так случилось, что после всяких-разных экспериментов с различными платами перестали прошиваться микросхемы Arduino Pro Mini с помощью USB-UART программатора на CP2102. 

Было очень приятно работать - соединяешь пять проводов программатора один в один, параллельно, с платой Pro Mini и отлаживаешь программу, сколько душе угодно, пока не добьёшься результата. Но, что-то сломалось и пришлось перейти на Usbasp.

Сейчас, очередной проект закончен, можно попытаться восстановить программатор.

Первое, что пришло в голову: "надо проверить исправность самого USB-UART преобразователя через терминальную программу". Воспользуюсь классикой - [***Terminal-ом v1.9b***](https://sites.google.com/site/terminalbpp/), который уже не раз выручал в разных делах.

Замкнул на преобразователе TX на RX, включил в USB-разъём, запустил программу Termina v1.9b, набрал произвольное сообщение, послал (Send), убедился, что сообщение пришло.

Следующий шаг ... 




### Библиография:

#### [Terminal 1.9b - сom port development tool (официальный сайт)](https://sites.google.com/site/terminalbpp/)

#### [Как пользоваться терминальной программой Terminal 1.9b](https://faq.radiofid.ru/knowledge-bases/2/articles/13-kak-polzovatsya-terminalnoj-programmoj-terminal-19b)

#### [Инструкция для ПО Terminal v1.9b by bray](https://ias.energotronika.ru/userHelp/instruktsiya_dlya_po_terminal_v1_9b_by_bray.htm)



