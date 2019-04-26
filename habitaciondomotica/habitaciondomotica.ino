#include <TimeLib.h>
#define PIN_ANALOGICO A0 // La patilla de Arduino a la que conectamos la entrada (LDR)
#define ESPERA_LECTURAS 1000 // tiempo en milisegundos entre lecturas de la intensidad de la luz
   
long cronometro_lecturas=0;
long tiempo_transcurrido;
unsigned int luminosidad;
float coeficiente_porcentaje=100.0/1023.0;
const int pir= 2;     //Pin es el  2
const int led= 3;     //LED en pin 3
int estadopir;        //Detección o no de presencia
time_t fecha;         // Declaramos la variable del tipo time_t

int luz (void)
  {
    int porcentaje;
    int dia=0;
    tiempo_transcurrido=millis()-cronometro_lecturas;
    if(tiempo_transcurrido>ESPERA_LECTURAS)
    {
    cronometro_lecturas=millis();
    luminosidad=analogRead(PIN_ANALOGICO);
    porcentaje= luminosidad*coeficiente_porcentaje;
      if (porcentaje>25)
        dia=1;
    }
     return dia;
   }

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
  
  int luz (void);
  int value= digitalRead(pir);         //Leer pir
  if (value == HIGH && luz==0)                  //Si detecta presencia y es de noche
    {
      digitalWrite(led,HIGH);         // Enciende el led
      delay (20000);                  //Espera 20 segundos para apagar la luz si no detecta movimiento
    }    
  else                                //Si no detecta presencia
      digitalWrite(led,LOW);          //Se apaga el led

  
}
  
 
