#define PIN_ANALOGICO A0 // El pin del LDR es el A0
#define ESPERA_LECTURAS 1000 // tiempo en milisegundos entre lecturas de la intensidad de la luz

long cronometro_lecturas=0; //Ponemos el cronometro a 0
long tiempo_transcurrido; //Tiempo que pasa
unsigned int luminosidad; //Luminosidad
float coeficiente_porcentaje=100.0/1023.0;

void setup() 
{
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
   
void loop() 
{
 luz (void);
}
