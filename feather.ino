/*
   Author: Aneesh Sharma
           Daniel Lozano
           Freddy
           Victor
   Date: April 2019
   Version: v1.1
*/

#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include <LowPower.h>   //https://github.com/rocketscream/Low-Power
#include <Adafruit_GPS.h>

#define DEBUG false
#define I2C_DEBUG false
#define UNIT_TYPE basic

// --------- PIN CONFIG -----------
#define BATTERY_PIN A0  // VOL_BAT
#define POLOLU_EN   A1  // EN
#define CTR1        A2  // CTR1
#define CTR2        A3  // CTR2
#define RPI_PWR_PIN A4  // RPI_ON
#define FONA_KEY    A5  // KEY 
#define GPS_TX      14  // TX_GPS
#define GPS_RX      15  // RX_GPS
#define SDA         17  // SDA
#define SCL         18  // SCL
#define B_RGB       10  // R
#define G_RGB       11  // R
#define R_RGB       12  // R
#define RED_LED     13  // RED LED on the feather board
#define EXT_PS      15  // EXT_PS
#define RPI_WAKE    16  // RPI_WAKE
#define SLEEP_PIN    6  // this pin puts unit to sleep

// -------- CONFIG VARIABLES---------
#define LOOP_SLEEP_INTERVAL 500 //ms
#define STATUS_BROADCAST_INTERVAL 3600 //number in terms of loops
#define V_THRES_INTERNAL_MIN 3.35
#define V_THRES_MIN 3.45
#define V_THRES_ON 3.65
#define V_THRES_COLLECTION 3.50
#define SHUTDOWN_COUNTER_THRES 5
#define TURNON_COUNTER_THRES 5

#define ScaleSerial         Serial1
#define I2C_SLAVE_ADDRESS 0x2b
#define nro_id            1
#define ver_rev           1

// below time is in microseconds
#define t_battery_status          5000000  //  5 secs =  5000000 microseconds
#define t_scale_read              1000000  //  1 sec = 1000000 microseconds //update rate is 1 hz
#define t_update_state           10000000  // 10 secs = 10000000 microseconds
#define t_rpi_ping_interval     120000000  // 120 secs = 120000000 microseconds - 1 minute before the rpi gets restarted
#define t_rpi_shutdown_interval  60000000  //  60 secs =  60000000 microseconds - 1 minute before the rpi gets restarted
#define t_led_blink_interval     800000  //  .3 secs =  300000 microseconds
#define t_led_read 1000000  //  1 secs =  1000000 microseconds

enum SYS_RECORD {number_id  ,//0
                 version_f  ,//1
                 status_bat   ,//2
                 date_now,//3
                 time_local,//4
                 time_gtm,//5
                 lat_entire ,//6
                 lon_entire ,//7
                 gps_speed  ,//8
                 gps_angle  ,//9
                 v_pwr    ,//10
                 v_battery,//11
                 status_code, //12
                 weight, //13
                 last_reg //14
                };
enum MODES     {mode_overcharged,   //    >4.2V
                mode_highcharged,   // 4.0-4.2V
                mode_charged,       // 3.4-4.0V
                mode_lowcharged,    // 3.2-3.4V
                mode_discharged
               };   //    <3.2V
enum COMMANDS  {how_many = 0xFF ,
                send_data = 0x14 ,
                command_shutdown = 0xFD,
                camera_state = 0xFE
               };
enum STATE { rpi_status,       //true -> raspberry pi is online, false-> raspberry pi is offline
             sending_i2c_data, //true->sending i2c data to the master so avoid sleeping in the middle of the transfer
             rpi_reachable,    //true-> raspberry pi is reachable, false-> raspberry pi is not reachable
             disable_charging,  //true-> disable battery charging, false-> enable battery charging
             battery_save,      //true->switches off rpi, false -> enables rpi
             scale_reachable,   //true->scale is reachable, false -> scale is offline
             green_led,
             last_state
           };
