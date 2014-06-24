//ENTRADAS Y SALIDAS
int pinIzq[]={2,3,4,5,6};   //bloque de led izquierdos del carro
int pinDer[]={17,16,15,14,13};  //bloque de led derechos del carro
//float  A=-2.667;
//float B=360;

//CONSTANTES Y VARIABLES
int Izq=0,Der=0; 
float PWMI=0,PWMD=0;  
int pinMI=9,pinMD=10;
int valbitI;
int ValbitD;

//CONFIGURACION
void setup() {
  Serial.begin(9600);      //toma de datos cada 5
  for (int i=0;i<5;i++){
    pinMode(pinIzq[i],INPUT);
    pinMode(pinDer[i],INPUT);
  }
}

//SECUENCIA PRINCIPAL
void loop() {
  sensores();        //Llamado a la funcion lentura de sensores
  condiciones();    //Llamado a la funcion para movimiento del carro
  motores();
}
void sensores(){
  for (int i=0;i<5;i++){
    valbitI= digitalRead(pinIzq[i]);
    bitWrite(Izq,i,valbitI);
    ValbitD= digitalRead(pinDer[i]);
    bitWrite(Der,i,ValbitD);
  }
}

void condiciones(){
  if (Izq== B00001 && Der== B00001){    //para cuando esta en carril blanco
    PWMI=150;
    PWMD=150;
  }
  else if(Izq== B11110 && Der== B11110){    //camino normal a velocidad media
    PWMI=170;
    PWMD=170;
  }
  else if(Izq== B11111 && Der== B11111){    //camino muerto velocidad media
    PWMI=0;
    PWMD=0;
  }
  
else if (Izq<Der){//giro izquierda
  float sumn=Der-Izq;
  //sumn=sumn*A+B;
  if (sumn==6){
  PWMI=100;
  PWMD=160;
}

else if (sumn==15){
  PWMI=100;
  PWMD=185;
}
else if (sumn==15){
  PWMI=100;
  PWMD=200;
}
  
}
else if (Izq>Der){//giro a la derecha
  float sump=Izq-Der;
  //sump=sump*A+B;
  if (sump==6){
  PWMI=160;
  PWMD=100;
}

else if (sump==15){
  PWMI=185;
  PWMD=100;
}
else if (sump==15){
  PWMI=200;
  PWMD=100;
}
  }
  
}

void motores(){
  analogWrite(pinMI,PWMI);
  analogWrite(pinMD,PWMD);
}


