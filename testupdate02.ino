int trig1 = A0, echo1 = A1;// cảm biến trái
int trig2 = A2, echo2 = A3;// cảm biến giữa
int trig3 = A4, echo3 = A5;// cảm biến phải

int in1 = 10, in2 = 9, in3 = 8, in4 = 7;
int ENA = 11, ENB = 3;

long Duration, Distance;
float soundSpeed = 0.0343;
int leftSpeed = 90, rightSpeed = 95;
long leftDistance = 0, middleDistance = 0, rightDistance = 0;
int Wall = 19;
float minDistance = 1.9;


void setup()
{
  Serial.begin(9600);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  stop();
  delay(3800);

}

long leftM()
{
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  Duration = pulseIn(echo1, HIGH);
  Distance = soundSpeed * (Duration / 2);//s =v*t
  return(Distance);
}
long middleM()
{
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  Duration = pulseIn(echo2, HIGH);
  Distance = soundSpeed * (Duration / 2);//s =v*t
  return(Distance);
}

long rightM()
{
  digitalWrite(trig3, LOW);
  delayMicroseconds(2);
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  Duration = pulseIn(echo3, HIGH);
  Distance = soundSpeed * (Duration / 2);//s =v*t
  return(Distance);
}

void forward()
{
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void back()
{
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void turnRight()
{
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void turnLeft()
{
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stop()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void checkDistance()
{
  delay(99);
  int leftDHT = leftM();
  delay(10);
  int middleDHT = middleM();
  delay(10);
  int rightDHT = rightM();
  delay(10);

  while(true)
  {
    if(abs(leftDistance - leftDHT) <= minDistance && abs(middleDistance - middleDHT) <= minDistance && abs(rightDistance - rightDHT) <= minDistance)
    {
      if(leftDHT < rightDHT)
      {
      Serial.println("Lui re phai!!");
      stop();
      delay(300);
      back();
      delay(800);
      turnRight();
      delay(25);
      break;
      }
      if(leftDHT > rightDHT)
      {
      Serial.println("Lui re trai!!");
      stop();
      delay(300);
      back();
      delay(800);
      turnLeft();
      delay(25);
      break;
      }
    }
    else
    {
      break;
    }
  }
}


void loop()
{
  leftDistance = leftM();
  delay(10);
  middleDistance = middleM();
  delay(10);
  rightDistance = rightM();
  delay(10);

  Serial.print("Khoang cach Trai: ");
  Serial.print(leftDistance);
  Serial.println("CM");
  Serial.print("Khoang cach Giua: ");
  Serial.print(middleDistance);
  Serial.println("CM");
  Serial.print("Khoang cach Phai: ");
  Serial.print(rightDistance);
  Serial.println("CM");
  

  if(leftDistance < Wall && middleDistance > Wall && rightDistance > Wall)
  {
    Serial.println("Xe lech trai!!");
    turnRight();
    delay(28);
  }
  else if(leftDistance > Wall && middleDistance > Wall && rightDistance < Wall)
  {
    Serial.println("Xe lech phai!!");
    turnLeft();
    delay(28);
  }
  else if(leftDistance < Wall && middleDistance < Wall && rightDistance > Wall)
  {
    Serial.println("Re phai!!");
    stop();
    delay(200);
    turnRight();
    delay(500);
  }
  else if(leftDistance > Wall && middleDistance < Wall && rightDistance < Wall)
  {
    Serial.println("Re trai!!");
    stop();
    delay(200);
    turnLeft();
    delay(500);
  }
  else if(leftDistance > Wall && middleDistance < Wall && rightDistance > Wall)
  {
    Serial.println("Re trai!!");
    stop();
    delay(200);
    turnLeft();
    delay(500);
  }
  else if(leftDistance < Wall && middleDistance > Wall && rightDistance < Wall)
  {
    if(leftDistance < rightDistance)
    {
    Serial.println("Lech Trai!!");
    turnRight();
    delay(20);
    }
    if(leftDistance > rightDistance)
    {
    Serial.println("Lech Phai!!");
    turnLeft();
    delay(20);
    }
  }
  else if(leftDistance < Wall && middleDistance < Wall && rightDistance < Wall)
  {
    Serial.println("Chu y quay dau!!");
    stop();
    delay(500);
    back();
    delay(900);
    turnLeft();
    delay(900);
  }

  else
  {
    forward();
  }

  checkDistance();
}