enum time_struct { scale,
                   battery,
                   system_state,
                   rpi_ping,
                   rpi_shutdown,   // counter to wait before shutting down,
                   led_blink,
                   led_status,
                   last_timestruct
                 };
//-----------------------------------

//----------Global variables---------
int WAKEUP_PIN = EXT_PS; // wakes up the unit when there is an rising interrupt
char data[90], v_camera_bat_str[6], gps_lat_str[10], gps_lon_str[15], update_interval_mins[5];
uint8_t state = 0, SHUTDOWN_COUNTER, TURNON_COUNTER, VSOL_INPUT;
float v_camera_bat = 3.35, v_camera_bat_old = 0.0;
bool CAMERA_STATUS, SLEEP_FLAG;
uint32_t LOOP_COUNTER = 0;
int OUTPUT_PINS[9] = {R_RGB, G_RGB, B_RGB, CTR2, POLOLU_EN, RPI_WAKE, RPI_PWR_PIN, CTR1, FONA_KEY};
long REGISTER[last_reg]; // need more bits for precision --- DO NOT EDIT THIS
boolean SYSTEM_STATE[last_state]; //storing boolean state of the system
int deltaV = 5, val_color;

void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy
unsigned long SYSTEM_TIMER[last_timestruct];
unsigned long timer_now; // range is from [0, 4294967295 (i.e. 2^32 - 1)]
int in_command, in_register;
// scale information
float SCALE_WEIGHT = 0.0;
String SCALE_UNIT = "";

void setup() {
  //initialize output pins
  for (int i = 0; i < 9; ++i) {
    pinMode(OUTPUT_PINS[i], OUTPUT);
    digitalWrite(OUTPUT_PINS[i], LOW);
  }

  // assume rpi is offline unless we receive a ping from the rpi
  for (int i = 0; i < 8; i++) {
    SYSTEM_STATE[i] = false;
  }
  //initialize input pins
  pinMode(EXT_PS, INPUT);
  REGISTER[v_pwr] = 0; //assume no external power supply

  //color(0xFF, 0xFF, 0xFF);
  digitalWrite(POLOLU_EN, LOW); //enable it by default
  digitalWrite(RPI_PWR_PIN, LOW); //enable rpi
  digitalWrite(RPI_WAKE, LOW);
  digitalWrite(CTR1, LOW);
  digitalWrite(CTR2, LOW);

  Serial.begin(9600);
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(indata);
  Wire.onRequest(outdata);
  ScaleSerial.begin(9600);
  REGISTER[number_id] = nro_id;
  REGISTER[version_f] = ver_rev;
  REGISTER[status_bat] = mode_charged;

  // initialize timers
  SYSTEM_TIMER[scale] = micros();
  SYSTEM_TIMER[battery] = SYSTEM_TIMER[scale];
  SYSTEM_TIMER[system_state] = SYSTEM_TIMER[scale];
  SYSTEM_TIMER[rpi_ping] = SYSTEM_TIMER[scale];
  SYSTEM_TIMER[rpi_shutdown] = SYSTEM_TIMER[scale];
  SYSTEM_TIMER[led_blink] = SYSTEM_TIMER[scale];
  SYSTEM_TIMER[led_status] = SYSTEM_TIMER[scale];

  delay(1000);
}

