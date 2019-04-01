#include <PS2X_lib.h> 
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
#define TrigPin 9 //发出超声波
#define EchoPin 8 //收到反射回来的超声波
float cm; //因为测得的距离是浮点型的
PS2X ps2x;

int error = 0;
byte type = 0;
byte vibrate = 0;

void distance();
void PS2();

void setup() {
  Serial.begin(57600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  delay(300);
  error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
  delay(3000);
}
void loop() {
  PS2();
  distance();
}
void PS2() {
  digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);       // delayMicroseconds在更小的时间内延时准确
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW); //通过这里控制超声波的发射
  cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm
  cm = (int(cm * 100.0)) / 100.0; //保留两位小数
  Serial.print("Distance:");
  Serial.print(cm);
  Serial.println("cm");
  if (cm < 20) {
    Serial.println("Wranning!");//距离太近，开始自动后退避障
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(1000);  //向后1S后停止
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}
void distance() {
  if (error == 1) //skip loop if no controller found
    return;
  if (type == 2) { //Guitar Hero Controller
    return;
  }
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate);

    if (ps2x.Button(PSB_START)) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    if (ps2x.Button(PSB_PAD_UP)) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    if (ps2x.Button(PSB_PAD_RIGHT)) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    if (ps2x.Button(PSB_PAD_LEFT)) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    if (ps2x.Button(PSB_PAD_DOWN)) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    if (ps2x.Analog(PSS_LY) < 127) { //UP

      Serial.println("up");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    if (ps2x.Analog(PSS_LY) > 127) { //DOWN
      Serial.println("DOWN");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    if (ps2x.Analog(PSS_LX) < 128) { //LEFT
      Serial.println("LEFT");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    if (ps2x.Analog(PSS_LX) > 128) { //RIGHT
      Serial.println("RIGHT");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    if(ps2x.Analog(PSS_LX)==128&ps2x.Analog(PSS_LY)==127){
      Serial.println("STOP");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    vibrate = ps2x.Analog(PSAB_BLUE);
  }
  delay(30);
}
