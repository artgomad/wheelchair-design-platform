/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

//#include <Adafruit_NeoPixel.h> 

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

/* This example demonstrates how to use Bluefruit callback API :
 * - setConnectCallback(), setDisconnectCallback(), setBleUartRxCallback(),
 * setBleGattRxCallback() are used to install callback function for specific
 * event. 
 * - Furthermore, update() must be called inside loop() for callback to
 * be executed.
 * 
 * The sketch will add an custom service with a writable characteristic,
 * and install callback to execute when there is an update from central device
 */
                            

#define FACTORYRESET_ENABLE        1
// Minimum firmware version to have some new features
#define MINIMUM_FIRMWARE_VERSION   "0.7.0"

// Create the bluefruit object, either software serial
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// Writable GATT Service
int32_t charid_led;


// Neo Pixel
#define DATA_PIN_leds 5
#define NUM_LEDS 16
#define LED_TYPE    WS2811
//#define LED_TYPE    WS2813
#define COLOR_ORDER GRB
#define BRIGHTNESS 60
#define FRAMES_PER_SECOND  100

CRGB leds[NUM_LEDS];
//Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN_leds, NEO_RGB + NEO_KHZ800); 
//uint8_t R = 0, G = 0, B = 0;

//Vibrator motor
#define VIB_PIN 10

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// What to do when a Bluetooth client connect to the Feather
void connected(void) {
  Serial.println( F("Connected") );
}

// What to do when a Bluetooth client disconnect from the Feather
void disconnected(void) {
  Serial.println( F("Disconnected") );
}

int behaviour = 0;
// What to do when a Bluetooth device writes on a GATT characteristic
void BleGattRX(int32_t chars_id, uint8_t data[], uint16_t len) {
  Serial.print( F("[BLE GATT RX] (" ) );
  Serial.print(chars_id);
  Serial.print(") ");


  if (chars_id == charid_led) {
    //Todos estos commandos Serial solo sirven para escribir en el terminal lo que le llega por bluetooth a la feather
    Serial.println("Data received on led characteristic: ");
    //Uso Serial.write para escribir el nuevo valor de la caracteristica en bytes (como viene por bluetooth)
    //De hecho me viene como un bytearray que puedo descomponer en data[0], data[1] y data[2]
    //Serial.write(buf, len)----->   buf: an array to send to the terminal as a series of bytes // len: the length of the buffer
    //Serial.write(data, len);
    //Serial.println(len);

    //el ",DEC" sirve para definir como quiero que me escriba en el terminal el valor de data[x], en este caso como un numero decimal (DEC)
    /*
    for (int i=0, i<len, i++){
      Serial.println(data[i], DEC);
    }
    */
    
    Serial.println(data[0], DEC);
    //Serial.println(data[1], DEC);
    //Serial.println(data[2], DEC);
    

    behaviour = (int)data[0];
  }
}
long counter = 0;
int vibrationValue = 10;
int vibrationDirection = 1; 
int lightValue=0;

void startVibrationLeft(){
  
  vibrationValue = 200;
  analogWrite(VIB_PIN, vibrationValue);
  for (int i=0;i<NUM_LEDS;i++){
     //leds.setPixelColor( i, leds.Color( 255, 0, 0 ) );
     leds[i] = CHSV( 100, 200, i);
     //FastLED.show();
  }
}

void respiration(){
  Serial.print("vibrationValue = ");
  Serial.print(vibrationValue);
  Serial.print(";   lightValue = ");
  Serial.println(lightValue);
  
  if (vibrationValue >= 200){//max vibration || min vibration
      vibrationValue = 200;
      vibrationDirection = -1;
  }else if(vibrationValue <= 0){
      vibrationValue = 0;
      vibrationDirection = 1;
  }

  if (millis() - counter > 80){//delay between increasing or decreasing vibration   
    
    vibrationValue = vibrationValue + 5*vibrationDirection;
    analogWrite(VIB_PIN, vibrationValue);
    
    lightValue=map(vibrationValue,0,200,0,100);
    
    for (int i=0;i<NUM_LEDS;i++){
       leds[i] = CHSV( 100, 200, lightValue);
    }

    
    
    counter = millis();  
  }
   //FastLED.delay(1000/FRAMES_PER_SECOND);
   //FastLED.show();
}
int ledNumber=0;
int ledOpuesto=NUM_LEDS-1;

