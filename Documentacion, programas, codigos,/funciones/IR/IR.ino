#include <IRremote.h>
int RECV_PIN = 11; 
int led=12;
int dato;
int numero;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Empezamos la recepción  por IR
  pinMode (led, OUTPUT);   //Es un pin de salida
}


int dump(decode_results *results) 
{
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
 
 dato=(results->value);
 return dato;

}

void loop() {
  if (irrecv.decode(&results)) 
  {
   numero=dump(&results);
   if (numero==765) //5
   {
    digitalWrite(led,HIGH);         // Enciende el led
    delay (2000); 
    digitalWrite(led,LOW);
   }
    irrecv.resume(); // empezamos una nueva recepción
  }
  delay(300);
}
