
void main(void)
{
	/* codigo principal */
	TRISB=0x00;
	PORTB=0b00000000;

/* semaforo A */
	PORTBbits.Rb0=1;//rojo
	PORTBbits.Rb1=0;//ambar
	PORTBbits.Rb2=0;//verde
/* semaforo B */
	PORTBbits.Rb3=0;//rojo
	PORTBbits.Rb4=0;//ambar
	PORTBbits.Rb5=1;//verde
	segundos(13);//llamamos a la funcion segundos que nos dara el ttiempo en que se encuentre encendido 

/* semaforo A */
	PORTBbits.Rb0=1;//rojo
	PORTBbits.Rb1=0;//ambar
	PORTBbits.Rb2=0;//verde
/* semaforo B */
	PORTBbits.Rb3=0;//rojo
	PORTBbits.Rb4=1;//ambar
	PORTBbits.Rb5=0;//verde
	segundos(2);

	/* semaforo A */
	PORTBbits.Rb0=0;//rojo
	PORTBbits.Rb1=0;//ambar
	PORTBbits.Rb2=1;//verde
/* semaforo B */
	PORTBbits.Rb3=0;//rojo
	PORTBbits.Rb4=1;//ambar
	PORTBbits.Rb5=0;//verde
	segundos(10);

	/* semaforo A */
	PORTBbits.Rb0=0;//rojo
	PORTBbits.Rb1=0;//ambar
	PORTBbits.Rb2=1;//verde
/* semaforo B */
	PORTBbits.Rb3=1;//rojo
	PORTBbits.Rb4=0;//ambar
	PORTBbits.Rb5=0;//verde
	segundos(3);

	/* semaforo A */
	PORTBbits.Rb0=0;//rojo
	PORTBbits.Rb1=1;//ambar
	PORTBbits.Rb2=0;//verde
/* semaforo B */
	PORTBbits.Rb3=1;//rojo
	PORTBbits.Rb4=0;//ambar
	PORTBbits.Rb5=0;//verde
	segundos(12);


	return 0;
}
/* funcion declarada para estableces el tiempo en que se quedara cada color de semaforo "void nombre_de_funcion(valor de entrada)"*/
void segundos(int t)// El int t declara que t es entero 
{
	int i;//variable para realizar el conteo
	for (i=0;i<t;i++) //bucle para repetir hasta el valor establecido 
		_delay_ms(1000);// Establece el tiempo de esper en milisegundos
	return;
}