#include<Servo.h>
#include <IRremote.h>
#define trigPin 10
#define echoPin 9

Servo sweepservo;
Servo rightwheel;
Servo leftwheel;

int pos,distance;
int RECV_PIN1 = 4;
int RECV_PIN2 = 5;
int RECV_PIN3 = 6;
int RECV_PIN4 = 7;

IRsend irsend;
IRrecv irrecv1(RECV_PIN1);
IRrecv irrecv2(RECV_PIN2);
IRrecv irrecv3(RECV_PIN3);
IRrecv irrecv4(RECV_PIN4);

decode_results results1;
decode_results results2;
decode_results results3;
decode_results results4;

void setup()
{
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  sweepservo.attach(13);
  Serial.println("Enabling IRin");
  irrecv1.enableIRIn(); // Start the receiver
  irrecv2.enableIRIn(); // Start the receiver
  irrecv3.enableIRIn(); // Start the receiver
  irrecv4.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop()
{
  long leftarea,centrearea,rightarea;
  
 if (irrecv1.decode(&results1))
  {
    if (results1.value==0xa90)
    {
      Serial.print("Signal received at sensor 1: ");
      Serial.println(results1.value, HEX);
      centralizescanner();
      obstacle();
      while (distance>=4)
      {
        forward();
        delay (500);
        obstacle();
      }
      while (distance<4)
      {
        irsend.sendSony(0xa90, 12);
        Serial.println("Broadcasting Signal");
        delay(40);
      }
      irrecv1.resume();
    }
  }
 if (irrecv2.decode(&results2))
  {
    if (results2.value==0xa90)
    {
      turnright();
      delay (250);
      turnright();
      Serial.print("Signal received at sensor 2: ");
      Serial.println(results2.value, HEX);
      centralizescanner();
      obstacle();
      while (distance>=4)
      {
        forward();
        delay (300);
        obstacle();
      }
      while (distance<4)
      {
        irsend.sendSony(0xa90, 12);
        Serial.println("Broadcasting Signal");
        delay(40);
      }
      irrecv2.resume();
    }
  }
 if (irrecv3.decode(&results3))
  {
    if (results3.value==0xa90)
    {
      uturn();
      delay (700);
      Serial.print("Signal received at sensor 3: ");
      Serial.println(results3.value, HEX);
      centralizescanner();
      obstacle();
      while (distance>=4)
      {
        forward();
        delay (500);
        obstacle();
      }
      while (distance<4)
      {
        irsend.sendSony(0xa90, 12);
        Serial.println("Broadcasting Signal");
        delay(40);
      }
      irrecv3.resume();
    }
  }
 if (irrecv4.decode(&results4))
  {
    if (results4.value==0xa90)
    {
      turnleft();
      delay (500);
      turnleft();
      Serial.print("Signal received at sensor 4: ");
      Serial.println(results4.value, HEX);
      centralizescanner();
      obstacle();
      while (distance>=4)
      {
        forward();
        delay (500);
        obstacle();
      }
      while (distance<4)
      {
        irsend.sendSony(0xa90, 12);
        Serial.println("Broadcasting Signal");
        delay(40);
      }
      irrecv4.resume();
    }
  }
  centralizescanner();
  delay(500);
  lookright();
  delay (500);
  obstacle();
  rightarea=distance;
  Serial.print(distance);
  Serial.println(" cm");
  align();
  delay (500);
  obstacle();
  centrearea=distance;
  Serial.print(distance);
  Serial.println(" cm");
  lookleft();
  delay (500);
  obstacle();
  leftarea=distance;
  Serial.print(distance);
  Serial.println(" cm");
    if ((leftarea<10) && (rightarea<10))
    {
      if (centrearea<10)
      {
        centralizescanner();
        uturn();
      }
      else if (centrearea>=10)
      {
        centralizescanner();
        forward();       
      }
    }
    else if (leftarea<10 && (rightarea>10))
    {
      centralizescanner();
      turnright();
    }
    else if (rightarea<10 && (leftarea>10))
    {
      centralizescanner();
      turnleft();
    }
    else if (centrearea<10)
    {
      if (leftarea>=10)
      {
        centralizescanner();
        turnleft();
      }
      else if (rightarea>=10)
      {
        centralizescanner();
        turnright();
      }
      else if ((leftarea<10) && (rightarea<10))
      {
        centralizescanner();
        uturn();
      }
    }
   else
    {
      centralizescanner();
      forward();
    }
}
void obstacle()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration) / 20;
}
void centralizescanner()
{
  pos=90;
  sweepservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(15);                       // waits 15ms for the servo to reach the position
}
void align()
{
  for (pos = 20; pos <= 90; pos +=5 )
  {
    sweepservo.write(pos);
    delay(15);
  }
}
void lookright()
{
  for (pos = 90; pos >= 20; pos -= 5)
  {
    sweepservo.write(pos);
    delay(15);
  }
}
void lookleft()
{
  for (pos = 90; pos <= 160; pos += 5)
  {
    sweepservo.write(pos);
    delay(15);
  }
}
void forward()
{
  leftwheel.attach(12);
  rightwheel.attach(11);
  leftwheel.writeMicroseconds(1700);
  rightwheel.writeMicroseconds(1300);
  delay(200);
  leftwheel.detach();
  rightwheel.detach();
}
void turnright()
{
  leftwheel.attach(12);
  rightwheel.attach(11);
  leftwheel.writeMicroseconds(1700);
  rightwheel.writeMicroseconds(1700);
  delay(200);
  leftwheel.detach();
  rightwheel.detach();
}
void turnleft()
{
  leftwheel.attach(12);
  rightwheel.attach(11);
  leftwheel.writeMicroseconds(1300);
  rightwheel.writeMicroseconds(1300);
  delay(300);
  leftwheel.detach();
  rightwheel.detach();
}
void uturn()
{
  leftwheel.attach(12);
  rightwheel.attach(11);
  leftwheel.writeMicroseconds(1700);
  rightwheel.writeMicroseconds(1700);
  delay(500);
  leftwheel.detach();
  rightwheel.detach();
}
