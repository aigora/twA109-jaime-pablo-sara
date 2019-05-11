const int pir= 2;     //Pin es el  2
int estadopir;        //Detección o no de presencia

void setup() 
{
Serial.begin(9600);
pinMode(pir, INPUT);     //Es un dispositivo de entrada
}

int detector_presencia (void)
  {
  int presencia=0;
  int value= digitalRead(pir);         //Leer pir
    if (value == HIGH )                  //Si detecta presencia
       presencia=1; 
    return presencia;    
  }
  
void loop() 
{
detector_presencia ()
}
