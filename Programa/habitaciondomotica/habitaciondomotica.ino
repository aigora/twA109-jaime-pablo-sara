#include <TimeLib.h>                //Libreria de la hora
#include <IRremote.h>               //Libreria del receptor IR
#include <Servo.h>                  //Librería del servo para la puerta
#define dir1PinL  2                 //Direccion de motor
#define dir2PinL  4                  //Direccion de motor
#define speedPinL 6                  // Para la velocidad
#define speedPinR 5                   //Para la velocidad
#define CINCO 765                     
 int estado;                          //Para la activación y desactivación de la placa
 char dato1;                          //Para la activación y desactivación de la placa



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Funciones
int descodificar(decode_results *);  //Descodifica la señal IR
int clave (int);                    //Comprueba si la clave es correcta
int ldr (void);                      // Mide la cantidad de luz
int detector_presencia (void);       //Observa si hay alguna perturbación en la habitación
void bombilla (int, int);            //Enciende el LED
void puerta (int);                   //Acciona el servo motor que bloquea la puerta
int hora (void);                     //Informa sobre el momento del día
void avanza (void);                  //Desenrolla el motor
void retrocede(void);                //Enrolla el motor   
void para (void);                    //Para el motor
void set_Motorspeed(int,int);        //Velocidad de los motores
void init_GPIO(void);                //Inicializa los motores
void motor (int);                    //Acciona el motor hacia delante o atrás segun quiera la persiana

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int movimiento;                              //Hay movimiento
int numero;                                   //Codigo del boton del mando
int dato;                                      //Para activar o desactivar la placa
int error;                                     //Para accionar o no el servomotor
int LDR_Pin = A0;                               //LDR en el pin analógico A0
int estadopir;                                   //Detección o no de presencia
time_t fecha;                                    // Declaramos la variable del tipo time_t
int luz, tiempo, correcto;                       //Indica si es de día por la luz, la hora y si el botón pulsado es el correcto
int repetir1=0, repetir2=0;                      //Para que no se mueva el motor constantemente
int pos;                                         //Posicion en angulos del servo
int dia;                                        //Indica si es de día o no

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Posiciones de los sensores
const int pir= 7;                               //PIR en pin 7
const int led= 12;                              //LED en pin 12
int RECV_PIN = 11;                                    //IR en pin 11
IRrecv irrecv(RECV_PIN);                        //Recibe del receptor IR
decode_results results;                        //Descodifica resultados
Servo servo;                                  //El servo de la puerta

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Estado inicial
void setup()    
{
  Serial.begin(9600);
  setTime(19, 59, 45, 13, 12, 2016);  // Establecemos la fecha
  pinMode(pir, INPUT);               //El pir es un dispositivo de entrada
  pinMode (led, OUTPUT);             //El led es un dispositivo de salida
  irrecv.enableIRIn();             // Empezamos la recepción  por IR
  servo.attach(9);                 //Pin del servo

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int dump(decode_results *results)      //Descodificación de los resultados
{
 dato=(results->value);
 return dato;                        //Nos devuelve el código del botón pulsado
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MANDO
int clave (void)
{
 error=1;
 if (irrecv.decode(&results)) 
  {
   numero=dump(&results);                        
   if (numero==CINCO) //5             //Si el pulsado es el cinco(en este caso), no hay error. Si es otro botón da error
    error=0;
   irrecv.resume();                  //Comienza la recepción de nuevo
  }
  delay(300);
  return error;                      //Devuelve error o no error
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LDR
int ldr (void)   //Funcion de la cantidad de luz
  {
dia=0;
int LDRReading = analogRead(LDR_Pin);
if (LDRReading>10)                       //Si supera esa cantidad de luz, es de día, si no, no.
 dia=1;
delay(250); 
return dia;                              //Devuelve si es de día o no
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PIR
int detector_presencia (void)
  {
  int presencia;                           
  int valor;
  valor= digitalRead(pir);                //Leer pir
    if (valor == HIGH )                  //Si detecta presencia, presencia=1.
       presencia=1; 
    else
      presencia=0;                       //Si no detecta presencia, presencia=0.
    return presencia;                    //Devuelve la presencia
  }
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HORA
int hora (void)
{
  dia=0;
  fecha = now();                                                                            //Mira la hora que es
  if (((hour(fecha))>=8 && (hour(fecha))<12)|| ((hour(fecha))>=18 && (hour(fecha))<20))     //Si está entre las 8 y las 12 ó entre las 18 y las 20, se considera de día para alzar la persiana
  dia=1;
  else                                                                                      //Si no, se considera de noche
  dia=0;
  return dia;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LED
void bombilla (int luz, int movimiento)
{
 if (luz==0 && movimiento==1)                        //Si es de noche y detecta movimiento
  {
  digitalWrite(led,HIGH);         // Enciende el led
  delay (2000);                   //Espera 20 segundos para apagar la luz si no detecta movimiento
  Serial.println("LED");          //Manda el mensaje para el registro
  }
  else 
  digitalWrite(led,LOW);          //Se apaga el led
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Persiana/motor

void go_Advance(void)                //Despliega la persiana
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL,LOW);

  
}

void go_Back(void)                     //Recoge la persiana
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,HIGH);
  
}
void stop_Stop()    //Stop            //Para el motor
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,LOW);
}


