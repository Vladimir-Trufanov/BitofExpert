## [Как сделать бегущую строку](https://github.com/Vladimir-Trufanov/BitofExpert/blob/main/bifePHP/skoncentrirovannye-svedeniya-o-phpini/skoncentrirovannye-svedeniya-o-phpini.md) [2025-01-08]

Да, когда-нибудь возникает необходимость сделать бегущую строку. Вариантов множество. Посмотрим несколько из них.

---

<style>
.titles 
{
   color:#7B68EE;
   background:transparent;
   font-size:1rem;  
   font-weight:bold;
}
</style>

<div class="titles">
- Первый, простейший, классический вариант. Когда-то, специально, был задуман для браузеров в HTML. Профессиональному сообществу CSS не очень понравился, но, в браузерах остался.
</div>

Вкладываем всё, что хочется, в тег ***\<marquee\>*** и оно поехало [https://htmlbook.ru/html/marquee/](https://htmlbook.ru/html/marquee/):

<marquee>
***Первый, простейший, классический вариант. Когда-то, специально, был задуман для браузеров в HTML. Профессиональному сообществу CSS не очень понравился, но, в браузерах остался.***
</marquee>

***HTML:***
```
<marquee>
   Бегущая строка, созданная при помощи HTML тега Marquee
   <img id="kwf" src="../Images/majskij-vecher-v-karelii_2416x503.jpg" alt="Kwinflat-близкий всем!" />
</marquee>
```
---

<div class="titles">
- Второй вариант [https://wpschool.ru/beguschaya-stroka-wordpress/](https://wpschool.ru/beguschaya-stroka-wordpress/). Текст размещается в диве, а анимация управляется в CSS. Такой вариант считается чистым и современным, 100% работающий в любом приложении, но требует некоторые познания CSS и HTML.
</div>

***PHP:***
```
echo '<div class="begush">';
echo 'Бегущая строка, размещенная в диве!';
echo '</div>';
```
***CSS:***
```
.begush 
{
   overflow: hidden;
   white-space: nowrap;
   animation: nazvanie-animacii 10s linear infinite normal;
}
@keyframes nazvanie-animacii 
{
   from { margin-left: 1px; }
   to { margin-left: 600px; }
}
```

<style>
.begush 
{
   overflow: hidden;
   white-space: nowrap;
   animation: nazvanie-animacii 10s linear infinite normal;
}
@keyframes nazvanie-animacii 
{
   from { margin-left: 1px; }
   to { margin-left: 600px; }
}
</style>

<div class="begush">
***Бегущая строка, размещенная в диве!***
</div>

---

<div class="titles">
- Третий вариант: [https://4xpro.ru/profblog/563/](https://4xpro.ru/profblog/563/) позволяет остановить бегущую строку курсором. 
</div>

***HTML:***
```
<div class="marquee">
<span>Бегущая строка, которую можно остановить курсором!</span>
</div>
```
***CSS:***
```
.marquee{text-align:left;width:100%;overflow:hidden;white-space: nowrap;}
.marquee span
{
   display:inline-block;width:100%;text-indent:0;
   animation: marquee 10s linear infinite
}
.marquee span:hover{animation-play-state: paused}
@keyframes marquee 
{
   0%   { transform: translate(100%, 0);  }
   100% { transform: translate(-100%, 0); }
}
@media only screen and (max-width: 440px) 
{
   .marquee span{animation:none;padding-left:0}
}
```

<style>
.marquee{text-align:left;width:100%;overflow:hidden;white-space: nowrap;}
.marquee span{display:inline-block;width:100%;text-indent:0;animation: marquee 15s linear infinite}
.marquee span:hover{animation-play-state: paused}
@keyframes marquee 
{
   0%   { transform: translate(100%, 0);  }
   100% { transform: translate(-100%, 0); }
}
@media only screen and (max-width: 440px) 
{
   .marquee span{animation:none;padding-left:0}
}
</style>

<div class="marquee">
<span>***Бегущая строка, которую можно остановить курсором!***</span>
</div>

---

<div class="titles">
- Четвертый вариант: [https://liondigital.ru/kak-sdelat-begushhuyu-stroku-v-css/](https://liondigital.ru/kak-sdelat-begushhuyu-stroku-v-css/), который хорошо объяснен в оригинале.
</div>

В этом примере элемент ***.marquee*** содержит бегущий текст, который находится внутри контейнера ***.marquee-container***. Контейнер ***.marquee-container*** имеет фиксированную высоту и свойство ***overflow***, имеющее значение ***hidden***, чтобы скрыть содержимое ***.marquee***, которое выходит за его границы.

Анимация определена с помощью ***@keyframes***. В начале анимации (***0%***), текст находится в начальном положении, а в конце (***100%***) текст сдвигается влево на ***100%*** от ширины элемента ***.marquee***.

С помощью ***animation*** анимация marquee применяется к элементу ***.marquee***, её длительность равна 20 секундам, ***linear*** определяет равномерную скорость перемещения текста, а ***infinite*** позволяет анимации циклически повторяться.

Чтобы текст перемещался горизонтально и не переносился на новую строку, свойство ***white-space*** установлено в значение ***nowrap***, а для сдвига содержимого внутри .marquee используется ***padding-right***, равное ***100%***.

***HTML:***

```
<div class="marquee-container">
  <div class="marquee2">
    <p>Четвертый вариант: 
    "https://liondigital.ru/kak-sdelat-begushhuyu-stroku-v-css/" 
    хорошо объяснен в оригиналеt.</p>
  </div>
</div>
```

***CSS:***

```
.marquee-container 
{
   overflow: hidden;
   height: 20px;
}
.marquee2 
{
   animation: marquee2 20s linear infinite;
   display: inline-block;
   white-space: nowrap;
   padding-right: 100%;
}
@keyframes marquee2 
{
   0% 
   {
      transform: translateX(0);
   }
   100% 
   {
      transform: translateX(-100%);
   }
}
```


<style>
.marquee-container 
{
   overflow: hidden;
   height: 20px;
}
.marquee2 
{
   animation: marquee2 20s linear infinite;
   display: inline-block;
   white-space: nowrap;
   padding-right: 100%;
}
@keyframes marquee2 
{
   0% 
   {
      transform: translateX(0);
   }
   100% 
   {
      transform: translateX(-100%);
   }
}
</style>


<div class="marquee-container">
<div class="marquee2">
<p>Четвертый вариант: [https://liondigital.ru/kak-sdelat-begushhuyu-stroku-v-css/](https://liondigital.ru/kak-sdelat-begushhuyu-stroku-v-css/) хорошо объяснен в оригиналеt.</p>
</div>
</div>

---



<div class="titles">
- Пятый вариант громоздкий, но интересный. Четвертый вариант: [https://liondigital.ru/kak-sdelat-begushhuyu-stroku-v-css/](https://liondigital.ru/kak-sdelat-begushhuyu-stroku-v-css/), который хорошо объяснен в оригинале.
</div>


