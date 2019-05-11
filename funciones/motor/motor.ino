#define dir1PinL  2    //Motor direction
#define dir2PinL  4    //Motor direction
#define speedPinL 6    // Needs to be a PWM pin to be able to control motor speed
#define speedPinR 5  

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

void setup()
{
	init_GPIO();
	go_Advance();//Forward
  set_Motorspeed(255,255);
  delay(5000);
  
  go_Back();//Reverse
  set_Motorspeed(255,255);
  delay(5000);
  
  stop_Stop();//Stop

}

void loop(){
}