void loop() {
  timer_now = micros();
  if (timer_now - SYSTEM_TIMER[scale] >= t_scale_read) {
#if DEBUG==true
    Serial.println("Refreshing SCALE=========");
#endif
    refresh_scale_data();
    SYSTEM_TIMER[scale] = timer_now;
  }
  // handling the situation to avoid putting feather to sleep during the data transfer over i2c
  if (!SYSTEM_STATE[sending_i2c_data] && timer_now - SYSTEM_TIMER[battery] >= t_battery_status) {
#if DEBUG==true
    Serial.println("Refreshing battery voltage=======");
#endif
    refresh_battery_state();
    battery_save_mode(); //checks to see if we need to put camera in the battery save mode
    SYSTEM_TIMER[battery] = timer_now;
  }
  if (timer_now - SYSTEM_TIMER[led_status] >= t_led_read) {
#if DEBUG==true
    Serial.println("Refreshing LEDS =========");
#endif
    set_led_status();
    SYSTEM_TIMER[led_status] = timer_now;
  }
  if (timer_now - SYSTEM_TIMER[system_state] >= t_update_state) {
#if DEBUG==true
    Serial.println("Updating camera state=======");
    Serial.print("Battery Save Option: "); Serial.println(SYSTEM_STATE[battery_save]);
    Serial.print("External pwr: "); Serial.println(REGISTER[v_pwr]);
    Serial.print("Scale status: "); Serial.println(SYSTEM_STATE[scale_reachable]);
    Serial.print("RPI status: "); Serial.println(SYSTEM_STATE[rpi_status]);
#endif
    control_system_state();
    if (!SYSTEM_STATE[battery_save]) {
      // when battery save is disabled
      switch (SYSTEM_STATE[rpi_status]) {
        case true:
          digitalWrite(RPI_PWR_PIN, HIGH); //turn on rpi
          break;
        case false:
          digitalWrite(RPI_PWR_PIN, LOW); //turn off rpi
          break;
      }
    } else {
      // if battery save is enabled
      digitalWrite(RPI_PWR_PIN, LOW); //turn off rpi
    }

    switch (SYSTEM_STATE[disable_charging]) {
      case true:
        digitalWrite(POLOLU_EN, HIGH); //disable the charging chip
        break;
      case false:
        digitalWrite(POLOLU_EN, LOW); //enable the charging chip
        break;
    }
    SYSTEM_TIMER[system_state] = timer_now;
  }

  if (timer_now - SYSTEM_TIMER[rpi_ping] >= t_rpi_ping_interval) {
    // we have exceeded the timeout to get a ping from raspberry pi and battery save is also disabled
    if (SYSTEM_STATE[rpi_status] && !SYSTEM_STATE[rpi_reachable] && !SYSTEM_STATE[battery_save]) {
#if DEBUG==true
      Serial.println("========== REBOOTING RPI ========");
      Serial.print(SYSTEM_STATE[rpi_status]);
      Serial.println(SYSTEM_STATE[rpi_reachable]);
#endif
      // restart raspberry pi
      reboot_rpi();
    }
    // resetting the counter
    SYSTEM_STATE[rpi_reachable] = false;
    SYSTEM_STATE[sending_i2c_data] = false;
    SYSTEM_TIMER[rpi_ping] = timer_now;
  }
}

void battery_save_mode() {
  //if there is no external power and scale is offline then switch off rpi
  if (REGISTER[v_pwr] == 0 && !SYSTEM_STATE[scale_reachable])
    SYSTEM_STATE[battery_save] = true;
  else
    SYSTEM_STATE[battery_save] = false;
}

void refresh_scale_data() {
  String inData = ScaleSerial.readStringUntil('\n');
  String raw_data = "";
  if (inData.length() == 16) {
    if (inData.indexOf(",GS") == 2) {
      raw_data = inData;
      // we got the data from the scale
      SYSTEM_STATE[scale_reachable] = true;
    }
  }
  else {
      SYSTEM_STATE[scale_reachable] = false;
  }
  parse_scale_data(raw_data);
  REGISTER[weight] = SCALE_WEIGHT * 100L;
#if DEBUG==true
  Serial.print("Live Scale data: ");
  Serial.print(SCALE_WEIGHT); Serial.println(SCALE_UNIT);
#endif
  
}

