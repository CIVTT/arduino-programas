/*
 * File:   newmain.c
 * Author: adadadad
 *
 * Created on 5 de julio de 2017, 08:16 PM
 */
#include <stdint.h>
#include <stdlib.h>
#include <xc.h>
//.. configuracopn de pic16F877 cp off debugg off wacthdog off 
//#pragma config cp = OFF//, DEBUGG = OFF, WRT = OFF, CPD =OFF,LVP=OFF,BOREN=OFF,PWRTE=ON,WDTE=OFF

#define _XTAL_FREQ 4000000 //..
void segundos(int t);

void main(void)
{
	/* codigo principal */
	TRISB=0x00; // pone el puerto B como salida
	PORTB=0b00000000;
    while(1){
/* semaforo A */
	PORTBbits.RB0=1;//rojo
	PORTBbits.RB1=0;//ambar
	PORTBbits.RB2=0;//verde
/* semaforo B */
	PORTBbits.RB3=0;//rojo
	PORTBbits.RB4=0;//ambar
	PORTBbits.RB5=1;//verde
	segundos(13);//llamamos a la funcion segundos que nos dara el ttiempo en que se encuentre encendido 

/* semaforo A */
	PORTBbits.RB0=1;//rojo
	PORTBbits.RB1=0;//ambar
	PORTBbits.RB2=0;//verde
/* semaforo B */
	PORTBbits.RB3=0;//rojo
	PORTBbits.RB4=1;//ambar
	PORTBbits.RB5=0;//verde
	segundos(2);

	/* semaforo A */
	PORTBbits.RB0=0;//rojo
	PORTBbits.RB1=0;//ambar
	PORTBbits.RB2=1;//verde
/* semaforo B */
	PORTBbits.RB3=0;//rojo
	PORTBbits.RB4=1;//ambar
	PORTBbits.RB5=0;//verde
	segundos(10);

	/* semaforo A */
	PORTBbits.RB0=0;//rojo
	PORTBbits.RB1=0;//ambar
	PORTBbits.RB2=1;//verde
/* semaforo B */
	PORTBbits.RB3=1;//rojo
	PORTBbits.RB4=0;//ambar
	PORTBbits.RB5=0;//verde
	segundos(3);

	/* semaforo A */
	PORTBbits.RB0=0;//rojo
	PORTBbits.RB1=1;//ambar
	PORTBbits.RB2=0;//verde
/* semaforo B */
	PORTBbits.RB3=1;//rojo
	PORTBbits.RB4=0;//ambar
	PORTBbits.RB5=0;//verde
	segundos(12);

    }
	return;
}
/* funcion declarada para estableces el tiempo en que se quedara cada color de semaforo "void nombre_de_funcion(valor de entrada)"*/
void segundos(int t){// El int t declara que t es entero {
	int i;//variable para realizar el conteo
	for (i=0;i<t;i++) //bucle para repetir hasta el valor establecido 
        _delay_ms(1000);// Establece el tiempo de esper en milisegundos
	
    return;
}
