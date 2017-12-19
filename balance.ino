#include <Stepper.h>
#include <Servo.h>
#include <Wire.h>

//ENTRADAS Y SALIDAS
int pinDirec[]={2,3,5,4};
int pinEnabl[]={10,11};
int addrIMU=0x68;

//CONSTANTES Y VARIABLES
#define R_D 57.295779
#define A_R 16384.0
#define G_R 131.0
#define LIM 0
#define LSM 255
unsigned long Tu=0.004;
float Ts=100;
float SetP=0,ValP=0,CVal=0;
float kp=32,ki=0.04,kd=0.08;
float E=0,Eu=0,dE=0,iE=0;
int Acce[]={0,0,0};
int Gyro[]={0,0,0};
float AccA[]={0,0};
float GyrA[]={0,0};
float Angle[]={0,0};
float ang;
boolean direc[]={1,0,0,1};
boolean front[]={1,0,0,1};
boolean back[]={0,1,1,0};
boolean left[]={1,0,1,0};
boolean rigth[]={0,1,0,1};

//SECUENCIA DE INICIO
void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(addrIMU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Tu=millis();
  for(int i=0;i<4;i++){
    pinMode(pinDirec[i],OUTPUT);
    digitalWrite(pinDirec[i],LOW);
  }
}

//SECUENCIA PRINCIPAL
void loop() {
  unsigned long now=millis();
  Ts=(float)(now-Tu)/1000;
  ValP=data(Ts);
  CVal=computer(SetP,ValP,Ts);
  motor(CVal);
  Tu=now;
  //Serial.print(ValP,4);
}

//PID CALCULATE
float computer(float targe, float value,float sample) {
  E = targe - value;
  iE+=E*sample;
  if(iE>255) iE=255;
  else if(iE<-255) iE=-255;
  dE=(E - Eu)/sample;
  float out = (float)kp * E + ki * iE - kd * dE;
  Eu = E;
  return out;
}

float data(float sample){
  Wire.beginTransmission(addrIMU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(addrIMU,6,true);
  for(int i=0;i<3;i++){
    Acce[i]=Wire.read()<<8|Wire.read();
  }
  AccA[1] = atan(-1*(Acce[0]/A_R)/sqrt(pow((Acce[1]/A_R),2) + pow((Acce[2]/A_R),2)))*R_D;
  AccA[0] = atan((Acce[1]/A_R)/sqrt(pow((Acce[0]/A_R),2) + pow((Acce[2]/A_R),2)))*R_D;
  Wire.beginTransmission(addrIMU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(addrIMU,6,true);
  for(int i=0;i<3;i++){
    Gyro[i]=Wire.read()<<8|Wire.read();
  }
  GyrA[0] = Gyro[0]/G_R;
  GyrA[1] = Gyro[1]/G_R; 
  Angle[0] = 0.98 *(Angle[0]+GyrA[0]*sample) + 0.02*AccA[0];
  Angle[1] = 0.98 *(Angle[1]+GyrA[1]*sample) + 0.02*AccA[1];
  Serial.print(Angle[1]);
  return Angle[1];
}

void motor(float vel){
  int PWM=(int)vel;
  if(PWM>0){
    PWM=PWM;
    for(int i=0;i<4;i++){
      digitalWrite(pinDirec[i],front[i]);
    }
  }
  else if(PWM<=0){
    PWM=-PWM;
    for(int i=0;i<4;i++){
      digitalWrite(pinDirec[i],back[i]);
    }
  }
  if(PWM>LSM) PWM=LSM;
  else if(PWM<LIM) PWM=LIM;
  analogWrite(pinEnabl[0],PWM);
  analogWrite(pinEnabl[1],PWM);
  Serial.print("  ");
  Serial.println(PWM);
}

