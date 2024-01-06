int trig1 = A0, echo1 = A1;  // cảm biến trái
int trig2 = A2, echo2 = A3;  // cảm biến giữa
int trig3 = A4, echo3 = A5;  // cảm biến phải

int ENA = 11, ENB = 3;  // chân điều khiển tốc độ
int in1 = 10, in2 = 9, in3 = 8, in4 = 7;  // chân điều khiển hướng đi

int leftSpeed = 79, rightSpeed = 89; // tốc độ bánh xe**
int lOutSpeed = 79, rOutSpeed = 89;
long Duration, Distance; // thời gian, khoảng cách phản lại
long leftDistance = 0, middleDistance = 0, rightDistance = 0;
const float soundSpeed = 0.0343;  //tôc độ âm thanh 343 m/s = 0.0343 cm/um
int wall = 15;//Khoảng cách vật chắn**


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

long leftMeasurement()  //Đo khoảng cách trái
{
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  Duration = pulseIn(echo1, HIGH);
  Distance = soundSpeed * (Duration / 2);  //s =v*t
  return (Distance);
}

long middleMeasurement()  // Đo khoảng cách phía trước
{
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  Duration = pulseIn(echo2, HIGH);
  Distance = soundSpeed * Duration / 2;
  return (Distance);
}

long rightMeasurement()  // Đo khoảng cách phải
{
  digitalWrite(trig3, LOW);
  delayMicroseconds(2);
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  Duration = pulseIn(echo3, HIGH);
  Distance = soundSpeed * Duration / 2;
  return (Distance);
}


void forward() {
  Serial.print("Đang đi Thẳng");
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
}
void turnLeft() 
{
  Serial.print("Rẽ trái!!");
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
}

void turnRight() 
{
  Serial.print("Rẽ Phải!!");
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
}



void stop()
{
  Serial.print("Phát hiện vật cản!!");
  analogWrite(ENA, LOW);
  analogWrite(ENB, LOW);
}
void lSpeedU()
{
  Serial.println("Tang Toc Trai$$$");
  analogWrite(ENA, lOutSpeed);
  analogWrite(ENB, rightSpeed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
}
void rSpeedU()
{
  Serial.println("Tang Toc Phai$$$&&&&&&&&&&&&&&&&");
  analogWrite(ENA, leftSpeed - 5);
  analogWrite(ENB, rOutSpeed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
}



void wallFollow()
{
  int ldiff = 0, rdiff = 0;
  leftDistance = leftMeasurement();
  delay(10);
  middleDistance = middleMeasurement();
  delay(10);
  rightDistance = rightMeasurement();
  delay(10);
  ldiff = wall - leftDistance;
  rdiff = wall - rightDistance;
  lOutSpeed = leftSpeed + (ldiff * 3);
  rOutSpeed = rightSpeed + (rdiff * 11);
  while(true)
  {
    if(leftDistance < wall && middleDistance > wall)
    {
      lSpeedU();
      break;
    }
    if(leftDistance < wall && middleDistance < wall )
    {
      Serial.println("Re Phai!!");
      stop();
      delay(1000);
      turnRight();
      delay(630);
      stop();
      delay(1000);
      int middleDistanceHT = middleMeasurement();
      if(middleDistanceHT < wall)
      {
        stop();
        delay(1000);
        turnRight();
        delay(800);
        stop();
        delay(1000);
        break;
      }
      else 
      {
        break;
      }
    }
    if(leftDistance > wall && middleDistance < wall )
    {
      Serial.println("Re Trai!!");
      stop();
      delay(1000);
      turnLeft();
      delay(800);
      stop();
      delay(1000);

      int middleDistanceHT1 = middleMeasurement();
      if(middleDistanceHT1 < wall)
      {
        Serial.print("Sai Duong Re Phai!!");
        turnRight();
        delay(1200);
        forward();
        delay(100);
        break;
        int middleDistance2HT = middleMeasurement();
        if(middleDistance2HT < wall)
        {
          stop();
          delay(1000);
          turnRight();
          delay(800);
          stop();
          delay(1000);
          break;
        }
        else
        {
          break;
        }
      }
      else if(middleDistanceHT1 > wall)
      {
        break;
      }
      break;  
    }
    if(leftDistance > 30 && middleDistance > wall)
    {
      rSpeedU();
      break;
    }
    else
    {
      forward();
      break;
    }
  }
}



void loop() 
{
  Serial.print("Khoảng cách trái: ");
  Serial.print(leftDistance);
  Serial.println("Cm");

  Serial.print("Khoảng cách phía trước: ");
  Serial.print(middleDistance);
  Serial.println("Cm");

  Serial.print("Khoảng cách Phải: ");
  Serial.print(rightDistance);
  Serial.println("Cm");

  wallFollow();  
}
