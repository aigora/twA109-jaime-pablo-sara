#include <TimeLib.h>
#define PIN_ANALOGICO A0 // El pin del LDR es el A0
#define ESPERA_LECTURAS 1000 // tiempo en milisegundos entre lecturas de la intensidad de la luz
   
long cronometro_lecturas=0; //Ponemos el cronometro a 0
long tiempo_transcurrido; //Tiempo que pasa
unsigned int luminosidad; //Luminosidad
float coeficiente_porcentaje=100.0/1023.0;
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

int luz (void)   //Funcion de la cantidad de luz
  {
    int porcentaje;
    int dia=0;
    tiempo_transcurrido=millis()-cronometro_lecturas;
    if(tiempo_transcurrido>ESPERA_LECTURAS)
    {
    cronometro_lecturas=millis();
    luminosidad=analogRead(PIN_ANALOGICO);
    porcentaje= luminosidad*coeficiente_porcentaje;
      if (porcentaje>25) //Si el porcentaje es mayor al 25%, se supone que es de día
        dia=1;  //Es de día
    }
     return dia;
   }


int detector_presencia (void)
  {
  int presencia=0;
  int value= digitalRead(pir);         //Leer pir
    if (value == HIGH )                  //Si detecta presencia
       presencia=1; 
    return presencia;    
  }


void bombilla (int dia, int presencia)
{
 if (dia==1 && presencia==1)
 {
  digitalWrite(led,HIGH);         // Enciende el led
  delay (20000);                  //Espera 20 segundos para apagar la luz si no detecta movimiento 
}
  else 
  digitalWrite(led,LOW);          //Se apaga el led
}

void loop() 
{
  int luz (void);
  int detector_presencia (void);
  int bombilla (luz, pir);  
}
  
 