void set_Motorspeed(int speed_L,int speed_R)    //Velocidad del motor
{
  analogWrite(speedPinL,speed_L); 
  analogWrite(speedPinR,speed_R);   
}

//Pins initialize
void init_GPIO()                      //Inicializar la salida de los pines para los métodos
{
  pinMode(dir1PinL, OUTPUT); 
  pinMode(dir2PinL, OUTPUT); 
  pinMode(speedPinL, OUTPUT);   
  pinMode(speedPinR, OUTPUT); 
  stop_Stop();
}

void motor (int tiempo)                        //Función del motor
{
  if (tiempo==1 && repetir2==0)                  //Si es de día y ya no se ha recogido
  {
  init_GPIO();
  go_Back();                                    //Se enrolla
  set_Motorspeed(255,255);
  delay(5000);
  Serial.println("AVANZA");                     //Manda el mensaje para el registro
  stop_Stop();
  repetir2=1;
  repetir1=0;
  } 
  else
  if (tiempo==0 && repetir1==0)                 //Si es de noche y no se ha desenrollado
  {
  init_GPIO();
  go_Advance();                                 //Se desenrolla
  set_Motorspeed(255,255);
  delay(5000);
  Serial.println("RETROCEDE");                 //Manda el mensaje para el registro    
  stop_Stop();
  repetir1=1;
  repetir2=0;
  }  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Servo puerta
void puerta (int correcto)
{
 if (correcto==0)                         //Si el boton pulsado es el correcto
 {
 for(pos = 0; pos <= 180; pos += 1)      // Se mueve de 0 a 180 grados
  {                                  
    servo.write(pos);                   //Se lo manda al servo
    delay(20);                          // Espera 15ms
  } 
  delay (2000);
    for(pos = 180; pos>=0; pos-=1)     //Va de 180 grados a 0 grados
  {                                
    servo.write(pos);                  //Se lo manda al servo 
    delay(20);                         // Espera 15ms
  }
  Serial.println("SERVO");             //Manda el mensaje para el registro 
 }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{

  if (Serial.available() > 0)            //Si recibe por el puerto serie
  {
  dato1=Serial.read();                   //Lo lee
  if (dato1=='1')                        //Si es uno
  estado=1;                              //Estado es uno
  else
  if (dato1=='0')                       //Si es 0
  estado=0;                             //Estado es 0
  } 
  
switch (estado)
{
case 0:                                   //Si el estado es 0, no hace nada
{
  estado=0;
    break;
}

case 1:                                  //Si es estado es 1, activa las funciones
{
tiempo=hora ();
motor (tiempo); 
luz=ldr ();
movimiento=detector_presencia ();
bombilla (luz,movimiento); 
correcto= clave();
puerta(correcto);
estado=1;
}
}
}
