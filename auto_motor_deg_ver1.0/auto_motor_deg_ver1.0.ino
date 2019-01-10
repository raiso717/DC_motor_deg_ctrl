#include <DCMotorIG42.h>

const uint8_t encoderPinA = 18, encoderPinB =19;
DCMotorIG42 Motor(encoderPinA, encoderPinB, 1);

long N = 0;
long last = 0;
long k = 980;
long II = 0;;
long nxt = k*10;
long deg = 1;
long mode = 0;
long zero = 0;
long V = 350; //0~400;
long a = -1;
long zerostop;
long bef = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("修正零點");
  bool is_ok = 0;
  while(is_ok == 0) {
      Motor.setSpeed(V);
      while(Motor.readEncoder() < nxt);
      last = Motor.readEncoder();
      Motor.setSpeed(5);
      a = Motor.readEncoder();
      delay(50);
      while(Motor.readEncoder() != a) {        
        a = Motor.readEncoder();
        delay(50);
      }
      //是否完全停止
      Serial.print("目前機械角度座標: ");
      Serial.println(a%980*18/49);
      Serial.println("輸入修正角度(0~360): ");
      while(Serial.available() == 0);
      deg=Serial.parseInt();
      Serial.println(deg);
      if(deg == 0) is_ok = 1, nxt = last + k*7;
      else nxt = last + k*7 + deg * 49/18;
      zerostop = last % k;
      zero = a % k;
  }
  

  if(is_ok == 1) Serial.println("修正0點成功~");
  Serial.println(zero);
  Serial.println(zerostop);
  Serial.println(a%980);
  Serial.println(last%980);
}

void loop() {
  Serial.print("輸入要轉到的角度: ");
  while(Serial.available() == 0);
  deg=Serial.parseInt();
  Serial.println(deg);

  bef = Motor.readEncoder();
  nxt = bef - ((bef - zero -  deg * 49 / 18) % 980)+ k;
  Motor.setSpeed(40);
  while(Motor.readEncoder() < (nxt-44));
  Motor.setSpeed(5);
  delay(500);
  Serial.println("slow down");
  Motor.setSpeed(30);
  while(Motor.readEncoder() < nxt-2);
  Motor.setSpeed(5);
  for(a = Motor.readEncoder()-10; a != Motor.readEncoder();) {
    a = Motor.readEncoder();
    delay(10);
  }
  



  

//  Serial.print("目前機械角度座標: ");
//  Serial.println(a%980*36/98);
  Serial.print("目前程式角度座標: ");
  Serial.println((a+980-zero)%980*36/98);
  delay(2000); 
}
