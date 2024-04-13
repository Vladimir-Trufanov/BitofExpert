
/** Arduino UNO ******************************************* *** PersonInfo.ino ***
 * 
 * -----Обеспечить мигание светодиода с интервалом в 1 сек (1Гц) на плате через 
 * прерывания 16-тиразрядного первого таймера - Timer1 по событию совпадения 
 * с регистром сравнения (то есть в режиме CTC)
 * 
 * v1.0, 28.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 28.03.2024
**/

struct Person {
  char name[50];  // Character array to store the name
  int age;       // Integer to store the age
  float salary;  // Float to store the salary
};

void printPersonInfo(const char* personName, const Person& person) {
  Serial.print(personName);
  Serial.println(":");
  Serial.print("Name: ");
  Serial.println(person.name);
  Serial.print("Age: ");
  Serial.println(person.age);
  Serial.print("Salary: $");
  Serial.println(person.salary);
  Serial.println();
}

void setup() {
  // Start the serial communication at a baud rate of 115200
  Serial.begin(115200);

  // Create three Person instances and populate their data
  Person person1, person2, person3;

  strcpy(person1.name, "John");
  person1.age = 30;
  person1.salary = 45000.0;

  strcpy(person2.name, "Alice");
  person2.age = 25;
  person2.salary = 63500.0;

  strcpy(person3.name, "Bob");
  person3.age = 33;
  person3.salary = 55000.0;

  // Calculate the average age and salary
  int totalAge = person1.age + person2.age + person3.age;
  float totalSalary = person1.salary + person2.salary + person3.salary;
  int numPeople = 3;
  float averageAge = totalAge / numPeople;
  float averageSalary = totalSalary / numPeople;

  // Print out the data for each person using the printPersonInfo function
  printPersonInfo("Person 1", person1);
  printPersonInfo("Person 2", person2);
  printPersonInfo("Person 3", person3);

  // Print the average age and salary
  Serial.print("Average Age: ");
  Serial.println(averageAge);
  Serial.print("Average Salary: $");
  Serial.println(averageSalary);
}

void loop() {
  // Empty loop for demonstration purposes
}

// ************************************************************ simple1.ino ***
