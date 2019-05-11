const int led= 3;     //LED en pin 3

void setup() 
{
  Serial.begin(9600);
  pinMode (led, OUTPUT);   //Es un pin de salida
}

void loop() 
{
 digitalWrite(led,HIGH);         // Enciende el led
 delay (20000);                  //Espera 20 segundos para apagar la luz si no detecta movimiento 
 digitalWrite(led,LOW);
}
