// Strob610-4.ino
// Увеличиваем интервал стробирования и вводим тайм-аут для импульса
// Более устойчивый прием сигнала, но не всегда повторяется при нажатии на одну и ту же клавишу
// Надо пробовать дальше

const int ir = 2;
int t_low = 600+10; // длительность "0" (с запасом), мкс
int t_max = t_low * 4; // таймаут, мкс

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ir, INPUT);
}

void loop() {
  int r = digitalRead(ir);
  digitalWrite(LED_BUILTIN, r!=HIGH);
  // если зафиксировали ИК излучение, обрабатываем команду пульта
  if (r==LOW) {
    precess_ir();
  }
}

// читает пакет ИК пульта
void precess_ir() {
  delay(13); // пропустим стандартное начало пакета
  byte bits[100];
  for (int i=0; i<100; i++){
    int bit = readBit();
    bits[i] = bit;
  }
  for (int i=0; i<100; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
}

// читает 1 бит пакета
int readBit() {
  int r1;
  do {
    r1 = digitalRead(ir);
  } while (r1 != HIGH); // ждём передний фронт импульса
  int t1 = micros(); // запоминаем время начала импульса

  int t2;
  int t; 
  do {
    r1 = digitalRead(ir);
    t2 = micros(); // запоминаем время опроса (оно же длительность импульса)
    t = t2 - t1; // длительность импульса
  } while ((r1 != LOW) && (t < t_max)); // ждём задний фронт импульса, но не больше таймаута
  //Serial.println(t); // можно вывести длительность импульса
  
  if (t < t_low) {
    return 0;
  }
  else {
    return 1;
  }
}
