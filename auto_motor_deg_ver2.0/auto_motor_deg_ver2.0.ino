/* arduino 控制 IG42 馬達轉到指定角度
 * 使用時監控視窗模式選擇 無換行
 */



#include <DCMotorIG42.h>

const uint8_t encoderPinA = 18, encoderPinB =19;
DCMotorIG42 Motor(encoderPinA, encoderPinB, 1);


long V_max = 350;
long X_round = 980; //一圈有980個刻度
long X_now, X_nxt, X_slowdown, X_zero, deg;
long deg_now, deg;


void setup() {
  Serial.begin(9600);
  Serial.println("修正零點");
  X_nxt = X_round*8;  //轉八圈到達穩態最大轉速
  bool is_ok = 0;
  while(is_ok == 0) {
      Motor.setSpeed(V_max); //全速運轉
      while(Motor.readEncoder() < X_nxt); //判斷是否到達預定位置 X_nxt
      X_slowdown = Motor.readEncoder(); //開始減速的位置 X_slowdown

      
      Motor.setSpeed(5);  //停止
      while(1) { //判斷是否靜止
         X_now = Motor.readEncoder();
         delay(10);
         if(X_now == Motor.readEncoder()) break;
      }

      
      Serial.print("目前機械角度座標: ");
      deg_now = X_now % 980 * 18 / 49;
      Serial.println(deg_now);
      Serial.print("輸入修正角度(1~360)(0為完成校正): ");
      while(Serial.available() == 0); //等待輸入
      deg = Serial.parseInt();
      Serial.println(deg); //顯示輸入
      
      if(deg == 0) {
        is_ok = 1, X_zero = X_now % X_round;
        Serial.println("修正0點成功~");
      }
      else X_nxt = X_slowdown + X_round*8 + deg * 49/18; //轉八圈與修正到達穩態最大轉速
      
  }
}

void loop() {
  Serial.print("輸入要轉到的角度: ");
  while(Serial.available() == 0);
  deg = Serial.parseInt();
  Serial.println(deg);

  X_now = Motor.readEncoder();
  X_nxt = X_now + (X_round - ((X_now - X_zero - deg * 49 / 18) % 980));
  //目前位置 -> 預期角度 - 歸零角度


  while(Motor.readEncoder() < X_nxt) {
    long V_now = (X_nxt - Motor.readEncoder()) * 0.5 + 20; //P控制器
    if(V_now > 100) V_now = 100;
    Motor.setSpeed(V_now);
  }
  
  Motor.setSpeed(5);  //停止
  while(1) { //判斷是否靜止
     X_now = Motor.readEncoder();
     delay(10);
     if(X_now == Motor.readEncoder()) break;
  }
  

  Serial.print("目前程式角度座標: ");
  deg_now = X_now % 980 * 18 / 49;
  Serial.println(deg_now);
  delay(2000); 
}
