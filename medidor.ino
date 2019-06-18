/* Programa anlalizador de baterias
 * mide amperage y voltage para determinar descargas en la bateria
 * 
 */

#include <SD.h>
#include <SPI.h>

int PinAmp1=A0;
int PinAmp2=A1;
int pinpwm=3;
int cs = 4; // pin escogido
int s_d=0;
int s_u=0;
int m_d=0;
int m_u=0;
int h_d=0;
int h_u=0;
int samples=100;

float sen_offset=2.5;
float sensibilidad=0.066;
float amp1=0;
float amp2=0;
long randomV;
float Vi=0;

unsigned long timer1=0;
unsigned long timer2=0;
File archivo; //variable archivo a guardar

 
void setup(){
  pinMode(pinpwm,OUTPUT);
  Serial.begin(9600);
  //setTime(00,00,00,11,05,2019);
  while (!Serial) {
  }
  Serial.println("Iniciando SD card...");
  Serial.println();
  pinMode(cs, OUTPUT);
  //pinMode(SS, OUTPUT);
  
  if (!SD.begin(cs)) {
    Serial.println("SD fallando o bloqueada o no tiene sd");
    }
  archivo=SD.open("testeo.txt",FILE_WRITE);
  if (archivo){
    archivo.println("TIME , VOLTAJE IN , AMPERIMETRO 1, AMPERIMETRO 2");
    archivo.close();
    Serial.println("hecho");
  }else{
    Serial.println("error abriendo testeo.txt");
  }
    
}
 
void loop(){
  randomV=random(1,255);
  analogWrite(pinpwm,randomV);
  //Vi = scale(randomV,0,255,0,9.6);
  Vi=R_P(randomV);
  un_seg();
  contador();
  medir(samples);
  up_date();
  delay(1000);
}

float R_P(float A_v){
  float result;
  result=0.062849+0.041422*A_v-0.000015*A_v*A_v;
  return result;
}
float scale(float x1,float in_min,float in_max,float out_min,float out_max){
  float result;
  result=(x1 - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  //y=(x*(c-d)+a*d-b*c)/(a-b)
  //result=(x1*(out_max-out_min)+in_max*out_min-in_min*out_max)/(in_max-in_min);
  return result;
}

void up_date(){
  archivo=SD.open("testeo.txt",FILE_WRITE);
  if (archivo){
    //Serial.print("subiendo lectura...");
    Serial.print(h_d);  
    Serial.print(h_u);
    Serial.print(+ ":") ;
    Serial.print(m_d);
    Serial.print(m_u);
    Serial.print(+":") ;
    Serial.print(s_d);
    Serial.print(s_u);
    Serial.print(" , ");
    Serial.print(Vi);
    Serial.print(" , ");
    Serial.print(amp1,3);
    Serial.print(" , ");
    Serial.println(amp2,3);
    
    //Serial.println(randomV);
   
    archivo.print(" ");
    archivo.print(h_d);  
    archivo.print(h_u);
    archivo.print(+ ":") ;
    archivo.print(m_d);
    archivo.print(m_u);
    archivo.print(+":") ;
    archivo.print(s_d);
    archivo.print(s_u);
    archivo.print(" , ");
    archivo.print(Vi);
    archivo.print(" , ");
    archivo.print(amp1);
    archivo.print(" , ");
    archivo.println(amp2);
    
    archivo.close();
    }
     else{
   Serial.println("error error horror testeo fallo");
  }

}

void un_seg(){
  timer2=(millis()/1000);
  if (timer1 !=timer2){
    timer1=timer2;
    s_u++;
  }
}
float medir(int samp1){
  for (int i=0; i<samp1; i++){
    float m_1 = (analogRead(PinAmp1)*5)/1023.0;
    float m_2 = (analogRead(PinAmp2)*5)/1023.0;
    amp1+=(m_1-2.5)/sensibilidad  ;
    amp2+=(m_2-2.5)/sensibilidad;
    }
  amp1=amp1/samples;
  amp2=amp2/samples;
  if((amp1<0)){
    amp1=0;
  }else{
  amp1=amp1+0.015;}
  if((amp2<0)){
    amp2=0;
  }else{
  amp2=amp2+0.015;}
  return(amp1,amp2);
}
void contador(){
  
  if (s_u==10){
  s_u=0;
  s_d++;
  }
  if((s_d==6)&&(s_u==0)){
    s_d=0;
    m_u++;
  }
  if(m_u==10){
    m_u=0;
    m_d++;
  }
  if((m_d==6)&&(m_u==0)){
    m_d=0;
    h_u++;
  }
  if((h_d==2)&&(h_u==4)){
    h_u=0;
    h_d=0;
  }
  }
