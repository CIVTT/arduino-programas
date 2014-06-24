// SEGUIDOR DE LINEA

//ENTRADAS Y SALIDAS
int pinIzq[]={2,3,4,5,6};   //bloque de led izquierdos del carro
int pinDer[]={17,16,15,14,13};  //bloque de led derechos del carro
float  A=-0.021;
float B=3.66;
float C=98.77;
int D=0;
//CONSTANTES Y VARIABLES
int Izq=0,Der=0; 
float PWMI=0,PWMD=0;  
int pinMI=9,pinMD=10;
int valbit;
//int ValbitD;

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
  Serial.print(Izq,BIN);
  Serial.print("  ");
 Serial.print(Der,BIN);
//Serial.print(" : "); 
// Serial.print(Izq);
//  Serial.print("  ");
// Serial.print(Der); 
 Serial.print("------>");
 Serial.print(PWMI);
//  Serial.print("  ");
//   Serial.print(A);
    Serial.print(":");
   Serial.println(PWMD);

// delay(200);
}

void sensores(){
  for (int i=0;i<5;i++){
    valbit= digitalRead(pinIzq[i]);
    bitWrite(Izq,i,valbit);
    valbit= digitalRead(pinDer[i]);
    bitWrite(Der,i,valbit);
  }
}

void condiciones(){
  if (Izq== B00001 && Der== B00001){    //para cuando esta en carril blanco
    PWMI=150-D;
    PWMD=150-D;
  }
  else if(Izq== B11110 && Der== B11110){    //camino normal a velocidad media
    PWMI=150-D;
    PWMD=150-D;
  }
  else if(Izq== B11111 && Der== B11111){    //camino muerto velocidad media
    PWMI=0;
    PWMD=0;
  }
//   else if(Izq== B00000 && Der== B11111){    //camino muerto velocidad media
//    PWMI=180;
//    PWMD=0;
//  }
//  else if(Izq== B11111 && Der== B00000){    //camino muerto velocidad media
//    PWMI=0;
//    PWMD=180;
//  }
//  
else if (Izq<Der){//giro izquierda
  float sumn=Der-Izq;
  sumn=sumn*sumn*A+B*sumn;
  PWMD=sumn+C-D;
  PWMI=90;
}
else if (Izq>Der){//giro a la derecha
  float sump=Izq-Der;
  sump=sump*sump*A+B*sump;
  PWMI=sump+C-D;
  PWMD=90;
  }
}

void motores(){
  analogWrite(pinMI,PWMI);
  analogWrite(pinMD,PWMD);
}
  
