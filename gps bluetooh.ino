#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial sss(2,3);
void void setup()
{
	Serial.begin(115200);
	sss.begin(9600);
}

void void loop()
{
	float flat,flon;
	unsigned long age;
	gps.f_get_position(&flat,&flon,&age);
	flat==TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;
	flon==TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
	Serial.print(flat);
	Serial.print(flon);
	delay(500);
}