void respiration_2(){
    
   if (ledNumber == NUM_LEDS/2 - 1 || ledNumber == 0){
       vibrationDirection = -vibrationDirection;
   }
   if (vibrationDirection == 1){
    
       leds[ledNumber] = CRGB::Blue;
       leds[ledOpuesto] = CRGB::Blue;
       //FastLED.show(); 
       //delay(60);
       
   }else if (vibrationDirection == -1){
       
       leds[ledNumber] = CRGB::Black;
       leds[ledOpuesto] = CRGB::Black;
       //FastLED.show();
       //delay(60); 
   }   
    
   ledNumber = ledNumber + vibrationDirection;
   ledOpuesto = NUM_LEDS - (1+ledNumber);
}

void off(){
   
   analogWrite(VIB_PIN, 0);
   /*
   for (int i=0;i<NUM_LEDS;i++){
           leds.setPixelColor( i, leds.Color( 0, 0, 0 ) );
           Serial.print("led ");
           Serial.print(i);
           Serial.println(" off");
        }
   */
   fadeToBlackBy( leds, NUM_LEDS, 255);
   Serial.println("off() function triggered");
   FastLED.show();
}


void decreaseVibrationLeft(){
  
  if (vibrationValue <= 0){
    vibrationValue = 0;
    //Send response to pi telling to change behaviour
  }else if (millis() - counter > 200){
    vibrationValue--;
    analogWrite(VIB_PIN, vibrationValue);
    counter = millis();
  }


}

void setup(void) {
  
  pinMode(VIB_PIN, OUTPUT);
  pinMode(DATA_PIN_leds, OUTPUT); 
  
  FastLED.addLeds<LED_TYPE,DATA_PIN_leds,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //leds.begin(); // We're starting up the library

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  //leds.setPixelColor( 0, leds.Color( 0, 150, 0 ) );
  //FastLED.show();
  off();

  delay(500);

  Serial.begin(115200);

  if ( !ble.begin(VERBOSE_MODE) ) {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  if ( FACTORYRESET_ENABLE ) {
    // Perform a factory reset to make sure everything is in a known state
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }
  
  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
    error( F("Callback requires at least 0.7.0") );
  }

  // Adding custom Service 0x1234 with characteristic 0x2345
  ble.sendCommandCheckOK( F("AT+GATTADDSERVICE=uuid=0x1234") );
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=0x2345,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_led);
  //PROPERTIES=0x08 is the Bit-field of the property and indicates how to interact with this property, 
  //the 0x08 value defines the property as write, permiting writes of the characteristic value with response

  ble.reset();

  // Disable command echo from Bluefruit
  ble.echo(false);

  // Print Bluefruit information
  ble.info();
  
  /* Set callbacks */
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnected);
  
  // Only one BLE GATT function should be set, it is possible to set it
  // multiple times for multiple Chars ID
  ble.setBleGattRxCallback(charid_led, BleGattRX);

  
}

long count = 0;

void showleds(){
  if (millis() - count > 1){
    FastLED.show();
    Serial.println ("show leds!!");
    count = millis();
  };
}

void loop(void) {
   
  // Check every 200ms for new command receive from Bluetooth
  ble.update(200);
  switch (behaviour){
      case 1://start audio
        //off();
        respiration();
        //respiration_2();
        break;
      /*case 2:
        decreaseVibrationLeft();
        break;
        */
      default:
        //respiration();
        Serial.println("default");
        off();
        break;
    }
  
   showleds();

}