void parse_scale_data(String raw_data) {
  String data_sign = raw_data.substring(4, 6);
  SCALE_UNIT = raw_data.substring(13, 15);
  if (data_sign == "S-")
  {
    if (SCALE_UNIT == "kg") {
      String kg = raw_data.substring(6, 13);
      SCALE_WEIGHT = kg.toFloat() * -1;
    }
    else if (SCALE_UNIT == "lb") {
      String lb = raw_data.substring(6, 13);
      SCALE_WEIGHT = lb.toFloat() * -0.453592;
    }
    else if (SCALE_UNIT == "oz") {
      String oz = raw_data.substring(6, 13);
      SCALE_WEIGHT = oz.toFloat() * -0.028349;
    }
    else if (SCALE_UNIT == "lz") {
      String oz = raw_data.substring(11, 13);
      String lb = raw_data.substring(6, 10);
      SCALE_WEIGHT = oz.toInt() * -0.028349 + lb.toInt() * -0.453592;
    }
  } else {
    if (SCALE_UNIT == "kg") {
      String kg = raw_data.substring(5, 13);
      SCALE_WEIGHT = kg.toFloat();
    }
    else if (SCALE_UNIT == "lb") {
      String lb = raw_data.substring(5, 13);
      SCALE_WEIGHT = lb.toFloat() * 0.453592;
    }
    else if (SCALE_UNIT == "oz") {
      String oz = raw_data.substring(5, 13);
      SCALE_WEIGHT = oz.toFloat() * 0.028349;
    }
    else if (SCALE_UNIT == "lz") {
      String oz = raw_data.substring(11, 13);
      String lb = raw_data.substring(5, 10);
      SCALE_WEIGHT = oz.toInt() * 0.028349 + lb.toInt() * 0.453592;
    }
  }
}

void reboot_rpi() {
#if DEBUG==true
  Serial.print("=================== Stopping rpi ===================");
#endif

  digitalWrite(RPI_PWR_PIN, LOW);
  delay(500); //0.5 second wait before restarting the rpi
  digitalWrite(RPI_PWR_PIN, HIGH);

#if DEBUG==true
  Serial.println("=================== Rebooted rpi! ==================");
#endif
}

/* TURN ON  raspberry RPI_WAKE=HIGH
                     RPI_ON =HIGH*/
/* TURN OFF raspberry RPI_WAKE=LOW
                     RPI_ON=LOW */
/*
   mode_overcharged:      >4.25v   RPI=ON FEATHER=ON
   mode_highcharged:  3.95-4.25v   RPI=ON FEATHER=ON
   mode_charged:      3.35-4.05v   RPI=ON FEATHER=ON
   mode_lowcharged:   3.25-3.35v  RPI=OFF FEATHER=ON
   mode_discharged:       <3.25v  RPI=OFF FEATHER=OFF
*/
void control_system_state() {
#if DEBUG==true
  Serial.print("RPI STATE: ");
  Serial.print("Running: "); Serial.print(SYSTEM_STATE[rpi_status] ? "Yes" : "No");
  Serial.print(", Reachable: "); Serial.println(SYSTEM_STATE[rpi_reachable] ? "Yes" : "No");
  Serial.print("MODE   : ");
  Serial.println(REGISTER[status_bat]);
  Serial.print("ext Power   : ");
  Serial.println(REGISTER[v_pwr]);
  Serial.print("voltage   : ");
  Serial.println(REGISTER[v_battery]);
  Serial.print("Weight: ");
  Serial.print(SCALE_WEIGHT); Serial.println(SCALE_UNIT);
#endif
  switch (REGISTER[status_bat]) {
    case mode_overcharged:
      if (REGISTER[v_battery] < 430 - deltaV) {
        REGISTER[status_bat] = mode_highcharged;
      }
      else {
        //color(0x00, 0x00, 0xFF); //blue
        SYSTEM_STATE[rpi_status] = true;
        SYSTEM_STATE[disable_charging] = true;
      }
      break;
    case mode_highcharged:
      if (REGISTER[v_battery] > 420 + deltaV) {
        REGISTER[status_bat] = mode_overcharged;
      }
      else if (REGISTER[v_battery] < 410 - deltaV) {
        REGISTER[status_bat] = mode_charged;
      }
      else {
        //color(0x00, 0xFF, 0x00); // green
        SYSTEM_STATE[rpi_status] = true;
        SYSTEM_STATE[disable_charging] = false;
      }
      break;
    case mode_charged:
      if (REGISTER[v_battery] > 400 + deltaV) {
        REGISTER[status_bat] = mode_highcharged;
      }
      else if (REGISTER[v_battery] < 340 - deltaV) {
        REGISTER[status_bat] = mode_lowcharged;
      }
      else {
        val_color = map(REGISTER[v_battery], 400 + deltaV, 340 - deltaV, 255, 0);
        //color(255 - val_color, val_color, 0x00); //orange
        SYSTEM_STATE[rpi_status] = true;
        SYSTEM_STATE[disable_charging] = false;
      }
      break;
    case mode_lowcharged:
      if (REGISTER[v_battery] > 330 + deltaV) {
        REGISTER[status_bat] = mode_charged;
      }
      else if (REGISTER[v_battery] < 330 - deltaV) {
        REGISTER[status_bat] = mode_discharged;
      }
      else {
        //color(0xFF, 0x00, 0x00);
        SYSTEM_STATE[rpi_status] = false; //turns off rpi
        SYSTEM_STATE[disable_charging] = false; //turns off rpi
      }
      break;
    case mode_discharged:
      if (REGISTER[v_battery] > 320 + deltaV) {
        REGISTER[status_bat] = mode_lowcharged;
      }
      else {
        //color(0xFF, 0x00, 0xFF);
        SYSTEM_STATE[rpi_status] = false; //turns off rpi
        SYSTEM_STATE[disable_charging] = false; //turns off rpi
      }
      break;
    default:
      Serial.println("Error: Invalid State");
  }
#if DEBUG==true
  Serial.print("State= ");
  Serial.println(REGISTER[status_bat]);
#endif
}

