int trig1 = A0, echo1 = A1;  // cảm biến trái
int trig2 = A2, echo2 = A3;  // cảm biến giữa
int trig3 = A4, echo3 = A5;  // cảm biến phải

int ENA = 11, ENB = 3;  // chân điều khiển tốc độ
int in1 = 10, in2 = 9, in3 = 8, in4 = 7;  // chân điều khiển hướng đi
int wheelSpeed = 110, leftSpeed = 0, rightSpeed = 0; // tốc độ bánh xe**
long Duration, Distance; // thời gian, khoảng cách phản lại
long leftDistance = 0, middleDistance = 0, rightDistance = 0;
const float soundSpeed = 0.0343;  //tôc độ âm thanh 343 m/s = 0.0343 cm/um
const double wall = 19;//Khoảng cách vật chắn**
const float kp = 2.6;
const float ki = 0;
const float kd = 1.2;
int T = 80; 

float sumError = 0;
float prevError = 0;

float pid = 0;
unsigned long lastTime = 0;

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
  pinMode(13, OUTPUT);

  Serial.println("Start!!!");
  stop();
  delay(1000);
  digitalWrite(13, 1), delay(500);
  digitalWrite(13, 0), delay(500);
  digitalWrite(13, 1), delay(500);
  digitalWrite(13, 0), delay(500);
  digitalWrite(13, 1), delay(500);
  digitalWrite(13, 0);
  
  forward(), delay(200);
}

long leftMeasurement()  //Đo khoảng cách trái
{
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  Duration = pulseIn(echo1, HIGH);
  leftDistance = soundSpeed * (Duration / 2);  //s =v*t
 
  if (leftDistance < wall) {
    return 1;
  }
  else {
    return 0;
  }
}

long middleMeasurement()  // Đo khoảng cách phía trước
{
  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  Duration = pulseIn(echo2, HIGH);
  middleDistance = soundSpeed * Duration / 2;
  if (middleDistance < wall) {
    return 1;
  }
  else {
    return 0;
  }
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
  Serial.print("Turn Left!!!");
  digitalWrite(13, 1);
  delay(2);
  analogWrite(ENA, 0);
  analogWrite(ENB, wheelSpeed);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  digitalWrite(13, 0);
  
}

void turnRight() 
{
  Serial.print("Turn Right!!!");
  digitalWrite(13, 1);
  delay(2);
  analogWrite(ENA, wheelSpeed);
  analogWrite(ENB, wheelSpeed);
  digitalWrite(in1, 0);
  digitalWrite(in2, 1);
  digitalWrite(in3, 0);
  digitalWrite(in4, 1);
  digitalWrite(13, 0);
  
}

void stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void wallFollow()
{  
  unsigned long currentTime = millis();
  int deltaTime  = currentTime - lastTime;
  if (deltaTime >= T)
  {
    float error = wall - leftDistance;
    sumError += error;
    float proportional = kp * error;

    float integral = (ki * T) * sumError; 

    float derivative = kd * ((error - prevError) / T);

    pid = proportional + integral + derivative;
    leftSpeed  = wheelSpeed + pid;
    rightSpeed  = wheelSpeed - pid;

    analogWrite(ENA, leftSpeed);
    analogWrite(ENB, rightSpeed);
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);
    digitalWrite(in3, 1);
    digitalWrite(in4, 0);

    prevError = error;
    lastTime = currentTime;
  }
}



void loop() 
{

  int left_status = leftMeasurement();
  delay(10);
  int middle_status = middleMeasurement();
  delay(10);

  Serial.print("Khoảng cách trái: ");
  Serial.print(leftDistance);
  Serial.println("Cm");

  Serial.print("Khoảng cách phía trước: ");
  Serial.print(middleDistance);
  Serial.println("Cm");

  if (left_status == 1 && middle_status == 0) {
    wallFollow();
  }
  else if (left_status == 0 && middle_status == 0)
  {
    turnLeft();
  }
  else 
  {
    turnRight();
  }
}
