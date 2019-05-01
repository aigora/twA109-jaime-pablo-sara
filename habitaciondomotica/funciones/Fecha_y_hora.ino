#include <TimeLib.h>
time_t fecha;         // Declaramos la variable del tipo time_t

void setup() 
{
Serial.begin(9600);
  setTime(12, 32, 0, 13, 12, 2016);  // Establecemos la fecha
  fecha = now();                     // Obtenemos la fecha actual 
}

void loop() 
{
  // Imprimimos la hora
  Serial.print("Hora: ");
  Serial.print(hour(fecha));
  Serial.print(":");
  Serial.print(minute(fecha));
  Serial.print(":");
  Serial.print(second(fecha));

  Serial.print("  ");
  Serial.print(day(fecha));
  Serial.print("/");
  Serial.print(month(fecha));
  Serial.print("/");
  Serial.println(year(fecha));

  delay(1000);
}