void refresh_battery_state() {
  REGISTER[v_pwr] = digitalRead(EXT_PS); //reading if the external power supply is connected
  REGISTER[v_battery] = vol_read() * 100.0;
  if (REGISTER[status_bat] == mode_overcharged && REGISTER[v_battery] < 420 - deltaV) {
    REGISTER[status_bat] = mode_highcharged;
  }
  if (REGISTER[status_bat] == mode_highcharged && REGISTER[v_battery] > 420 + deltaV) {
    REGISTER[status_bat] = mode_overcharged;
  }
  else if (REGISTER[status_bat] == mode_highcharged && REGISTER[v_battery] < 400 - deltaV) {
    REGISTER[status_bat] = mode_charged;
  }
  if (REGISTER[status_bat] == mode_charged && REGISTER[v_battery] > 400 + deltaV) {
    REGISTER[status_bat] = mode_highcharged;
  }
  else if (REGISTER[status_bat] == mode_charged && REGISTER[v_battery] < 340 - deltaV) {
    REGISTER[status_bat] = mode_lowcharged;
  }
  if (REGISTER[status_bat] == mode_lowcharged && REGISTER[v_battery] > 340 + deltaV) {
    REGISTER[status_bat] = mode_charged;
  }
  else if (REGISTER[status_bat] == mode_lowcharged && REGISTER[v_battery] < 320 - deltaV) {
    REGISTER[status_bat] = mode_discharged;
  }
  if (REGISTER[status_bat] == mode_discharged && REGISTER[v_battery] > 320 + deltaV) {
    REGISTER[status_bat] = mode_lowcharged;
  }
}

// Battery related functions
float vol_read() {
  digitalWrite(CTR2, HIGH);
  digitalWrite(POLOLU_EN, HIGH);
  delay(5); //it is important to wait a little before doing the measurements to avoid error --> IMPORTANT
  float value = analogRead(BATTERY_PIN) * 0.0032; //the max value which can be read at V_bat is 3.22V not 4.2V
  value = (value * 43) / 33; // voltage divider of R1=10k and R2=33k
  digitalWrite(CTR2, LOW);
  digitalWrite(POLOLU_EN, LOW);
  return value;
}

// STATUS LED
void color(byte red, byte green, byte blue) {
  digitalWrite(R_RGB, red);
  digitalWrite(G_RGB, green);
  digitalWrite(B_RGB, blue);
}

