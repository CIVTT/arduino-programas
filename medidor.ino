/* Programa anlalizador de baterias
   mide amperage y voltage para determinar descargas en la bateria
  posible circuito https://www.askix.com/controlador-de-carga-solar-de-arduino-version-1_7.html
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include "GFButton.h"
LiquidCrystal_I2C lcd(0x3f, 16, 2);
//const int rs = 10 , en = 9 , d4 = 8 , d5 = 7 , d6 = 6 , d7 = 5 ;
const int PinAmp1 = A0, PinAmp2 = A1, pinpwm = 3, pin_rasb = 7, pin_est = 6, pin_button1 = 2, pin_button2 = 5;
int cs = 4; // pin escogido lcd
int s_d = 0, s_u = 0, m_d = 0, m_u = 0, h_d = 0, h_u = 0;
//valores de seteo de sensor
int S_V = 0;
int samples = 100;
float sen_offset = 2.5;
float sensibilidad = 0.066;
float amp1 = 0, amp2 = 0;
//valores de simulador de voltaje
long randomV;
float Vi = 0;
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
boolean INICIO = false;
boolean b_1 = false, b_2 = false, b_3 = false;
String ESTADO_1 ;//= "OFF";
String ESTADO_2 ;//= "ON";
int EST_R = 0, EST_V = 0, estado = 0, status_1 = 0, status_2 = 0;
int valor_2 = 0, old_valor_2 = 0, valor_1 = 0, old_valor_1 = 0, voltron = 0;
File archivo;
//variable archivo a guardar

GFButton boton1(pin_button1);
GFButton boton2(pin_button2);

void setup() {

  pinMode(pinpwm, OUTPUT);
  //pinMode(pin_rasb, INPUT);
  //pinMode(pin_est, INPUT);
  pinMode(pin_button1, INPUT);
  pinMode(pin_button2, INPUT);
  lcd.init(); // lcd usado para display
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(".INICIANDO");
  lcd.setCursor(3, 1);
  lcd.print(".ESPERE...");
  delay(500);
  lcd.clear();
  Serial.begin(9600);
  //setTime(00,00,00,11,05,2019);
  while (!Serial) {
  }
  //Serial.println("Iniciando SD card...");
  //Serial.println();
  pinMode(cs, OUTPUT);
  //pinMode(SS, OUTPUT);
  if (!SD.begin(cs)) {
    Serial.println("SD fallando o bloqueada o no tiene sd");
  }
  archivo = SD.open("testeo.txt", FILE_WRITE);
  if (archivo) {
    archivo.println("HORA , MINUTO , SEGUNDO , VOLTAJE IN , AMPERIMETRO 1, AMPERIMETRO 2 , RASBERRY , FUENTE EXTERNA");
    archivo.close();
    //Serial.println("hecho");
    estado = 0;
    //INICIO = true;

  } else {
    Serial.println("error abriendo testeo.txt");
    estado = 5;
  }//delay(1000);
}

void loop() {
  lcd_info();
  botonA();
  //if (archivo){
  if (INICIO == true) {
    //botonA();
    volt();
    un_seg();
    hora();
    medir(samples);
    up_date();
    EST_RASB();
    delay(1000);
  }//}
  /*else {
    // estado = 5;
    }*/
}

