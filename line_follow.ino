#include <Servo.h>
#include <IRremote.h>
#define trigPin 10
#define echoPin 9

Servo sweepservo;  // create servo object to control a servo
Servo rightwheel;
Servo leftwheel;
// twelve servo objects can be created on most boards

int pos = 30;    // variable to store the servo position
int distance;
int RECV_PIN1 = 7;
int RECV_PIN2 = 6;
int RECV_PIN3 = 5;
int RECV_PIN4 = 4;

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
  sweepservo.attach(13);  // attaches the servo on pin 9 to the servo object
  Serial.println("Serial Monitor Enabled");
}

void loop()
{
  int front, left, right;
  moveservo(60);
  delay(30);
  left=obstacle();
  moveservo(90);
  delay(30);
  front=obstacle();
  moveservo(120);
  delay(30);
  right=obstacle();
  if ((left>50)&&(right>50))
  {
    servomotor(40,40); 
  }
  else if (left<50)
  {
    servomotor(-40,40);
  }
  else if (right<50)
  {
    servomotor(40,-40);
  }
}


void moveservo(int pos)
{
  sweepservo.write(pos);
  delay(150);
}

int obstacle()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

void servomotor(int speedLeft, int speedRight)
{
  leftwheel.attach(12);
  rightwheel.attach(11);
  leftwheel.writeMicroseconds(1500+speedLeft);
  rightwheel.writeMicroseconds(1500-speedRight);
  delay(200);
  leftwheel.detach();
  rightwheel.detach();
}

