#include <TimeLib.h>                //Libreria de la hora
#include <IRremote.h>               //Libreria del receptor IR
#include <Servo.h>                  //Librería del servo para la puerta
#define dir1PinL  2                 //Direccion de motor
#define dir2PinL  4                  //Direccion de motor
#define speedPinL 6                  // Para la velocidad
#define speedPinR 5                   //Para la velocidad

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
int numero;
int LDR_Pin = A0;
int estadopir;                                   //Detección o no de presencia
time_t fecha;                                    // Declaramos la variable del tipo time_t
int luz, tiempo, movimiento, correcto;
int repetir1=0, repetir2=0;
int pos;                                         //Posicion en angulos del servo
int dia;                                    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Posiciones de los sensores
const int pir= 8;                               //PIR en pin 8
const int led= 12;                              //LED en pin 12
const int ir= 11;                                     //IR en pin 11
IRrecv irrecv(ir);           
decode_results resultados;
Servo servo;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Estado inicial
void setup()    
{
  Serial.begin(9600);
  setTime(19, 59, 45, 13, 12, 2016);  // Establecemos la fecha
  pinMode(pir, INPUT);               //El pir es un dispositivo de entrada
  pinMode (led, OUTPUT);             //El led es un dispositivo de salida
  irrecv.enableIRIn();               // Empezamos la recepción  por IR
  servo.attach(9);

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
  irrecv.enableIRIn();                        // Empezamos la recepción  por IR
  if (irrecv.decode(&resultados)) 
   numero=descodificar(&resultados);           //Codigo del botón del mando
  delay(300);
  return numero;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MANDO
 int clave (int numero)
  {
  int error;
  switch (numero)
  {
    case -23971: 
                 {
                 error=1;
                 break;
                 }
    case 25245: 
                 {
                 error=0;
                 break;
                 }  
    case -7651: 
                  {
                 error=1;
                 break;
                 }  
     case 8925:
                 {
                 error=1;
                 break;
                 }  
     case 765: 
                 {
                  error=1;
                 break;
                 }  
     case -15811: 
                 {
                  error=1;
                 break;
                 } 
     case -8161: 
                 {
                  error=1;
                 break;
                 }  
     case -22441: 
                 {
                 error=1;
                 break;
                 }  
     case -28561: 
                 {
                  error=1;
                 break;
                 }
     case -26521: 
                 {
                 error=1;
                 break;
                 }
     default: 
     error=1;
  }              
  return error;
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LDR
int ldr (void)   //Funcion de la cantidad de luz
  {
dia=0;
int LDRReading = analogRead(LDR_Pin);
if (LDRReading>10)
 dia=1;
delay(250); 
return dia;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PIR
int detector_presencia (void)
  {
  int presencia=0;
  int valor= digitalRead(pir);         //Leer pir
    if (valor == HIGH )                  //Si detecta presencia
       presencia=1; 
    return presencia;    
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
  else
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
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Servo puerta
void puerta (int fallido)
{
 if (fallido==0)
 {
 for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 15ms for the servo to reach the position 
  } 
    for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(20);                       // waits 15ms for the servo to reach the position 
  }   
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  correcto=clave (numero);
  puerta (correcto);
}
