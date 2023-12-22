int trig1 = A0, echo1 = A1;// cảm biến trái
int trig2 = A2, echo2 = A3;// cảm biến giữa
int trig3 = A4, echo3 = A5;// cảm biến phải

int  ENA = 11, ENB = 3;// chân điều khiển tốc độ
int  in1 = 10, in2 = 9, in3 = 8, in4 = 7;// chân điều khiển hướng đi

int leftSpeed = 93, rightSpeed = 80;// tốc độ bánh xe**
long Duration, Distance;// thời gian, khoảng cách phản lại
long leftDistance = 0, middleDistance = 0, rightDistance = 0;
float soundSpeed = 0.0343;//tôc độ âm thanh 343 m/s = 0.0343 cm/um
int Wall = 12;//Khoảng cách vật chắn**
long minDistance = 1.9;

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
  forward();
  delay(100);
}

long leftMeasurement()//Đo khoảng cách trái
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

long middleMeasurement()// Đo khoảng cách phía trước
{
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  Duration = pulseIn(echo2, HIGH);
  Distance = soundSpeed * Duration / 2;
  return(Distance);
}

long rightMeasurement()// Đo khoảng cách phải
{
  digitalWrite(trig3, LOW);
  delayMicroseconds(2);
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  Duration = pulseIn(echo3, HIGH);
  Distance = soundSpeed * Duration / 2;
  return(Distance);
}


void forward()
{
  Serial.println("Đang đi Thẳng");
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
}

void turnLeft()
{
  Serial.println("Rẽ trái!!");
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
}

void turnRight()
{
  Serial.println("Rẽ Phải!!");
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
}

void back()
{
  Serial.println("Chú ý de xe!!");
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
}



void stop()
{
  Serial.print("Phát hiện vật cản!!");
  analogWrite(ENA, LOW);
  analogWrite(ENB, LOW);
}

void checkDistance()
{
  delay(100);
  int DistanceHT1 = leftMeasurement();
  delay(10);
  int DistanceHT2 = middleMeasurement();
  delay(10);
  int DistanceHT3 = rightMeasurement();
  delay(10);
  while(true)
  {
    if(abs(DistanceHT1 - leftDistance) <= minDistance && abs(DistanceHT2 - middleDistance) <= minDistance && abs(DistanceHT3 - rightDistance) <= minDistance)
    {
      if(DistanceHT1 < DistanceHT3)
      {
        Serial.println("Xe lech trai!!");
        stop();
        delay(400);
        back();
        delay(800);
        turnRight();
        delay(55);
        break;
      }
      if(DistanceHT1 > DistanceHT3)
      {
        Serial.println("Xe lech phai!!");
        stop();
        delay(400);
        back();
        delay(800);
        turnLeft();
        delay(55);
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
  leftDistance = leftMeasurement();
  delay(10);
  middleDistance = middleMeasurement();
  delay(10);
  rightDistance = rightMeasurement();
  delay(10);

  Serial.print("Khoảng cách trái: ");
  Serial.print(leftDistance);
  Serial.println("Cm");

  Serial.print("Khoảng cách phía trước: ");
  Serial.print(middleDistance);
  Serial.println("Cm");

  Serial.print("Khoảng cách Phải: ");
  Serial.print(rightDistance);
  Serial.println("Cm");


  if(leftDistance < Wall && middleDistance > Wall && rightDistance > Wall)
  {
    stop();
    delay(20);
    turnRight();
    delay(30);
  }
  else if(leftDistance > Wall && middleDistance > Wall && rightDistance < Wall)
  {
    stop();
    delay(20);
    turnLeft();
    delay(30); 
  }
  else if(leftDistance > Wall && middleDistance < Wall && rightDistance > Wall)
  {
    if(leftDistance > rightDistance)
    {
      stop();
      delay(200);
      turnLeft();
      delay(400);
    }
    if(leftDistance < rightDistance)
    {
      stop();
      delay(200);
      turnRight();
      delay(400);  
    }
  }
  else if(leftDistance < Wall && middleDistance < Wall && rightDistance > Wall)
  {
    stop();
    delay(200);
    turnRight();
    delay(400);
  }
  else if(leftDistance > Wall && middleDistance < Wall && rightDistance < Wall)
  {
    stop();
    delay(200);
    turnLeft();
    delay(400);
  }

  else if(leftDistance < Wall && middleDistance > Wall && rightDistance < Wall)
  {
    if(leftDistance > rightDistance)
    {
      stop();
      delay(20);
      turnLeft();
      delay(50);
    }
    if(leftDistance < rightDistance)
    {
      stop();
      delay(20);
      turnRight();
      delay(50);  
    }
  }
  else if(leftDistance < Wall && middleDistance < Wall && rightDistance < Wall)
  {
    stop();
    delay(500);
    back();
    delay(1000);
    turnLeft();
    delay(900);
  }
  else
  {
    forward();
  }
  
  checkDistance();
  
}
