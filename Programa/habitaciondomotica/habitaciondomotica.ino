#include <TimeLib.h> 
#include <IRremote.h>
#include <Servo.h>
#define PIN_ANALOGICO A0 // El pin del LDR es el A0
#define ESPERA_LECTURAS 1000 // tiempo en milisegundos entre lecturas de la intensidad de la luz
#define dir1PinL  2    //Direccion de motor
#define dir2PinL  4    //Direccion de motro
#define speedPinL 6    // Para la velocidad
#define speedPinR 5    //Para la velocidad

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Funciones
int descodificar(decode_results *);
int clave (void);
int ldr (void);
int detector_presencia (void);
void bombilla (int, int);
void persiana (int);
void puerta (int);
int hora (void);
void go_Advance(void);
void go_Back(void);
void stop_Stop(void);
void set_Motorspeed(int,int);
void init_GPIO(void);
void motor (int);
int dia;
void puerta (int);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Variables
long cronometro_lecturas=0;                         //Ponemos el cronometro a 0
long tiempo_transcurrido;                          //Tiempo que pasa
unsigned int luminosidad;                         //Luminosidad
float coeficiente_porcentaje=100.0/1023.0;
int estadopir;                                   //Detección o no de presencia
time_t fecha;                     // Declaramos la variable del tipo time_t
int luz, tiempo, movimiento, correcto;
int repetir1=0, repetir2=0;
int pos;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Posiciones de los sensores
const int pir= 8;                               //PIR en pin 8
const int led= 12;                              //LED en pin 12
int ir= 11;                                     //IR en pin 11
IRrecv irrecv(ir);           
decode_results resultados;
Servo servo;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Estado inicial
void setup()    
{
  Serial.begin(9600);
  setTime(7, 59, 45, 13, 12, 2016);  // Establecemos la fecha
  pinMode(pir, INPUT);               //El pir es un dispositivo de entrada
  pinMode (led, OUTPUT);             //El led es un dispositivo de salida
  irrecv.enableIRIn();               // Empezamos la recepción  por IR
  servo.attach(2);
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
int ldr (void)   //Funcion de la cantidad de luz
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
//HORA
int hora (void)
{
  fecha = now();                     // Obtenemos la fecha actual
  dia=0;
  if ((hour(fecha))>=8 && (hour(fecha))<20)
  dia=1;
  return dia;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LED
void bombilla (int luz, int movimiento)
{
 if (luz==0 && movimiento==1)
  {
  digitalWrite(led,HIGH);         // Enciende el led
  delay (20000);                  //Espera 20 segundos para apagar la luz si no detecta movimiento 
  digitalWrite(led,LOW);
  }
  else 
  digitalWrite(led,LOW);          //Se apaga el led
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Persiana/motor
/*motor control*/
void go_Advance(void)  //Forward
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL,LOW);
}

void go_Back(void)  //Reverse
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,HIGH);
}
void stop_Stop()    //Stop
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,LOW);
}

/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL,speed_L); 
  analogWrite(speedPinR,speed_R);   
}

//Pins initialize
void init_GPIO()
{
  pinMode(dir1PinL, OUTPUT); 
  pinMode(dir2PinL, OUTPUT); 
  pinMode(speedPinL, OUTPUT);   
  pinMode(speedPinR, OUTPUT); 
  stop_Stop();
}

void motor (int tiempo)
{
  if (tiempo==0 && repetir1==0)
  {
  init_GPIO();
  go_Advance();//Desenrolla
  set_Motorspeed(255,255);
  delay(5000);
  stop_Stop();
  repetir1=1;
  repetir2=0;
  }
  else
  if (tiempo==1 && repetir2==0)
  {
  init_GPIO();
  go_Back();//Enrolla
  set_Motorspeed(255,255);
  delay(5000);
  stop_Stop();
  repetir2=1;
  repetir1=0;
  } 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Servo puerta
void puerta (int correcto)
{
 if (correcto==1)
 {
 for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
    for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }   
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  luz=ldr ();
  movimiento=detector_presencia ();
  bombilla (luz, movimiento);  
  tiempo=hora ();
  motor (tiempo);
  correcto=clave ();
  puerta (correcto);
}
