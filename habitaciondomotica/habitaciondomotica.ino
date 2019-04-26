#include <Time.h>
const int pir= 2;     //Pin es el  2
const int led= 3;     //LED en pin 3
int estadopir;        //Detección o no de presencia
time_t fecha;         // Declaramos la variable del tipo time_t

void setup() 
{
  Serial.begin(9600);
  setTime(12, 32, 0, 13, 12, 2016);  // Establecemos la fecha
  fecha = now();           // Obtenemos la fecha actual
  pinMode(pir, INPUT);     //Es un dispositivo de entrada
  pinMode (led, OUTPUT);   //Es un pin de salida
}

void loop() 
{
  int value= digitalRead(pir);         //Leer pir
  if (value == HIGH)                  //Si detecta presencia
    {
      digitalWrite(led,HIGH);         // Enciende el led
      delay (20000);                  //Espera 20 segundos para apagar la luz si no detecta movimiento
    }    
  else                                //Si no detecta presencia
      digitalWrite(led,LOW);          //Se apaga el led
}

 
