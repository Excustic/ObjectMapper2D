/****************************************************************
2D_Object_Mapper.ino

XYZ Interactive ZX Sensor
RobotDyn Touchsensor
CJMCU 8x8 LED Matrix

Michael Kushnir @ Unemployed
Sep 13, 2023


Displays the location of an object on a LED matrix using ZX sensor by pressing a touch sensor.

Hardware Connections:
 
 Arduino Pin  ZX Sensor Board  RobotDyn Touchsensor  CJMCU 8x8 LED Matrix  Function
 ----------------------------------------------------------------------------------
 5V/5V/External     VCC                     VCC                 5V          Power
 GND                GND                     GND                 GND         Ground
 A4/D3/D5           DA                      DIN                 DOUT        Data
 A5/-/-             CL                      -                               I2C Clock

Resources:
Include Wire.h, ZX_Sensor.h and FastLED.h

Development environment specifics:
Written in Arduino 2.2.1
Tested with an Arduino UNO

Distributed as-is; no warranty is given.
****************************************************************/
#include <Wire.h>
#include <ZX_Sensor.h>
#include <FastLED.h>

// Constants
const int ZX_ADDR = 0x10;  // ZX Sensor I2C address
const int ZX_MAX_VAL = 240;
const int ZX_MIN_VAL = 0;
const int MATRIX_SIDE = 8; // Number of rows/columns in our square matrix.
const int ZX_MATRIX_INTERVAL = ZX_MAX_VAL/MATRIX_SIDE; // When displayed on an 8*8 matrix, one cell takes a range from 0-30 in increments.
constexpr auto COLOR_ORDER = GRB;  

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
uint8_t x_pos;
uint8_t z_pos;
uint8_t key_pressed[2];

// Macros
#define TouchInput 3
#define DATA_PIN 5
#define NUM_LEDS 64
#define LED_TYPE WS2812B 
// Declare matrix
CRGBArray<NUM_LEDS> led_matrix;

// Function prototype for init_matrix
void init_matrix(CRGB fillColor);

void setup() {
  uint8_t ver;
  pinMode(TouchInput, INPUT);
  // Initialize Serial port
  Serial.begin(9600);
  // Initialize ZX Sensor (configure I2C and read model ID)
  if ( zx_sensor.init() ) {
    Serial.println("ZX Sensor initialization complete");
  } else {
    Serial.println("Something went wrong during ZX Sensor init!");
  }
  // Read the model version number and ensure the library will work
  ver = zx_sensor.getModelVersion();
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading model version number");
  } else {
    Serial.print("Model version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_MODEL_VER ) {
    Serial.print("Model version needs to be ");
    Serial.print(ZX_MODEL_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }
  // Read the register map version and ensure the library will work
  ver = zx_sensor.getRegMapVersion();
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading register map version number");
  } else {
    Serial.print("Register Map Version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_REG_MAP_VER ) {
    Serial.print("Register map version needs to be ");
    Serial.print(ZX_REG_MAP_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }

  //Initialize led matrix
  LEDS.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(led_matrix,NUM_LEDS);
  FastLED.setBrightness(128);
  set_max_power_in_volts_and_milliamps(5, 500);
  init_matrix(CRGB::Red);
  //Initialize keypress
  key_pressed[0] = -1;
  key_pressed[1] = -1;
}

void loop() {

  init_matrix(CRGB::Red);
  // If there is position data available, update matrix
  if ( zx_sensor.positionAvailable() ) {
    x_pos = zx_sensor.readX();
    z_pos = zx_sensor.readZ();
    if ( x_pos != ZX_ERROR && z_pos != ZX_ERROR) {
      // The led matrix is represented as an array in code (that's just how the library works).
      // To convert from a n X n matrix to a n^2, for indices i and j:newpos = i*coulums + j.
      // to get i and j we convert the sensor's working range of X and Z which is 0<=x or z<=240
      // to 0<=i or j<=n.
      led_matrix[(z_pos/ZX_MATRIX_INTERVAL)+(x_pos/ZX_MATRIX_INTERVAL-1)*MATRIX_SIDE] = CRGB::Green;
  }
  
  //First data register
  if(key_pressed[0] == -1){
    key_pressed[0] = digitalRead(TouchInput);
  }
  else{
    //Second data register
    if(key_pressed[1] == -1){
      key_pressed[1] = digitalRead(TouchInput);
    }
    else{
      //Third or later register
      //Rearrange pair
      key_pressed[0] = key_pressed[1];
      key_pressed[1] = digitalRead(TouchInput);
      //Recognise a key press (down, up)
      if(key_pressed[0] == 1 && key_pressed[1] == 0){
        FastLED.show();
        Serial.println("pressed");
      }
    }
  }
  
  } 
}

void init_matrix(CRGB fillColor){
  for(byte i = 0; i < NUM_LEDS; i++){
    led_matrix[i] = fillColor;
  }
}