void set_led_status () {
#if DEBUG==true
  Serial.print("rpi status = ");
  Serial.println(SYSTEM_STATE[rpi_status]);
  Serial.print("Led status = ");
  Serial.print(REGISTER[status_code]);
#endif
  if (SYSTEM_STATE[rpi_status]) {
    if (REGISTER[status_code] == 0) {// STAND_BY:
      digitalWrite(R_RGB  , LOW);
      digitalWrite(G_RGB  , LOW);
    }
    else if (REGISTER[status_code] == 1) { // BLINKING_GREEN:
      digitalWrite(R_RGB    , LOW);
      if (micros() - SYSTEM_TIMER[led_blink] > t_led_blink_interval) {
        digitalWrite(G_RGB, (SYSTEM_STATE[green_led] ? 1 : 0));
        SYSTEM_STATE[green_led] = !SYSTEM_STATE[green_led];
        SYSTEM_TIMER[led_blink] = micros();
      }
    }
    else if (REGISTER[status_code] == 2) { // SOLID_GREEN:
      digitalWrite(R_RGB  , LOW);
      digitalWrite(G_RGB  , HIGH);
    }
    else if (REGISTER[status_code] == 3) { // SOLID_RED:
      digitalWrite(G_RGB, LOW);
      digitalWrite(R_RGB  , HIGH );
    }
  } else {
    digitalWrite(G_RGB    , LOW);
    if (micros() - SYSTEM_TIMER[led_blink] > t_led_blink_interval) {
      digitalWrite(R_RGB, (SYSTEM_STATE[green_led] ? 1 : 0));
      SYSTEM_STATE[green_led] = !SYSTEM_STATE[green_led];
      SYSTEM_TIMER[led_blink] = micros();
    }
  }
}

static int n;
// I2C commands
void indata(int bytes) {
#if I2C_DEBUG==true
  Serial.print("i2c indata. #bytes: ");
  Serial.println(bytes);
#endif
  // any kind of activity from rpi will assume that rpi is reachable
  SYSTEM_STATE[rpi_reachable] = true;
  if (bytes == 2) {
    in_command = Wire.read();
    in_register = Wire.read();
    n = 0;
#if I2C_DEBUG==true
    Serial.print("Data received: ");
    Serial.print(in_command); Serial.print(", "); Serial.println(in_register);
#endif
  }
  else {
    while (Wire.available() > 0) {
      byte error = Wire.read();
    }
#if I2C_DEBUG==true
    Serial.println("error: i2c onReceive");
#endif
  }
}


void outdata() {
  SYSTEM_STATE[sending_i2c_data] = true;
#if I2C_DEBUG==true
  Serial.println("i2c outdata");
#endif
  String target = String(REGISTER[in_register]);
#if I2C_DEBUG==true
  Serial.print("Requested data: "); Serial.println(target);
#endif
  switch (in_command) {
    case how_many:
#if I2C_DEBUG==true
      Serial.print("Sending size: "); Serial.println(target.length());
#endif
      Wire.write(target.length());
      n = 0;
      break;
    case send_data:
#if I2C_DEBUG==true
      Serial.print("Sending char: "); Serial.println(target[n]);
#endif
      Wire.write(target[n]);
      n++;
      if (n > target.length()) {
        n = 0;
        SYSTEM_STATE[sending_i2c_data] = false;
      }
      break;
    case command_shutdown:
#if I2C_DEBUG==true
      Serial.print("commanding rpi: "); Serial.println(SYSTEM_STATE[rpi_status]);
#endif
      Wire.write((SYSTEM_STATE[rpi_status]) ? '1' : '0');
      SYSTEM_TIMER[battery] = in_register * 1000; //milliseconds
      break;
    case camera_state:
      // command red and green led according to the rpi input
      Wire.write(0x30);
      REGISTER[status_code] = in_register;
      Serial.print ("Status: ");
      Serial.println (REGISTER[status_code]);
      break;
    default:
      Serial.print("Err: Command - "); Serial.println(in_command);
      Serial.println("error: not command valid");
  }
}
