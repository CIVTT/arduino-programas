
//ENTRADAS Y SALIDAS
int pinIzq[]={2,3,4,5,6};   //bloque de led izquierdos del carro
int pinDer[]={17,16,15,14,13};  //bloque de led derechos del carro

//CONSTANTES Y VARIABLES
int Izq=0,Der=0; 
float PWMI=0,PWMD=0;  
int pinMI=9,pinMD=10;
int valbitiz;
int valbitde;
int vel=75;
void setup() {
  Serial.begin(9600);      //toma de datos cada 5
  for (int i=0;i<5;i++){
    pinMode(pinIzq[i],INPUT);
    pinMode(pinDer[i],INPUT);
  }
}

void loop() {
   sensores();        //Llamado a la funcion lentura de sensores
  condiciones();    //Llamado a la funcion para movimiento del carro
  motores();
  Serial.print(Izq,BIN);
  Serial.print("  ");
 Serial.print(Der,BIN);
Serial.print(" : "); 
 Serial.print(Izq);
  Serial.print("  ");
 Serial.print(Der); 
 Serial.print("------>");
 Serial.print(PWMI);
 Serial.print(":");
 Serial.println(PWMD);
}

void sensores(){
  for (int i=0;i<5;i++){
    valbitiz= digitalRead(pinIzq[i]);
    bitWrite(Izq,i,valbitiz);
    valbitde= digitalRead(pinDer[i]);
    bitWrite(Der,i,valbitde);
  }
}

void condiciones(){
 //if(Izq=Der){
  if (Izq== B00001 && Der== B00001){    //para cuando esta en carril blanco
    PWMI=vel;
    PWMD=vel;
  }
  else if(Izq== B11110 && Der== B11110){    //camino normal a velocidad media
    PWMI=vel;
    PWMD=vel;
  }
  else if(Izq== B11111 && Der== B11111){    //camino muerto velocidad media
    PWMI=vel;
    PWMD=vel;
  }
   else if(Izq== B00000 && Der== B00000){    //sale error e 11100 a 11000
    PWMI=190;
    PWMD=0;
  }
 //}
else if (Izq<Der){//giro izquierda
 switch (Izq){
   case 0:
  PWMI=200;
  PWMD=0;
  break;
  
  case  1:
  PWMI=180;
  PWMD=vel;
  break;
  case  3:
  PWMI=185;
  PWMD=vel;
  break;
  case  7:
  PWMI=190;
  PWMD=vel;
  break;
  case  15:
  PWMI=195;
  PWMD=vel;
  break;
   case  16:
  PWMI=150;
  PWMD=vel;
  break;
   case  24:
  PWMI=123;
  PWMD=vel;
  break;
  case  28:
  PWMI=110;
  PWMD=vel;
  break;


  }
}
else if (Izq>Der){//giro a la derecha
  switch (Der){
  case 0:
  PWMD=200;
  PWMI=0;
  break; 
  case  1:
  PWMD=180;
  PWMI=vel;
  break;
  case  3:
  PWMD=185;
  PWMI=vel;
  break;
  case  7:
  PWMD=190;
  PWMI=vel;
  break;
  case  15:
  PWMD=195;
  PWMI=vel;
  break;
  case  16:
  PWMD=150;
  PWMI=vel;
  break;
   case  24:
  PWMD=123;
  PWMI=vel;
  break;
   case  28:
  PWMD=110;
  PWMI=vel;
  break;
 }
  }
}

void motores(){
  analogWrite(pinMI,PWMI);
  analogWrite(pinMD,PWMD);
}
