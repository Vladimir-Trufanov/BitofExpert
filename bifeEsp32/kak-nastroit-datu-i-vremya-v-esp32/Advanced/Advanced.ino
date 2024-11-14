// Advanced.ino

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Вводим имя и пароль точки доступа
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Инициируем объект NTPClient, указываем российский публичный пул серверов 
// точного времени и смещение (в секундах, может быть изменено позже 
// с помощью setTimeOffset()). Дополнительно указываем интервал обновления 
// (в миллисекундах, может быть изменен с помощью setUpdateInterval()).
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.msk-ix.ru", 3600, 60000);

void setup()
{
  Serial.begin(115200);
  // Подключаемся к WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay (500);
    Serial.print (".");
  }
  Serial.println("");
  // Переопределяем смещение на 3 часа от "Гринвича"
  timeClient.setTimeOffset(3*60*60);
  // Запускаем NTPClient
  timeClient.begin();
}

// Обновляем и выводим время с интервалом в 1 сек
void loop() 
{
  timeClient.update();
  Serial.print("Время в Петрозаводске: ");
  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}
