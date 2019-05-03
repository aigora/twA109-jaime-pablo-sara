#include <TimeLib.h> 
#define PIN_ANALOGICO A0 // El pin del LDR es el A0
#define ESPERA_LECTURAS 1000 // tiempo en milisegundos entre lecturas de la intensidad de la luz
#include <IRremote.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int descodificar(decode_results *);
int clave (void);
int luz (void);
int detector_presencia (void);
void bombilla (int, int);
void persiana (int);
void puerta (int);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long cronometro_lecturas=0;                         //Ponemos el cronometro a 0
long tiempo_transcurrido;                          //Tiempo que pasa
unsigned int luminosidad;                         //Luminosidad
float coeficiente_porcentaje=100.0/1023.0;
int estadopir;                                   //Detección o no de presencia
time_t fecha;                     // Declaramos la variable del tipo time_t
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Posiciones de los sensores
const int pir= 2;                               //PIR en pin  2
const int led= 12;                              //LED en pin 3
int ir= 11;                                     //IR en pin 11
IRrecv irrecv(ir);           
decode_results resultados;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()    //Estado inicial
{
  Serial.begin(9600);
  setTime(12, 32, 0, 13, 12, 2016);  // Establecemos la fecha
  fecha = now();                     // Obtenemos la fecha actual
  pinMode(pir, INPUT);               //El pir es un dispositivo de entrada
  pinMode (led, OUTPUT);             //El led es un dispositivo de salida
  irrecv.enableIRIn();               // Empezamos la recepción  por IR
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int descodificar(decode_results *resultados) //Función de descodificación de señal
{
  int dato;
 dato=(resultados->value);                   //Codigo del botón del mando
 return dato;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int digito (void)
{          
  int numero;
  if (irrecv.decode(&resultados)) 
   numero=descodificar(&resultados);           //Codigo del botón del mando
  delay(300);
  return numero;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MANDO
 int clave ()
  {
  int contrasena [50];
  int password []={0,0,0,0};
  int error=0;
  int i;
  for (i=0;digito()!=14535;i++)
  {
  switch (digito())
  {
    case -23971: 
                 {
                 contrasena[i]=1;
                 break;
                 }
    case 25245: 
                 {
                 contrasena[i]=2;
                 break;
                 }  
    case -7651: 
                  {
                 contrasena[i]=3;
                 break;
                 }  
     case 8925:
                 {
                 contrasena[i]=4;
                 break;
                 }  
     case 765: 
                 {
                 contrasena[i]=5;
                 break;
                 }  
     case -15811: 
                 {
                 contrasena[i]=6;
                 break;
                 } 
     case -8161: 
                 {
                 contrasena[i]=7;
                 break;
                 }  
     case -22441: 
                 {
                 contrasena[i]=8;
                 break;
                 }  
     case -28561: 
                 {
                 contrasena[i]=9;
                 break;
                 }
     case -26521: 
                 {
                 contrasena[i]=0;
                 break;
                 }
  }
  }
  if (digito==14535)
     for (i=0;i<4 && error==1;i++)
       if (contrasena[i]!=password[i])
           error=1;
 
                
  return error;
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LDR
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
      if (porcentaje>25)                             //Si el porcentaje es mayor al 25%, se supone que es de día
        dia=1;                                      //Es de día
    }
     return dia;
   }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PIR
int detector_presencia (void)
  {
  int presencia=0;
  int value= digitalRead(pir);         //Leer pir
    if (value == HIGH )                  //Si detecta presencia
       presencia=1; 
    return presencia;    
  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LED
void bombilla (int dia, int presencia)
{
 if (dia==1 && presencia==1)
  {
  digitalWrite(led,HIGH);         // Enciende el led
  delay (20000);                  //Espera 20 segundos para apagar la luz si no detecta movimiento 
  digitalWrite(led,LOW);
  }
  else 
  digitalWrite(led,LOW);          //Se apaga el led
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  luz ();
  detector_presencia ();
  clave ();
  bombilla (luz, detector_presencia);  
}