float R_P(float A_v) {
  float result;
  result = 0.062849 + 0.041422 * A_v - 0.000015 * A_v * A_v;
  return result;
}
float scale(float x1, float in_min, float in_max, float out_min, float out_max) {
  float result;
  result = (x1 - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  //y=(x*(c-d)+a*d-b*c)/(a-b)
  //result=(x1*(out_max-out_min)+in_max*out_min-in_min*out_max)/(in_max-in_min);
  return result;
}
//pulsador A
void botonA() {
  int conter_1 = 0;
  int conter_2 = 0;
  int cont_1 = 0;
  valor_1 = digitalRead(pin_button1);
  valor_2 = digitalRead(pin_button2);
  if ((valor_1 == HIGH) && (old_valor_1 == LOW)) {
    /*while (digitalRead(pin_button1)) {
      delay(10);
      }*/
    conter_1++;
    conter_2++;
    delay(100);
  }
  old_valor_1 = valor_1;
  if (conter_1 == 1) {
    b_1 = !b_1;
    //lcd.clear();
    delay(100);
  }
  if (conter_2 == 2) {
    b_2 = !b_2;
  }
  if ((valor_2 == HIGH) && (old_valor_2 == LOW)) {
    cont_1++;
    delay(100);
  }
  if (cont_1 == 1 ) {
    b_3 = !b_3;
    //lcd.clear();
    delay(100);
  }
  /*if (estado == 1 && b_3 == true) {
    S_V = S_V + 1;
    //b_3 = !b_3;
    delay(100);
  } if (S_V <= 9) {
    S_V = 1;
  }*/
}

void volt() {
  if (status_1 == 0) {
    randomV = random(1, 255);
    analogWrite(pinpwm, randomV);
    //Vi = scale(randomV,0,255,0,9.6);
    Vi = R_P(randomV);
  }
  if (status_1 == 1) {
    //status_2=0;
    randomV=scale(S_V,1,9,0,255);
    Vi = S_V;
    analogWrite(pinpwm,randomV);
  }

}
void up_date() {
  archivo = SD.open("testeo.txt", FILE_WRITE);
  if (archivo) {
    //
    //data enviada a archivo TESTEO.txt
    archivo.print(" ");
    archivo.print(h_d);
    archivo.print(h_u);
    archivo.print(",") ;
    archivo.print(m_d);
    archivo.print(m_u);
    archivo.print(",") ;
    archivo.print(s_d);
    archivo.print(s_u);
    archivo.print(",");
    archivo.print(Vi);
    archivo.print(",");
    archivo.print(amp1);
    archivo.print(",");
    archivo.print(amp2);
    archivo.print(",");
    archivo.print(ESTADO_1);
    archivo.print(",");
    archivo.println(ESTADO_2);
    archivo.close();
  }
  else {
    estado = 5;
  }
}

void EST_RASB() {
  int B1_state;
  int B2_state;
  B1_state = digitalRead(pin_rasb);
  B2_state = digitalRead(pin_est);
  if (B1_state == HIGH) {
    ESTADO_1 = "ON";
    EST_R = 1;
  } else {
    ESTADO_1 = "OFF";
    EST_R = 0;
  }
  if (B2_state == HIGH) {
    ESTADO_2 = "ON";
    EST_V = 1;
  } else {
    ESTADO_2 = "OFF";
    EST_V = 0;
  }
}

void lcd_info() {

  switch (estado) {
    case 0:
      lcd.setCursor(1, 0);
      lcd.print("Set-Ramdon V");
      lcd.setCursor(0, 1);
      lcd.print("mode:");
      if (b_3) {
        lcd.setCursor(7, 1);
        lcd.print("Set");
        status_1 = 1;

      } else {
        lcd.setCursor(7, 1);
        lcd.print("Ram");
        status_1 = 0;

        //b_3=!b_3;
      }
      if (b_1 == true && status_1 == 1) {
        estado = 1;
        b_1 = !b_1;
        lcd.clear();
      } if (b_1 == true && status_1 == 0) {
        estado = 2;
        b_1 = !b_1;
        lcd.clear();
        INICIO = !INICIO;
      }

      break;
    case 1:
      //lcd.print("  ");
      //lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("SET - V");
      lcd.setCursor(2, 1);
      lcd.print("V : ");
      lcd.setCursor(7, 1);
      lcd.print(S_V);
      //V_S();
      if(b_3){
        S_V = S_V + 1;
    b_3 = !b_3;
      }
      if(S_V>=10){
        S_V=1;
      }
      if (b_1) {
        estado = 2;
        b_1 = !b_1;
        INICIO = !INICIO;
      }
      break;
    case 2:

      lcd.print("  ");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vi=");
      lcd.setCursor(9, 0);
      lcd.print("A1=");
      lcd.setCursor(9, 1);
      lcd.print("A2=");
      //data enviada a lcd
      lcd.setCursor(3, 0);
      lcd.print(Vi);
      //lcd.print("|");
      lcd.setCursor(12, 0);
      lcd.print(amp1);
      lcd.setCursor(12, 1);
      lcd.print(amp2);
      if (b_3) {
        estado = 3;
        b_3 = !b_3;
      }
      if (b_2) {
        INICIO = !INICIO;
        estado = 1;
        b_2 = !b_2;
      }
      break;
    case 3:
      lcd.print("  ");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RASPBERRY =");
      lcd.setCursor(12, 0);
      lcd.print(ESTADO_1);
      lcd.setCursor(2, 1);
      lcd.print("VOLTAJE =");
      //data enviada a lcd
      lcd.setCursor(12, 1);
      lcd.print(ESTADO_2);
      if (b_3) {
        estado = 4;
        b_3 = !b_3;
      } if (b_2) {
        INICIO = !INICIO;
        estado = 1;
        b_2 = !b_2;
      }
      break;
    case 4:
      lcd.print("  ");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Hr : Min : Seg");
      //data enviada a lcd
      lcd.setCursor(1, 1);
      lcd.print(h_d);
      lcd.setCursor(2, 1);
      lcd.print(h_u);
      lcd.setCursor(4, 1);
      lcd.print(":");
      lcd.setCursor(7, 1);
      lcd.print(m_d);
      lcd.setCursor(8, 1);
      lcd.print(m_u);
      lcd.setCursor(10, 1);
      lcd.print(":");
      lcd.setCursor(13, 1);
      lcd.print(s_d);
      lcd.setCursor(14, 1);
      lcd.print(s_u);
      if (b_3) {
        estado = 2;
        b_3 = !b_3;
      } if (b_2) {
        INICIO = !INICIO;
        estado = 1;
        b_2 = !b_2;
      }
      break;
    case 5:
      //Serial.println("error error horror testeo fallo");
      //lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NO MEMORY SDcard");
      lcd.setCursor(0, 1);
      lcd.print("INSERT SD, RESET");
      break;
      if (b_2) {
        estado = 1;
        b_2 = !b_2;
      }
  }
}

void un_seg() {
  tiempo2 = (millis() / 1000);
  //for (tiempo2)
  if (tiempo1 != tiempo2) {
    tiempo1 = tiempo2;
    s_u++;
  }
}
void hora() {
  if (s_u == 10) {
    s_u = 0;
    s_d++;
  }
  if ((s_d == 6) && (s_u == 0)) {
    s_d = 0;
    m_u++;
  }
  if (m_u == 10) {
    m_u = 0;
    m_d++;
  }
  if ((m_d == 6) && (m_u == 0)) {
    m_d = 0;
    h_u++;
  }
  if ((h_d == 2) && (h_u == 4)) {
    h_u = 0;
    h_d = 0;
  }
}
float medir(int samp1) {
  for (int i = 0; i < samp1; i++) {
    float m_1 = (analogRead(PinAmp1) * 5) / 1023.0;
    float m_2 = (analogRead(PinAmp2) * 5) / 1023.0;
    amp1 += (m_1 - 2.5) / sensibilidad  ;
    amp2 += (m_2 - 2.5) / sensibilidad;
  }
  amp1 = amp1 / samples;
  amp2 = amp2 / samples;
  if ((amp1 < 0)) {
    amp1 = 0;
  } else {
    amp1 = amp1 + 0.015;
  }
  if ((amp2 < 0)) {
    amp2 = 0;
  } else {
    amp2 = amp2 + 0.015;
  }
  return (amp1, amp2);
}
