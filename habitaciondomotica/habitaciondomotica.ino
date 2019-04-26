
const int pir= 2;     //Pin es el  2
int estadopir;          //Detección o no de presencia

void setup() 
{
  pinMode(pir, INPUT);     //Es un dispositivo de entrada
}

void loop() 
{
  int value= digitalRead(pir); //Leer pir
  if (value == HIGH)                  //Si detecta presencia
  //Va un else pero no me deja ponerlo de momento
  {
                                     // Deteccion de presencia activado
  }    
}
 
