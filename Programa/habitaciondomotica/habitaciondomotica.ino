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
///
int movimiento;                              //Hay movimiento
int numero;                                   //Codigo del boton del mando
int dato;
int error;
int LDR_Pin = A0;
int estadopir;                                   //Detección o no de presencia
time_t fecha;                                    // Declaramos la variable del tipo time_t
int luz, tiempo, correcto;
int repetir1=0, repetir2=0;
int pos;                                         //Posicion en angulos del servo
int dia;                                    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Posiciones de los sensores
const int pir= 7;                               //PIR en pin 7
const int led= 12;                              //LED en pin 12
int RECV_PIN = 11;                                    //IR en pin 11
IRrecv irrecv(RECV_PIN);
decode_results results;
Servo servo;
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
  fecha = now();
  if ((hour(fecha))>=8 && (hour(fecha))<20)
  dia=1;
  else
  dia=0;
  return dia;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LED
void bombilla (int luz)
{
 if (luz==0)
  {
  digitalWrite(led,HIGH);         // Enciende el led
  delay (2000);                  //Espera 20 segundos para apagar la luz si no detecta movimiento
  Serial.println("LED"); 
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
  if (tiempo==1 && repetir2==0)
  {
  init_GPIO();
  go_Back();//Enrolla
  set_Motorspeed(255,255);
  delay(5000);
  Serial.println("AVANZA");
  stop_Stop();
  repetir2=1;
  repetir1=0;
  } 
  else
  if (tiempo==0 && repetir1==0)
  {
  init_GPIO();
  go_Advance();//Desenrolla
  set_Motorspeed(255,255);
  delay(5000);
  Serial.println("RETROCEDE");
  stop_Stop();
  repetir1=1;
  repetir2=0;
  }  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Servo puerta
void puerta (int correcto)
{
 if (correcto==0)
 {
 for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 15ms for the servo to reach the position 
  } 
  delay (2000);
    for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 15ms for the servo to reach the position 
  }
  Serial.println("SERVO");   
 }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{

  if (Serial.available() > 0)
  {
  dato1=Serial.read();
  if (dato1=='1')
  estado=1;
  else
  if (dato1=='0')
  estado=0;
  }
  
switch (estado)
{
case 0:
{
  estado=0;
    break;
}

case 1:
{
tiempo=hora ();
motor (tiempo); 
luz=ldr ();
movimiento=detector_presencia ();
bombilla (luz); 
correcto= clave();
puerta(correcto);
estado=1;
}
}
}
