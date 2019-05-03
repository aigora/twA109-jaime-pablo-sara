#include <TimeLib.h> 
#define dir1PinL  2    //Direccion de motor
#define dir2PinL  4    //Direccion de motro
#define speedPinL 6    // Para la velocidad
#define speedPinR 5    //Para la velocidad
void persiana (int);
int hora (void);
void go_Advance(void);
void go_Back(void);
void stop_Stop(void);
void motor (int);
time_t fecha;                     // Declaramos la variable del tipo time_t
int luz, tiempo, movimiento, dia, correcto;
int repetir1=0, repetir2=0;
                    // Obtenemos la fecha actual

void setup() 
{
 Serial.begin(9600);
 setTime(7, 59, 45, 13, 12, 2016);  // Establecemos la fecha
  fecha = now();
                   
}

int hora (void)
{
  dia=0;
  if ((hour(fecha))>=8 && (hour(fecha))<20)
  dia=1;
  else
  dia=0;
  return dia;
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
  }
}

void loop() 
{
  tiempo=hora ();
  motor (tiempo);
}
