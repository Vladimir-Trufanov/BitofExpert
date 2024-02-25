/* ========================================================================= */
/* PointersAndLinks.ino                                                      */
/* (c) 2024-02-03 Труфанов Владимир                                          */
/*                                                                           */
/*                                   Примеры работы с указателями и ссылками */
/* ========================================================================= */

/**
 * Запрограммировать:
 * 
 * const char *ssid = "YourAp"; 
 * const char *password = ""; //"ax93k6kq6wf";
 * 
 * bool isWiFi;
 * if (*password == "") isWiFi=WiFi.softAP(ssid)
 * else isWiFi=WiFi.softAP(ssid, password);
 * if (!isWiFi) 
 * {
 * }
 * 
**/

   
void pArCharShow(char *pabc, int asize)
{
   for (int i=0; i<asize; i++)
   {
      Serial.print(*pabc++); 
   }
   Serial.println(' ');
}



void setup() 
{
   delay(1000);

   Serial.begin(115200);
   Serial.println();

   int a = 5; 
   char str[] = "World = Мир!"; 
   double nn = 1.01;
   int asize; 
   int y;

   int arr[]= {1, 2, 3, 4, 5, 6, 8};
   int *parr = &arr[0];
   asize = sizeof(arr)/sizeof(arr[0]); //length calculation 
   ArIntShow(arr,asize);
   pArIntShow(parr,asize);

   char abc[] = "Привет, Мир and Arduino!";
   char *pabc = &abc[0];
   asize = sizeof(abc)/sizeof(abc[0]); 
   ArCharShow(abc,asize);
   pArCharShow(pabc,asize);

   char *password = ""; //"***345ERTY"; //"ax93k6kq6wf";
   //*password = ""; //"ax93k6kq6wf";

   int iy = *password;
   Serial.print("Value   of iy = ");  Serial.println(iy);
   Serial.print("Address of iy = ");  Serial.println(int(&iy)); Serial.println("");
   char cy = *password;
   Serial.print("Value   of cy = ");  Serial.println(cy);
   Serial.print("Address of cy = ");  Serial.println(int(&cy)); Serial.println("");

   //asize = sizeof(password)/sizeof(password[0]); 
   //Serial.print("Value   of asize = ");  Serial.println(asize);
   
   //if (char(*password) == "") Serial.println("Нет пароля!");
   //if (char(*password) == null) Serial.println("Нет пароля!");

   
   //else Serial.println("Пароль есть!") ;




   
   
   y = 0;

   int x;
   x = 13;
   Serial.print("Value   of x = ");  Serial.println(x);
   Serial.print("Address of x = ");  Serial.println(int(&x)); Serial.println("");

   int *p;
   p = &x;
   Serial.print("Value   of p = ");  Serial.println(int(p));
   Serial.print("Address of p = ");  Serial.println(int(&p)); Serial.println("");

   y = *p;
   Serial.print("Value   of y = ");  Serial.println(y);
   Serial.print("Address of y = ");  Serial.println(int(&y)); Serial.println("");

   /*
   char *ssidi = "YourAp";
   char c[] = "MyAp";
   //p = &c[0];
   Serial.print("Value   of c = ");  Serial.println(c);
   //Serial.print("Address of c = ");  Serial.println(int(p)); Serial.println("");
   //Serial.print("Address of c = ");  Serial.println(int(&c)); Serial.println("");
  */
}

int PrintStr(char str) 
{ 
  Serial.print("Hello "); Serial.println(str);
}

void ArIntShow(int arr[], int asize)
{
   for (int i=0; i<asize; i++)
   {
      Serial.print(arr[i]); Serial.print(',');
   }
   Serial.println(' ');
}

void pArIntShow(int *parr, int asize)
{
   for (int i=0; i<asize; i++)
   {
      Serial.print(*parr++); Serial.print(',');
   }
   Serial.println(' ');
}

void ArCharShow(char abc[], int asize)
{
    for (int i=0; i<asize; i++)
    {
       Serial.print(abc[i]);
    }
    Serial.println(' ');
}

void loop() 
{
}
