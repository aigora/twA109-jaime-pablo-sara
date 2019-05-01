//De recibo de DevC++
const int LedPin = 12;

void setup()
{
Serial.begin(9600);
pinMode(LedPin, OUTPUT);
digitalWrite(LedPin, LOW);
}

void loop()
{
char dato;
if (Serial.available() > 0)
{
dato = Serial.read();
if (dato == '1')
digitalWrite(LedPin, HIGH);
else
if (dato == '0')
digitalWrite(LedPin, LOW);
}
}

//De envio a DevC++
const int ButtonPin = 2;
void setup()
{
Serial.begin(9600);
pinMode(ButtonPin, INPUT);
}
void loop()
{
char dato;
dato = digitalRead(ButtonPin); //lectura digital de pin
//Configurado con resistencia de pull-up 0=Pulsado
if (dato == LOW)
{
Serial.println("Pulsado");
}
delay
