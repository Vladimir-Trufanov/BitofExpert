// AllOnSram.ino


void setup() 
{
   Serial.begin(115200);
   // Получаем полный размер кучи в памяти
   //printf("Переход в глубокий сон на %d секунд!\n", deep_sleep_sec);
   printf("Total heap: %u\n",    ESP.getHeapSize());
   // Возвращаем возвращает количество доступной кучи в памяти
   printf("Free heap: %u\n",     ESP.getFreeHeap());


   // uint32_t getMinFreeHeap();   //lowest level of free heap since boot = возвращает самый низкий уровень свободной кучи с момента загрузки
   printf("MinFreeHeap: %u\n",     ESP.getMinFreeHeap());
   //uint32_t getMaxAllocHeap();  //largest block of heap that can be allocated at once = размер самого большого блока кучи, который может быть выделен. 
   printf("MaxAllocHeap: %u\n",     ESP.getMaxAllocHeap());

   
   //  возвращает размер общей кучи SPI PSRAM
   printf("Total PSRAM: %u\n",   ESP.getPsramSize());
   // возвращает количество свободной PSRAM
   printf("Free PSRAM: %d\n",    ESP.getFreePsram());

   // uint32_t getMinFreePsram(); = минимальный размер свободной памяти в SPI RAM
   printf("MinFreePsram: %d\n",    ESP.getMinFreePsram());
   // uint32_t getMaxAllocPsram(); =  размер самого большого блока PSRAM, который может быть выделен
   printf("MaxAllocPsram: %d\n",    ESP.getMaxAllocPsram());

   // uint16_t getChipRevision();
   printf("getChipRevision: %d\n",    ESP.getChipRevision());
   uint8_t getChipCores();
   printf("getChipCores: %d\n",    ESP.getChipCores());
   uint32_t getCpuFreqMHz();
   printf("getCpuFreqMHz: %d\n",    ESP.getCpuFreqMHz());

   
   //const char *getChipModel();
   char buffer[60];
   sprintf(buffer,"getChipModel: (%s) !\n",  ESP.getChipModel());
   Serial.println(buffer);
 
   // const char *getSdkVersion();   //version of ESP-IDF
   sprintf(buffer,"getSdkVersion: (%s) такой !\n",  ESP.getSdkVersion());
   Serial.println(buffer);

   // const char *getCoreVersion();  //version of this core
   sprintf(buffer,"getCoreVersion: (%s) !\n",  ESP.getCoreVersion());
   Serial.println(buffer);


}

void loop() 
{

}