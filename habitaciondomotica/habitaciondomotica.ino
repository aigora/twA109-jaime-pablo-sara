const int pir= 2;     //Pin es el  2
const int led= 3;     //LED en pin 3
int estadopir;          //Detección o no de presencia

void setup() 
{
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

 
