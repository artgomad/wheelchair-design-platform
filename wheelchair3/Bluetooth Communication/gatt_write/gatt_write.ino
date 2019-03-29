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

#include <Adafruit_NeoPixel.h> 

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
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
#define LED_PIN 6
#define NUM_LEDS 10
Adafruit_NeoPixel LED_controller = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800); 
uint8_t R = 0, G = 0, B = 0;

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
    

    int behaviour = (int)data[0];

    switch (behaviour){
      case 1://start audio
        respiration();
        break;
      /*case 2:
        decreaseVibrationLeft();
        break;
        */
      default:
        off();
        break;
    }
  }
}
int counter = 0;
int vibrationValue = 10;
int vibrationDirection = 1; 
int lightValue=0;

void startVibrationLeft(){
  
  vibrationValue = 200;
  analogWrite(VIB_PIN, vibrationValue);
  for (int i=0;i<NUM_LEDS;i++){
     LED_controller.setPixelColor( i, LED_controller.Color( 255, 0, 0 ) );
     LED_controller.show();
  }
}

void respiration(){
  Serial.print("vibrationValue = ");
  Serial.print(vibrationValue);
  Serial.print(";   lightValue = ");
  Serial.println(lightValue);
  
  if (vibrationValue >= 200 || vibrationValue <= 0){//max vibration || min vibration
      vibrationDirection = -vibrationDirection;
    };

  if (millis() - counter > 1){//delay between increasing or decreasing vibration   
    vibrationValue = vibrationValue + 10*vibrationDirection;
    
    analogWrite(VIB_PIN, vibrationValue);
    
    lightValue=map(vibrationValue,0,200,0,100);
    
    for (int i=0;i<NUM_LEDS;i++){
       LED_controller.setPixelColor( i, LED_controller.Color( lightValue, 0, 0 ) );
    }
    LED_controller.show();
    counter = millis();
  }
}

void off(){
   
   analogWrite(VIB_PIN, 0);
   for (int i=0;i<NUM_LEDS;i++){
           LED_controller.setPixelColor( i, LED_controller.Color( 0, 0, 0 ) );
           Serial.print("led ");
           Serial.print(i);
           Serial.println(" off");
        }
   LED_controller.show();
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
  
  LED_controller.begin(); // We're starting up the library

  pinMode(VIB_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  //LED_controller.setPixelColor( 0, LED_controller.Color( 0, 150, 0 ) );
  //LED_controller.show();
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

void loop(void) {
  // Check every 200ms for new command receive from Bluetooth
  ble.update(200);
}

